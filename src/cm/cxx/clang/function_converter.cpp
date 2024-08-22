// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file function_converter.cpp
/// Contains implementation of function_converter class.

#include "pch.hpp"

#include "function_converter.hpp"
#include "converter_impl.hpp"
#include "convert_function.hpp"
#include "convert_loc.hpp"
#include "convert_record.hpp"


namespace cm::cxx::clang {


bool function_converter::match(const clang_decl_t * decl) {
    // skipping template functions
    if (decl->getDescribedTemplate() != nullptr) {
        return false;
    }

    return true;
}


function * function_converter::create_entity(converter_impl & conv, const clang_decl_t * decl) {
    auto parent = conv.parent_ctx(decl);

    // creating new function or method
    // TODO: move method converting code to separate converter 
    auto nm = decl->getNameAsString();
    if (auto method_decl = ::clang::dyn_cast<::clang::CXXMethodDecl>(decl);
       method_decl != nullptr && !method_decl->isStatic())
    {
        // context must be a record
        auto rec = dynamic_cast<record*>(parent);
        assert(rec && "context must be a record for method declaration");

        return rec->create_method(nm);
    } else {
        return parent->create_function(nm);
    }

}

function * function_converter::convert_decl(converter_impl & conv, const clang_decl_t * decl) {
    auto func = conv.decl_entity_as<function>(decl);

    // performing conversion only for definition or canonical declaration if there is
    // no definition
    auto def_decl = decl->getDefinition();
    if (def_decl != nullptr && def_decl != decl ||
        def_decl == nullptr && !decl->isCanonicalDecl()) {
        return func;
    }

    func->set_access_lev(get_clang_decl_acc_level(decl));
    func->set_loc(convert_loc(conv, decl->getLocation()));

    // setting function as current decl context
    converter_impl::decl_context_setter csetter{conv, func, decl};

    // converting function return type and parameters
    convert_function_ret_type_and_params(conv, func, decl);

    return func;
}


}
