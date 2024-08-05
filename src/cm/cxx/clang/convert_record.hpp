// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file convert_record.hpp
/// Contains definition of functions for converting clang records to code model records.

#pragma once

#include "cm/cxx/clang/conv_context.hpp"


namespace cm::cxx::clang {


/// Converts clang tag kind to record kind
record_kind clang_tag_kind_to_record_kind(::clang::TagTypeKind clang_knd);

/// Returns access level for declaration
access_level get_clang_decl_acc_level(const ::clang::Decl * decl);

/// Creates new empty record for clang record declaration
record_type * create_new_record(conv_context & ctx,
                                context * parent_ctx,
                                const ::clang::RecordDecl * clang_rec_decl);

/// Converts all record contents and adds it to code model record
void fill_record_contents(conv_context & ctx,
                          cm::record * rec,
                          const ::clang::RecordDecl * clang_record_decl);

/// Converts record
record_type * convert_record(conv_context & ctx, const ::clang::RecordDecl * clang_record_decl);

}
