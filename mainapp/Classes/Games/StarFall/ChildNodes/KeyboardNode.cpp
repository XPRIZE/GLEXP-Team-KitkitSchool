//
//  KeyboardNode.cpp on Jul 25, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "KeyboardNode.h"
#include "../Utils/StarFallDepot.h"


BEGIN_NS_STARFALL

namespace {
    string contentSkin() {
        return StarFallDepot().assetPrefix() + "/KeyboardNode/bottom_woodpanel.png";
    }
    Size contentSize() {
        return Size(2104.f, 650.f);
    }
}  // unnamed namespace


KeyboardNode::KeyboardNode() {
}

bool KeyboardNode::init() {
    if (!Super::init()) { return false; }

    clearInternals();
    refreshChildNodes();
    return true;
}

void KeyboardNode::clearInternals() {
    StarFallDepot Depot;

    // NB(xenosoz, 2016): Default values.

    setContentSize(contentSize());
    SymbolLayout.update(Depot.defaultSymbolLayout());
    EnabledSymbols.update(Depot.defaultEnabledSymbolSet());
    
    SymbolLayout.OnValueUpdate = [this](SymbolLayoutType&) {
        refreshChildNodes();
    };
    EnabledSymbols.OnValueUpdate = [this](SymbolSetType&) {
        // XXX: We can just update enabled/disabled status here.
        refreshChildNodes();
    };
}

void KeyboardNode::refreshChildNodes()
{
    removeAllChildren();
    SymbolToKeyNode.clear();

    Size CS = getContentSize();

    if (true) {
        // NB(xenosoz, 2016): Create background node.
        auto It = Sprite::create(contentSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Point(CS.width / 2.f, 0.f));
        
        addChild(It);
    }

    // NB(xenosoz, 2016): Create KeyNodes.
    Size KeyNodeSize = KeyNode::defaultSize();
    Rect KeyboardNodeRect;

    size_t MaxColCount = max_element(SymbolLayout().begin(),
                                     SymbolLayout().end(),
                                     [] (const SymbolRowType& L, const SymbolRowType& R) {
                                         return L.size() < R.size();
                                     })->size();

    for (auto& Row : SymbolLayout())
        MaxColCount = max(MaxColCount, Row.size());

    for (size_t RowID = 0; RowID < SymbolLayout().size(); ++RowID)
    {
        const SymbolRowType& SymbolRow = SymbolLayout()[RowID];
        size_t ColIDMargin = MaxColCount - SymbolRow.size();
        
        for (size_t ColID = 0; ColID < SymbolRow.size(); ++ColID)
        {
            const SymbolType& Symbol = SymbolRow[ColID];
            
            KeyNode* KN = ([&] {
                float X = (ColID + ColIDMargin / 2.f) * (KeyNodeSize.width + 16.f);
                float Y = (SymbolLayout().size()-1-RowID) * (KeyNodeSize.height+12.f) + 8.f;
                bool Enabled = (EnabledSymbols().find(Symbol) != EnabledSymbols().end());
                
                Rect KeyNodeRect(X, Y, KeyNodeSize.width, KeyNodeSize.height);
                KeyboardNodeRect = KeyboardNodeRect.unionWithRect(KeyNodeRect);
                
                KeyNode* It = KeyNode::create();
                It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                It->setPosition(Point(X, Y));
                
                It->Symbol.update(Symbol);
                It->OnKeyPressed = [this](KeyNode& Sender) {
                    auto Guard = ScopeGuard([this] { retain(); },
                                            [this] { release(); });
                    if (OnKeyPressed)
                        OnKeyPressed(*this, Sender.Symbol());
                };
                It->Enabled.update(Enabled);
                addChild(It);
                return It;
            }());
            
            SymbolToKeyNode.emplace(Symbol, KN);
        }
    }
}

END_NS_STARFALL