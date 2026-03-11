/**
 * @file LevelConfig.h
 * @brief 关卡静态配置模型：主牌堆、底牌堆、备用牌堆的卡牌与位置
 */
#pragma once

#include "cocos2d.h"
#include <vector>
#include <string>

namespace config {

/** 单张牌在配置中的描述 */
struct CardEntry {
    int face = 0;        /** 点数 0-12 */
    int suit = 0;        /** 花色 0-3 */
    cocos2d::Vec2 position;
    bool faceUp = false; /** 初始是否正面朝上 */
};

/**
 * 关卡配置：从 JSON 解析后的只读结构
 * Playfield=主牌堆, Hand=底牌堆(初始1张翻开), Reserve=备用牌堆
 */
struct LevelConfig {
    std::vector<CardEntry> playfield;
    std::vector<CardEntry> hand;
    std::vector<CardEntry> reserve;
};

} // namespace config
