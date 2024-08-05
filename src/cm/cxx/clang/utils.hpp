// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file utils.hpp
/// Contains definitions of utitlity functions for clang to code model converter.

#pragma once

#include <clang/AST/Decl.h>


namespace cm::cxx::clang {


/// Dumps clang declaration to string
std::string dump_decl_to_string(const ::clang::Decl * decl);

/// Dumps clang type to string
std::string dump_type_to_string(const ::clang::Type * type, const ::clang::ASTContext & ctx);


}
