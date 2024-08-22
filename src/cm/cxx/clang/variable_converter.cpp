// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file variable_converter.cpp
/// Contains implementation of variable_converter class.

#include "pch.hpp"

#include "variable_converter.hpp"
#include "converter_impl.hpp"
#include "convert_loc.hpp"
#include "convert_record.hpp"


namespace cm::cxx::clang {


variable * variable_converter::create_entity(converter_impl & conv, const clang_decl_t * decl) {
    auto parent = conv.parent_ctx(decl);

    // converting variable type
    // TODO: should we move type conversion to convert_decl?
    auto var_type = conv.types().type(decl->getType());

    // creating new variable
    return parent->create_var(decl->getNameAsString(), var_type);
}

variable * variable_converter::convert_decl(converter_impl & conv, const clang_decl_t * decl) {
    auto var = conv.decl_entity_as<variable>(decl);

    // performing conversion only for definition or canonical declaration if there is
    // no definition
    auto def_decl = decl->getDefinition();
    if (def_decl != nullptr && def_decl != decl ||
        def_decl == nullptr && !decl->isCanonicalDecl()) {
        return var;
    }

    var->set_access_lev(get_clang_decl_acc_level(decl));
    var->set_loc(convert_loc(conv, decl->getLocation()));

    return var;
}


}
