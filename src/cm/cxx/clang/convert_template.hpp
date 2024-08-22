// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file convert_template.hpp
/// Contains definition of functions for converting clang templates.

#pragma once

#include "converter_impl.hpp"


namespace cm::cxx::clang {


/// Converts and adds template parameters from clang AST to code model template
void convert_template_params(converter_impl & conv,
                             templated_entity * templ,
                             const ::clang::TemplateParameterList * clang_params);

/// Converts template arguments
template_argument_desc_vector
convert_template_arguments(converter_impl & conv,
                           const ::clang::ArrayRef<::clang::TemplateArgument> & args);

/// Converts class template declaration
template_record * convert_template_class(converter_impl & conv,
                                         const ::clang::ClassTemplateDecl * clang_templ_decl);

/// Converts class template specialization declaration to code model record
record *
convert_template_class_spec(converter_impl & conv,
                            cm::template_record * templ,
                            const ::clang::ClassTemplateSpecializationDecl * clang_spec_decl);

/// Converts class template partial specialization
template_record_partial_specialization * convert_template_partial_specialization(
        converter_impl & conv,
        const ::clang::ClassTemplatePartialSpecializationDecl * clang_decl);

/// Converts function template declaration
template_function *
convert_template_function(converter_impl & conv,
                          const ::clang::FunctionTemplateDecl * clang_templ_decl);

/// Converts funciton template instantiation declaration
template_function_instantiation *
convert_template_function_inst(converter_impl & conv,
                               cm::template_function * templ,
                               const ::clang::FunctionDecl * clang_func);

}
