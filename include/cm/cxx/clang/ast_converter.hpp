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

    /// Converts clang qual type to code model type. Creates composite types if needed.
    qual_type convert_type(const ::clang::QualType & clang_type);

    /// Converts bultin clang type to code model type.
    type_t * convert_builtin_type(const ::clang::BuiltinType * clang_bt_type);

    /// Converts pointer clang type to code model type.
    pointer_type * convert_pointer_type(const ::clang::PointerType * clang_ptr_type);

    /// Converts lvalue reference clang type to code model type.
    lvalue_reference_type *
    convert_lvalue_reference_type(const ::clang::LValueReferenceType * clang_ref_type);

    /// Converts rvalue reference clang type to code model type.
    rvalue_reference_type *
    convert_rvalue_reference_type(const ::clang::RValueReferenceType * clang_ref_type);

    /// Converts array clang type to code model type.
    array_type * convert_array_type(const ::clang::ConstantArrayType * clang_arr_type);

    /// Converts type template parameter type to code model type
    type_template_parameter *
    convert_type_template_param_type(const ::clang::TemplateTypeParmType * clang_tpar_type);

    /// Converts template specialization type to code model type
    type_t *
    convert_template_spec_type(const ::clang::TemplateSpecializationType * clang_templ_spec);

    /// Converts clang function type to code model type. Creates new composite types if needed.
    function_type * convert_function_type(const ::clang::FunctionType * clang_func_type);

    /// Converts clang record type to code model type. Creates new empty record if needed.
    record_type * convert_record_type(const ::clang::RecordType * clang_rec_type);

    /// Converts dependent type
    dependent_type * convert_dependent_type(const ::clang::DependentNameType * clang_type);

    /// Converts decltype type
    decltype_type * convert_decltype_type(const ::clang::DecltypeType * clang_type);


    //////////////////////////////////////////////////////////////////////
    // Declarations

    /// Converts declaration
    void convert_decl(const ::clang::Decl * clang_decl);


    //////////////////////////////////////////////////////////////////////
    // Entities

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
