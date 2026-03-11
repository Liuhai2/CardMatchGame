/**
 * @file GameModelFromLevelGenerator.h
 * @brief 无状态服务：从关卡配置生成运行时 GameModel，不持有数据
 */
#pragma once

#include "../configs/LevelConfig.h"
#include "../models/GameModel.h"

namespace services {

/**
 * 从关卡配置生成游戏数据模型
 * 主牌堆：第一张翻开，其余覆盖；底牌堆第一张翻开；备用堆全部覆盖
 * @param levelConfig 关卡静态配置
 * @return 填充好的 GameModel
 */
GameModel generateGameModelFromLevel(const config::LevelConfig& levelConfig);

} // namespace services
