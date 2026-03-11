/**
 * @file GameController.h
 * @brief 游戏控制器：协调 Model 与 View，处理点击/回退/抽牌逻辑
 */
#pragma once

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../models/UndoModel.h"
#include "../managers/UndoManager.h"
#include "../views/GameView.h"
#include <memory>
#include <string>

/**
 * 游戏控制器：持有 Model、UndoManager、View，处理用户操作与业务规则
 */
class GameController {
public:
    GameController() = default;

    /**
    
     * 加载关卡并生成 GameModel、创建 GameView
     * @param levelJsonPath 关卡 JSON 路径（Resources 相对路径）
     * @return 创建好的 GameView，调用方加入场景；失败返回 nullptr
     */
    GameView* loadLevel(const std::string& levelJsonPath);

    /** 卡牌被点击时由 View 回调 */
    void onCardClicked(int cardRuntimeId);

    /** 回退按钮点击 */
    void onUndoClicked();

    /** 从备用堆抽牌替换手牌（可由 View 的 Draw 按钮触发） */
    void onDrawFromReserveClicked();

    GameModel* getModel() { return _gameModel.get(); }
    const GameModel* getModel() const { return _gameModel.get(); }
    GameView* getView() { return _view; }

private:
    std::unique_ptr<GameModel> _gameModel;
    UndoManager _undoManager;
    GameView* _view = nullptr;

    /** 手牌顶部显示位置（与配置中第一张手牌一致，或首张手牌位置） */
    cocos2d::Vec2 _handTopPosition;

    int _getHandIndex(int runtimeId) const;
    int _getMainCardIndex(int runtimeId) const;
    int _getReserveIndex(int runtimeId) const;
    bool _isMainCardClickable(int mainIndex) const;
    void _doHandReorder(int cardRuntimeId, int fromHandIndex);
    void _doMatchFromMain(int cardRuntimeId, int mainIndex);
    void _doDrawFromReserve();
    /** 移除一张主牌堆牌后，检查并翻开所有不再被覆盖的牌 */
    void _checkAndFlipUncoveredCards();
};
