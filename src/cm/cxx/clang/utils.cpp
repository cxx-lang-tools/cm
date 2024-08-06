// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file utils.cpp
/// Contains implementations of utility functions fo clang to code model converter.

#include "utils.hpp"
#include <sstream>


namespace cm::cxx::clang {


std::string dump_decl_to_string(const ::clang::Decl * decl) {
    std::string str;
    llvm::raw_string_ostream lstr{str};
    decl->dump(lstr);
    return str;
}


std::string dump_type_to_string(const ::clang::Type * type, const ::clang::ASTContext & ctx) {
    std::string str;
    llvm::raw_string_ostream lstr{str};
    type->dump(lstr, ctx);
    return str;
}


std::string clang_decl_desc(const ::clang::Decl * decl) {
    std::ostringstream str;
    str << decl->getDeclKindName() << " [" << decl << ']';

    if (auto n_decl = ::clang::dyn_cast<::clang::NamedDecl>(decl)) {
        str << " (" << n_decl->getNameAsString() << ')';
    }

    return str.str();
}


}
