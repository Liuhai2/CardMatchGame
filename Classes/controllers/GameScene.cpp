/**
 * @file GameScene.cpp
 */
#include "GameScene.h"
#include "GameController.h"

USING_NS_CC;

/** 默认关卡配置路径（相对 Resources） */
static const char* kDefaultLevelPath = "level.json";

Scene* GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::init()) return false;
    _controller = std::make_unique<GameController>();
    GameView* view = _controller->loadLevel(kDefaultLevelPath);
    if (view) addChild(view);
    return true;
}