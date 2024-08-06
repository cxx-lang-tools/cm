// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file convert_loc.cpp
/// Contains implementations of functions for converting clang source locations.

#include "pch.hpp"

#include "convert_loc.hpp"
#include <clang/Basic/SourceManager.h>


namespace cm::cxx::clang {


source_location convert_loc(const conv_context & ctx, const ::clang::SourceLocation & loc) {
    auto ploc = ctx.clang_ctx().getSourceManager().getPresumedLoc(loc);
    if (!ploc.isValid()) {
        return {};
    }

    auto file = ctx.mdl().source(ploc.getFilename());
    return source_location{file, ploc.getLine(), ploc.getColumn()};
}


}
