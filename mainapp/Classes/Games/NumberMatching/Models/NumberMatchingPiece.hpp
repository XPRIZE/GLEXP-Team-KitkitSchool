//
//  Piece.hpp -- A card piece setting
//  TodoSchool on Oct 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once
#include <istream>


namespace todoschool {
namespace numbermatching {

class Piece {
public:
    int pieceID;
    std::string matchSound;
    std::string imageNameA;
    std::string imageNameB;
    
public:
    Piece();
    Piece(int pieceID,
          const std::string& matchSound,
          const std::string& imageNameA,
          const std::string& imageNameB);
    
public:
    static Piece fromInputStream(std::istream& stream);
    friend std::istream& operator>>(std::istream& stream, Piece& problem);
};

}  // namespace numbermatching
}  // namespace todoschool
