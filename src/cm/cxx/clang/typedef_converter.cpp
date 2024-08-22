// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file typedef_converter.cpp
/// Contains implementation of the typedef_converter class

#include "pch.hpp"

#include "typedef_converter.hpp"
#include "converter_impl.hpp"
#include "convert_record.hpp"
#include "convert_loc.hpp"


namespace cm::cxx::clang {


typedef_type * typedef_converter::create_entity(converter_impl & conv,
                                                const clang_decl_t * decl) {
    return conv.parent_ctx(decl)->create_typedef(decl->getNameAsString());
}

typedef_type * typedef_converter::convert_decl(converter_impl & conv,
                                               const clang_decl_t * decl) {
    // getting existing or creating new typedef entity
    auto td = conv.decl_entity_as<typedef_type>(decl);
    assert(td && "typedef entity must not be null");

    // skipping non canonical declarations
    if (!decl->isCanonicalDecl()) {
        return td;
    }

    // getting or creating entity for base type
    auto clang_base_type = decl->getUnderlyingType();
    auto base_type = conv.types().type(clang_base_type);
    td->set_base(base_type);

    // converting location and access level
    auto nm = decl->getNameAsString();
    td->set_access_lev(get_clang_decl_acc_level(decl));
    td->set_loc(convert_loc(conv, decl->getLocation()));

    return td;
}


}
