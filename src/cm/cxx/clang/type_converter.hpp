// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file type_converter.hpp
/// Contains definition of the type_converter concept

#pragma once

#include "converter_impl.hpp"
#include <clang/AST/Type.h>


namespace cm::cxx::clang {

class converter_impl;


/// Single clang type converter
template <typename Converter>
concept type_converter =
std::is_default_constructible_v<Converter> &&
requires (Converter & conv, converter_impl & dconv) {
    /// Required clang type
    typename Converter::clang_type_t;
    requires std::derived_from<typename Converter::clang_type_t, ::clang::Type>;

    /// Checks if converter matches clang type
    { conv.match(std::declval<const typename Converter::clang_type_t *>()) } ->
        std::convertible_to<bool>;

    /// Returns existing or creates new code model type for clang type
    { conv.type(dconv, std::declval<const typename Converter::clang_type_t *>()) } ->
        std::convertible_to<type_t *>;
};


}
