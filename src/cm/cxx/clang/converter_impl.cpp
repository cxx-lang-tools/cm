// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file converter_impl.cpp
/// Contains implementation of the converter_impl class.

#include "pch.hpp"

#include "converter_impl.hpp"
#include "log.hpp"


namespace cm::cxx::clang {


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


context_entity * converter_impl::convert_decl(const ::clang::Decl * decl) {
    return convert_decl_impl<0>(decl);
}


context_entity * converter_impl::create_decl_entity(const ::clang::Decl * decl) {
    return create_decl_entity_impl<0>(decl);
}


template <size_t I>
context_entity * converter_impl::create_decl_entity_impl(const ::clang::Decl * decl) {
    if constexpr (I == std::tuple_size_v<converters_tuple>) {
        // matching converter not found
        // TODO
        // CM_CLANG_LOG_SCAT_DECL_ERROR(decl, "unknown clang declaration", decl);
        // assert(false && "unknown clang declaration");
        return nullptr;
    } else {
        // trying current converter
        using converter_t = std::tuple_element_t<I, converters_tuple>;
        if (auto conv_decl = ::clang::dyn_cast<typename converter_t::clang_decl_t>(decl)) {
            auto & conv = std::get<I>(converters_);
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
context_entity * converter_impl::convert_decl_impl(const ::clang::Decl * decl) {
    if constexpr (I == std::tuple_size_v<converters_tuple>) {
        // matching converter not found
        // TODO
        // CM_CLANG_LOG_SCAT_DECL_ERROR(decl, "unknown clang declaration", decl);
        // assert(false && "unknown clang declaration");
        return nullptr;
    } else {
        // trying current converter
        using converter_t = std::tuple_element_t<I, converters_tuple>;
        if (auto conv_decl = ::clang::dyn_cast<typename converter_t::clang_decl_t>(decl)) {
            auto & conv = std::get<I>(converters_);
            if (conv.match(conv_decl)) {
                auto res = conv.convert_decl(*this, conv_decl);
                assert(res != nullptr && "converter create_entity returned nullptr");
                return res;
            }
        }

        // trying next converters
        return convert_decl_impl<I + 1>(decl);
    }
}


}
