/**
 * @file GameModel.h
 * @brief 游戏运行时数据：主牌堆、底牌堆、备用牌堆的卡牌列表及状态
 */
#pragma once

#include "CardModel.h"
#include "UndoModel.h"
#include <vector>
#include <memory>

/**
 * 游戏核心数据模型
 * 主牌堆：桌面待消除，从上到下为索引 0 为顶部
 * 底牌堆：手牌，索引 0 为顶部（当前可匹配的牌）
 * 备用牌堆：索引 0 为顶部（下次可抽的牌）
 */
class GameModel {
public:
    using CardModelPtr = std::shared_ptr<CardModel>;

    /** 主牌堆（桌面），顶部可操作 */
    std::vector<CardModelPtr>& getMainPile() { return _mainPile; }
    const std::vector<CardModelPtr>& getMainPile() const { return _mainPile; }

    /** 底牌堆（手牌），顶部一张为当前牌 */
    std::vector<CardModelPtr>& getHandPile() { return _handPile; }
    const std::vector<CardModelPtr>& getHandPile() const { return _handPile; }

    /** 备用牌堆 */
    std::vector<CardModelPtr>& getReservePile() { return _reservePile; }
    const std::vector<CardModelPtr>& getReservePile() const { return _reservePile; }

    /** 根据 runtimeId 查找卡牌（任意堆） */
    CardModel* findCardById(int runtimeId);

private:
    std::vector<CardModelPtr> _mainPile;
    std::vector<CardModelPtr> _handPile;
    std::vector<CardModelPtr> _reservePile;
};
