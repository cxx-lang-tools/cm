// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file template_specialization_type_converter.cpp
/// Contains implementation of the template_specialization_type_converter class.

#include "pch.hpp"

#include "convert_template.hpp"
#include "template_specialization_type_converter.hpp"
#include "log.hpp"


namespace cm::cxx::clang {


type_t *
template_specialization_type_converter::type(converter_impl & conv, const clang_type_t * type) {
    if (type->isTypeAlias()) {
        assert(false && "type aliases are not implemented yet");
        return nullptr;
    } else {
        // getting record declaration for template specialization or template declaration
        auto rec_decl = type->getAsCXXRecordDecl();
        if (rec_decl) {
            if (auto templ_decl = rec_decl->getDescribedClassTemplate()) {
                // template instantiation is just a template declaration being processed now
                auto rec = conv.get_decl_entity_as<template_record>(rec_decl);
                return rec->this_type();
            } else {
                // std::cout << "TEMPLATE SPECIALIZATION TYPE: " << rec_decl << std::endl;
                // rec_decl->dump();

                // looking for existing CM entity associated with specialization record decl
                auto rec = conv.get_decl_entity_as<template_record_instantiation_type>(rec_decl);
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
            auto templ = conv.get_decl_entity_as<template_record>(templ_rec_decl);
            assert(templ != nullptr && "can't find CM template for tempalte decl");

            // converting template arguments
            auto args = convert_template_arguments(conv, type->template_arguments());

            // looking for existing instantiation
            if (auto inst = templ->find_dependent_instantiation(args)) {
                return inst;
            }

            // creating template dependent instantiation
            return templ->create_dependent_instantiation(args);
        }
    }
}


}
