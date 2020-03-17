#include "Map.h"
#include "Config.h"
#include "CommonTypes.h"

Map::Map() :
    mCellSize(cfg::field_cell_size, cfg::field_cell_size)
{
    mCells = new Cell * [cfg::field_rows];
    for (int i = 0; i < cfg::field_rows; i++)
    {
        mCells[i] = new Cell[cfg::field_cols];
    }
}

Map::~Map()
{
    for (int i = 0; i < cfg::field_rows; i++)
    {
        delete[] mCells[i];
    }

    delete[] mCells;
}

void Map::ResetColor()
{
    for (int i = 0; i < cfg::field_rows; i++)
    {
        for (int j = 0; j < cfg::field_cols; j++)
        {
            mCells[i][j].mShape.setFillColor(color::Water);
        }
    }
}

void Map::Initialize()
{
    for (int i = 0; i < cfg::field_rows; i++)
    {
        for (int j = 0; j < cfg::field_cols; j++)
        {
            sf::RectangleShape shape;
            shape.setSize(mCellSize);
            shape.setFillColor(color::Water);
            short borderThick = 2;
            shape.setOutlineThickness(borderThick);
            shape.setOutlineColor(color::WaterBorder);
            shape.setPosition((i + borderThick) * mCellSize.x, (j + borderThick) * mCellSize.y);

            mCells[i][j].mShape = shape;
        }
    }
}