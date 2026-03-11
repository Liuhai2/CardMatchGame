/**
 * @file UndoManager.h
 * @brief 回退管理器：维护回退栈，执行逆序回退，禁止单例、禁止反向依赖 Controller
 */
#pragma once

#include "../models/UndoModel.h"
#include "../models/GameModel.h"
#include <vector>

/**
 * 回退管理器：作为 Controller 成员，持有并加工回退记录
 * 不持有 Controller 引用，仅通过 GameModel 与 UndoRecord 操作数据
 */
class UndoManager {
public:
    UndoManager() = default;

    /** 记录一次操作，供后续回退 */
    void pushRecord(const UndoRecord& record);

    /** 是否可回退 */
    bool canUndo() const { return !_undoStack.empty(); }

    /**
     * 执行一次回退：弹出栈顶记录，根据类型还原模型状态（牌位置/堆顺序）
     * @param gameModel 游戏模型，用于查找卡牌并还原
     * @return 被回退的 UndoRecord，供 View 播放反向动画；若不可回退则 type 为无效
     */
    UndoRecord popAndApply(GameModel& gameModel);

private:
    std::vector<UndoRecord> _undoStack;

    void applyHandReorderUndo(GameModel& model, const UndoRecord& rec);
    void applyMatchFromMainUndo(GameModel& model, const UndoRecord& rec);
    void applyDrawFromReserveUndo(GameModel& model, const UndoRecord& rec);
};
