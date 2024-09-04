// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file array_type_converter.hpp
/// Contains definition of array_type_converter class

#pragma once

#include "type_converter.hpp"


namespace cm::cxx::clang {


/// Array type converter
struct array_type_converter {
    /// Type of clang type
    using clang_type_t = ::clang::ConstantArrayType;

    /// Checks if clang type matches converter
    static constexpr bool match(const clang_type_t * type) { return true; }

    /// Gets existing or creates new code model type for clang type
    static array_type * type(converter_impl & conv, const clang_type_t * type) {
        auto elt_type = conv.type(type->getElementType());
        return conv.mdl().get_or_create_arr_type(elt_type.type(),
                                                 type->getSize().getLimitedValue());
    }
};


static_assert(type_converter<array_type_converter>);


}
