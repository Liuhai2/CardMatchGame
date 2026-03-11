/**
 * @file CardView.h
 * @brief 单张卡牌视图：由底图 + 大数字 + 花色图标 + 小数字组合而成
 * 正面翻开时显示完整内容，覆盖状态显示蓝色背面
 */
#pragma once

#include "cocos2d.h"

class CardModel;

/**
 * 卡牌视图：继承 Node，组合多张子 Sprite
 * 不含业务逻辑，仅负责展示与动画
 */
class CardView : public cocos2d::Node {
public:
    /** 根据模型创建视图，位置与翻面状态均由模型决定 */
    static CardView* createWithModel(const CardModel* model);

    /** 播放平移到目标位置的动画 */
    void playMoveToAnimation(const cocos2d::Vec2& targetPos, float duration);

    /** 将卡牌视图翻转为正面（清除背面子节点，重建正面内容） */
    void flipToFaceUp();
    /** 将卡牌视图翻转为背面（清除正面子节点，重建背面内容） */
    void flipToFaceDown();

    /** 获取绑定的卡牌 runtimeId，供触摸识别使用 */
    int getRuntimeId() const;

private:
    const CardModel* _model = nullptr;

    void buildFaceUp(int face, int suit);
    void buildFaceDown();

    static std::string getFaceStr(int face);
    static bool isRedSuit(int suit);
    static std::string getSuitFileName(int suit);
};
