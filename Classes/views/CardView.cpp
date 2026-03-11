/**
 * @file CardView.cpp
 */
#include "CardView.h"
#include "../models/CardModel.h"

USING_NS_CC;

/** 卡牌显示尺寸（与 card_general.png 匹配后的逻辑尺寸） */
static const float kCardW = 120.f;
static const float kCardH = 160.f;

/** 各元素在卡片内的锚点均为中心，位置相对于卡片左下角原点 */
static const Vec2 kBigNumPos  = Vec2(kCardW * 0.5f,  kCardH * 0.38f);
static const Vec2 kSuitPos    = Vec2(kCardW * 0.5f,  kCardH * 0.72f);
static const Vec2 kSmallNumPos = Vec2(kCardW * 0.22f, kCardH * 0.84f);
static const Vec2 kSmallSuitPos = Vec2(kCardW * 0.22f, kCardH * 0.70f);

static const char* kFaceNames[] = {
    "A","2","3","4","5","6","7","8","9","10","J","Q","K"
};

std::string CardView::getFaceStr(int face) {
    if (face < 0 || face > 12) return "A";
    return kFaceNames[face];
}

bool CardView::isRedSuit(int suit) {
    return (suit == 1 || suit == 2);  // 1=方块, 2=红桃
}

std::string CardView::getSuitFileName(int suit) {
    switch (suit) {
        case 0: return "res/suits/club.png";
        case 1: return "res/suits/diamond.png";
        case 2: return "res/suits/heart.png";
        case 3: return "res/suits/spade.png";
        default: return "res/suits/club.png";
    }
}

CardView* CardView::createWithModel(const CardModel* model) {
    if (!model) return nullptr;
    auto view = new (std::nothrow) CardView();
    if (view && view->init()) {
        view->autorelease();
        view->_model = model;
        view->setContentSize(Size(kCardW, kCardH));
        view->setAnchorPoint(Vec2(0.5f, 0.5f));
        view->setPosition(model->getPosition());
        if (model->isFaceUp()) {
            view->buildFaceUp(model->getFace(), model->getSuit());
        } else {
            view->buildFaceDown();
        }
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

void CardView::buildFaceUp(int face, int suit) {
    auto bg = Sprite::create("res/card_general.png");
    if (bg) {
        bg->setContentSize(Size(kCardW, kCardH));
        bg->setAnchorPoint(Vec2(0.5f, 0.5f));
        bg->setPosition(Vec2(kCardW * 0.5f, kCardH * 0.5f));
        addChild(bg, 0);
    }
    std::string color = isRedSuit(suit) ? "red" : "black";
    std::string faceStr = getFaceStr(face);

    auto bigNum = Sprite::create("res/number/big_" + color + "_" + faceStr + ".png");
    if (bigNum) {
        bigNum->setAnchorPoint(Vec2(0.5f, 0.5f));
        bigNum->setPosition(kBigNumPos);
        addChild(bigNum, 1);
    }
    auto suitIcon = Sprite::create(getSuitFileName(suit));
    if (suitIcon) {
        suitIcon->setAnchorPoint(Vec2(0.5f, 0.5f));
        suitIcon->setPosition(kSuitPos);
        suitIcon->setScale(0.6f);
        addChild(suitIcon, 1);
    }
    auto smallNum = Sprite::create("res/number/small_" + color + "_" + faceStr + ".png");
    if (smallNum) {
        smallNum->setAnchorPoint(Vec2(0.5f, 0.5f));
        smallNum->setPosition(kSmallNumPos);
        addChild(smallNum, 1);
    }
    auto smallSuit = Sprite::create(getSuitFileName(suit));
    if (smallSuit) {
        smallSuit->setAnchorPoint(Vec2(0.5f, 0.5f));
        smallSuit->setPosition(kSmallSuitPos);
        smallSuit->setScale(0.3f);
        addChild(smallSuit, 1);
    }
}

void CardView::buildFaceDown() {
    auto bg = Sprite::create("res/card_general.png");
    if (bg) {
        bg->setContentSize(Size(kCardW, kCardH));
        bg->setAnchorPoint(Vec2(0.5f, 0.5f));
        bg->setPosition(Vec2(kCardW * 0.5f, kCardH * 0.5f));
        bg->setColor(Color3B(70, 130, 200));
        addChild(bg, 0);
    }
}

void CardView::flipToFaceUp() {
    if (!_model) return;
    removeAllChildren();
    buildFaceUp(_model->getFace(), _model->getSuit());
}

void CardView::flipToFaceDown() {
    removeAllChildren();
    buildFaceDown();
}

void CardView::playMoveToAnimation(const cocos2d::Vec2& targetPos, float duration) {
    runAction(MoveTo::create(duration, targetPos));
}

int CardView::getRuntimeId() const {
    return _model ? _model->getRuntimeId() : -1;
}
