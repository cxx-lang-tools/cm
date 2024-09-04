// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file lvalue_reference_type_converter.hpp
/// Contains definition of lvalue_reference_type_converter class

#pragma once

#include "type_converter.hpp"


namespace cm::cxx::clang {


/// Lvalue reference type converter
struct lvalue_reference_type_converter {
    /// Type of clang type
    using clang_type_t = ::clang::LValueReferenceType;

    /// Checks if clang type matches converter
    static constexpr bool match(const clang_type_t * type) { return true; }

    /// Gets existing or creates new code model type for clang type
    static lvalue_reference_type * type(converter_impl & conv, const clang_type_t * type) {
        auto base_type = conv.type(type->getPointeeType());
        return conv.mdl().get_or_create_lvalue_ref_type(base_type);
    }
};


static_assert(type_converter<lvalue_reference_type_converter>);


}
