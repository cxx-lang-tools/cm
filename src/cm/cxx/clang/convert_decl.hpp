// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file convert_decl.hpp
/// Contains definitions of functions for converting clang declarations.

#pragma once

#include "cm/cxx/clang/conv_context.hpp"


namespace cm::cxx::clang {


/// Converts declaration
void convert_decl(conv_context & ctx, const ::clang::Decl * clang_decl);

/// Converts clang namespace declaration to code model namespace
namespace_ * convert_namespace(conv_context & ctx, const ::clang::NamespaceDecl * clang_ns);

/// Converts function return type and parameters and adds them into function entity
void convert_function_ret_type_and_params(conv_context & ctx,
                                          function * func,
                                          const ::clang::FunctionDecl * clang_func);


}
