// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file template_record_converter.cpp
/// Contains implementation of the template_record_converter class

#include "pch.hpp"

#include "template_record_converter.hpp"
#include "converter_impl.hpp"
#include "convert_loc.hpp"
#include "convert_record.hpp"
#include "convert_template.hpp"


namespace cm::cxx::clang {


template_record * template_record_converter::create_entity(converter_impl & conv,
                                                           const clang_decl_t * decl) {
    auto parent_ctx = conv.parent_ctx(decl);
    auto rec_decl = decl->getTemplatedDecl();
    auto knd = clang_tag_kind_to_record_kind(rec_decl->getTagKind());
    auto rec = parent_ctx->create_template_record(decl->getNameAsString(), knd);

    // adding additional map for described record declaration to make sure it's possible
    // to find record entity from CXXRecordDecl 
    conv.add_decl_entity(decl->getTemplatedDecl(), rec);

    return rec;
}

template_record * template_record_converter::convert_decl(converter_impl & conv,
                                                          const clang_decl_t * decl) {
    // looking for existing CM declaration associated with clang template declaration
    auto rec = conv.decl_entity_as<template_record>(decl);

    auto rec_decl = decl->getTemplatedDecl();

    rec->set_loc(convert_loc(conv, decl->getLocation()));
    rec->this_type()->set_loc(rec->loc());

    // setting new context before converting tempalte parameters
    converter_impl::decl_context_setter csetter{conv, rec, rec_decl};

    // converting template parameters only for definition or for canonical declaration
    // if there is no definition
    if (rec_decl->isCompleteDefinition() ||
        !rec_decl->hasDefinition() && rec_decl->isCanonicalDecl()) {

        auto clang_templ_pars = decl->getTemplateParameters();
        convert_template_params(conv, rec, clang_templ_pars);
    }

    // filling template record contents
    fill_record_contents(conv, rec, rec_decl);

    // converting template specializations only for canonical decl
    // (canonical decl is only one in all translation unit)
    if (decl->isCanonicalDecl()) {
        for (auto && spec : decl->specializations()) {
            convert_template_class_spec(conv, rec, spec);
        }
    }

    return rec;
}


}
