/**
 * @file GameModel.cpp
 * @brief 游戏核心数据模型实现
 */
#include "GameModel.h"

CardModel* GameModel::findCardById(int runtimeId) {
    for (auto& p : _mainPile)   if (p && p->getRuntimeId() == runtimeId) return p.get();
    for (auto& p : _handPile)   if (p && p->getRuntimeId() == runtimeId) return p.get();
    for (auto& p : _reservePile) if (p && p->getRuntimeId() == runtimeId) return p.get();
    return nullptr;
}
