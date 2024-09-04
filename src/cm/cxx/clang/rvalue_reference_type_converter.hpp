// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file rvalue_reference_type_converter.hpp
/// Contains definition of rvalue_reference_type_converter class

#pragma once

#include "type_converter.hpp"


namespace cm::cxx::clang {


/// Lvalue reference type converter
struct rvalue_reference_type_converter {
    /// Type of clang type
    using clang_type_t = ::clang::RValueReferenceType;

    /// Checks if clang type matches converter
    static constexpr bool match(const clang_type_t * type) { return true; }

    /// Gets existing or creates new code model type for clang type
    static rvalue_reference_type * type(converter_impl & conv, const clang_type_t * type) {
        auto base_type = conv.type(type->getPointeeType());
        return conv.mdl().get_or_create_rvalue_ref_type(base_type);
    }
};


static_assert(type_converter<rvalue_reference_type_converter>);


}
