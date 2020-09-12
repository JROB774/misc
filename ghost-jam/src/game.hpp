#pragma once

GLOBAL Catalog    area_catalog;
GLOBAL Catalog    data_catalog;
GLOBAL Catalog  effect_catalog;
GLOBAL Catalog    font_catalog;
GLOBAL Catalog    icon_catalog;
GLOBAL Catalog  shader_catalog;
GLOBAL Catalog     sky_catalog;
GLOBAL Catalog  sprite_catalog;
GLOBAL Catalog texture_catalog;
GLOBAL Catalog anims_catalog;

struct Game
{
    Player player;
    Area area;
};

GLOBAL Game game;

FILDEF void init_game ();
FILDEF void quit_game ();
FILDEF void do_game   ();
