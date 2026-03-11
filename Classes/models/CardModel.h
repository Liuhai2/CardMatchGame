/**
 * @file CardModel.h
 * @brief 单张卡牌运行时数据模型，支持序列化/反序列化
 */
#pragma once

#include "cocos2d.h"

/** 牌所在区域 */
enum class CardArea {
    MainPile,   /** 主牌堆（桌面） */
    HandPile,   /** 底牌堆（手牌区） */
    ReservePile /** 备用牌堆 */
};

/**
 * 卡牌模型：点数、花色、位置、是否翻开、所属区域
 * Model 层仅存储数据，无业务逻辑
 */
class CardModel {
public:
    CardModel(int face, int suit, const cocos2d::Vec2& position);

    int getFace() const { return _face; }
    int getSuit() const { return _suit; }
    cocos2d::Vec2 getPosition() const { return _position; }
    bool isFaceUp() const { return _isFaceUp; }
    CardArea getArea() const { return _area; }

    void setPosition(const cocos2d::Vec2& pos) { _position = pos; }
    void setFaceUp(bool up) { _isFaceUp = up; }
    void setArea(CardArea area) { _area = area; }

    /** 唯一 ID，用于回退时定位 */
    int getRuntimeId() const { return _runtimeId; }
    static void setNextRuntimeId(int id) { _nextRuntimeId = id; }
    static int getNextRuntimeId() { return _nextRuntimeId; }

private:
    int _face;
    int _suit;
    cocos2d::Vec2 _position;
    bool _isFaceUp;
    CardArea _area;
    int _runtimeId;
    static int _nextRuntimeId;
};
