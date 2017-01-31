//
//  Piece.cpp -- A Piece for the game
//  TodoSchool on Oct 26, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Piece.hpp"
#include <cocos/cocos2d.h>

using namespace std;
using namespace cocos2d;


namespace todoschool {
namespace lettermatching {

Piece::Piece() {
}

Piece::Piece(int pieceID,
             const std::string& matchSound,
             const std::string& imageNameA,
             const std::string& imageNameB)
: pieceID(pieceID)
, matchSound(matchSound)
, imageNameA(imageNameA)
, imageNameB(imageNameB)
{
}

Piece Piece::fromInputStream(istream &stream) {
    int pieceID;
    string matchSound;
    string imageNameA;
    string imageNameB;
    
    if (!(stream >> pieceID >> matchSound >> imageNameA >> imageNameB)) {
        CCLOGERROR("Unexpected end of stream in %s", __PRETTY_FUNCTION__);
        return Piece();
    }
    
    // NB(xenosoz, 2016): Ignore the piece ID.
    return Piece(pieceID, matchSound, imageNameA, imageNameB);
}

istream& operator>>(istream& stream, Piece& Piece) {
    Piece = Piece::fromInputStream(stream);
    return stream;
}

}  // namespace lettermatching
}  // namespace todoschool
