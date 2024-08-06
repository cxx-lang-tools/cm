// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file convert_decl.cpp
/// Contains implementations of fucntions for converting clang declarations.

#include "pch.hpp"

#include "convert_decl.hpp"
#include "convert_loc.hpp"
#include "convert_record.hpp"
#include "convert_template.hpp"
#include "log.hpp"
#include "utils.hpp"

#include <clang/AST/DeclCXX.h>
#include <clang/AST/DeclTemplate.h>


namespace cm::cxx::clang {


/// Creates context entity for clang declaration
static context_entity * create_decl_entity(conv_context & ctx, const ::clang::Decl * clang_decl) {
    CM_CLANG_LOG_SCAT_DECL(decl, "create code model entity for decl", clang_decl);

    // getting parent context
    auto clang_decl_ctx = clang_decl->getDeclContext();
    assert(clang_decl_ctx != nullptr && "invalid parent clang decl context");
    auto parent_clang_decl = ::clang::dyn_cast<::clang::Decl>(clang_decl_ctx);
    assert(parent_clang_decl != nullptr && "parent clang decl context is not a declaration");
    auto parent_ctx = get_or_create_decl_entity_as<context>(ctx, parent_clang_decl);

    if (auto ns = ::clang::dyn_cast<::clang::NamespaceDecl>(clang_decl)) {
        auto parent_ns = dynamic_cast<namespace_*>(parent_ctx);
        assert(parent_ns && "parent context for namespace is not a namespace");
        if (ns->getName().empty()) {
            return parent_ns->create_anon_namespace();
        } else {
            return parent_ns->create_namespace(ns->getNameAsString());
        }
    } else if (auto * tcls = ::clang::dyn_cast<::clang::ClassTemplateDecl>(clang_decl)) {
        auto clang_rec_decl = tcls->getTemplatedDecl();
        auto knd = clang_tag_kind_to_record_kind(clang_rec_decl->getTagKind());
        return parent_ctx->create_template_record(tcls->getNameAsString(), knd);
    } else if (auto p_spec =
               ::clang::dyn_cast<::clang::ClassTemplatePartialSpecializationDecl>(clang_decl)) {
        // TODO: do we need this case?
        // convert_template_partial_specialization(ctx, p_spec);
        assert(false && "should not reach here");
        return nullptr;
    } else if (auto * clang_record_decl = ::clang::dyn_cast<::clang::RecordDecl>(clang_decl)) {
        return create_new_record(ctx, parent_ctx, clang_record_decl);
    } else if (auto * td_clang_decl = ::clang::dyn_cast<::clang::TypedefNameDecl>(clang_decl)) {
        return parent_ctx->create_typedef(td_clang_decl->getNameAsString());
    } else if (auto * clang_func_decl = ::clang::dyn_cast<::clang::FunctionDecl>(clang_decl)) {
        // TODO: do we need this case?
        // convert_function(ctx, clang_func_decl);
        assert(false && "should not reach here");
        return nullptr;
    } else if (auto * clang_var_decl = ::clang::dyn_cast<::clang::VarDecl>(clang_decl)) {
        // TODO: do we need this case?
        // convert_variable(ctx, clang_var_decl);
        assert(false && "should not reach here");
        return nullptr;
    } else if (auto * tfunc_decl = ::clang::dyn_cast<::clang::FunctionTemplateDecl>(clang_decl)) {
        // TODO: do we need this case?
        // convert_template_function(ctx, tfunc_decl);
        assert(false && "should not reach here");
        return nullptr;
    } else if (auto lspec_decl = ::clang::dyn_cast<::clang::LinkageSpecDecl>(clang_decl)) {
        // TODO: do we need this case?
        // convert_linkage_spec(ctx, lspec_decl);
        assert(false && "should not reach here");
        return nullptr;
    } else {
        CM_CLANG_LOG_SCAT_DECL_ERROR(decl, "unknown decl for creating entity", clang_decl);
        assert(false && "unknown decl");
        return nullptr;
    }
}


context_entity * get_or_create_decl_entity(conv_context & ctx, const ::clang::Decl * clang_decl) {
    // looking for existing entity assoicated with clang declaration
    if (auto ent = ctx.get_cm_entity(clang_decl)) {
        return ent;
    }
 
    auto ent = create_decl_entity(ctx, clang_decl);
    ctx.add_cm_entity(clang_decl, ent);

    return ent;
}


/// Converts clang typedef declaration to code model typedef type
static typedef_type * convert_typedef(conv_context & ctx,
                                      const ::clang::TypedefNameDecl * clang_typedef_decl) {

    // looking for existing or creating new typedef
    auto td_type = get_or_create_decl_entity_as<typedef_type>(ctx, clang_typedef_decl);

    // getting or creating entity for base type
    auto clang_base_type = clang_typedef_decl->getUnderlyingType();
    auto base_type = ctx.types().type(ctx, clang_base_type);
    td_type->set_base(base_type);

    // creating typedef type in namespace or record
    auto nm = clang_typedef_decl->getNameAsString();
    td_type->set_access_lev(get_clang_decl_acc_level(clang_typedef_decl));
    td_type->set_loc(convert_loc(ctx, clang_typedef_decl->getCanonicalDecl()->getLocation()));

    return td_type;
}


/// Converts clang function declaration to code model function
static function * convert_function(conv_context & ctx,
                                   const ::clang::FunctionDecl * clang_func_decl) {

    // looking for existing function for declaration
    auto func = ctx.get_cm_entity_as<named_function>(clang_func_decl);

    if (func != nullptr) {
        // TODO: check equality of existing function type with another declaration
        return func;
    }

    // creating new function
    auto nm = clang_func_decl->getNameAsString();
    if (auto method_decl = ::clang::dyn_cast<::clang::CXXMethodDecl>(clang_func_decl);
       method_decl != nullptr && !method_decl->isStatic())
    {
        // context must be a record
        auto rec = dynamic_cast<record*>(ctx.decl_ctx());
        assert(rec && "context must be a record for method declaration");

        func = rec->create_method(nm);
    } else {
        func = ctx.decl_ctx()->create_function(nm);
    }

    func->set_access_lev(get_clang_decl_acc_level(clang_func_decl));
    func->set_loc(convert_loc(ctx, clang_func_decl->getCanonicalDecl()->getLocation()));

    // adding function entity mapping
    ctx.add_cm_entity(clang_func_decl, func);

    // setting function as current decl context
    conv_context::decl_context_setter csetter{ctx, func, clang_func_decl};

    // converting function return type and parameters
    convert_function_ret_type_and_params(ctx, func, clang_func_decl);

    return func;
}


/// Converts clang variable decl to code model variable
static variable * convert_variable(conv_context & ctx, const ::clang::VarDecl * clang_var_decl) {
    // looking for existing variable for declaration
    auto var = ctx.get_cm_entity_as<variable>(clang_var_decl);
    if (var != nullptr) {
        // TODO: check equality of existing variable type
        return var;
    }

    // converting variable type
    auto var_type = ctx.types().type(ctx, clang_var_decl->getType());

    // creating new variable
    auto nm = clang_var_decl->getNameAsString();
    var = ctx.decl_ctx()->create_var(nm, var_type);
    var->set_access_lev(get_clang_decl_acc_level(clang_var_decl));

    ctx.add_cm_entity(clang_var_decl, var);
    return var;
}


namespace_ * convert_namespace(conv_context & ctx, const ::clang::NamespaceDecl * clang_ns) {
    // creating new or getting existing namespace entity
    auto ns = get_or_create_decl_entity_as<namespace_>(ctx, clang_ns);

    // setting new decl context
    conv_context::decl_context_setter csetter{ctx, ns, clang_ns};

    // converting top level declarations in namespace
    for (auto && decl : clang_ns->decls()) {
        if (auto decl_ns = ::clang::dyn_cast<::clang::NamespaceDecl>(decl)) {
            convert_namespace(ctx, decl_ns);
        } else {
            convert_decl(ctx, decl);
        }
    }

    return ns;
}


static void convert_linkage_spec(conv_context & ctx,
                                 const ::clang::LinkageSpecDecl * decl) {

    // ignore linkage specification for now
    // TODO: implement in code model

    // converting nested declarations
    for (auto && nested_decl : decl->decls()) {
        if (auto nested_ns = ::clang::dyn_cast<::clang::NamespaceDecl>(nested_decl)) {
            convert_namespace(ctx, nested_ns);
        } else {
            convert_decl(ctx, nested_decl);
        }
    }
}


void convert_decl(conv_context & ctx, const ::clang::Decl * clang_decl) {
    CM_CLANG_LOG_SCAT_DECL(decl, "converting decl", clang_decl);

    if (auto * ns = ::clang::dyn_cast<::clang::NamespaceDecl>(clang_decl)) {
        // should not be namespace here
        assert(false && "namespace decl should not be passed here");
    }

    if (auto * clang_class_templ_decl = ::clang::dyn_cast<::clang::ClassTemplateDecl>(clang_decl)) {
        convert_template_class(ctx, clang_class_templ_decl);
    } else if (auto p_spec =
               ::clang::dyn_cast<::clang::ClassTemplatePartialSpecializationDecl>(clang_decl)) {
        convert_template_partial_specialization(ctx, p_spec);
    } else if (auto * clang_record_decl = ::clang::dyn_cast<::clang::RecordDecl>(clang_decl)) {
        convert_record(ctx, clang_record_decl);
    } else if (auto * clang_typedef_decl = ::clang::dyn_cast<::clang::TypedefNameDecl>(clang_decl)) {
        convert_typedef(ctx, clang_typedef_decl);
    } else if (auto * clang_func_decl = ::clang::dyn_cast<::clang::FunctionDecl>(clang_decl)) {
        convert_function(ctx, clang_func_decl);
    } else if (auto * clang_var_decl = ::clang::dyn_cast<::clang::VarDecl>(clang_decl)) {
        convert_variable(ctx, clang_var_decl);
    } else if (auto * tfunc_decl = ::clang::dyn_cast<::clang::FunctionTemplateDecl>(clang_decl)) {
        convert_template_function(ctx, tfunc_decl);
    } else if (auto lspec_decl = ::clang::dyn_cast<::clang::LinkageSpecDecl>(clang_decl)) {
        convert_linkage_spec(ctx, lspec_decl);
    } else {
        std::cerr << "UNKNOWN DECL:\n";
        clang_decl->dump();
    }
}


void convert_function_ret_type_and_params(conv_context & ctx,
                                          function * func,
                                          const ::clang::FunctionDecl * clang_func) {
    // converting function return type
    func->set_ret_type(ctx.types().type(ctx, clang_func->getReturnType()));

    // converting function parameters
    for (auto && par : clang_func->parameters()) {
        if (!par->getName().empty()) {
            func->add_param(par->getNameAsString(), ctx.types().type(ctx, par->getType()));
        } else {
            func->add_param(ctx.types().type(ctx, par->getType()));
        }
    }
}


}
