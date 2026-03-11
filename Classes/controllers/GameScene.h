/**
 * @file GameScene.h
 * @brief 游戏主场景：持有 GameController，挂载 GameView
 */
#pragma once

#include "cocos2d.h"
#include "GameController.h"
#include <memory>

/** 游戏主场景：入口场景，持有控制器以保证回调有效 */
class GameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(GameScene);
    virtual bool init() override;

private:
    std::unique_ptr<GameController> _controller;
};
