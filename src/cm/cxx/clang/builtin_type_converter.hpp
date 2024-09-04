// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file builtin_type_converter.hpp
/// Contains definition of builtin_type_converter class

#pragma once

#include "type_converter.hpp"


namespace cm::cxx::clang {


/// Builtin type converter
struct builtin_type_converter {
    /// Type of clang type
    using clang_type_t = ::clang::BuiltinType;

    /// Checks if clang type matches converter
    static constexpr bool match(const clang_type_t * type) { return true; }

    /// Gets existing or creates new code model type for clang type
    static type_t * type(converter_impl & conv, const clang_type_t * type);
};


static_assert(type_converter<builtin_type_converter>);


}
