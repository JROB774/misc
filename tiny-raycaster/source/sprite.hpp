#pragma once

struct Sprite
{
    float x,y;
    int texid;

    float player_dist;

    bool operator< (const Sprite& s) const;
};
