// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file log.hpp
/// Contains macro definitions for logging in clang code model converter

#pragma once

#include "cm/log/log.hpp"
#include "utils.hpp"


#define CM_CLANG_LOG_TRACE          CM_LOG(cm-cxx-clang, ::boost::log::trivial::trace)
#define CM_CLANG_LOG_DEBUG          CM_LOG(cm-cxx-clang, ::boost::log::trivial::debug)
#define CM_CLANG_LOG_INFO           CM_LOG(cm-cxx-clang, ::boost::log::trivial::info)
#define CM_CLANG_LOG_WARNING        CM_LOG(cm-cxx-clang, ::boost::log::trivial::warning)
#define CM_CLANG_LOG_ERROR          CM_LOG(cm-cxx-clang, ::boost::log::trivial::error)
#define CM_CLANG_LOG_FATAL          CM_LOG(cm-cxx-clang, ::boost::log::trivial::fatal)

#define CM_CLANG_LOG_SCAT_TRACE(scat)   CM_LOG_SCAT(cm-cxx-clang, scat, ::boost::log::trivial::trace)
#define CM_CLANG_LOG_SCAT_DEBUG(scat)   CM_LOG_SCAT(cm-cxx-clang, scat, ::boost::log::trivial::debug)
#define CM_CLANG_LOG_SCAT_INFO(scat)    CM_LOG_SCAT(cm-cxx-clang, scat, ::boost::log::trivial::info)
#define CM_CLANG_LOG_SCAT_WARNING(scat) CM_LOG_SCAT(cm-cxx-clang, scat, ::boost::log::trivial::warning)
#define CM_CLANG_LOG_SCAT_ERROR(scat)   CM_LOG_SCAT(cm-cxx-clang, scat, ::boost::log::trivial::error)
#define CM_CLANG_LOG_SCAT_FATAL(scat)   CM_LOG_SCAT(cm-cxx-clang, scat, ::boost::log::trivial::fatal)

#define CM_CLANG_LOG_DECL(msg, decl) \
    CM_CLANG_LOG_DEBUG << msg << ": " << clang_decl_desc(decl); \
    CM_CLANG_LOG_TRACE << "\n" << dump_decl_to_string(decl)

#define CM_CLANG_LOG_DECL_ERROR(msg, decl) \
    CM_CLANG_LOG_ERROR << msg << ": " << clang_decl_desc(decl); \
    CM_CLANG_LOG_TRACE << "\n" << dump_decl_to_string(decl)

#define CM_CLANG_LOG_SCAT_DECL(scat, msg, decl) \
    CM_CLANG_LOG_SCAT_DEBUG(scat) << msg << ": " << clang_decl_desc(decl); \
    CM_CLANG_LOG_SCAT_TRACE(scat) << "\n" << dump_decl_to_string(decl)

#define CM_CLANG_LOG_SCAT_DECL_ERROR(scat, msg, decl) \
    CM_CLANG_LOG_SCAT_ERROR(scat) << msg << ": " << clang_decl_desc(decl); \
    CM_CLANG_LOG_SCAT_TRACE(scat) << "\n" << dump_decl_to_string(decl)
