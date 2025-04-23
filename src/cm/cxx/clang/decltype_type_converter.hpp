
/// \file decltype_type_converter.hpp
/// Contains definition of the decltype_type_converter type.

#pragma once

#include "type_converter.hpp"
#include <clang/AST/DeclCXX.h>


namespace cm::cxx::clang {


/// decltype type converter
struct decltype_type_converter {
    /// Type of clang type
    using clang_type_t = ::clang::DecltypeType;

    /// Checks if clang type matches converter
    static constexpr bool match(const clang_type_t * type) { return true; }

    /// Gets existing or creates new code model type for clang type
    static decltype_type * type(converter_impl & conv, const clang_type_t * type);
};


static_assert(type_converter<decltype_type_converter>);


}
