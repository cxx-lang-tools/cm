// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

#include "convert_record.hpp"
#include "convert_decl.hpp"
#include "convert_loc.hpp"
#include "convert_type.hpp"
#include "log.hpp"
#include "utils.hpp"

#include <clang/AST/DeclCXX.h>


namespace cm::cxx::clang {


record_kind clang_tag_kind_to_record_kind(::clang::TagTypeKind clang_knd) {
    switch (clang_knd) {
    case ::clang::TagTypeKind::Struct:
        return record_kind::struct_;
    case ::clang::TagTypeKind::Class:
        return record_kind::class_;
    case ::clang::TagTypeKind::Union:
        return record_kind::union_;
    case ::clang::TagTypeKind::Enum:
        assert(false && "should not be enum here");
        // fall through in release mode
    default:
        // unknown record kind. Leave struct
        return record_kind::struct_;
    }
}


access_level get_clang_decl_acc_level(const ::clang::Decl * decl) {
    switch (decl->getAccess()) {
    case ::clang::AS_none:
    case ::clang::AS_public:
        return access_level::public_;
    case ::clang::AS_protected:
        return access_level::protected_;
    case ::clang::AS_private:
        return access_level::private_;
    default:
        assert(false && "unknown clang access level");
        return access_level::public_;
    }
}


record_type * create_new_record(conv_context & ctx,
                                context * parent_ctx,
                                const ::clang::RecordDecl * clang_rec_decl) {
    CM_CLANG_LOG_DEBUG << "creating new record for clang decl: " << clang_rec_decl;
    CM_CLANG_LOG_TRACE << "clang record decl dump:\n" << dump_decl_to_string(clang_rec_decl);

    // getting record kind
    auto knd = clang_tag_kind_to_record_kind(clang_rec_decl->getTagKind());

    // creating new record
    record_type * rec = nullptr;
    if (clang_rec_decl->getName().empty()) {
        rec = parent_ctx->create_record(knd);
    } else {
        auto nm = clang_rec_decl->getNameAsString();
        rec = parent_ctx->create_named_record(nm, knd);
    }

    rec->set_access_lev(get_clang_decl_acc_level(clang_rec_decl));

    const ::clang::TagDecl * clang_def_rec = clang_rec_decl->getDefinition();
    if (!clang_def_rec) {
        clang_def_rec = clang_rec_decl->getCanonicalDecl();
    }
    rec->set_loc(convert_loc(ctx, clang_def_rec->getSourceRange().getBegin()));

    return rec;
}


static field * convert_field(conv_context & ctx,
                             cm::record * rec,
                             const ::clang::FieldDecl * clang_field_decl) {
    // looking for existing variable for declaration
    auto var = ctx.get_cm_entity_as<field>(clang_field_decl);
    if (var != nullptr) {
        // TODO: check equality of existing variable type
        return var;
    }

    // converting variable type
    auto var_type = get_or_create_type(ctx, clang_field_decl->getType());

    // creating new variable
    auto nm = clang_field_decl->getNameAsString();
    auto acc = get_clang_decl_acc_level(clang_field_decl);
    var = rec->create_field(nm, var_type, acc);
    var->set_loc(convert_loc(ctx, clang_field_decl->getCanonicalDecl()->getLocation()));
    ctx.add_cm_entity(clang_field_decl, var);
    return var;
}


void fill_record_contents(conv_context & ctx,
                          cm::record * rec,
                          const ::clang::RecordDecl * clang_record_decl) {

    // skipping declarations without definition
    if (!clang_record_decl->isCompleteDefinition()) {
        return;
    }

    // setting current declaration context
    conv_context::decl_context_setter csetter{ctx, rec, clang_record_decl};

    // adding record bases
    if (auto clang_cxx_record_decl = ::clang::dyn_cast<::clang::CXXRecordDecl>(clang_record_decl)) {
        for (auto && base : clang_cxx_record_decl->bases()) {
            auto clang_base_type = base.getType().getTypePtr();
            auto base_type = get_or_create_type(ctx, ::clang::QualType{clang_base_type, 0});
            rec->add_base(base_type.type());
        }
    }

    // converting record members
    for (auto && decl : clang_record_decl->decls()) {
        // skipping "injected class name" records
        if (auto rec_decl = ::clang::dyn_cast<::clang::RecordDecl>(decl)) {
            if (rec_decl->isInjectedClassName()) {
                continue;
            }
        }

        if (auto acc_decl = ::clang::dyn_cast<::clang::AccessSpecDecl>(decl)) {
            // doing nothing for access levels
        } else if (auto fld_decl = ::clang::dyn_cast<::clang::FieldDecl>(decl)) {
            // field
            convert_field(ctx, rec, fld_decl);
        } else {
            // other declaration are converted via standard convert_decl
            convert_decl(ctx, decl);
        }
    }
}


record_type * convert_record(conv_context & ctx, const ::clang::RecordDecl * clang_record_decl) {
    CM_CLANG_LOG_TRACE << "converting record:\n" << dump_decl_to_string(clang_record_decl);

    // getting existing or creating new record entity
    auto rec = get_or_create_decl_entity_as<record_type>(ctx, clang_record_decl);

    // filling record contents
    assert(rec && "record type should not be null here");
    fill_record_contents(ctx, rec, clang_record_decl);
    return rec;
}


}
