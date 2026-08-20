// Copyright (c) 2024, Alexandr Esilevich
//
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file template_record_specialization_converter.cpp
/// Contains implementation of the template_record_specialization_converter class.

#include "pch.hpp"

#include "template_record_specialization_converter.hpp"
#include "converter_impl.hpp"
#include "convert_loc.hpp"
#include "convert_record.hpp"
#include "convert_template.hpp"


namespace cm::cxx::clang {


bool template_record_specialization_converter::match(const clang_decl_t * decl) {
    // skipping template record partial specializations
    return ::clang::dyn_cast<::clang::ClassTemplatePartialSpecializationDecl>(decl) == nullptr;
}


record * template_record_specialization_converter::create_entity(converter_impl & conv,
                                                                 const clang_decl_t * decl) {
    // getting existing or creating new code model entity for template record
    auto templ_decl = decl->getSpecializedTemplate();
    assert(templ_decl && "can't get clang template decl for specialization");
    auto templ = conv.decl_entity_as<template_record>(templ_decl);

    // creating new template record instantiation or specialization
    auto args = convert_template_arguments(conv, decl->getTemplateArgs().asArray());
    record * rec = decl->isExplicitSpecialization() ?
                   static_cast<record*>(templ->create_specialization(args)) :
                   static_cast<record*>(templ->create_instantiation(args));

    rec->set_loc(convert_loc(conv, templ_decl->getLocation()));

    return rec;
}


record * template_record_specialization_converter::convert_decl(converter_impl & conv,
                                                                const clang_decl_t * decl) {
    // getting existing or creating new entity for template record specialization
    record * rec = conv.decl_entity_as<template_record_instantiation_type>(decl);

    // filling record contents
    fill_record_contents(conv, rec, decl);

    return rec;
}


}
