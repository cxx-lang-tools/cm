// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file template_function_converter.hpp
/// Contains definition of the template_function_converter class.

#pragma once

#include "decl_converter.hpp"
#include <clang/AST/DeclTemplate.h>


namespace cm::cxx::clang {


/// Namespace converter
struct template_function_converter {
    /// Type of required clang node
    using clang_decl_t = ::clang::FunctionTemplateDecl;

    /// Checks if clang decl matches converter
    static constexpr bool match(const clang_decl_t * decl) { return true; }

    /// Creates entity for clang namespace decl
    static template_function * create_entity(converter_impl & conv, const clang_decl_t * decl);

    /// Converts clang namespace decl
    static template_function * convert_decl(converter_impl & conv, const clang_decl_t * decl);
};


static_assert(decl_converter<template_function_converter>);


}
