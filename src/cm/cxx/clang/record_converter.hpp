// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file record_converter.hpp
/// Contains definition of record_converter class

#pragma once

#include "decl_converter.hpp"
#include <clang/AST/DeclCXX.h>

namespace cm::cxx::clang {


/// Record converter
struct record_converter {
    /// Type of required clang node
    using clang_decl_t = ::clang::CXXRecordDecl;

    /// Checks if clang decl matches converter
    static bool match(const clang_decl_t * decl);

    /// Creates entity for clang namespace decl
    static record_type * create_entity(converter_impl & conv, const clang_decl_t * decl);

    /// Converts clang namespace decl
    static record_type * convert_decl(converter_impl & conv, const clang_decl_t * decl);
};


static_assert(decl_converter<record_converter>);


}
