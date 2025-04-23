
/// \file dependent_type_converter.cpp
/// Contains implementation of the dependent_type_converter type.

#include "dependent_type_converter.hpp"


namespace cm::cxx::clang {


dependent_type * dependent_type_converter::type(converter_impl & conv, const clang_type_t * type) {
    return conv.decl_ctx()->create_entity<dependent_type>();
}


}
