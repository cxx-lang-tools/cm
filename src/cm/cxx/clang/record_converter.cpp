// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file record_converter.cpp
/// Contains implementation of the record_converter class

#include "pch.hpp"

#include "record_converter.hpp"
#include "converter_impl.hpp"
#include "convert_record.hpp"


namespace cm::cxx::clang {


bool record_converter::match(const clang_decl_t * decl) {
    // skipping template record partial specializations
    if (::clang::dyn_cast<::clang::ClassTemplatePartialSpecializationDecl>(decl)) {
        return false;
    }

    return true;
}


record_type * record_converter::create_entity(converter_impl & conv,
                                              const clang_decl_t * decl) {
    return create_new_record(conv, conv.parent_ctx(decl), decl);
}

record_type * record_converter::convert_decl(converter_impl & conv,
                                             const clang_decl_t * decl) {
    // getting existing or creating new record entity
    auto rec = conv.decl_entity_as<record_type>(decl);
    assert(rec && "record entity must not be null");

    // filling record contents
    fill_record_contents(conv, rec, decl);
    return rec;
}


}
