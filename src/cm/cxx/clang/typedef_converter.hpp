// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file typedef_converter.hpp
/// Contains definition of typedef_converter class

#pragma once

#include "decl_converter.hpp"
#include <clang/AST/DeclCXX.h>

namespace cm::cxx::clang {


/// Typedef converter
struct typedef_converter {
    /// Type of required clang node
    using clang_decl_t = ::clang::TypedefNameDecl;

    /// Checks if clang decl matches converter
    static constexpr bool match(const clang_decl_t * decl) { return true; }

    /// Creates entity for clang namespace decl
    static typedef_type * create_entity(converter_impl & conv, const clang_decl_t * decl);

    /// Converts clang namespace decl
    static typedef_type * convert_decl(converter_impl & conv, const clang_decl_t * decl);
};


static_assert(decl_converter<typedef_converter>);


}
