#include <algorithm>
#include <array>
#include <cstdint>
#include <minesweeper/data.hpp>

Cell data[GRID_HEIGHT][GRID_WIDTH]{
    {
        {false, false, false, 1},
        {false, false, false, 2},
        {false, false, false, 3},
        {false, false, false, 4},
        {false, false, false, 5},
        {false, false, false, 6},
        {false, false, false, 7},
        {false, false, false, 8},
        {false, true, true, 1},
    },
    {
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
    },
    {
        {false, true, false, 1},
        {false, false, false, 1},
        {false, false, false, 2},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
    },
    {
        {false, true, false, 1},
        {false, false, false, 3},
        {false, false, false, 4},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
    },
    {
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
    },
    {
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
    },
    {
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
    },
    {
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
    },
    {
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
        {false, true, false, 1},
    }};