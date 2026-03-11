/**
 * @file LevelConfigLoader.cpp
 */
#include "LevelConfigLoader.h"
#include "cocos2d.h"
#include "json/document.h"
#include "json/rapidjson.h"

namespace config {

static void parseCardArray(const rapidjson::Value& arr, std::vector<CardEntry>& out) {
    if (!arr.IsArray()) return;
    for (rapidjson::SizeType i = 0; i < arr.Size(); ++i) {
        const auto& o = arr[i];
        if (!o.IsObject()) continue;
        CardEntry e;
        if (o.HasMember("CardFace")) e.face = o["CardFace"].GetInt();
        if (o.HasMember("CardSuit")) e.suit = o["CardSuit"].GetInt();
        if (o.HasMember("FaceUp"))   e.faceUp = o["FaceUp"].GetBool();
        if (o.HasMember("Position") && o["Position"].IsObject()) {
            const auto& p = o["Position"];
            e.position.x = p.HasMember("x") ? (float)p["x"].GetDouble() : 0;
            e.position.y = p.HasMember("y") ? (float)p["y"].GetDouble() : 0;
        }
        out.push_back(e);
    }
}

LevelConfig loadLevelConfig(const std::string& fileName) {
    LevelConfig cfg;
    std::string path = cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName);
    std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
    if (content.empty()) return cfg;

    rapidjson::Document doc;
    doc.Parse(content.c_str());
    if (doc.HasParseError()) return cfg;

    if (doc.HasMember("Playfield")) parseCardArray(doc["Playfield"], cfg.playfield);
    if (doc.HasMember("Hand")) parseCardArray(doc["Hand"], cfg.hand);
    if (doc.HasMember("Reserve")) parseCardArray(doc["Reserve"], cfg.reserve);
    /** 兼容旧格式：仅有 Stack 时当作 Reserve，且若 Hand 为空可把 Stack 第一张作为手牌 */
    if (doc.HasMember("Stack") && cfg.reserve.empty()) {
        parseCardArray(doc["Stack"], cfg.reserve);
        if (cfg.hand.empty() && !cfg.reserve.empty()) {
            cfg.hand.push_back(cfg.reserve.front());
            cfg.reserve.erase(cfg.reserve.begin());
        }
    }
    return cfg;
}

} // namespace config
