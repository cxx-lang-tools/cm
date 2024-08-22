// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file variable_converter.hpp
/// Contains definition of the variable_converter class.

#pragma once

#include "decl_converter.hpp"


namespace cm::cxx::clang {


/// Namespace converter
struct variable_converter {
    /// Type of required clang node
    using clang_decl_t = ::clang::VarDecl;

    /// Checks if clang decl matches converter
    static constexpr bool match(const clang_decl_t * decl) { return true; }

    /// Creates entity for clang namespace decl
    static variable * create_entity(converter_impl & conv, const clang_decl_t * decl);

    /// Converts clang namespace decl
    static variable * convert_decl(converter_impl & conv, const clang_decl_t * decl);
};


static_assert(decl_converter<variable_converter>);


}
