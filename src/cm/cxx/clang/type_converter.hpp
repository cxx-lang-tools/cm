// Copyright (c) 2024, Alexandr Esilevich
//
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file type_converter.hpp
/// Contains declaration of the type_converter class.

#pragma once

#include "decl_map.hpp"
#include "cm/cm.hpp"
#include <clang/AST/TypeLoc.h>


namespace cm::cxx::clang {


class converter_impl;


/// Clang type converter. Maps clang types to code model types.
class type_converter {
public:
    /// Constructs type converter
    explicit type_converter(decl_map & decls, converter_impl & conv):
        decls_{decls}, conv_{conv} {}

    /// Gets existing or creates new code model type for clang builtin type.
    /// NOTE: return type is not always code model builting type in case of objc types.
    type_t * builtin(const ::clang::BuiltinType * clang_bt_type);

    /// Gets existing or creates new code model type for clang pointer type
    pointer_type * pointer(const ::clang::PointerType * clang_ptr_type);

    /// Gets existing or creates new code model type for clang lvalue reference type.
    lvalue_reference_type * lvalue_reference(const ::clang::LValueReferenceType * clang_ref_type);

    /// Gets existing or creates new code model type for clang rvalue reference type.
    rvalue_reference_type * rvalue_reference(const ::clang::RValueReferenceType * clang_ref_type);

    /// Gets existing or creates new code model type for clang array type
    array_type * array(const ::clang::ConstantArrayType * clang_arr_type);

    /// Gets existing or creates new code model type for clang template parameter type
    type_template_parameter *
    template_parameter(const ::clang::TemplateTypeParmType * clang_tpar_type);

    /// Gets existing or creates new code model type for clang template specialization type
    type_t * template_spec(const ::clang::TemplateSpecializationType * clang_templ_spec);

    /// Gets existing or creates new code model type for clang function type
    function_type * function( const ::clang::FunctionType * clang_func_type);

    /// Gets existing or creates new code model type for clang record type
    record_type * record(const ::clang::RecordType * clang_rec_type);

    /// Gets existing or creates new code model type for clang dependent type
    dependent_type * dependent(const ::clang::DependentNameType * clang_type);

    /// Gets existing or creates new code model type for clang decltype type
    decltype_type * decltype_(const ::clang::DecltypeType * clang_type);

    /// Gets existing or creates new code model type for clang typedef type
    typedef_type * typedef_(const ::clang::TypedefType * clang_type);

    /// Gets existing or creates new code model type for clang type.
    qual_type type(const ::clang::Type * clang_type);

    /// Gets existing or creates new code model qual type for clang qual type.
    qual_type type(const ::clang::QualType & clang_type);


private:
    /// Creates code model type for clang type
    type_t * create(const ::clang::Type * clang_type);


    decl_map & decls_;          ///< Declarations map
    converter_impl & conv_;     ///< Reference to converter

    /// Map from clang types for code model types
    std::map<const ::clang::Type *, type_t *> types_;
};


}
