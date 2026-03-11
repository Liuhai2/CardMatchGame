/**
 * @file GameController.cpp
 */
#include "GameController.h"
#include "../configs/LevelConfigLoader.h"
#include "../configs/CardTypes.h"
#include "../services/GameModelFromLevelGenerator.h"
#include "../models/CardModel.h"
#include "cocos2d.h"

static const float kMoveDuration = 0.25f;

static int findIndexInPile(const std::vector<GameModel::CardModelPtr>& pile, int runtimeId) {
    for (size_t i = 0; i < pile.size(); ++i)
        if (pile[i] && pile[i]->getRuntimeId() == runtimeId) return (int)i;
    return -1;
}

GameView* GameController::loadLevel(const std::string& levelJsonPath) {
    config::LevelConfig cfg = config::loadLevelConfig(levelJsonPath);
    _gameModel = std::make_unique<GameModel>(services::generateGameModelFromLevel(cfg));
    if (_gameModel->getHandPile().empty()) return nullptr;

    _handTopPosition = _gameModel->getHandPile()[0]->getPosition();
    _view = GameView::createWithModel(_gameModel.get());
    if (!_view) return nullptr;

    _view->setCardClickCallback([this](int id) { onCardClicked(id); });
    _view->setUndoClickCallback([this]() { onUndoClicked(); });
    _view->setDrawClickCallback([this]() { onDrawFromReserveClicked(); });
    return _view;
}

int GameController::_getHandIndex(int runtimeId) const {
    return findIndexInPile(_gameModel->getHandPile(), runtimeId);
}

/** 在主牌堆中查找指定 runtimeId 的牌，返回其索引；-1 代表不在主牌堆 */
int GameController::_getMainCardIndex(int runtimeId) const {
    return findIndexInPile(_gameModel->getMainPile(), runtimeId);
}

int GameController::_getReserveIndex(int runtimeId) const {
    return findIndexInPile(_gameModel->getReservePile(), runtimeId);
}

/**
 * 判断某张主牌堆的牌是否"可操作"：
 * 正面朝上 且 没有被其他牌覆盖（当前实现：只要 faceUp 即可点击）
 */
bool GameController::_isMainCardClickable(int mainIndex) const {
    const auto& main = _gameModel->getMainPile();
    if (mainIndex < 0 || mainIndex >= (int)main.size()) return false;
    return main[mainIndex]->isFaceUp();
}

void GameController::_doHandReorder(int cardRuntimeId, int fromHandIndex) {
    auto& hand = _gameModel->getHandPile();
    if (fromHandIndex <= 0 || fromHandIndex >= (int)hand.size()) return;
    auto card = hand[fromHandIndex];
    cocos2d::Vec2 fromPos = card->getPosition();

    UndoRecord rec;
    rec.type = UndoActionType::HandReorder;
    rec.cardRuntimeId = cardRuntimeId;
    rec.fromPosition = fromPos;
    rec.toPosition = _handTopPosition;
    rec.fromHandIndex = fromHandIndex;
    _undoManager.pushRecord(rec);

    hand.erase(hand.begin() + fromHandIndex);
    hand.insert(hand.begin(), card);
    card->setPosition(_handTopPosition);

    _view->playCardMoveTo(cardRuntimeId, _handTopPosition, kMoveDuration);
}

void GameController::_doMatchFromMain(int cardRuntimeId, int mainIndex) {
    auto& main = _gameModel->getMainPile();
    auto& hand = _gameModel->getHandPile();
    if (hand.empty()) return;

    auto card = main[mainIndex];
    int replacedId = hand[0]->getRuntimeId();
    cocos2d::Vec2 fromPos = card->getPosition();

    main.erase(main.begin() + mainIndex);
    hand.insert(hand.begin(), card);
    card->setArea(CardArea::HandPile);
    card->setPosition(_handTopPosition);
    _view->bringCardToFront(cardRuntimeId);
    _view->playCardMoveTo(cardRuntimeId, _handTopPosition, kMoveDuration);

    // 先快照本轮哪些牌是背面，翻牌检查后取差集 = 本次被揭开的牌
    std::vector<int> faceDownBefore;
    for (const auto& c : _gameModel->getMainPile())
        if (!c->isFaceUp()) faceDownBefore.push_back(c->getRuntimeId());

    _checkAndFlipUncoveredCards();

    UndoRecord rec;
    rec.type = UndoActionType::MatchFromMain;
    rec.cardRuntimeId = cardRuntimeId;
    rec.fromPosition = fromPos;
    rec.toPosition = _handTopPosition;
    rec.replacedHandCardId = replacedId;
    for (int id : faceDownBefore) {
        CardModel* c = _gameModel->findCardById(id);
        if (c && c->isFaceUp()) rec.revealedCardIds.push_back(id);
    }
    _undoManager.pushRecord(rec);
}

void GameController::_doDrawFromReserve() {
    auto& reserve = _gameModel->getReservePile();
    auto& hand = _gameModel->getHandPile();
    if (reserve.empty()) return;
    auto card = reserve[0];
    cocos2d::Vec2 fromPos = card->getPosition();

    UndoRecord rec;
    rec.type = UndoActionType::DrawFromReserve;
    rec.drawnCardId = card->getRuntimeId();
    rec.fromPosition = fromPos;
    rec.toPosition = _handTopPosition;
    _undoManager.pushRecord(rec);

    reserve.erase(reserve.begin());
    hand.insert(hand.begin(), card);
    card->setArea(CardArea::HandPile);
    card->setFaceUp(true);
    card->setPosition(_handTopPosition);

    _view->bringCardToFront(card->getRuntimeId());
    _view->flipCardToFaceUp(card->getRuntimeId());
    _view->playCardMoveTo(card->getRuntimeId(), _handTopPosition, kMoveDuration);
}

void GameController::_checkAndFlipUncoveredCards() {
    auto& main = _gameModel->getMainPile();
    static const float kCardW = 120.f;
    static const float kCardH = 160.f;

    for (auto& card : main) {
        if (card->isFaceUp()) continue;
        bool covered = false;
        for (auto& other : main) {
            if (other.get() == card.get()) continue;
            float dy = other->getPosition().y - card->getPosition().y;
            float dx = std::abs(other->getPosition().x - card->getPosition().x);
            if (dy > 0 && dx < kCardW && dy < kCardH) {
                covered = true;
                break;
            }
        }
        if (!covered) {
            card->setFaceUp(true);
            _view->flipCardToFaceUp(card->getRuntimeId());
        }
    }
}

void GameController::onCardClicked(int cardRuntimeId) {
    if (!_gameModel || !_view) return;

    if (_getReserveIndex(cardRuntimeId) >= 0) {
        _doDrawFromReserve();
        return;
    }

    int handIdx = _getHandIndex(cardRuntimeId);
    if (handIdx > 0) {
        _doHandReorder(cardRuntimeId, handIdx);
        return;
    }

    int mainIdx = _getMainCardIndex(cardRuntimeId);
    if (mainIdx >= 0 && _isMainCardClickable(mainIdx)) {
        auto& hand = _gameModel->getHandPile();
        if (hand.empty()) return;
        int handTopFace = hand[0]->getFace();
        int mainFace = _gameModel->getMainPile()[mainIdx]->getFace();
        if (isFaceDiffOne(handTopFace, mainFace)) {
            _doMatchFromMain(cardRuntimeId, mainIdx);
        }
    }
}

void GameController::onUndoClicked() {
    if (!_gameModel || !_view || !_undoManager.canUndo()) return;
    UndoRecord rec = _undoManager.popAndApply(*_gameModel);

    switch (rec.type) {
        case UndoActionType::HandReorder:
            if (rec.cardRuntimeId >= 0)
                _view->playCardMoveTo(rec.cardRuntimeId, rec.fromPosition, kMoveDuration);
            break;

        case UndoActionType::MatchFromMain:
            if (rec.cardRuntimeId >= 0)
                _view->playCardMoveTo(rec.cardRuntimeId, rec.fromPosition, kMoveDuration);
            for (int revId : rec.revealedCardIds)
                _view->flipCardToFaceDown(revId);
            break;

        case UndoActionType::DrawFromReserve:
            if (rec.drawnCardId >= 0) {
                _view->flipCardToFaceDown(rec.drawnCardId);
                _view->playCardMoveTo(rec.drawnCardId, rec.fromPosition, kMoveDuration);
            }
            break;
    }
}

void GameController::onDrawFromReserveClicked() {
    if (!_gameModel || !_view) return;
    _doDrawFromReserve();
}
