#pragma once

struct Map
{
    int w, h;

    Map ();

    int  get_tile (int x, int y) const;
    bool is_empty (int x, int y) const;
};
