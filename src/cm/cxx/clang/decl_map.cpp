// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file decl_map.cpp
/// Contains implementation of the decl_map class.

#include "pch.hpp"

#include "cm/cxx/clang/decl_map.hpp"


namespace cm::cxx::clang {


context_entity * decl_map::get(const ::clang::Decl * clang_decl) const {
    auto canon_decl = clang_decl->getCanonicalDecl();

    auto it = decls_.find(canon_decl);
    if (it == decls_.end()) {
        return nullptr;
    }

    return it->second;
}


void decl_map::add(const ::clang::Decl * clang_decl, context_entity * cm_ent) {
    auto canon_decl = clang_decl->getCanonicalDecl();
    auto [it, inserted] = decls_.emplace(canon_decl, cm_ent);
    assert(inserted &&
            "code model context_entity is already associated with clang declaration");
}


}
