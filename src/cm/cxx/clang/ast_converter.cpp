// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file ast_converter.cpp
/// Contains implementation of the ast_converter class.

#include "pch.hpp"

#include "cm/cxx/clang/ast_converter.hpp"

#include "utils.hpp"
#include "converter_impl.hpp"
#include "convert_loc.hpp"
#include "convert_record.hpp"
#include "convert_template.hpp"

#include "cm/namespace.hpp"
#include "cm/record_kind.hpp"
#include "cm/record_type.hpp"
#include "cm/template_instantiation.hpp"
#include "log.hpp"
#include <clang/AST/Decl.h>
#include <clang/AST/DeclBase.h>
#include <clang/AST/Type.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/Basic/Specifiers.h>
#include <clang/Basic/SourceManager.h>
#include <llvm/Support/raw_ostream.h>
#include <ranges>


namespace cm::cxx::clang {


ast_converter::ast_converter(code_model & mdl, const ::clang::ASTContext & ctx):
impl_{std::make_unique<converter_impl>(mdl, ctx)} {
}


ast_converter::~ast_converter() = default;


void ast_converter::convert() {
    impl_->convert();
}


type_t * ast_converter::get_type(const ::clang::Type * clang_type) const {
    return impl_->get_type(clang_type);
}


qual_type ast_converter::get_type(const ::clang::QualType & clang_type) const {
    return impl_->get_type(clang_type);
}


source_location ast_converter::convert_loc(const ::clang::SourceLocation & loc) const {
    return ::cm::cxx::clang::convert_loc(*impl_, loc);
}


context_entity * ast_converter::get_decl_entity(const ::clang::Decl * clang_decl) {
    return impl_->get_decl_entity(clang_decl);
}


void ast_converter::add_decl_entity(const ::clang::Decl * clang_decl, context_entity * cm_ent) {
    impl_->add_decl_entity(clang_decl, cm_ent);
}


}
