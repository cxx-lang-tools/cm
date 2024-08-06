// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file convert_template.cpp
/// Contains implementations of functions for converting clang templates.

#include "convert_template.hpp"
#include "convert_decl.hpp"
#include "convert_loc.hpp"
#include "convert_record.hpp"
#include "convert_type.hpp"
#include "log.hpp"
#include "utils.hpp"

#include <clang/AST/DeclTemplate.h>


namespace cm::cxx::clang {


/// Updates template parameters locations and names to match parameters from the list
static void update_template_params(conv_context & ctx,
                                   templated_entity * templ,
                                   const ::clang::TemplateParameterList * clang_params) {
    auto templ_params = templ->template_params();
    auto par_it = std::ranges::begin(templ_params);

    for (auto && clang_par : *clang_params) {
        assert(par_it != std::ranges::end(templ_params) &&
               "template parameters inconsisteny between declarations");
        auto par = *par_it;

        par->ctx()->rename_entity(par, clang_par->getName());
        par->set_loc(convert_loc(ctx, clang_par->getLocation()));

        ++par_it;
    }
}


/// Updates function parameter names and locations to match marapeters from clang declaration
void update_function_params(conv_context & ctx,
                            function * func,
                            const ::clang::FunctionDecl * clang_func) {
    auto params = func->params();
    auto par_it = std::ranges::begin(params);

    for (auto && clang_par : clang_func->parameters()) {
        assert(par_it != std::ranges::end(params) && "function parameters inconsistency");
        auto named_par = dynamic_cast<named_function_parameter*>(*par_it);
        if (!named_par) {
            continue;
        }

        named_par->set_name(clang_par->getName());

        // TODO: support locations in function parameters
        //named_par->set_loc(convert_loc(clang_par->getLocation()));

        ++par_it;
    }
}


void convert_template_params(conv_context & ctx,
                             templated_entity * templ,
                             const ::clang::TemplateParameterList * clang_params) {

    templ->set_is_variadic(clang_params->hasParameterPack());

    for (auto && par : *clang_params) {
        // adding template parameter to code model template
        auto par_name = par->getNameAsString();
        template_parameter * cm_par = nullptr;
        if (auto clang_type_par = ::clang::dyn_cast<::clang::TemplateTypeParmDecl>(par)) {
            cm_par = templ->add_type_template_param(par_name);
        } else if (auto clang_val_par = ::clang::dyn_cast<::clang::NonTypeTemplateParmDecl>(par)) {
            auto par_type = get_or_create_type(ctx, clang_val_par->getType());
            cm_par = templ->add_value_template_param(par_name, par_type.type());
        } else if (auto clang_templ_par = ::clang::dyn_cast<::clang::TemplateTemplateParmDecl>(par)) {
            // TODO: implement template template parameters
            cm_par = templ->add_type_template_param(par_name);
        
        // C++20 support in latest clang
        // } else if (auto clang_obj_par = ::clang::dyn_cast<::clang::TemplateParamObjectDecl>(par)) {
        //     // TODO: check if we need something special for template object parameters other
        //     // than name and type
        //     auto par_type = get_or_create_type(clang_val_par->getType());
        //     rec->add_value_template_param(par_name, par_type.type());

        } else {
            assert(false && "Unknown template parameter type");
        }

        cm_par->set_loc(convert_loc(ctx, par->getLocation()));

        // ading entity map for template parameter
        ctx.add_cm_entity(par, cm_par);
    }
}


template_argument_desc_vector
convert_template_arguments(conv_context & ctx,
                           const ::clang::ArrayRef<::clang::TemplateArgument> & args) {

    // converting template arguments
    template_argument_desc_vector res;
    for (unsigned i = 0, e = args.size(); i < e; ++i) {
        const auto & targ = args[i];

        switch (targ.getKind()) {
        case ::clang::TemplateArgument::ArgKind::Type: {
            res.push_back(get_or_create_type(ctx, targ.getAsType()));
            break;
        }
        case ::clang::TemplateArgument::ArgKind::Integral: {
            ::llvm::SmallString<16> sstr;
            targ.getAsIntegral().toString(sstr, 10);
            res.push_back(value{sstr.str().str()});
            break;
        }
        default:
            assert(false && "Unknown template argument type");
        }
    }

    return res;
}


template_record * convert_template_class(conv_context & ctx,
                                         const ::clang::ClassTemplateDecl * clang_templ_decl) {

    auto clang_rec_decl = clang_templ_decl->getTemplatedDecl();

    // setter for setting current contexts
    conv_context::decl_context_setter csetter{ctx};

    // looking for existing CM declaration associated with clang template declaration
    auto rec = ctx.get_cm_entity_as<template_record>(clang_rec_decl);
    if (rec != nullptr) {
        csetter.set(rec, clang_rec_decl);
    } else {
        auto clang_rec_def = clang_rec_decl;
        if (auto def = clang_rec_decl->getDefinition()) {
            clang_rec_def = ::clang::dyn_cast<::clang::CXXRecordDecl>(def);
        }

        assert(clang_rec_def != nullptr && "can't get clang template record definition");
        auto clang_templ_def = clang_rec_def->getDescribedTemplate();
        assert(clang_templ_def != nullptr && "can't get clang template class definition");

        // creating new template record
        auto knd = clang_tag_kind_to_record_kind(clang_rec_decl->getTagKind());
        auto trec = ctx.decl_ctx()->create_template_record(clang_rec_decl->getNameAsString(), knd);
        trec->set_loc(convert_loc(ctx, clang_rec_def->getLocation()));
        trec->this_type()->set_loc(trec->loc());
        rec = trec;

        // adding clang decl -> record_type association
        ctx.add_cm_entity(clang_rec_decl, rec);

        // setting new context before converting tempalte parameters
        csetter.set(rec, clang_rec_decl);

        // converting template parameters
        auto clang_templ_pars = clang_templ_def->getTemplateParameters();
        convert_template_params(ctx, rec, clang_templ_pars);
    }

    // filling template record contents
    fill_record_contents(ctx, rec, clang_rec_decl);

    // converting template specializations only for canonical decl
    // (canonical decl is only one in all translation unit)
    if (clang_templ_decl->isCanonicalDecl()) {
        for (auto && spec : clang_templ_decl->specializations()) {
            convert_template_class_spec(ctx, rec, spec);
        }
    }

    return rec;
}


record *
convert_template_class_spec(conv_context & ctx,
                            cm::template_record * templ,
                            const ::clang::ClassTemplateSpecializationDecl * clang_spec_decl) {

    // looking for existing CM declaration associated with clang declaration
    record * rec = ctx.get_cm_entity_as<template_record_instantiation_type>(clang_spec_decl);
    if (rec == nullptr) {
        auto knd = clang_tag_kind_to_record_kind(clang_spec_decl->getTagKind());
        auto clang_args = clang_spec_decl->getTemplateArgs().asArray();

        // creating new template instantiation, specialization or partial specialization
        if (auto p_spec = ::clang::dyn_cast<::clang::ClassTemplatePartialSpecializationDecl>(
                clang_spec_decl)) {
            assert(false && "should not reach here");
        } else if (clang_spec_decl->isExplicitSpecialization()) {
            auto args = convert_template_arguments(ctx, clang_args);
            rec = templ->create_specialization(args);
        } else {
            auto args = convert_template_arguments(ctx, clang_args);
            rec = templ->create_instantiation(args);
        }

        auto templ = clang_spec_decl->getSpecializedTemplate();
        rec->set_loc(convert_loc(ctx, templ->getLocation()));

        ctx.add_cm_entity(clang_spec_decl, rec);
    }

    // filling record content
    assert(rec && "record type should not be null here");
    fill_record_contents(ctx, rec, clang_spec_decl);

    return rec;
}


template_record_partial_specialization * convert_template_partial_specialization(
        conv_context & ctx,
        const ::clang::ClassTemplatePartialSpecializationDecl * clang_decl) {

    // skipping declaration without definition
    if (clang_decl->getDefinition() != clang_decl) {
        return nullptr;
    }

    // context setter for template specialization
    conv_context::decl_context_setter csetter{ctx};

    // looking for existing CM entity associated with clang declaration
    auto spec = ctx.get_cm_entity_as<template_record_partial_specialization>(clang_decl);
    if (spec != nullptr) {
        // setting current decl context to existing CM entity
        csetter.set(spec, clang_decl);
    } else {
        // creating new CM entity for template specialization

        // getting code model tempalte record entity
        auto clang_templ_decl = clang_decl->getSpecializedTemplate()->getTemplatedDecl();
        assert(clang_templ_decl && "can't get clang template record decl for specialization");
        auto templ = ctx.get_cm_entity_as<template_record>(clang_templ_decl);
        assert(templ && "can't find template record for partial template specialization");

        // creating new template specialization record
        spec = templ->create_partial_specialization();

        // converting template parameters
        convert_template_params(ctx, spec, clang_decl->getTemplateParameters());

        // adding clang decl -> specialization association before converting template
        // arguments because they can depend on temple context
        ctx.add_cm_entity(clang_decl, spec);

        // setting current context before converting template arguments
        csetter.set(spec, clang_decl);

        // converting template arguments
        auto args = convert_template_arguments(ctx, clang_decl->getTemplateArgs().asArray());
        for (auto && arg : args) {
            spec->add_arg(arg);
        }
    }

    // filling template record contents
    fill_record_contents(ctx, spec, clang_decl);

    return spec;
}


template_function * convert_template_function(conv_context & ctx,
                                              const ::clang::FunctionTemplateDecl * clang_decl) {

    auto clang_func_decl = clang_decl->getTemplatedDecl();
    auto clang_templ_pars = clang_decl->getTemplateParameters();

    // looking for existing CM declaration associated with clang template declaration
    auto func = ctx.get_cm_entity_as<template_function>(clang_func_decl);
    if (func != nullptr) {
        if (clang_decl->isThisDeclarationADefinition()) {
            // updating template parameters to match definition
            update_template_params(ctx, func, clang_templ_pars);

            // updating function parameters to match definition
            update_function_params(ctx, func, clang_func_decl);
        }

        // TODO: check equality of function declarations
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

        func = rec->create_template_method(nm);
    } else {
        func = ctx.decl_ctx()->create_template_function(nm);
    }

    func->set_access_lev(get_clang_decl_acc_level(clang_func_decl));

    // adding entity before converting template parameters, return type and
    // function parameters
    ctx.add_cm_entity(clang_func_decl, func);

    // setting current context to function
    conv_context::decl_context_setter csetter{ctx, func, clang_func_decl};

    // converting template parameters
    convert_template_params(ctx, func, clang_templ_pars);

    // converting function return type and parameters
    convert_function_ret_type_and_params(ctx, func, clang_func_decl);

    // converting template specializations only for canonical decl
    // (canonical decl is only one in all translation unit)
    if (clang_func_decl->isCanonicalDecl()) {
        for (auto && spec : clang_decl->specializations()) {
            convert_template_function_inst(ctx, func, spec);
        }
    }

    return func;
}


template_function_instantiation *
convert_template_function_inst(conv_context & ctx,
                               cm::template_function * templ,
                               const ::clang::FunctionDecl * clang_func) {
    assert(clang_func->isTemplateInstantiation() && "function is not a template instantiation");

    // checking that there is no existing entity associated with template instantiation
    auto func = ctx.get_cm_entity_as<template_function_instantiation>(clang_func);
    assert(func == nullptr && "found existing associated template function instantiation entity");

    // converting template arguments
    auto clang_args = clang_func->getTemplateSpecializationArgs();
    assert(clang_args != nullptr && "no template arguments for function instantiation");
    auto args = convert_template_arguments(ctx, clang_args->asArray());

    // creating new template function instantiation
    func = templ->create_instantiation(args);

    // converting function return type and parameters
    convert_function_ret_type_and_params(ctx, func, clang_func);

    ctx.add_cm_entity(clang_func, func);

    return func;
}


}
