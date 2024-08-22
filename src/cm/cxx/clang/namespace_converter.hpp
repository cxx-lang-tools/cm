// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file namespace_converter.hpp
/// Contains definition of the namespace_converter class.

#pragma once

#include "decl_converter.hpp"


namespace cm::cxx::clang {


/// Namespace converter
struct namespace_converter {
    /// Type of required clang node
    using clang_decl_t = ::clang::NamespaceDecl;

    /// Checks if clang decl matches converter
    static constexpr bool match(const clang_decl_t * decl) { return true; }

    /// Creates entity for clang namespace decl
    static namespace_ * create_entity(converter_impl & conv, const clang_decl_t * decl);

    /// Converts clang namespace decl
    static namespace_ * convert_decl(converter_impl & conv, const clang_decl_t * decl);
};


static_assert(decl_converter<namespace_converter>);


}
