// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file decl_map.hpp
/// Contains definition of the decl_map class.

#pragma once

#include "cm/cm.hpp"
#include <clang/AST/Decl.h>
#include <map>


namespace cm::cxx::clang {


/// Map between clang declarations and code model entities
class decl_map {
public:
    /// Finds code model entity associated with clang declaration.
    /// First gets canonical declaration of clang declaration.
    /// Returns nullptr if associated context_entity not found
    context_entity * get(const ::clang::Decl * clang_decl) const;

    /// Finds code model enitty associated with clang declaration and converts it to
    /// specified type. Found context_entity must be convertible to specified type.
    /// Returns nullptr if context_entity is not found.
    template <typename Entity>
    Entity * get_as(const ::clang::Decl * clang_decl) const {
        auto ent = get(clang_decl);
        if (!ent) {
            return nullptr;
        }

        auto casted_ent = dynamic_cast<Entity*>(ent);
        assert(casted_ent && "existing context_entity type mismatch");
        return casted_ent;
    }

    /// Adds code model context entity associated with clang declaration.
    void add(const ::clang::Decl * clang_decl, context_entity * cm_ent);

private:
    /// Map from clang canonical declarations to code model entities
    std::map<const ::clang::Decl *, context_entity *> decls_;
};


}
