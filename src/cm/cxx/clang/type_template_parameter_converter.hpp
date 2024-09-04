// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file type_template_parameter_converter.hpp
/// Contains definition of type_template_parameter_converter class

#pragma once

#include "type_converter.hpp"


namespace cm::cxx::clang {


/// Type template parameter converter
struct type_template_parameter_converter {
    /// Type of clang type
    using clang_type_t = ::clang::TemplateTypeParmType;

    /// Checks if clang type matches converter
    static constexpr bool match(const clang_type_t * type) { return true; }

    /// Gets existing or creates new code model type for clang type
    static type_template_parameter * type(converter_impl & conv, const clang_type_t * type);
};


static_assert(type_converter<type_template_parameter_converter>);


}
