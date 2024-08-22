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
    // traversing over all top level declarations in translation unit
    auto tu_decl = impl_->clang_ctx().getTranslationUnitDecl();

    // adding association with entire code model for translation unit decl
    impl_->add_decl_entity(tu_decl, &impl_->mdl());

    // setting translation unit as current decl context
    converter_impl::decl_context_setter csetter{*impl_, &impl_->mdl(), tu_decl};

    CM_CLANG_LOG_DECL("converting translation unit", tu_decl);

    for (auto && decl : tu_decl->decls()) {
        CM_CLANG_LOG_DECL("converting top level declaration", decl);
        convert_decl(decl);
    }
}


qual_type ast_converter::convert_type(const ::clang::QualType & clang_qual_type) {
    return impl_->types().type(clang_qual_type);
}


type_t * ast_converter::convert_builtin_type(const ::clang::BuiltinType * clang_bt_type) {
    return impl_->types().builtin(clang_bt_type);
}


pointer_type * ast_converter::convert_pointer_type(const ::clang::PointerType * clang_ptr_type) {
    return impl_->types().pointer(clang_ptr_type);
}


lvalue_reference_type *
ast_converter::convert_lvalue_reference_type(const ::clang::LValueReferenceType * clang_ref_type) {
    return impl_->types().lvalue_reference(clang_ref_type);
}


rvalue_reference_type *
ast_converter::convert_rvalue_reference_type(const ::clang::RValueReferenceType * clang_ref_type) {
    return impl_->types().rvalue_reference(clang_ref_type);
}


array_type * ast_converter::convert_array_type(const ::clang::ConstantArrayType * clang_arr_type) {
    return impl_->types().array(clang_arr_type);
}


type_template_parameter * ast_converter::convert_type_template_param_type(
        const ::clang::TemplateTypeParmType * type) {

    return impl_->types().template_parameter(type);
}


type_t *
ast_converter::convert_template_spec_type(const ::clang::TemplateSpecializationType * type) {
    return impl_->types().template_spec(type);
}


function_type * ast_converter::convert_function_type(const ::clang::FunctionType * type) {
    return impl_->types().function(type);
}


record_type * ast_converter::convert_record_type(const ::clang::RecordType * type) {
    return impl_->types().record(type);
}


dependent_type * ast_converter::convert_dependent_type(const ::clang::DependentNameType * type) {
    return impl_->types().dependent(type);
}


decltype_type * ast_converter::convert_decltype_type(const ::clang::DecltypeType * type) {
    return impl_->types().decltype_(type);
}


void ast_converter::convert_decl(const ::clang::Decl * decl) {
    impl_->convert_decl(decl);
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
