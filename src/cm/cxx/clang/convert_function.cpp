// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file convert_function.cpp
/// Contains implementations of utility functions for converting function declarations.

#include "pch.hpp"

#include "convert_function.hpp"
#include "converter_impl.hpp"


namespace cm::cxx::clang {


void convert_function_ret_type_and_params(converter_impl & conv,
                                          function * func,
                                          const ::clang::FunctionDecl * clang_func) {
    // converting function return type
    func->set_ret_type(conv.types().type(clang_func->getReturnType()));

    // converting function parameters
    for (auto && par : clang_func->parameters()) {
        if (!par->getName().empty()) {
            func->add_param(par->getNameAsString(), conv.types().type(par->getType()));
        } else {
            func->add_param(conv.types().type(par->getType()));
        }
    }
}


}
