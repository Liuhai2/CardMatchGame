/**
 * @file GameModelFromLevelGenerator.cpp
 */
#include "GameModelFromLevelGenerator.h"
#include "../models/CardModel.h"

namespace services {

static void fillPile(const std::vector<config::CardEntry>& entries,
    std::vector<GameModel::CardModelPtr>& pile,
    CardArea area) {
    for (const auto& e : entries) {
        auto card = std::make_shared<CardModel>(e.face, e.suit, e.position);
        card->setArea(area);
        card->setFaceUp(e.faceUp);
        pile.push_back(card);
    }
}

GameModel generateGameModelFromLevel(const config::LevelConfig& levelConfig) {
    GameModel model;
    fillPile(levelConfig.playfield, model.getMainPile(), CardArea::MainPile);
    fillPile(levelConfig.hand, model.getHandPile(), CardArea::HandPile);
    fillPile(levelConfig.reserve, model.getReservePile(), CardArea::ReservePile);
    return model;
}

} // namespace services
