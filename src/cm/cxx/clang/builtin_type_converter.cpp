// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file builtin_type_converter.cpp
/// Contains implementation of the builtin_type_converter class.

#include "pch.hpp"

#include "builtin_type_converter.hpp"
#include "log.hpp"


namespace cm::cxx::clang {


type_t * builtin_type_converter::type(converter_impl & conv, const clang_type_t * type) {
    switch (type->getKind()) {
    case ::clang::BuiltinType::Void:
        return conv.mdl().bt_void();

    case ::clang::BuiltinType::Bool:
        return conv.mdl().bt_bool();

    // integer types
    case ::clang::BuiltinType::Char_U:
        return conv.mdl().bt_char();
    case ::clang::BuiltinType::UChar:
        return conv.mdl().bt_unsigned_char();
    case ::clang::BuiltinType::WChar_U:
        return conv.mdl().bt_wchar_t();
    case ::clang::BuiltinType::Char8:
        return conv.mdl().bt_char8_t();
    case ::clang::BuiltinType::Char16:
        return conv.mdl().bt_char16_t();
    case ::clang::BuiltinType::Char32:
        return conv.mdl().bt_char32_t();
    case ::clang::BuiltinType::UShort:
        return conv.mdl().bt_unsigned_short();
    case ::clang::BuiltinType::UInt:
        return conv.mdl().bt_unsigned_int();
    case ::clang::BuiltinType::ULong:
        return conv.mdl().bt_unsigned_long();
    case ::clang::BuiltinType::ULongLong:
        return conv.mdl().bt_unsigned_long_long();
    case ::clang::BuiltinType::UInt128:
        return conv.mdl().bt_uint128();
    case ::clang::BuiltinType::Char_S:
        return conv.mdl().bt_char();
    case ::clang::BuiltinType::SChar:
        return conv.mdl().bt_signed_char();
    case ::clang::BuiltinType::WChar_S:
        return conv.mdl().bt_wchar_t();
    case ::clang::BuiltinType::Short:
        return conv.mdl().bt_short();
    case ::clang::BuiltinType::Int:
        return conv.mdl().bt_int();
    case ::clang::BuiltinType::Long:
        return conv.mdl().bt_long();
    case ::clang::BuiltinType::LongLong:
        return conv.mdl().bt_long_long();
    case ::clang::BuiltinType::Int128:
        return conv.mdl().bt_int128();

    // GNU C extension fixed point types. Just treat them as integer types for now
    // TODO: support fixed point types
    case ::clang::BuiltinType::ShortAccum:
        return conv.mdl().bt_short();
    case ::clang::BuiltinType::Accum:
        return conv.mdl().bt_int();
    case ::clang::BuiltinType::LongAccum:
        return conv.mdl().bt_long();
    case ::clang::BuiltinType::UShortAccum:
        return conv.mdl().bt_unsigned_short();
    case ::clang::BuiltinType::UAccum:
        return conv.mdl().bt_unsigned_int();
    case ::clang::BuiltinType::ULongAccum:
        return conv.mdl().bt_unsigned_long();
    case ::clang::BuiltinType::ShortFract:
        return conv.mdl().bt_short();
    case ::clang::BuiltinType::Fract:
        return conv.mdl().bt_int();
    case ::clang::BuiltinType::LongFract:
        return conv.mdl().bt_long();
    case ::clang::BuiltinType::UShortFract:
        return conv.mdl().bt_unsigned_short();
    case ::clang::BuiltinType::UFract:
        return conv.mdl().bt_unsigned_int();
    case ::clang::BuiltinType::ULongFract:
        return conv.mdl().bt_unsigned_long();
    case ::clang::BuiltinType::SatShortAccum:
        return conv.mdl().bt_short();
    case ::clang::BuiltinType::SatAccum:
        return conv.mdl().bt_int();
    case ::clang::BuiltinType::SatLongAccum:
        return conv.mdl().bt_long();
    case ::clang::BuiltinType::SatUShortAccum:
        return conv.mdl().bt_unsigned_short();
    case ::clang::BuiltinType::SatUAccum:
        return conv.mdl().bt_unsigned_int();
    case ::clang::BuiltinType::SatULongAccum:
        return conv.mdl().bt_unsigned_long();
    case ::clang::BuiltinType::SatShortFract:
        return conv.mdl().bt_short();
    case ::clang::BuiltinType::SatFract:
        return conv.mdl().bt_int();
    case ::clang::BuiltinType::SatLongFract:
        return conv.mdl().bt_long();
    case ::clang::BuiltinType::SatUShortFract:
        return conv.mdl().bt_unsigned_short();
    case ::clang::BuiltinType::SatUFract:
        return conv.mdl().bt_unsigned_int();
    case ::clang::BuiltinType::SatULongFract:
        return conv.mdl().bt_unsigned_long();

    // float types
    case ::clang::BuiltinType::Half:
        // TODO: support OpenCL half float
        return conv.mdl().bt_float();
    case ::clang::BuiltinType::Float:
        return conv.mdl().bt_float();
    case ::clang::BuiltinType::Double:
        return conv.mdl().bt_double();
    case ::clang::BuiltinType::LongDouble:
        return conv.mdl().bt_long_double();
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
        return conv.mdl().get_or_create_ptr_type(conv.mdl().bt_void());

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
        return conv.mdl().bt_int();

    case ::clang::BuiltinType::OMPArraySection:
        // TODO: add support of openmp
        return conv.mdl().bt_int();

    case ::clang::BuiltinType::SveInt8:
        return conv.mdl().bt_arm_sve_int8x1();
    case ::clang::BuiltinType::SveInt8x2:
        return conv.mdl().bt_arm_sve_int8x2();
    case ::clang::BuiltinType::SveInt8x3:
        return conv.mdl().bt_arm_sve_int8x3();
    case ::clang::BuiltinType::SveInt8x4:
        return conv.mdl().bt_arm_sve_int8x4();
    case ::clang::BuiltinType::SveInt16:
        return conv.mdl().bt_arm_sve_int16x1();
    case ::clang::BuiltinType::SveInt16x2:
        return conv.mdl().bt_arm_sve_int16x2();
    case ::clang::BuiltinType::SveInt16x3:
        return conv.mdl().bt_arm_sve_int16x3();
    case ::clang::BuiltinType::SveInt16x4:
        return conv.mdl().bt_arm_sve_int16x4();
    case ::clang::BuiltinType::SveInt32:
        return conv.mdl().bt_arm_sve_int32x1();
    case ::clang::BuiltinType::SveInt32x2:
        return conv.mdl().bt_arm_sve_int32x2();
    case ::clang::BuiltinType::SveInt32x3:
        return conv.mdl().bt_arm_sve_int32x3();
    case ::clang::BuiltinType::SveInt32x4:
        return conv.mdl().bt_arm_sve_int32x4();
    case ::clang::BuiltinType::SveInt64:
        return conv.mdl().bt_arm_sve_int64x1();
    case ::clang::BuiltinType::SveInt64x2:
        return conv.mdl().bt_arm_sve_int64x2();
    case ::clang::BuiltinType::SveInt64x3:
        return conv.mdl().bt_arm_sve_int64x3();
    case ::clang::BuiltinType::SveInt64x4:
        return conv.mdl().bt_arm_sve_int64x4();

    case ::clang::BuiltinType::SveUint8:
        return conv.mdl().bt_arm_sve_uint8x1();
    case ::clang::BuiltinType::SveUint8x2:
        return conv.mdl().bt_arm_sve_uint8x2();
    case ::clang::BuiltinType::SveUint8x3:
        return conv.mdl().bt_arm_sve_uint8x3();
    case ::clang::BuiltinType::SveUint8x4:
        return conv.mdl().bt_arm_sve_uint8x4();
    case ::clang::BuiltinType::SveUint16:
        return conv.mdl().bt_arm_sve_uint16x1();
    case ::clang::BuiltinType::SveUint16x2:
        return conv.mdl().bt_arm_sve_uint16x2();
    case ::clang::BuiltinType::SveUint16x3:
        return conv.mdl().bt_arm_sve_uint16x3();
    case ::clang::BuiltinType::SveUint16x4:
        return conv.mdl().bt_arm_sve_uint16x4();
    case ::clang::BuiltinType::SveUint32:
        return conv.mdl().bt_arm_sve_uint32x1();
    case ::clang::BuiltinType::SveUint32x2:
        return conv.mdl().bt_arm_sve_uint32x2();
    case ::clang::BuiltinType::SveUint32x3:
        return conv.mdl().bt_arm_sve_uint32x3();
    case ::clang::BuiltinType::SveUint32x4:
        return conv.mdl().bt_arm_sve_uint32x4();
    case ::clang::BuiltinType::SveUint64:
        return conv.mdl().bt_arm_sve_uint64x1();
    case ::clang::BuiltinType::SveUint64x2:
        return conv.mdl().bt_arm_sve_uint64x2();
    case ::clang::BuiltinType::SveUint64x3:
        return conv.mdl().bt_arm_sve_uint64x3();
    case ::clang::BuiltinType::SveUint64x4:
        return conv.mdl().bt_arm_sve_uint64x4();

    case ::clang::BuiltinType::SveFloat16:
        return conv.mdl().bt_arm_sve_float16x1();
    case ::clang::BuiltinType::SveFloat16x2:
        return conv.mdl().bt_arm_sve_float16x2();
    case ::clang::BuiltinType::SveFloat16x3:
        return conv.mdl().bt_arm_sve_float16x3();
    case ::clang::BuiltinType::SveFloat16x4:
        return conv.mdl().bt_arm_sve_float16x4();
    case ::clang::BuiltinType::SveFloat32:
        return conv.mdl().bt_arm_sve_float32x1();
    case ::clang::BuiltinType::SveFloat32x2:
        return conv.mdl().bt_arm_sve_float32x2();
    case ::clang::BuiltinType::SveFloat32x3:
        return conv.mdl().bt_arm_sve_float32x3();
    case ::clang::BuiltinType::SveFloat32x4:
        return conv.mdl().bt_arm_sve_float32x4();
    case ::clang::BuiltinType::SveFloat64:
        return conv.mdl().bt_arm_sve_float64x1();
    case ::clang::BuiltinType::SveFloat64x2:
        return conv.mdl().bt_arm_sve_float64x2();
    case ::clang::BuiltinType::SveFloat64x3:
        return conv.mdl().bt_arm_sve_float64x3();
    case ::clang::BuiltinType::SveFloat64x4:
        return conv.mdl().bt_arm_sve_float64x4();

    case ::clang::BuiltinType::SveBFloat16:
        return conv.mdl().bt_arm_sve_bfloat16x1();
    case ::clang::BuiltinType::SveBFloat16x2:
        return conv.mdl().bt_arm_sve_bfloat16x2();
    case ::clang::BuiltinType::SveBFloat16x3:
        return conv.mdl().bt_arm_sve_bfloat16x3();
    case ::clang::BuiltinType::SveBFloat16x4:
        return conv.mdl().bt_arm_sve_bfloat16x4();

    case ::clang::BuiltinType::SveBool:
        return conv.mdl().bt_arm_sve_boolx1();
    case ::clang::BuiltinType::SveBoolx2:
        return conv.mdl().bt_arm_sve_boolx2();
    case ::clang::BuiltinType::SveBoolx4:
        return conv.mdl().bt_arm_sve_boolx4();

    case ::clang::BuiltinType::SveCount:
        return conv.mdl().bt_arm_sve_count();

    // case ::clang::BuiltinType::SveBFloat16x2:
    //     return conv.mdl().bt_arm_sve_bfloat16x2();
    // case ::clang::BuiltinType::SveBFloat16x3:
    //     return conv.mdl().bt_arm_sve_bfloat16x3();

    // case ::clang::BuiltinType::sve:
    //     return conv.mdl().bt_arm_sve_bfloat16x4();

    default:
        CM_CLANG_LOG_SCAT_TYPE_ERROR(decl, "unknown clang declaration", type, conv.clang_ctx());
        assert(false && "unknown clang builtin type");
        return nullptr;
    }
}


}
