// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file convert_function.hpp
/// Contains utility functions for converting function declarations.

#pragma once

#include "cm/cm.hpp"
#include <clang/AST/DeclCXX.h>


namespace cm::cxx::clang {

class converter_impl;


/// Converts function return type and parameters and adds them into function entity
void convert_function_ret_type_and_params(converter_impl & conv,
                                          function * func,
                                          const ::clang::FunctionDecl * clang_func);


}
