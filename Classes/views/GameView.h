/**
 * @file GameView.h
 * @brief 游戏主视图：布局主牌区/手牌区/备用区，捕获点击并回调 Controller
 */
#pragma once

#include "cocos2d.h"
#include "CardView.h"
#include <functional>
#include <unordered_map>

class GameModel;  // 仅持有 const 指针，前置声明

/**
 * 游戏视图层：仅负责 UI 展示与输入捕获，通过回调通知 Controller
 * 可持有 const 的模型指针用于初始布局
 */
class GameView : public cocos2d::Layer {
public:
    /** 卡牌被点击时的回调参数：卡牌 runtimeId */
    using CardClickCallback = std::function<void(int cardRuntimeId)>;
    /** 回退按钮回调 */
    using UndoClickCallback = std::function<void()>;
    /** 从备用堆抽牌回调 */
    using DrawClickCallback = std::function<void()>;

    static GameView* createWithModel(const GameModel* model);

    virtual bool init() override;

    /** 设置卡牌点击回调（由 Controller 注册） */
    void setCardClickCallback(CardClickCallback cb) { _cardClickCb = std::move(cb); }
    /** 设置回退按钮回调 */
    void setUndoClickCallback(UndoClickCallback cb) { _undoClickCb = std::move(cb); }
    void setDrawClickCallback(DrawClickCallback cb) { _drawClickCb = std::move(cb); }

    /** 播放指定卡牌平移到目标位置的动画 */
    void playCardMoveTo(int cardRuntimeId, const cocos2d::Vec2& toPos, float duration = 0.25f);

    /** 将指定卡牌视图刷新为正面朝上（用于抽牌/翻牌后更新显示） */
    void flipCardToFaceUp(int cardRuntimeId);
    /** 将指定卡牌视图刷新为背面朝上（回退时恢复被覆盖的牌） */
    void flipCardToFaceDown(int cardRuntimeId);

    /** 将指定卡牌提升到所有卡牌最顶层（手牌入场时调用） */
    void bringCardToFront(int cardRuntimeId);

    /** 根据 runtimeId 获取 CardView，用于回退时播放反向动画 */
    CardView* getCardViewByRuntimeId(int runtimeId) const;

private:
    const GameModel* _model = nullptr;
    std::unordered_map<int, CardView*> _cardViews;
    int _topCardZOrder = 1;
    CardClickCallback _cardClickCb;
    UndoClickCallback _undoClickCb;
    DrawClickCallback _drawClickCb;

    void buildCardViews();
    bool onTouchCard(cocos2d::Touch* touch, cocos2d::Event* event);
};
