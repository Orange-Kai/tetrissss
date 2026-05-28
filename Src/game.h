#pragma once

#include <vector>
#include <raylib.h>

#include "grid.h"
#include "block.h"
#include "blocks.h"


class Game
{
public:
    Game();
    ~Game();
    void Draw(int boardOffsetX, int boardOffsetY, int panelX, int nextPreviewY, int nextNextPreviewY);
    void HandleInput();
    void MoveBlockDown();
    void Reset();

    bool gameOver;
    int score;
    Music music;

private:
    void InstantFallBlock();
    void MoveBlockLeft();
    void MoveBlockRight();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    bool IsBlockOutside();
    void RotateBlock();
    void LockBlock();
    bool BlockFits();
    void SwapToNextBlock();
    void UpdateScore(int linesCleared, int moveDownPoints);

    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    Block nextNextBlock;
    Sound rotateSound;
    Sound clearSound;
    bool swapAvailable;
};

