// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file decl_converter.hpp
/// Contains definition of the decl_converter concept

#pragma once

#include <clang/AST/Decl.h>


namespace cm::cxx::clang {

class converter_impl;


/// Single clang declaration converter
template <typename Converter>
concept decl_converter =
std::is_default_constructible_v<Converter> &&
requires (Converter & conv, converter_impl & dconv) {
    /// Required clang AST node type
    typename Converter::clang_decl_t;
    requires std::derived_from<typename Converter::clang_decl_t, ::clang::Decl>;

    /// Checks if converter matches clang declaration
    { conv.match(std::declval<const typename Converter::clang_decl_t *>()) } ->
        std::convertible_to<bool>;

    /// Creates code model entity for clang declaration
    { conv.create_entity(dconv, std::declval<const typename Converter::clang_decl_t *>()) } ->
        std::convertible_to<context_entity *>;

    /// Converts declaration to code model entity. Creates entity if does not exist
    { conv.convert_decl(dconv, std::declval<const typename Converter::clang_decl_t *>()) } ->
        std::convertible_to<context_entity *>;
};


}
