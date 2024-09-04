// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file function_type_converter.cpp
/// Contains implementation of the function_type_converter class.

#include "pch.hpp"

#include "function_type_converter.hpp"
#include "log.hpp"


namespace cm::cxx::clang {


function_type * function_type_converter::type(converter_impl & conv, const clang_type_t * type) {

    // converting return type
    auto ret_type = conv.type(type->getReturnType());

    if (::clang::dyn_cast<::clang::FunctionNoProtoType>(type)) {
        // K&R function without parameters
        return conv.mdl().get_or_create_func_type(ret_type);
    }

    auto clang_func_proto_type = ::clang::dyn_cast<::clang::FunctionProtoType>(type);
    assert(clang_func_proto_type &&
           "function type must be FunctionNoProtoType or FunctionProtoType");

    // creating range view that contains clang parameter types converted to code model types
    auto params_idxs = std::ranges::views::iota(0U, clang_func_proto_type->getNumParams());
    auto clang_params = params_idxs |
        std::ranges::views::transform([clang_func_proto_type](auto idx) {
            return clang_func_proto_type->getParamType(idx);
    });

    auto convert_param = [&conv](auto && clang_param) {
        return conv.type(clang_param);
    };
    auto converted_params = clang_params | std::ranges::views::transform(convert_param);

    return conv.mdl().get_or_create_func_type_r(ret_type, converted_params);

}


}
