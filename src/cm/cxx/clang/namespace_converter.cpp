// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file namespace_converter.cpp
/// Contains implementation of the namespace_converter class.

#include "pch.hpp"

#include "namespace_converter.hpp"
#include "converter_impl.hpp"


namespace cm::cxx::clang {


namespace_ * namespace_converter::create_entity(converter_impl & conv, const clang_decl_t * decl) {
    auto parent = conv.parent_ctx(decl);
    auto parent_ns = dynamic_cast<namespace_*>(parent);
    assert(parent_ns && "parent context for namespace is not a namespace");

    if (decl->getName().empty()) {
        return parent_ns->create_anon_namespace();
    } else {
        return parent_ns->create_namespace(decl->getNameAsString());
    }
}

namespace_ * namespace_converter::convert_decl(converter_impl & conv, const clang_decl_t * decl) {
    auto ns = conv.decl_entity_as<namespace_>(decl);

    // converting top level declarations in namespace
    for (auto && nested_decl : decl->decls()) {
        conv.convert_decl(nested_decl);
    }

    return ns;
}


}
