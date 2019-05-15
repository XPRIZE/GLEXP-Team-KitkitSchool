//
//  CustomFacet.h -- Customize separator for streams.
//  TodoSchool on Oct 31, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "CustomFacet.h"
#include <array>
#include <numeric>


namespace todoschool {

CustomFacet::CustomFacet(const std::locale& loc) : Base(table_)
{
    auto& ctype = std::use_facet<Base>(loc);
    
 //   std::array<char, 256> targets;
 //   std::iota(targets.begin(), targets.end(), 0);
 //   ctype.is(targets.begin(), targets.end(), table_);

	char targets[256];
	for (int i = 0; i < 256; i++) targets[i] = i;
	ctype.is(&targets[0], &targets[255], table_);


}

void CustomFacet::setSpaceFlagsFor(const std::string& chars) {
    for (auto c : chars)
        table_[c] |= Base::space;
}

void CustomFacet::clearSpaceFlagsFor(const std::string& chars) {
    for (auto c : chars)
        table_[c] &= (~Base::space);
}

}  // namespace todoschool
