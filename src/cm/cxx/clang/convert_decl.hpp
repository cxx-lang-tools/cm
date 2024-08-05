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


/// Searches for existing code model context entity associated with clang declaration
/// or creates a new one if entity not found.
context_entity * get_or_create_decl_entity(conv_context & ctx, const ::clang::Decl * clang_decl);


/// Searches for existing code model context entity associated with clang declaration or creates
/// a new one. Found entity must be convertible to specified type.
template <std::derived_from<context_entity> Entity>
Entity * get_or_create_decl_entity_as(conv_context & ctx, const ::clang::Decl * clang_decl) {
    auto ent = get_or_create_decl_entity(ctx, clang_decl);
    assert(ent != nullptr && "get_or_create_entity returned null");
    auto casted_ent = dynamic_cast<Entity*>(ent);
    assert(casted_ent != nullptr && "invalid entity type");
    return casted_ent;
}


/// Converts declaration
void convert_decl(conv_context & ctx, const ::clang::Decl * clang_decl);

/// Converts clang namespace declaration to code model namespace
namespace_ * convert_namespace(conv_context & ctx, const ::clang::NamespaceDecl * clang_ns);

/// Converts function return type and parameters and adds them into function entity
void convert_function_ret_type_and_params(conv_context & ctx,
                                          function * func,
                                          const ::clang::FunctionDecl * clang_func);


}
