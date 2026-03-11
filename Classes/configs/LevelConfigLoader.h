/**
 * @file LevelConfigLoader.h
 * @brief 加载 JSON 关卡配置，填充 LevelConfig
 */
#pragma once

#include "LevelConfig.h"
#include <string>

namespace config {

/** 从 fileName（Resources 相对路径）加载关卡配置，失败时返回的 playfield/hand/reserve 为空 */
LevelConfig loadLevelConfig(const std::string& fileName);

} // namespace config
