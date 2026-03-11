/**
 * @file CardResConfig.h
 * @brief 卡牌资源配置：点数/花色与贴图路径的映射（可扩展为图集键名）
 */
#pragma once

#include <string>

namespace config {

/** 根据点数、花色返回贴图资源名或图集键，未实现时返回占位图 */
std::string getCardTextureName(int face, int suit);

} // namespace config
