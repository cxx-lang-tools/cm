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
#include "convert_decl.hpp"
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


void ast_converter::convert() {
    // traversing over all top level declarations in translation unit
    auto tu_decl = ctx_.clang_ctx().getTranslationUnitDecl();

    // adding association with entire code model for translation unit decl
    ctx_.add_cm_entity(tu_decl, &ctx_.mdl());

    // setting translation unit as current decl context
    conv_context::decl_context_setter csetter{ctx_, &ctx_.mdl(), tu_decl};

    CM_CLANG_LOG_DECL("converting translation unit", tu_decl);

    for (auto && decl : tu_decl->decls()) {
        CM_CLANG_LOG_DECL("converting top level declaration", decl);

        // converting namespaces separately from other declarations
        if (auto ns = ::clang::dyn_cast<::clang::NamespaceDecl>(decl)) {
            convert_namespace(ctx_, ns);
        } else {
            convert_decl(decl);
        }
    }
}


qual_type ast_converter::convert_type(const ::clang::QualType & clang_qual_type) {
    return ctx_.types().type(ctx_, clang_qual_type);
}


type_t * ast_converter::convert_builtin_type(const ::clang::BuiltinType * clang_bt_type) {
    return ctx_.types().builtin(ctx_, clang_bt_type);
}


pointer_type * ast_converter::convert_pointer_type(const ::clang::PointerType * clang_ptr_type) {
    return ctx_.types().pointer(ctx_, clang_ptr_type);
}


lvalue_reference_type *
ast_converter::convert_lvalue_reference_type(const ::clang::LValueReferenceType * clang_ref_type) {
    return ctx_.types().lvalue_reference(ctx_, clang_ref_type);
}


rvalue_reference_type *
ast_converter::convert_rvalue_reference_type(const ::clang::RValueReferenceType * clang_ref_type) {
    return ctx_.types().rvalue_reference(ctx_, clang_ref_type);
}


array_type * ast_converter::convert_array_type(const ::clang::ConstantArrayType * clang_arr_type) {
    return ctx_.types().array(ctx_, clang_arr_type);
}


type_template_parameter * ast_converter::convert_type_template_param_type(
        const ::clang::TemplateTypeParmType * type) {

    return ctx_.types().template_parameter(ctx_, type);
}


type_t *
ast_converter::convert_template_spec_type(const ::clang::TemplateSpecializationType * type) {
    return ctx_.types().template_spec(ctx_, type);
}


function_type * ast_converter::convert_function_type(const ::clang::FunctionType * type) {
    return ctx_.types().function(ctx_, type);
}


record_type * ast_converter::convert_record_type(const ::clang::RecordType * type) {
    return ctx_.types().record(ctx_, type);
}


dependent_type * ast_converter::convert_dependent_type(const ::clang::DependentNameType * type) {
    return ctx_.types().dependent(ctx_, type);
}


decltype_type * ast_converter::convert_decltype_type(const ::clang::DecltypeType * type) {
    return ctx_.types().decltype_(ctx_, type);
}


void ast_converter::convert_decl(const ::clang::Decl * decl) {
    clang::convert_decl(ctx_, decl);
}


source_location ast_converter::convert_loc(const ::clang::SourceLocation & loc) const {
    return ::cm::cxx::clang::convert_loc(ctx_, loc);
}


context_entity * ast_converter::get_cm_entity(const ::clang::Decl * clang_decl) {
    return ctx_.get_cm_entity(clang_decl);
}


void ast_converter::add_cm_entity(const ::clang::Decl * clang_decl, context_entity * cm_ent) {
    ctx_.add_cm_entity(clang_decl, cm_ent);
}


}
