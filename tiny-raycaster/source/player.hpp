#pragma once

struct Player
{
    float x,y;
    float a;
    float fov;

    int turn;
    int walk;

    Player (float px, float py, float pa, float pfov);
};
