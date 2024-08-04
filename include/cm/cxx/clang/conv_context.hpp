// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file conv_context.hpp
/// Contains definition of the conv_context class.

#pragma once

#include "../../cm.hpp"
#include <clang/AST/ASTContext.h>


namespace cm::cxx::clang {


/// Context for converting clang AST to code model. Stores maps between AST and code
/// model entities and current conveting state.
class conv_context {
public:
    /// Helper class for settings current declaration contexts and restoring them
    /// at scope exit
    struct decl_context_setter {
    public:
        /// Constructs setter with invalid state
        explicit decl_context_setter(conv_context & conv_ctx):
            conv_ctx_{&conv_ctx}, old_ctx_{nullptr}, old_clang_ctx_{nullptr} {}

        /// Constructs setter. Sets new current context and clang declaration context
        /// and stores old contexts in this setter instance.
        explicit decl_context_setter(conv_context & conv_ctx,
                                     context * ctx,
                                     const ::clang::DeclContext * clang_ctx):
        conv_ctx_{&conv_ctx},
        old_ctx_{conv_ctx.decl_ctx()},
        old_clang_ctx_{conv_ctx.clang_decl_ctx()} {
            assert(ctx != nullptr && "invalid context");
            assert(clang_ctx != nullptr && "invalid clang decl context");

            conv_ctx_->set_decl_ctx(ctx);
            conv_ctx_->set_clang_decl_ctx(clang_ctx);
        }

        /// Restores old current context and clang declaration context if setter state is valid
        ~decl_context_setter() {
            restore();
        }

        decl_context_setter(const decl_context_setter &) = delete;
        decl_context_setter & operator=(const decl_context_setter &) = delete;
        decl_context_setter(decl_context_setter && other) = delete;
        decl_context_setter & operator=(decl_context_setter && other) = delete;

        /// Sets current context and clang context in AST converter.
        /// Saves previous contexts. The state of setter must be invalid
        void set(context * ctx, const ::clang::DeclContext * clang_ctx) {
            assert(old_ctx_ == nullptr && "the state of setter is not invalid");
            assert(old_clang_ctx_ == nullptr && "the state of setter is not invalid");

            assert(ctx != nullptr && "context is null");
            assert(clang_ctx != nullptr && "clang decl context is null");

            old_ctx_ = conv_ctx_->decl_ctx();;
            old_clang_ctx_ = conv_ctx_->clang_decl_ctx();

            conv_ctx_->set_decl_ctx(ctx);
            conv_ctx_->set_clang_decl_ctx(clang_ctx);
        }

        /// Restores old contexts in AST converter. Sets state of setter to invalid.
        void restore() {
            if (conv_ctx_) {
                conv_ctx_->set_decl_ctx(old_ctx_);
                conv_ctx_->set_clang_decl_ctx(old_clang_ctx_);
                old_ctx_ = nullptr;
                old_clang_ctx_ = nullptr;
            }
        }

    private:
        conv_context * conv_ctx_;                       ///< Pointer to converter context
        context * old_ctx_;                             ///< Previous context
        const ::clang::DeclContext * old_clang_ctx_;    ///< Previous clang context
    };


    /// Constructs converter context
    explicit conv_context(code_model & mdl, const ::clang::ASTContext & clang_ctx):
        mdl_{mdl}, clang_ctx_{clang_ctx} {}

    conv_context(const conv_context &) = delete;
    conv_context(conv_context &&) = delete;
    conv_context & operator=(const conv_context &) = delete;
    conv_context & operator=(conv_context &&) = delete;

    /// Returns reference to code model
    code_model & mdl() const { return mdl_; }

    /// Returns reference to clang AST context
    const ::clang::ASTContext & clang_ctx() const { return clang_ctx_; }

    /// Returns pointer to current declaration context in code model
    context * decl_ctx() const { return decl_ctx_; }

    /// Sets pointer to current declaration context in code model
    void set_decl_ctx(context * c) { decl_ctx_ = c; }

    /// Returns pointer to current clang declaration context
    const ::clang::DeclContext * clang_decl_ctx() const { return clang_decl_ctx_; }

    /// Sets pointer to current clang declaration context
    void set_clang_decl_ctx(const ::clang::DeclContext * c) { clang_decl_ctx_ = c; }

    /// Finds code model entity associated with clang declaration.
    /// First gets canonical declaration of clang declaration.
    /// Returns nullptr if associated context_entity not found
    context_entity * get_cm_entity(const ::clang::Decl * clang_decl) {
        auto canon_decl = clang_decl->getCanonicalDecl();

        auto it = decls_.find(canon_decl);
        if (it == decls_.end()) {
            return nullptr;
        }

        return it->second;
    }

    /// Finds code model enitty associated with clang declaration and converts it to
    /// specified type. Found context_entity must be convertible to specified type.
    /// Returns nullptr if context_entity is not found.
    template <typename Entity>
    Entity * get_cm_entity_as(const ::clang::Decl * clang_decl) {
        auto ent = get_cm_entity(clang_decl);
        if (!ent) {
            return nullptr;
        }

        auto casted_ent = dynamic_cast<Entity*>(ent);
        assert(casted_ent && "existing context_entity type mismatch");
        return casted_ent;
    }

    /// Adds code model context entity associated with clang declaration.
    void add_cm_entity(const ::clang::Decl * clang_decl, context_entity * cm_ent) {
        auto canon_decl = clang_decl->getCanonicalDecl();
        auto [it, inserted] = decls_.emplace(canon_decl, cm_ent);
        assert(inserted &&
               "code model context_entity is already associated with clang declaration");
    }

public:
    code_model & mdl_;                                      ///< Reference to code model
    const ::clang::ASTContext & clang_ctx_;                 ///< Reference to Clang AST context
    context * decl_ctx_ = nullptr;                          ///< Current code model context
    const ::clang::DeclContext * clang_decl_ctx_ = nullptr; ///< Current clang decl context

    /// Map from clang canonical declarations to code model entities
    std::map<const ::clang::Decl *, context_entity *> decls_;
};


}
