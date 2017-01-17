//
//  Piece.hpp -- A problem for the game
//  TodoSchool on Oct 26, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once
#include <string>
#include <istream>


namespace todoschool {
namespace lettermatching {

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

}  // namespace lettermatching
}  // namespace todoschool
