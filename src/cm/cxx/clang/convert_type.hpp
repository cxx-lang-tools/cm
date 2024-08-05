// Copyright (c) 2024, Alexandr Esilevich
//
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file convert_type.hpp
/// Contains declarations of functions for converting clang types

#include "cm/cxx/clang/conv_context.hpp"
#include "cm/cm.hpp"
#include <clang/AST/TypeLoc.h>

namespace cm::cxx::clang {


/// Gets existing or creates new code model type for clang builtin type.
/// NOTE: return type is not always code model builting type in case of objc types.
type_t * get_or_create_builtin_type(conv_context & ctx, const ::clang::BuiltinType * clang_bt_type);

/// Gets existing or creates new code model type for clang pointer type
pointer_type * get_or_create_pointer_type(conv_context & ctx,
                                          const ::clang::PointerType * clang_ptr_type);

/// Gets existing or creates new code model type for clang lvalue reference type.
lvalue_reference_type *
get_or_create_lvalue_reference_type(conv_context & ctx,
                                    const ::clang::LValueReferenceType * clang_ref_type);

/// Gets existing or creates new code model type for clang rvalue reference type.
rvalue_reference_type *
get_or_create_rvalue_reference_type(conv_context & ctx,
                                    const ::clang::RValueReferenceType * clang_ref_type);

/// Gets existing or creates new code model type for clang array type
array_type * get_or_create_array_type(conv_context & ctx,
                                      const ::clang::ConstantArrayType * clang_arr_type);

/// Gets existing or creates new code model type for clang template parameter type
type_template_parameter *
get_or_create_type_template_param_type(conv_context & ctx,
                                       const ::clang::TemplateTypeParmType * clang_tpar_type);

/// Gets existing or creates new code model type for clang template specialization type
type_t *
get_or_create_template_spec_type(conv_context & ctx,
                                 const ::clang::TemplateSpecializationType * clang_templ_spec);

/// Gets existing or creates new code model type for clang function type
function_type * get_or_create_function_type(conv_context & ctx,
                                            const ::clang::FunctionType * clang_func_type);

/// Gets existing or creates new code model type for clang record type
record_type * get_or_create_record_type(conv_context & ctx,
                                        const ::clang::RecordType * clang_rec_type);

/// Gets existing or creates new code model type for clang dependent type
dependent_type * get_or_create_dependent_type(conv_context & ctx,
                                              const ::clang::DependentNameType * clang_type);

/// Gets existing or creates new code model type for clang decltype type
decltype_type * get_or_create_decltype_type(conv_context & ctx,
                                            const ::clang::DecltypeType * clang_type);

/// Gets existing or creates new code model type for clang typedef type
typedef_type * get_or_create_typedef_type(conv_context & ctx,
                                          const ::clang::TypedefType * clang_type);

/// Gets existing or creates new code model type for clang type.
qual_type get_or_create_type(conv_context & ctx, const ::clang::Type * clang_type);

/// Gets existing or creates new code model qual type for clang qual type.
qual_type get_or_create_type(conv_context & ctx, const ::clang::QualType & clang_type);


}
