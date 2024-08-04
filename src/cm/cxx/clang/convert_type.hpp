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


/// Converts clang qual type to code model type. Creates composite types if needed.
qual_type convert_type(conv_context & ctx, const ::clang::QualType & clang_type);

/// Converts bultin clang type to code model type.
type_t * convert_builtin_type(conv_context & ctx, const ::clang::BuiltinType * clang_bt_type);

/// Converts pointer clang type to code model type.
pointer_type * convert_pointer_type(conv_context & ctx,
                                    const ::clang::PointerType * clang_ptr_type);

/// Converts lvalue reference clang type to code model type.
lvalue_reference_type *
convert_lvalue_reference_type(conv_context & ctx,
                              const ::clang::LValueReferenceType * clang_ref_type);

/// Converts rvalue reference clang type to code model type.
rvalue_reference_type *
convert_rvalue_reference_type(conv_context & ctx,
                              const ::clang::RValueReferenceType * clang_ref_type);

/// Converts array clang type to code model type.
array_type * convert_array_type(conv_context & ctx,
                                const ::clang::ConstantArrayType * clang_arr_type);

/// Converts type template parameter type to code model type
type_template_parameter *
convert_type_template_param_type(conv_context & ctx,
                                 const ::clang::TemplateTypeParmType * clang_tpar_type);

/// Converts template specialization type to code model type
type_t * convert_template_spec_type(conv_context & ctx,
                                    const ::clang::TemplateSpecializationType * clang_templ_spec);

/// Converts clang function type to code model type. Creates new composite types if needed.
function_type * convert_function_type(conv_context & ctx,
                                      const ::clang::FunctionType * clang_func_type);

/// Converts clang record type to code model type. Creates new empty record if needed.
record_type * convert_record_type(conv_context & ctx, const ::clang::RecordType * clang_rec_type);

/// Converts dependent type
dependent_type * convert_dependent_type(conv_context & ctx,
                                        const ::clang::DependentNameType * clang_type);

/// Converts decltype type
decltype_type * convert_decltype_type(conv_context & ctx, const ::clang::DecltypeType * clang_type);


}
