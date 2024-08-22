// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file function_converter.hpp
/// Contains definition of the function_converter class.

#pragma once

#include "decl_converter.hpp"


namespace cm::cxx::clang {


/// Namespace converter
struct function_converter {
    /// Type of required clang node
    using clang_decl_t = ::clang::FunctionDecl;

    /// Checks if clang decl matches converter
    static bool match(const clang_decl_t * decl);

    /// Creates entity for clang namespace decl
    static function * create_entity(converter_impl & conv, const clang_decl_t * decl);

    /// Converts clang namespace decl
    static function * convert_decl(converter_impl & conv, const clang_decl_t * decl);
};


static_assert(decl_converter<function_converter>);


}
