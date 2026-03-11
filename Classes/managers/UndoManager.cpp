/**
 * @file UndoManager.cpp
 */
#include "UndoManager.h"
#include "../models/CardModel.h"

void UndoManager::pushRecord(const UndoRecord& record) {
    _undoStack.push_back(record);
}

static int findIndexById(const std::vector<GameModel::CardModelPtr>& pile, int runtimeId) {
    for (size_t i = 0; i < pile.size(); ++i)
        if (pile[i] && pile[i]->getRuntimeId() == runtimeId) return (int)i;
    return -1;
}

static GameModel::CardModelPtr removeFromPile(std::vector<GameModel::CardModelPtr>& pile, int runtimeId) {
    int idx = findIndexById(pile, runtimeId);
    if (idx < 0) return nullptr;
    auto card = pile[idx];
    pile.erase(pile.begin() + idx);
    return card;
}

void UndoManager::applyHandReorderUndo(GameModel& model, const UndoRecord& rec) {
    auto& hand = model.getHandPile();
    int idx = findIndexById(hand, rec.cardRuntimeId);
    if (idx < 0 || rec.fromHandIndex < 0) return;
    auto card = hand[idx];
    card->setPosition(rec.fromPosition);
    hand.erase(hand.begin() + idx);
    hand.insert(hand.begin() + rec.fromHandIndex, card);
}

void UndoManager::applyMatchFromMainUndo(GameModel& model, const UndoRecord& rec) {
    auto card = removeFromPile(model.getHandPile(), rec.cardRuntimeId);
    if (!card) return;
    card->setPosition(rec.fromPosition);
    card->setArea(CardArea::MainPile);
    model.getMainPile().insert(model.getMainPile().begin(), card);

    // 将本次消牌时自动翻开的所有牌翻回背面
    for (int revId : rec.revealedCardIds) {
        CardModel* revCard = model.findCardById(revId);
        if (revCard) revCard->setFaceUp(false);
    }

    auto& hand = model.getHandPile();
    int repIdx = findIndexById(hand, rec.replacedHandCardId);
    if (repIdx > 0) {
        auto rep = hand[repIdx];
        hand.erase(hand.begin() + repIdx);
        hand.insert(hand.begin(), rep);
    }
}

void UndoManager::applyDrawFromReserveUndo(GameModel& model, const UndoRecord& rec) {
    auto card = removeFromPile(model.getHandPile(), rec.drawnCardId);
    if (!card) return;
    card->setPosition(rec.fromPosition);
    card->setArea(CardArea::ReservePile);
    card->setFaceUp(false);   // 抽牌时翻开，回退时还原为背面
    model.getReservePile().insert(model.getReservePile().begin(), card);
}

UndoRecord UndoManager::popAndApply(GameModel& gameModel) {
    UndoRecord invalid;
    invalid.cardRuntimeId = -1;
    if (_undoStack.empty()) return invalid;
    UndoRecord rec = _undoStack.back();
    _undoStack.pop_back();
    switch (rec.type) {
        case UndoActionType::HandReorder:
            applyHandReorderUndo(gameModel, rec);
            break;
        case UndoActionType::MatchFromMain:
            applyMatchFromMainUndo(gameModel, rec);
            break;
        case UndoActionType::DrawFromReserve:
            applyDrawFromReserveUndo(gameModel, rec);
            break;
    }
    return rec;
}
