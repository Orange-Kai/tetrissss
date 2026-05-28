#include "game.h"
#include <random>

static std::pair<int, int> GetDefaultBlockOffset(int id)
{
    switch (id)
    {
    case 3:
        return {-1, 3};
    case 4:
        return {0, 4};
    default:
        return {0, 3};
    }
}

Game::Game()
{
    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    nextNextBlock = GetRandomBlock();

    swapAvailable = true;

    gameOver = false;
    score = 0;


    InitAudioDevice();
    music = LoadMusicStream("Sounds/music.mp3");
    PlayMusicStream(music);
    rotateSound = LoadSound("Sounds/rotate.mp3");
    clearSound = LoadSound("Sounds/clear.mp3");
}

Game::~Game()
{
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
    UnloadMusicStream(music);
    CloseAudioDevice();
}

Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        blocks = GetAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}


static int GetPreviewBlockX(int id, int previewCenterX)
{
    switch (id)
    {
    case 3:
        return previewCenterX - 135;
    case 4:
        return previewCenterX - 135;
    default:
        return previewCenterX - 120;
    }
}

static int GetPreviewBlockY(int id, int boxTopY, int boxHeight)
{
    int previewBlockHeightPixels = 0;
    switch (id)
    {
    case 3:
        previewBlockHeightPixels = 120;
        break;
    case 4:
        previewBlockHeightPixels = 60;
        break;
    default:
        previewBlockHeightPixels = 90;
        break;
    }

    return boxTopY + (boxHeight - previewBlockHeightPixels) / 2;
}




void Game::Draw(int boardOffsetX, int boardOffsetY, int panelX, int nextPreviewY, int nextNextPreviewY)
{
    grid.Draw(boardOffsetX, boardOffsetY);

    // Ghost
    {
        Block ghost = currentBlock;


        while (true)
        {
            ghost.Move(1, 0);

            std::vector<Position> tiles = ghost.GetCellPositions();
            bool outsideOrBlocked = false;

            for (Position item : tiles)
            {
                if (grid.IsCellOutside(item.row, item.column))
                {
                    outsideOrBlocked = true;
                    break;
                }

                if (!grid.IsCellEmpty(item.row, item.column))
                {
                    outsideOrBlocked = true;
                    break;
                }
            }

            if (outsideOrBlocked)
            {
                ghost.Move(-1, 0);
                break;
            }
        }

        ghost.Draw(boardOffsetX, boardOffsetY, 1.0f, 0.25f);
    }

    currentBlock.Draw(boardOffsetX, boardOffsetY);

    const int panelWidth = 170;
    const int previewCenterX = panelX + panelWidth / 2;

    const int previewBoxHeight = 160;
    nextBlock.Draw(GetPreviewBlockX(nextBlock.id, previewCenterX), GetPreviewBlockY(nextBlock.id, nextPreviewY, previewBoxHeight));
    nextNextBlock.Draw(GetPreviewBlockX(nextNextBlock.id, previewCenterX), GetPreviewBlockY(nextNextBlock.id, nextNextPreviewY, previewBoxHeight), 0.8f);
}

void Game::HandleInput()
{
    int keyPressed = GetKeyPressed();
    if (gameOver && keyPressed != 0)
    {
        gameOver = false;
        Reset();
    }
    switch (keyPressed)
    {
    case KEY_LEFT:
        MoveBlockLeft();
        break;
    case KEY_RIGHT:
        MoveBlockRight();
        break;
    case KEY_DOWN:
        MoveBlockDown();
        UpdateScore(0, 1);
        break;
    case KEY_UP:
        RotateBlock();
        break;

    case KEY_SPACE:
        InstantFallBlock();
        UpdateScore(0, 5);
        break;
    }

    if (!gameOver && swapAvailable && (IsKeyPressed(KEY_LEFT_SHIFT) || IsKeyPressed(KEY_RIGHT_SHIFT)))
    {
        SwapToNextBlock();
    }
}

void Game::SwapToNextBlock()
{
    if (!swapAvailable)
        return;

    Block temp = currentBlock;
    currentBlock = nextBlock;
    nextBlock = temp;

    auto currentOffset = GetDefaultBlockOffset(currentBlock.id);
    auto nextOffset = GetDefaultBlockOffset(nextBlock.id);
    currentBlock.SetPosition(currentOffset.first, currentOffset.second);
    nextBlock.SetPosition(nextOffset.first, nextOffset.second);

    swapAvailable = false;
}

void Game::InstantFallBlock()
{
    if (!gameOver)
    {
        while (true)
        {
            currentBlock.Move(1, 0);
            if (IsBlockOutside() || BlockFits() == false)
            {
                currentBlock.Move(-1, 0);
                LockBlock();
                break;
            }
        }
    }
}

void Game::MoveBlockLeft()
{
    if (!gameOver)
    {
        currentBlock.Move(0, -1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, 1);
        }
    }
}

void Game::MoveBlockRight()
{
    if (!gameOver)
    {
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, -1);
        }
    }
}


void Game::MoveBlockDown()
{
    if (gameOver)
        return;

    currentBlock.Move(1, 0);

    if (IsBlockOutside() || BlockFits() == false)
    {
        currentBlock.Move(-1, 0);
        LockBlock();
    }
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellOutside(item.row, item.column))
        {
            return true;
        }
    }
    return false;
}

void Game::RotateBlock()
{
    if (!gameOver)
    {
        currentBlock.Rotate();
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.UndoRotation();
        }
        else
        {
            PlaySound(rotateSound);
        }
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
    }

    currentBlock = nextBlock;
    if (BlockFits() == false)
    {
        gameOver = true;
    }

    nextBlock = nextNextBlock;
    nextNextBlock = GetRandomBlock();
    swapAvailable = true;

    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0)
    {
        PlaySound(clearSound);
        UpdateScore(rowsCleared, 0);
    }
}

bool Game::BlockFits()
{

    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellOutside(item.row, item.column))
        {
            if (item.row < 0 && grid.IsColumnInside(item.column))
                continue;
            return false;
        }

        if (grid.IsCellEmpty(item.row, item.column) == false)
        {
            return false;
        }
    }
    return true;
}

void Game::Reset()
{
    gameOver = false;
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    nextNextBlock = GetRandomBlock();
    swapAvailable = true;
    score = 0;

}

void Game::UpdateScore(int linesCleared, int moveDownPoints)
{
    switch (linesCleared)
    {
    case 1:
        score += 100;
        break;
    case 2:
        score += 300;
        break;
    case 3:
        score += 500;
        break;
    default:
        break;
    }

    score += moveDownPoints;
}

