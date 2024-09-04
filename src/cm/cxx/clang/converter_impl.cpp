// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file converter_impl.cpp
/// Contains implementation of the converter_impl class.

#include "pch.hpp"

#include "converter_impl.hpp"
#include "convert_template.hpp"
#include "log.hpp"
#include "types_converter.hpp"


namespace cm::cxx::clang {


converter_impl::converter_impl(code_model & mdl, const ::clang::ASTContext & clang_ctx):
mdl_{mdl},
clang_ctx_{clang_ctx},
types_conv_{std::make_unique<types_converter>(*this)} {
}


converter_impl::~converter_impl() = default;


void converter_impl::convert() {
    auto tu_decl = clang_ctx().getTranslationUnitDecl();

    // adding association with entire code model for translation unit decl
    add_decl_entity(tu_decl, &mdl());

    // setting translation unit as current decl context
    converter_impl::decl_context_setter csetter{*this, &mdl(), tu_decl};

    CM_CLANG_LOG_DECL("converting translation unit", tu_decl);

    // traversing over all top level declarations in translation unit
    for (auto && decl : tu_decl->decls()) {
        CM_CLANG_LOG_DECL("converting top level declaration", decl);
        convert_decl(decl);
    }
}


context_entity * converter_impl::decl_entity(const ::clang::Decl * decl) {
    // looking for existing entity assoicated with clang declaration
    if (auto ent = decls_.get(decl)) {
        return ent;
    }

    // creating new entity and adding it to map
    auto ent = create_decl_entity(decl);
    // TODO
    // assert(ent != nullptr && "create_entity returned nullptr");

    if (ent) {
        decls_.add(decl, ent);
    }

    return ent;
}


context * converter_impl::parent_ctx(const ::clang::Decl * decl) {
    // getting parent clang declaration
    auto decl_ctx = decl->getDeclContext();
    assert(decl_ctx != nullptr && "invalid parent clang decl context");
    auto parent_decl = ::clang::dyn_cast<::clang::Decl>(decl_ctx);
    assert(parent_decl != nullptr && "parent clang decl context is not a declaration");

    // getting existing or creating new code model context
    return decl_entity_as<context>(parent_decl);
}


void converter_impl::convert_decl(const ::clang::Decl * decl) {
    convert_decl_impl<0>(decl);
}


type_t * converter_impl::type(const ::clang::Type * clang_type) {
    assert(::clang::dyn_cast<::clang::ParenType>(clang_type) == nullptr &&
           "clang ParenType is no allowed here");

    // if type has declaration then using declarations map and conversion functions to get type
    if (auto decl = get_type_decl(clang_type)) {
        return decl_entity_as<context_type>(decl);
    }

    // searching for existing code model type associated with clang type
    if (auto it = types_.find(clang_type); it != types_.end()) {
        assert(it->second != nullptr && "null code model type assoicated with clang type");
        return it->second;
    }

    // trying types converter
    auto tp = types_conv_->type(clang_type);

    // creating new type
    if (!tp) {
        tp = create_type(clang_type);
    }

    // adding created type to map of types
    types_.emplace(clang_type, tp);

    return tp;
}


/// Gets existing or creates new code model qual type for clang qual type.
qual_type converter_impl::type(const ::clang::QualType & clang_type) {
    auto real_clang_type = clang_type;

    // special case for paren type
    if (auto p_type = ::clang::dyn_cast<::clang::ParenType>(real_clang_type.getTypePtr())) {
        // ignoring parens in code model for now
        real_clang_type = p_type->getInnerType();
    }

    // converting base type
    auto type = this->type(real_clang_type.getTypePtr());

    // adding cv-qualifiers

    qual_type qtype{type};

    if (clang_type.isLocalConstQualified() || real_clang_type.isLocalConstQualified()) {
        qtype.set_const(true);
    }

    if (clang_type.isLocalVolatileQualified() || real_clang_type.isLocalVolatileQualified()) {
        qtype.set_volatile(true);
    }

    return qtype;
}


type_t * converter_impl::get_type(const ::clang::Type * clang_type) const {
    assert(::clang::dyn_cast<::clang::ParenType>(clang_type) == nullptr &&
           "clang ParenType is not allowed here");

    // if type has declaration then using declarations map and conversion functions to get type
    if (auto decl = get_type_decl(clang_type)) {
        return get_decl_entity_as<context_type>(decl);
    }

    // searching for existing code model type associated with clang type
    if (auto it = types_.find(clang_type); it != types_.end()) {
        assert(it->second != nullptr && "null code model type assoicated with clang type");
        return it->second;
    }

    assert(false && "can't find existing code model type for clang type");
    return nullptr;
}


qual_type converter_impl::get_type(const ::clang::QualType & clang_type) const {
    auto real_clang_type = clang_type;

    // special case for paren type
    if (auto p_type = ::clang::dyn_cast<::clang::ParenType>(real_clang_type.getTypePtr())) {
        // ignoring parens in code model for now
        real_clang_type = p_type->getInnerType();
    }

    // getting code model type for base type
    auto type = this->get_type(real_clang_type.getTypePtr());

    // adding cv-qualifiers

    qual_type qtype{type};

    if (clang_type.isLocalConstQualified() || real_clang_type.isLocalConstQualified()) {
        qtype.set_const(true);
    }

    if (clang_type.isLocalVolatileQualified() || real_clang_type.isLocalVolatileQualified()) {
        qtype.set_volatile(true);
    }

    return qtype;
}


context_entity * converter_impl::create_decl_entity(const ::clang::Decl * decl) {
    return create_decl_entity_impl<0>(decl);
}


template <size_t I>
context_entity * converter_impl::create_decl_entity_impl(const ::clang::Decl * decl) {
    if constexpr (I == std::tuple_size_v<decl_converters>) {
        // matching converter not found
        // TODO
        // CM_CLANG_LOG_SCAT_DECL_ERROR(decl, "unknown clang declaration", decl);
        // assert(false && "unknown clang declaration");
        return nullptr;
    } else {
        // trying current converter
        using converter_t = std::tuple_element_t<I, decl_converters>;
        if (auto conv_decl = ::clang::dyn_cast<typename converter_t::clang_decl_t>(decl)) {
            auto & conv = std::get<I>(decl_convs_);
            if (conv.match(conv_decl)) {
                auto res = conv.create_entity(*this, conv_decl);
                assert(res != nullptr && "converter create_entity returned nullptr");
                return res;
            }
        }

        // trying next converters
        return create_decl_entity_impl<I + 1>(decl);
    }
}


template <size_t I>
void converter_impl::convert_decl_impl(const ::clang::Decl * decl) {
    if constexpr (I == std::tuple_size_v<decl_converters>) {
        // matching converter not found
        CM_CLANG_LOG_SCAT_DECL_ERROR(decl, "unknown clang declaration", decl);
        // TODO: consider enabling assertion after all declaration converters implemented 
        //assert(false && "unknown clang declaration");
    } else {
        // trying current converter
        using converter_t = std::tuple_element_t<I, decl_converters>;
        if (auto conv_decl = ::clang::dyn_cast<typename converter_t::clang_decl_t>(decl)) {
            auto & conv = std::get<I>(decl_convs_);
            if (conv.match(conv_decl)) {
                conv.convert_decl(*this, conv_decl);
            }
        }

        // trying next converters
        convert_decl_impl<I + 1>(decl);
    }
}


type_t * converter_impl::get_template_spec_type(const ::clang::TemplateSpecializationType * type) {

    if (type->isTypeAlias()) {
        assert(false && "type aliases are not implemented yet");
        return nullptr;
    } else {
        // getting record declaration for template specialization or template declaration
        auto rec_decl = type->getAsCXXRecordDecl();
        if (rec_decl) {
            if (auto templ_decl = rec_decl->getDescribedClassTemplate()) {
                // template instantiation is just a template declaration being processed now
                auto rec = decls_.get_as<template_record>(rec_decl);
                return rec->this_type();
            } else {
                // std::cout << "TEMPLATE SPECIALIZATION TYPE: " << rec_decl << std::endl;
                // rec_decl->dump();

                // looking for existing CM entity associated with specialization record decl
                auto rec = decls_.get_as<template_record_instantiation_type>(rec_decl);
                assert(rec != nullptr &&
                       "no CM record associated with template specialization type");
                return rec;
            }
        } else {
            // template specialization without record declaration must be a dependent type
            assert(type->isDependentType() && "required dependent type here");

            // getting template declaration
            auto templ_decl = type->getTemplateName().getAsTemplateDecl();
            assert(templ_decl && "template delcaration is null for dependent template name");
            auto templ_rec_decl = templ_decl->getTemplatedDecl();
            assert(templ_rec_decl && "record delcaration is null for dependent template name");

            // getting code model template associated with template declaration
            auto templ = decls_.get_as<template_record>(templ_rec_decl);
            assert(templ != nullptr && "can't find CM template for tempalte decl");

            // converting template arguments
            auto args = convert_template_arguments(*this, type->template_arguments());

            // looking for existing instantiation
            if (auto inst = templ->find_dependent_instantiation(args)) {
                return inst;
            }

            // creating template dependent instantiation
            return templ->create_dependent_instantiation(args);
        }
    }
}


dependent_type * converter_impl::get_dependent_type(const ::clang::DependentNameType * clang_type) {
    return decl_ctx()->create_entity<dependent_type>();
}


decltype_type * converter_impl::get_decltype_type(const ::clang::DecltypeType * clang_type) {
    return decl_ctx()->create_entity<decltype_type>();
}


type_t * converter_impl::create_type(const ::clang::Type * type) {
    if (auto tspec = ::clang::dyn_cast<::clang::TemplateSpecializationType>(type)) {
        return get_template_spec_type(tspec);
    } else if (auto subst_tpar = ::clang::dyn_cast<::clang::SubstTemplateTypeParmType>(type)) {
        // TODO: do we need store this info in code model?
        return this->type(subst_tpar->getReplacementType()).type();
    } else if (auto dep_type = ::clang::dyn_cast<::clang::DependentNameType>(type)) {
        return get_dependent_type(dep_type);
    } else if (auto dt_type = ::clang::dyn_cast<::clang::DecltypeType>(type)) {
        return get_decltype_type(dt_type);
    } else {
        CM_CLANG_LOG_ERROR << "don't know how to create code model type for clang type:\n"
                           << dump_type_to_string(type, clang_ctx());
        assert(false && "Don't not know how to convert type");
        return {};
    }
}


const ::clang::TypeDecl * converter_impl::get_type_decl(const ::clang::Type * type) {
    if (auto clang_tag_type = ::clang::dyn_cast<::clang::TagType>(type)) {
        auto decl = clang_tag_type->getDecl();
        assert(decl && "clang tag type must have declaration");
        return decl;
    } else if (auto clang_td_type = ::clang::dyn_cast<::clang::TypedefType>(type)) {
        auto decl = clang_td_type->getDecl();
        assert(decl && "clang typedef type must have declaration");
        return decl;
    }

    return nullptr;
}


}
