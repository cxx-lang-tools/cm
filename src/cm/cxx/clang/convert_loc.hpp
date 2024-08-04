// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file convert_loc.hpp
/// Contains definitions of functions for converting clang source locations

#pragma once

#include "cm/cxx/clang/conv_context.hpp"


namespace cm::cxx::clang {


/// Converts source location
source_location convert_loc(const conv_context & ctx, const ::clang::SourceLocation & loc);


}
