// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file ast_converter.hpp
/// Contains definition of the ast_converter class.

#pragma once

#include "../../cm.hpp"
#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/TypeLoc.h>
#include <clang/AST/ASTContext.h>


namespace cm::cxx::clang {

class converter_impl;


/// clang AST to code model converter
class ast_converter {
public:
    /// Constructs AST converter with specified reference to global code model
    /// and clang AST context
    ast_converter(code_model & mdl, const ::clang::ASTContext & ctx);

    /// Default destructor
    ~ast_converter();

    /// Deleted copy constructor
    ast_converter(const ast_converter &) = delete;

    /// Deleted copy assignment operator
    ast_converter & operator=(const ast_converter &) = delete;

    /// Deleted move constructor
    ast_converter(ast_converter &&) = delete;

    /// Converts AST context to code model
    void convert();


    //////////////////////////////////////////////////////////////////////
    // Types conversion

    /// Gets existing code model type for clang type. Type must exist in code model.
    type_t * get_type(const ::clang::Type * clang_type) const;

    /// Gets existing code model type for clang type. Type must exist in code model.
    qual_type get_type(const ::clang::QualType & clang_type) const;

    /// Gets existing code model type for clang type and converts it to specified type.
    /// Type must exist in code model and be convertible to specified type.
    template <std::derived_from<type_t> Type>
    Type * get_type_as(const ::clang::Type * clang_type) const {
        auto type = dynamic_cast<Type*>(get_type(clang_type));
        assert(type && "code model type can't be converted to specified type");
        return type;
    }

    /// Gets existing code model type for clang type and converts it to specified type.
    /// Type must exist in code model and be convertible to specified type.
    template <std::derived_from<type_t> Type>
    qual_type_t<Type> get_type_as(const ::clang::QualType & clang_type) const {
        auto type = get_type(clang_type).cast<Type>();
        assert(!type.is_null() && "code model type can't be converted to specified type");
        return type;
    }


    //////////////////////////////////////////////////////////////////////
    // Declarations


    /// Finds code model entity associated with clang declaration.
    /// First gets canonical declaration of clang declaration.
    /// Returns nullptr if associated context_entity not found
    context_entity * get_decl_entity(const ::clang::Decl * clang_decl);

    /// Finds code model enitty associated with clang declaration and converts it to
    /// specified type. Found context_entity must be convertible to specified type.
    /// Returns nullptr if context_entity is not found.
    template <typename Entity>
    Entity * get_decl_entity_as(const ::clang::Decl * clang_decl) {
        auto ent = get_decl_entity(clang_decl);
        if (!ent) {
            return nullptr;
        }

        auto casted_ent = dynamic_cast<Entity*>(ent);
        assert(casted_ent && "existing context entity type mismatch");
        return casted_ent;
    }

    /// Adds code model context_entity associated with clang declaration.
    void add_decl_entity(const ::clang::Decl * clang_decl, context_entity * cm_ent);

private:
    /// Converts source location
    source_location convert_loc(const ::clang::SourceLocation & loc) const;

    std::unique_ptr<converter_impl> impl_;
};


}
