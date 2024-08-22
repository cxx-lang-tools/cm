// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file linkage_spec_converter.cpp
/// Contains implementation of the namespace_converter class.

#include "pch.hpp"

#include "linkage_spec_converter.hpp"
#include "converter_impl.hpp"


namespace cm::cxx::clang {


context_entity * linkage_spec_converter::create_entity(converter_impl & conv,
                                                       const clang_decl_t * decl) {
    assert(false && "should not reach here");
    return nullptr;
}

context_entity * linkage_spec_converter::convert_decl(converter_impl & conv,
                                                      const clang_decl_t * decl) {

    // ignore linkage specification for now
    // TODO: implement in code model

    // converting nested declarations
    for (auto && nested_decl : decl->decls()) {
        conv.convert_decl(nested_decl);
    }

    return nullptr;
}


}
