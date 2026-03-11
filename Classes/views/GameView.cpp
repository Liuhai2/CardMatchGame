/**
 * @file GameView.cpp
 */
#include "GameView.h"
#include "../models/GameModel.h"

USING_NS_CC;

const float kUndoButtonMargin = 80.f;

GameView* GameView::createWithModel(const GameModel* model) {
    auto view = new (std::nothrow) GameView();
    if (view && view->init()) {
        view->autorelease();
        view->_model = model;
        view->buildCardViews();

        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = [view](Touch* t, Event* e) {
            return view->onTouchCard(t, e);
        };
        listener->setSwallowTouches(false);
        view->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, view);

        auto undoItem = MenuItemFont::create("Undo", [view](Ref*) {
            if (view->_undoClickCb) view->_undoClickCb();
        });
        auto drawItem = MenuItemFont::create("Draw", [view](Ref*) {
            if (view->_drawClickCb) view->_drawClickCb();
        });
        if (undoItem && drawItem) {
            auto vis = Director::getInstance()->getVisibleSize();
            undoItem->setPosition(Vec2(vis.width - kUndoButtonMargin, vis.height - kUndoButtonMargin));
            drawItem->setPosition(Vec2(vis.width - kUndoButtonMargin, vis.height - kUndoButtonMargin - 50));
            auto menu = Menu::create(undoItem, drawItem, nullptr);
            menu->setPosition(Vec2::ZERO);
            view->addChild(menu, 10);
        }
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool GameView::init() {
    if (!Layer::init()) return false;
    return true;
}

void GameView::buildCardViews() {
    if (!_model) return;
    auto addFromPile = [this](const std::vector<GameModel::CardModelPtr>& pile) {
        for (const auto& ptr : pile) {
            if (!ptr) continue;
            auto* cv = CardView::createWithModel(ptr.get());
            if (cv) {
                _cardViews[ptr->getRuntimeId()] = cv;
                addChild(cv);
            }
        }
    };
    addFromPile(_model->getMainPile());
    addFromPile(_model->getHandPile());
    addFromPile(_model->getReservePile());
}

bool GameView::onTouchCard(cocos2d::Touch* touch, cocos2d::Event* /*event*/) {
    Vec2 loc = touch->getLocation();
    for (auto& kv : _cardViews) {
        auto* card = kv.second;
        if (card->getBoundingBox().containsPoint(convertToNodeSpace(loc))) {
            if (_cardClickCb) _cardClickCb(kv.first);
            return true;
        }
    }
    return false;
}

void GameView::playCardMoveTo(int cardRuntimeId, const cocos2d::Vec2& toPos, float duration) {
    auto it = _cardViews.find(cardRuntimeId);
    if (it != _cardViews.end() && it->second)
        it->second->playMoveToAnimation(toPos, duration);
}

void GameView::flipCardToFaceUp(int cardRuntimeId) {
    auto it = _cardViews.find(cardRuntimeId);
    if (it != _cardViews.end() && it->second)
        it->second->flipToFaceUp();
}

void GameView::flipCardToFaceDown(int cardRuntimeId) {
    auto it = _cardViews.find(cardRuntimeId);
    if (it != _cardViews.end() && it->second)
        it->second->flipToFaceDown();
}

void GameView::bringCardToFront(int cardRuntimeId) {
    auto it = _cardViews.find(cardRuntimeId);
    if (it != _cardViews.end() && it->second)
        it->second->setLocalZOrder(++_topCardZOrder);
}

CardView* GameView::getCardViewByRuntimeId(int runtimeId) const {
    auto it = _cardViews.find(runtimeId);
    return it != _cardViews.end() ? it->second : nullptr;
}
