// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file function_type_converter.hpp
/// Contains definition of pointer_type_converter class

#pragma once

#include "type_converter.hpp"


namespace cm::cxx::clang {


/// Function type converter
struct function_type_converter {
    /// Type of clang type
    using clang_type_t = ::clang::FunctionType;

    /// Checks if clang type matches converter
    static constexpr bool match(const clang_type_t * type) { return true; }

    /// Gets existing or creates new code model type for clang type
    static function_type * type(converter_impl & conv, const clang_type_t * type);
};


static_assert(type_converter<function_type_converter>);


}
