// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file convert_type.cpp
/// Contains implementations of functions for converting clang types to code model types.

#include "cm/cxx/clang/type_converter.hpp"
#include "convert_decl.hpp"
#include "convert_record.hpp"
#include "convert_template.hpp"
#include "log.hpp"
#include "utils.hpp"

#include <clang/AST/DeclTemplate.h>


namespace cm::cxx::clang {


type_t * type_converter::builtin(conv_context & ctx, const ::clang::BuiltinType * clang_bt_type) {
    switch (clang_bt_type->getKind()) {
    case ::clang::BuiltinType::Void:
        return ctx.mdl().bt_void();

    case ::clang::BuiltinType::Bool:
        return ctx.mdl().bt_bool();

    // integer types
    case ::clang::BuiltinType::Char_U:
        return ctx.mdl().bt_char();
    case ::clang::BuiltinType::UChar:
        return ctx.mdl().bt_unsigned_char();
    case ::clang::BuiltinType::WChar_U:
        return ctx.mdl().bt_wchar_t();
    case ::clang::BuiltinType::Char8:
        return ctx.mdl().bt_char8_t();
    case ::clang::BuiltinType::Char16:
        return ctx.mdl().bt_char16_t();
    case ::clang::BuiltinType::Char32:
        return ctx.mdl().bt_char32_t();
    case ::clang::BuiltinType::UShort:
        return ctx.mdl().bt_unsigned_short();
    case ::clang::BuiltinType::UInt:
        return ctx.mdl().bt_unsigned_int();
    case ::clang::BuiltinType::ULong:
        return ctx.mdl().bt_unsigned_long();
    case ::clang::BuiltinType::ULongLong:
        return ctx.mdl().bt_unsigned_long_long();
    case ::clang::BuiltinType::UInt128:
        return ctx.mdl().bt_uint128();
    case ::clang::BuiltinType::Char_S:
        return ctx.mdl().bt_char();
    case ::clang::BuiltinType::SChar:
        return ctx.mdl().bt_signed_char();
    case ::clang::BuiltinType::WChar_S:
        return ctx.mdl().bt_wchar_t();
    case ::clang::BuiltinType::Short:
        return ctx.mdl().bt_short();
    case ::clang::BuiltinType::Int:
        return ctx.mdl().bt_int();
    case ::clang::BuiltinType::Long:
        return ctx.mdl().bt_long();
    case ::clang::BuiltinType::LongLong:
        return ctx.mdl().bt_long_long();
    case ::clang::BuiltinType::Int128:
        return ctx.mdl().bt_int128();

    // GNU C extension fixed point types. Just treat them as integer types for now
    // TODO: support fixed point types
    case ::clang::BuiltinType::ShortAccum:
        return ctx.mdl().bt_short();
    case ::clang::BuiltinType::Accum:
        return ctx.mdl().bt_int();
    case ::clang::BuiltinType::LongAccum:
        return ctx.mdl().bt_long();
    case ::clang::BuiltinType::UShortAccum:
        return ctx.mdl().bt_unsigned_short();
    case ::clang::BuiltinType::UAccum:
        return ctx.mdl().bt_unsigned_int();
    case ::clang::BuiltinType::ULongAccum:
        return ctx.mdl().bt_unsigned_long();
    case ::clang::BuiltinType::ShortFract:
        return ctx.mdl().bt_short();
    case ::clang::BuiltinType::Fract:
        return ctx.mdl().bt_int();
    case ::clang::BuiltinType::LongFract:
        return ctx.mdl().bt_long();
    case ::clang::BuiltinType::UShortFract:
        return ctx.mdl().bt_unsigned_short();
    case ::clang::BuiltinType::UFract:
        return ctx.mdl().bt_unsigned_int();
    case ::clang::BuiltinType::ULongFract:
        return ctx.mdl().bt_unsigned_long();
    case ::clang::BuiltinType::SatShortAccum:
        return ctx.mdl().bt_short();
    case ::clang::BuiltinType::SatAccum:
        return ctx.mdl().bt_int();
    case ::clang::BuiltinType::SatLongAccum:
        return ctx.mdl().bt_long();
    case ::clang::BuiltinType::SatUShortAccum:
        return ctx.mdl().bt_unsigned_short();
    case ::clang::BuiltinType::SatUAccum:
        return ctx.mdl().bt_unsigned_int();
    case ::clang::BuiltinType::SatULongAccum:
        return ctx.mdl().bt_unsigned_long();
    case ::clang::BuiltinType::SatShortFract:
        return ctx.mdl().bt_short();
    case ::clang::BuiltinType::SatFract:
        return ctx.mdl().bt_int();
    case ::clang::BuiltinType::SatLongFract:
        return ctx.mdl().bt_long();
    case ::clang::BuiltinType::SatUShortFract:
        return ctx.mdl().bt_unsigned_short();
    case ::clang::BuiltinType::SatUFract:
        return ctx.mdl().bt_unsigned_int();
    case ::clang::BuiltinType::SatULongFract:
        return ctx.mdl().bt_unsigned_long();

    // float types
    case ::clang::BuiltinType::Half:
        // TODO: support OpenCL half float
        return ctx.mdl().bt_float();
    case ::clang::BuiltinType::Float:
        return ctx.mdl().bt_float();
    case ::clang::BuiltinType::Double:
        return ctx.mdl().bt_double();
    case ::clang::BuiltinType::LongDouble:
        return ctx.mdl().bt_long_double();
    case ::clang::BuiltinType::Float16:
        // TODO: support float16
        assert(false && "don't know how to convert float16 type");
        return nullptr;
    case ::clang::BuiltinType::Float128:
        // TODO: support float128
        assert(false && "don't know how to convert float128 type");
        return nullptr;

    // other types
    case ::clang::BuiltinType::NullPtr:
        assert(false && "don't know how to convet nullptr type");
        return nullptr;

    case ::clang::BuiltinType::ObjCId:
    case ::clang::BuiltinType::ObjCClass:
    case ::clang::BuiltinType::ObjCSel:
    case ::clang::BuiltinType::OCLSampler:
    case ::clang::BuiltinType::OCLEvent:
    case ::clang::BuiltinType::OCLClkEvent:
    case ::clang::BuiltinType::OCLQueue:
    case ::clang::BuiltinType::OCLReserveID:
        // TODO: support objective-c types
        return ctx.mdl().get_or_create_ptr_type(ctx.mdl().bt_void());

    case ::clang::BuiltinType::Dependent:
        // TODO: check if we need it
        assert(false && "don't know how to convert dependent type");
        return nullptr;

    case ::clang::BuiltinType::Overload:
        // TODO: check if we need it
        assert(false && "don't know how to convert overload type");
        return nullptr;

    case ::clang::BuiltinType::BoundMember:
        // TODO: check if we need it
        assert(false && "don't know how to convert bound member type");
        return nullptr;

    case ::clang::BuiltinType::PseudoObject:
        // TODO: check if we need it
        assert(false && "don't know how to convert pseudo object type");
        return nullptr;

    case ::clang::BuiltinType::UnknownAny:
        // TODO: check if we need it
        assert(false && "don't know how to convert unknown any type");
        return nullptr;

    case ::clang::BuiltinType::BuiltinFn:
        // TODO: check if we need it
        assert(false && "don't know how to convert builtin fn type");
        return nullptr;

    case ::clang::BuiltinType::ARCUnbridgedCast:
        // TODO: add support of objective-c
        return ctx.mdl().bt_int();

    case ::clang::BuiltinType::OMPArraySection:
        // TODO: add support of openmp
        return ctx.mdl().bt_int();

    case ::clang::BuiltinType::SveInt8:
        return ctx.mdl().bt_arm_sve_int8x1();
    case ::clang::BuiltinType::SveInt8x2:
        return ctx.mdl().bt_arm_sve_int8x2();
    case ::clang::BuiltinType::SveInt8x3:
        return ctx.mdl().bt_arm_sve_int8x3();
    case ::clang::BuiltinType::SveInt8x4:
        return ctx.mdl().bt_arm_sve_int8x4();
    case ::clang::BuiltinType::SveInt16:
        return ctx.mdl().bt_arm_sve_int16x1();
    case ::clang::BuiltinType::SveInt16x2:
        return ctx.mdl().bt_arm_sve_int16x2();
    case ::clang::BuiltinType::SveInt16x3:
        return ctx.mdl().bt_arm_sve_int16x3();
    case ::clang::BuiltinType::SveInt16x4:
        return ctx.mdl().bt_arm_sve_int16x4();
    case ::clang::BuiltinType::SveInt32:
        return ctx.mdl().bt_arm_sve_int32x1();
    case ::clang::BuiltinType::SveInt32x2:
        return ctx.mdl().bt_arm_sve_int32x2();
    case ::clang::BuiltinType::SveInt32x3:
        return ctx.mdl().bt_arm_sve_int32x3();
    case ::clang::BuiltinType::SveInt32x4:
        return ctx.mdl().bt_arm_sve_int32x4();
    case ::clang::BuiltinType::SveInt64:
        return ctx.mdl().bt_arm_sve_int64x1();
    case ::clang::BuiltinType::SveInt64x2:
        return ctx.mdl().bt_arm_sve_int64x2();
    case ::clang::BuiltinType::SveInt64x3:
        return ctx.mdl().bt_arm_sve_int64x3();
    case ::clang::BuiltinType::SveInt64x4:
        return ctx.mdl().bt_arm_sve_int64x4();

    case ::clang::BuiltinType::SveUint8:
        return ctx.mdl().bt_arm_sve_uint8x1();
    case ::clang::BuiltinType::SveUint8x2:
        return ctx.mdl().bt_arm_sve_uint8x2();
    case ::clang::BuiltinType::SveUint8x3:
        return ctx.mdl().bt_arm_sve_uint8x3();
    case ::clang::BuiltinType::SveUint8x4:
        return ctx.mdl().bt_arm_sve_uint8x4();
    case ::clang::BuiltinType::SveUint16:
        return ctx.mdl().bt_arm_sve_uint16x1();
    case ::clang::BuiltinType::SveUint16x2:
        return ctx.mdl().bt_arm_sve_uint16x2();
    case ::clang::BuiltinType::SveUint16x3:
        return ctx.mdl().bt_arm_sve_uint16x3();
    case ::clang::BuiltinType::SveUint16x4:
        return ctx.mdl().bt_arm_sve_uint16x4();
    case ::clang::BuiltinType::SveUint32:
        return ctx.mdl().bt_arm_sve_uint32x1();
    case ::clang::BuiltinType::SveUint32x2:
        return ctx.mdl().bt_arm_sve_uint32x2();
    case ::clang::BuiltinType::SveUint32x3:
        return ctx.mdl().bt_arm_sve_uint32x3();
    case ::clang::BuiltinType::SveUint32x4:
        return ctx.mdl().bt_arm_sve_uint32x4();
    case ::clang::BuiltinType::SveUint64:
        return ctx.mdl().bt_arm_sve_uint64x1();
    case ::clang::BuiltinType::SveUint64x2:
        return ctx.mdl().bt_arm_sve_uint64x2();
    case ::clang::BuiltinType::SveUint64x3:
        return ctx.mdl().bt_arm_sve_uint64x3();
    case ::clang::BuiltinType::SveUint64x4:
        return ctx.mdl().bt_arm_sve_uint64x4();

    case ::clang::BuiltinType::SveFloat16:
        return ctx.mdl().bt_arm_sve_float16x1();
    case ::clang::BuiltinType::SveFloat16x2:
        return ctx.mdl().bt_arm_sve_float16x2();
    case ::clang::BuiltinType::SveFloat16x3:
        return ctx.mdl().bt_arm_sve_float16x3();
    case ::clang::BuiltinType::SveFloat16x4:
        return ctx.mdl().bt_arm_sve_float16x4();
    case ::clang::BuiltinType::SveFloat32:
        return ctx.mdl().bt_arm_sve_float32x1();
    case ::clang::BuiltinType::SveFloat32x2:
        return ctx.mdl().bt_arm_sve_float32x2();
    case ::clang::BuiltinType::SveFloat32x3:
        return ctx.mdl().bt_arm_sve_float32x3();
    case ::clang::BuiltinType::SveFloat32x4:
        return ctx.mdl().bt_arm_sve_float32x4();
    case ::clang::BuiltinType::SveFloat64:
        return ctx.mdl().bt_arm_sve_float64x1();
    case ::clang::BuiltinType::SveFloat64x2:
        return ctx.mdl().bt_arm_sve_float64x2();
    case ::clang::BuiltinType::SveFloat64x3:
        return ctx.mdl().bt_arm_sve_float64x3();
    case ::clang::BuiltinType::SveFloat64x4:
        return ctx.mdl().bt_arm_sve_float64x4();

    case ::clang::BuiltinType::SveBFloat16:
        return ctx.mdl().bt_arm_sve_bfloat16x1();
    case ::clang::BuiltinType::SveBFloat16x2:
        return ctx.mdl().bt_arm_sve_bfloat16x2();
    case ::clang::BuiltinType::SveBFloat16x3:
        return ctx.mdl().bt_arm_sve_bfloat16x3();
    case ::clang::BuiltinType::SveBFloat16x4:
        return ctx.mdl().bt_arm_sve_bfloat16x4();

    case ::clang::BuiltinType::SveBool:
        return ctx.mdl().bt_arm_sve_boolx1();
    case ::clang::BuiltinType::SveBoolx2:
        return ctx.mdl().bt_arm_sve_boolx2();
    case ::clang::BuiltinType::SveBoolx4:
        return ctx.mdl().bt_arm_sve_boolx4();

    case ::clang::BuiltinType::SveCount:
        return ctx.mdl().bt_arm_sve_count();

    // case ::clang::BuiltinType::SveBFloat16x2:
    //     return ctx.mdl().bt_arm_sve_bfloat16x2();
    // case ::clang::BuiltinType::SveBFloat16x3:
    //     return ctx.mdl().bt_arm_sve_bfloat16x3();

    // case ::clang::BuiltinType::sve:
    //     return ctx.mdl().bt_arm_sve_bfloat16x4();

    default:
        std::cout << "UNKNOWN BUILTIN TYPE:\n" << std::endl;
        clang_bt_type->dump();
        assert(false && "Unknown builtin type");
        return nullptr;
    }
}


pointer_type * type_converter::pointer(conv_context & ctx,
                                       const ::clang::PointerType * clang_ptr_type) {
    auto pointee_type = type(ctx, clang_ptr_type->getPointeeType());
    return ctx.mdl().get_or_create_ptr_type(pointee_type);
}


lvalue_reference_type *
type_converter::lvalue_reference(conv_context & ctx,
                                 const ::clang::LValueReferenceType * clang_ref_type) {
    auto base_type = type(ctx, clang_ref_type->getPointeeType());
    return ctx.mdl().get_or_create_lvalue_ref_type(base_type);
}


rvalue_reference_type *
type_converter::rvalue_reference(conv_context & ctx,
                                 const ::clang::RValueReferenceType * clang_ref_type) {
    auto base_type = type(ctx, clang_ref_type->getPointeeType());
    return ctx.mdl().get_or_create_rvalue_ref_type(base_type);
}


array_type * type_converter::array(conv_context & ctx,
                                   const ::clang::ConstantArrayType * clang_arr_type) {
    auto elt_type = type(ctx, clang_arr_type->getElementType());
    return ctx.mdl().get_or_create_arr_type(elt_type.type(),
                                            clang_arr_type->getSize().getLimitedValue());
}


type_template_parameter *
type_converter::template_parameter(conv_context & ctx,
                                   const ::clang::TemplateTypeParmType * type) {

    templated_entity * templ = nullptr;

    // searching for template declaration in the chain of parent declaration contexts,

    auto c_decl_ctx = ctx.clang_decl_ctx();
    while (true) {
        if (auto decl = ::clang::dyn_cast<::clang::Decl>(c_decl_ctx)) {
            if (auto pars = decl->getDescribedTemplateParams(); pars != nullptr) {
                assert(pars->getDepth() >= type->getDepth() &&
                       "missed template parameter list in parents chain");
                if (pars->getDepth() == type->getDepth()) {
                    templ = ctx.get_cm_entity_as<templated_entity>(decl);
                    assert(templ && "can't find templated entity");
                    break;
                }
            }
        }

        // moving to the parent context
        c_decl_ctx = c_decl_ctx->getParent();
    }

    // getting template parameter from templated entity
    auto pars = templ->template_params();
    assert(type->getIndex() < std::ranges::distance(pars) && "invalid template parameter index");
    auto it = std::ranges::begin(pars);
    std::advance(it, type->getIndex());
    auto par = *it;

    // checking parameter type
    auto type_par = dynamic_cast<type_template_parameter*>(par);
    assert(type_par && "template parameter is not a type");
    return type_par;

    // // getting declaration context for template parameter type
    // auto type_decl = clang_type->getDecl();
    // auto decl_ctx = type_decl->getDeclContext();
    // auto parent_decl = ::clang::dyn_cast<::clang::Decl>(decl_ctx);

    // // getting CM template associated with clang record
    // auto rec = get_cm_entity_as<template_>(parent_decl);
    // assert(rec != nullptr && "no CM template associated with template parameter context");

    // // getting template parameter
    // auto idx = clang_type->getIndex();
    // assert(idx < std::ranges::size(rec->template_params()) && "invalid template parameter index");
    // auto tpar = dynamic_cast<type_template_parameter*>(rec->template_params()[idx]);
    // assert(tpar && "template parameter is not a type template parameter in code model");
    // return tpar;
}


type_t * type_converter::template_spec(conv_context & ctx,
                                       const ::clang::TemplateSpecializationType * type) {

    if (type->isTypeAlias()) {
        assert(false && "type aliases are not implemented yet");
        return nullptr;
    } else {
        // getting record declaration for template specialization or template declaration
        auto rec_decl = type->getAsCXXRecordDecl();
        if (rec_decl) {
            if (auto templ_decl = rec_decl->getDescribedClassTemplate()) {
                // template instantiation is just a template declaration being processed now
                auto rec = ctx.get_cm_entity_as<template_record>(rec_decl);
                return rec->this_type();
            } else {
                // std::cout << "TEMPLATE SPECIALIZATION TYPE: " << rec_decl << std::endl;
                // rec_decl->dump();

                // looking for existing CM entity associated with specialization record decl
                auto rec = ctx.get_cm_entity_as<template_record_instantiation_type>(rec_decl);
                assert(rec != nullptr && "no CM record associated with template specialization type");
                return rec;
            }
        } else {
            // template specialization without record declaration must be a dependent type
            assert(type->isDependentType() && "required dependent type here");

            // getting template declaration
            auto templ_decl = type->getTemplateName().getAsTemplateDecl();
            assert(templ_decl && "template delcaration is null for dependent template name");
            auto templ_rec_decl = templ_decl->getTemplatedDecl();
            assert(templ_rec_decl && "record delcaration is null for dependent template name");

            // getting code model template associated with template declaration
            auto templ = ctx.get_cm_entity_as<template_record>(templ_rec_decl);
            assert(templ != nullptr && "can't find CM template for tempalte decl");

            // converting template arguments
            auto args = convert_template_arguments(ctx, type->template_arguments());

            // looking for existing instantiation
            if (auto inst = templ->find_dependent_instantiation(args)) {
                return inst;
            }

            // creating template dependent instantiation
            return templ->create_dependent_instantiation(args);
        }
    }
}


function_type * type_converter::function(conv_context & ctx,
                                         const ::clang::FunctionType * clang_func_type) {

    assert(clang_func_type && "clang function type should not be null here");

    // converting return type
    auto ret_type = type(ctx, clang_func_type->getReturnType());

    if (::clang::dyn_cast<::clang::FunctionNoProtoType>(clang_func_type)) {
        // K&R function without parameters
        return ctx.mdl().get_or_create_func_type(ret_type);
    }

    auto clang_func_proto_type = ::clang::dyn_cast<::clang::FunctionProtoType>(clang_func_type);
    assert(clang_func_proto_type && "function type must be FunctionNoProtoType or FunctionProtoType");

    // creating range view that contains clang parameter types converted to code model types
    auto params_idxs = std::ranges::views::iota(0U, clang_func_proto_type->getNumParams());
    auto clang_params = params_idxs | std::ranges::views::transform([clang_func_proto_type](auto idx) {
        return clang_func_proto_type->getParamType(idx);
    });

    auto convert_param = [&ctx, this](auto && clang_param) {
        return type(ctx, clang_param);
    };
    auto converted_params = clang_params | std::ranges::views::transform(convert_param);

    return ctx.mdl().get_or_create_func_type_r(ret_type, converted_params);
}


record_type * type_converter::record(conv_context & ctx,
                                     const ::clang::RecordType * clang_rec_type) {
    return get_or_create_decl_entity_as<record_type>(ctx, clang_rec_type->getDecl());
}


dependent_type * type_converter::dependent(conv_context & ctx,
                                           const ::clang::DependentNameType * clang_type) {
    return ctx.decl_ctx()->create_entity<dependent_type>();
}


decltype_type * type_converter::decltype_(conv_context & ctx,
                                          const ::clang::DecltypeType * clang_type) {
    return ctx.decl_ctx()->create_entity<decltype_type>();
}


typedef_type * type_converter::typedef_(conv_context & ctx,
                                        const ::clang::TypedefType * clang_type) {
    return get_or_create_decl_entity_as<typedef_type>(ctx, clang_type->getDecl());
}


qual_type type_converter::type(conv_context & ctx, const ::clang::Type * type) {
    if (auto clang_bt_type = ::clang::dyn_cast<::clang::BuiltinType>(type)) {
        return builtin(ctx, clang_bt_type);
    } else if (auto clang_ptr_type = ::clang::dyn_cast<::clang::PointerType>(type)) {
        return pointer(ctx, clang_ptr_type);
    } else if (auto clang_ref_type = ::clang::dyn_cast<::clang::LValueReferenceType>(type)) {
        return lvalue_reference(ctx, clang_ref_type);
    } else if (auto clang_ref_type = ::clang::dyn_cast<::clang::RValueReferenceType>(type)) {
        return rvalue_reference(ctx, clang_ref_type);
    } else if (auto clang_func_type = ::clang::dyn_cast<::clang::FunctionType>(type)) {
        return function(ctx, clang_func_type);
    } else if (auto clang_rec_type = ::clang::dyn_cast<::clang::RecordType>(type)) {
        return record(ctx, clang_rec_type);
    } else if (auto clang_arr_type = ::clang::dyn_cast<::clang::ConstantArrayType>(type)) {
        return array(ctx, clang_arr_type);
    } else if (auto clang_elab_type = ::clang::dyn_cast<::clang::ElaboratedType>(type)) {
        return this->type(ctx, clang_elab_type->getNamedType()).type();
    } else if (auto clang_td_type = ::clang::dyn_cast<::clang::TypedefType>(type)) {
        return typedef_(ctx, clang_td_type);
    } else if (auto clang_tpar_type = ::clang::dyn_cast<::clang::TemplateTypeParmType>(type)) {
        return template_parameter(ctx, clang_tpar_type);
    } else if (auto tspec = ::clang::dyn_cast<::clang::TemplateSpecializationType>(type)) {
        return template_spec(ctx, tspec);
    } else if (auto subst_tpar = ::clang::dyn_cast<::clang::SubstTemplateTypeParmType>(type)) {
        // TODO: do we need store this info in code model?
        return this->type(ctx, subst_tpar->getReplacementType()).type();
    } else if (auto dep_type = ::clang::dyn_cast<::clang::DependentNameType>(type)) {
        return dependent(ctx, dep_type);
    } else if (auto dt_type = ::clang::dyn_cast<::clang::DecltypeType>(type)) {
        return decltype_(ctx, dt_type);
    } else if (auto p_type = ::clang::dyn_cast<::clang::ParenType>(type)) {
        // ignoring parens in code model for now
        return this->type(ctx, p_type->getInnerType());
    } else {
        CM_CLANG_LOG_ERROR << "don't know how to convert clang type:\n"
                           << dump_type_to_string(type, ctx.clang_ctx());
        assert(false && "Don't not know how to convert type");
        return {};
    }
}


/// Gets existing or creates new code model qual type for clang qual type.
qual_type type_converter::type(conv_context & ctx,
                               const ::clang::QualType & clang_type) {
    // converting base type
    auto type = this->type(ctx, clang_type.getTypePtr());

    // adding cv-qualifiers

    if (clang_type.isLocalConstQualified()) {
        type.set_const(true);
    }

    if (clang_type.isLocalVolatileQualified()) {
        type.set_volatile(true);
    }

    return type;
}


}
