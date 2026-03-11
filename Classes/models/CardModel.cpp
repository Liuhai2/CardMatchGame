/**
 * @file CardModel.cpp
 */
#include "CardModel.h"

int CardModel::_nextRuntimeId = 0;

CardModel::CardModel(int face, int suit, const cocos2d::Vec2& position)
    : _face(face), _suit(suit), _position(position), _isFaceUp(false),
      _area(CardArea::MainPile), _runtimeId(_nextRuntimeId++) {}
