// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file template_record_partial_specialization_converter.cpp
/// Contains implementation of the template_record_partial_specialization_converter class.

#include "pch.hpp"

#include "template_record_partial_specialization_converter.hpp"
#include "converter_impl.hpp"
#include "convert_record.hpp"
#include "convert_template.hpp"


namespace cm::cxx::clang {


template_record_partial_specialization *
template_record_partial_specialization_converter::create_entity(
        converter_impl & conv,
        const clang_decl_t * decl) {

    // getting existing or creating new code model entity for tempalte record
    auto templ_decl = decl->getSpecializedTemplate();
    assert(templ_decl && "can't get clang template decl for specialization");
    auto templ = conv.decl_entity_as<template_record>(templ_decl);

    return templ->create_partial_specialization();
}


template_record_partial_specialization *
template_record_partial_specialization_converter::convert_decl(
        converter_impl & conv,
        const clang_decl_t * decl) {

    // getting existing or creating new entity for partial specialization
    auto spec = conv.decl_entity_as<template_record_partial_specialization>(decl);

    // converting specialization only for definition of canonical declaration if there is
    // no definition
    if (!(decl->isCompleteDefinition() ||
          !decl->hasDefinition() && decl->isCanonicalDecl())) {
        return spec;
    }

    // setting context to template specialization
    converter_impl::decl_context_setter csetter{conv, spec, decl};

    // converting template parameters
    convert_template_params(conv, spec, decl->getTemplateParameters());

    // converting template arguments
    auto args = convert_template_arguments(conv, decl->getTemplateArgs().asArray());
    for (auto && arg : args) {
        spec->add_arg(arg);
    }

    // filling template record contents
    fill_record_contents(conv, spec, decl);

    return spec;
}


}
