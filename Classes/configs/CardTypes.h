/**
 * @file CardTypes.h
 * @brief 纸牌花色与点数枚举（与 Thrift/配置约定一致，便于扩展）
 * 职责：静态类型定义，供 configs/models 使用
 */
#pragma once

/** 花色枚举：梅花、方块、红桃、黑桃 */
enum CardSuitType {
    kCSTNone = -1,
    kCSTClubs = 0,     /** 梅花 */
    kCSTDiamonds = 1,  /** 方块 */
    kCSTHearts = 2,    /** 红桃 */
    kCSTSpades = 3     /** 黑桃 */
};

/** 点数枚举：A-K，内部用 0-12 表示 */
enum CardFaceType {
    kCFTNone = -1,
    kCFTAce = 0, kCFTTwo = 1, kCFTThree = 2, kCFTFour = 3,
    kCFTFive = 4, kCFTSix = 5, kCFTSeven = 6, kCFTEight = 7,
    kCFTNine = 8, kCFTTen = 9, kCFTJack = 10, kCFTQueen = 11, kCFTKing = 12
};

/** 点数数量 */
const int kCardFaceCount = 13;

/** 判断两点数是否相差 1（可匹配） */
inline bool isFaceDiffOne(int faceA, int faceB) {
    int d = faceA - faceB;
    return (d == 1 || d == -1);
}
