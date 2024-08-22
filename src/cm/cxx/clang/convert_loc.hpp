// Copyright (c) 2024, Alexandr Esilevich
// 
// Distributed under the BSD 2-Clause License.
// See accompanying file LICENSE for license information.
//

/// \file convert_loc.hpp
/// Contains definitions of functions for converting clang source locations

#pragma once

#include "converter_impl.hpp"


namespace cm::cxx::clang {


/// Converts source location
source_location convert_loc(const converter_impl & conv, const ::clang::SourceLocation & loc);


}
