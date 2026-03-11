/**
 * @file UndoModel.h
 * @brief 单步回退记录：操作类型 + 相关卡牌与位置，支持序列化
 */
#pragma once

#include "cocos2d.h"
#include <vector>

/** 回退操作类型 */
enum class UndoActionType {
    HandReorder,   /** 手牌区翻牌替换：某牌移到顶部 */
    MatchFromMain, /** 桌面牌匹配：主牌堆牌移到手牌顶部 */
    DrawFromReserve /** 从备用堆抽牌替换手牌 */
};

/** 单步回退所需数据 */
struct UndoRecord {
    UndoActionType type = UndoActionType::HandReorder;
    int cardRuntimeId = -1;       /** 被移动的牌 */
    cocos2d::Vec2 fromPosition;   /** 移动前位置 */
    cocos2d::Vec2 toPosition;     /** 移动后位置（回退时 to→from） */
    /** HandReorder 时：该牌在手牌堆中的原索引（回退时插回该位置） */
    int fromHandIndex = -1;
    /** MatchFromMain 时：被替换掉的手牌顶牌 id，用于还原手牌顺序 */
    int replacedHandCardId = -1;
    /** DrawFromReserve 时：抽出的牌 id；回退时需将该牌放回备用堆顶 */
    int drawnCardId = -1;
    /** MatchFromMain 时：由本次消牌触发自动翻开的主牌堆牌 id 列表；回退时需翻回背面 */
    std::vector<int> revealedCardIds;
};
