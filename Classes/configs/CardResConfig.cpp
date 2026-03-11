/**
 * @file CardResConfig.cpp
 */
#include "CardResConfig.h"

namespace config {

std::string getCardTextureName(int /*face*/, int /*suit*/) {
    /** 暂无卡牌图集时使用占位图；扩展时可按 face*4+suit 或图集 key 返回 */
    return "HelloWorld.png";
}

} // namespace config
