// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file template_specialization_type_converter.hpp
/// Contains definition of template_specialization_type_converter class

#pragma once

#include "type_converter.hpp"


namespace cm::cxx::clang {


/// Template specialization type converter
struct template_specialization_type_converter {
    /// Type of clang type
    using clang_type_t = ::clang::TemplateSpecializationType;

    /// Checks if clang type matches converter
    static constexpr bool match(const clang_type_t * type) { return true; }

    /// Gets existing or creates new code model type for clang type
    static type_t * type(converter_impl & conv, const clang_type_t * type);
};


static_assert(type_converter<template_specialization_type_converter>);


}
