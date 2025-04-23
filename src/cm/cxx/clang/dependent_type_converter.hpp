
/// \file dependent_type_converter.hpp
/// Contains definition of the dependent_type_converter type.

#pragma once

#include "type_converter.hpp"
#include <clang/AST/DeclCXX.h>


namespace cm::cxx::clang {


/// Dependent type converter
struct dependent_type_converter {
    /// Type of clang type
    using clang_type_t = ::clang::DependentNameType;

    /// Checks if clang type matches converter
    static constexpr bool match(const clang_type_t * type) { return true; }

    /// Gets existing or creates new code model type for clang type
    static dependent_type * type(converter_impl & conv, const clang_type_t * type);
};


static_assert(type_converter<dependent_type_converter>);


}
