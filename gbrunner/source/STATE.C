#define GAME_STATE_LOGO 0x00 // 0
#define GAME_STATE_MENU 0x01 // 1
#define GAME_STATE_GAME 0x02 // 2

global U8 game_state;

internal void set_game_state (U8 state)
{
    game_state = state;

    switch (game_state)
    {
        case (GAME_STATE_LOGO):
        {
            set_bkg_data(0,TILELOGO_NUM_TILES, TILELOGO);
            set_bkg_tiles(0,0, MAPLOGO_WIDTH,MAPLOGO_HEIGHT, MAPLOGO);
        } break;
        case (GAME_STATE_MENU):
        {
            set_bkg_data(0,TILEMENU_NUM_TILES, TILEMENU);
            set_bkg_tiles(0,0, MAPMENU_WIDTH,MAPMENU_HEIGHT, MAPMENU);
        } break;
        case (GAME_STATE_GAME):
        {
            set_bkg_data(0,TILEBKG_NUM_TILES, TILEBKG);
            set_bkg_tiles(0,0, MAPGAME_WIDTH,MAPGAME_HEIGHT, MAPGAME);
        } break;
    }
}
