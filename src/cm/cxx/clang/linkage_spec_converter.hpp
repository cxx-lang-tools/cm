// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file linkage_spec_converter.hpp
/// Contains definition of the linkage_spec_converter class.

#pragma once

#include "decl_converter.hpp"
#include <clang/AST/DeclCXX.h>


namespace cm::cxx::clang {


/// Converter for linkage spec declarations
struct linkage_spec_converter {
    /// Type of required clang node
    using clang_decl_t = ::clang::LinkageSpecDecl;

    /// Checks if clang decl matches converter
    static constexpr bool match(const clang_decl_t * decl) { return true; }

    /// Should not be called, asserts false.
    static context_entity * create_entity(converter_impl & conv, const clang_decl_t * decl);

    /// Converts linkage spec declaration
    static context_entity * convert_decl(converter_impl & conv, const clang_decl_t * decl);
};


static_assert(decl_converter<linkage_spec_converter>);


}
