#include "block.h"

Block::Block()
{
    cellSize = 30;
    rotationState = 0;
    colors = GetCellColors();
    rowOffset = 0;
    columnOffset = 0;
}

void Block::Draw(int offsetX, int offsetY, float scale, float alpha)
{
    std::vector<Position> tiles = GetCellPositions();
    float scaledSize = cellSize * scale;
    for (Position item : tiles)
    {
        int drawX = offsetX + static_cast<int>(item.column * scaledSize);
        int drawY = offsetY + static_cast<int>(item.row * scaledSize);
        Color c = colors[id];
        c.a = static_cast<unsigned char>(255.0f * alpha);
        DrawRectangle(drawX, drawY, static_cast<int>(scaledSize) - 1, static_cast<int>(scaledSize) - 1, c);
    }
}

void Block::Move(int rows, int columns)
{
    rowOffset += rows;
    columnOffset += columns;
}
void Block::SetPosition(int rows, int columns)
{
    rowOffset = rows;
    columnOffset = columns;
}

std::vector<Position> Block::GetCellPositions()
{
    std::vector<Position> tiles = cells[rotationState];
    std::vector<Position> movedTiles;
    for (Position item : tiles)
    {
        Position newPos = Position(item.row + rowOffset, item.column + columnOffset);
        movedTiles.push_back(newPos);
    }
    return movedTiles;
}

void Block::Rotate()
{
    rotationState++;
    if (rotationState == (int)cells.size())
    {
        rotationState = 0;
    }
}

void Block::UndoRotation()
{
    rotationState--;
    if (rotationState == -1)
    {
        rotationState = cells.size() - 1;
    }
}
