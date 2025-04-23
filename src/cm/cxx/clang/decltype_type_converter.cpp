
/// \file decltype_type_converter.cpp
/// Contains implementation of the decltype_type_converter type.

#include "decltype_type_converter.hpp"


namespace cm::cxx::clang {


decltype_type * decltype_type_converter::type(converter_impl & conv, const clang_type_t * type) {
    return conv.decl_ctx()->create_entity<decltype_type>();
}


}
