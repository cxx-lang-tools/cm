// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file template_function_converter.cpp
/// Contains implementation of template_function_converter class.

#include "pch.hpp"

#include "template_function_converter.hpp"
#include "converter_impl.hpp"
#include "convert_function.hpp"
#include "convert_loc.hpp"
#include "convert_record.hpp"
#include "convert_template.hpp"


namespace cm::cxx::clang {


template_function * template_function_converter::create_entity(converter_impl & conv,
                                                               const clang_decl_t * decl) {
    auto parent = conv.parent_ctx(decl);

    auto func_decl = decl->getTemplatedDecl();

    // creating new function or method
    // TOOD: move method converting code to separater converter
    template_function * func = nullptr;
    auto nm = func_decl->getNameAsString();
    if (auto method_decl = ::clang::dyn_cast<::clang::CXXMethodDecl>(func_decl);
        method_decl != nullptr && !method_decl->isStatic())
    {
        // context must be a record
        auto rec = dynamic_cast<record*>(parent);
        assert(rec && "context must be a record for method declaration");

        func = rec->create_template_method(nm);
    } else {
        func = parent->create_template_function(nm);
    }

    // adding additional map for described function declaration to make sure it's possible
    // to find function entity from FunctionDecl 
    conv.add_decl_entity(decl->getTemplatedDecl(), func);

    return func;
}

template_function * template_function_converter::convert_decl(converter_impl & conv,
                                                              const clang_decl_t * decl) {
    auto func = conv.decl_entity_as<template_function>(decl);

    // performing conversion only for definition or canonical declaration if there is
    // no definition
    auto func_decl = decl->getTemplatedDecl();
    auto def_decl = func_decl->getDefinition();
    if (def_decl != nullptr && def_decl != func_decl ||
        def_decl == nullptr && !decl->isCanonicalDecl()) {
        return func;
    }

    func->set_access_lev(get_clang_decl_acc_level(decl));
    func->set_loc(convert_loc(conv, decl->getLocation()));

    // setting function as current decl context
    converter_impl::decl_context_setter csetter{conv, func, func_decl};

    // converting template parameters
    convert_template_params(conv, func, decl->getTemplateParameters());

    // converting function return type and parameters
    convert_function_ret_type_and_params(conv, func, func_decl);

    // converting template specializations, using canonical declaration to get specializations
    auto canon_decl = decl->getCanonicalDecl();
    for (auto && spec : canon_decl->specializations()) {
        convert_template_function_inst(conv, func, spec);
    }

    return func;
}


}
