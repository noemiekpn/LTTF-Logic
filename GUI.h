#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

#include "Map.h"

enum IMAGES {TITLE, TILES, LINK, MONSTER, RUPEE, HEART, SWORDS, VORTEX, PENDANTS};

void GUI_InitializeAllegroAddons();

void GUI_CreateDisplay();
void GUI_DestroyDisplay();

void GUI_LoadImages();
void GUI_DestroyImages();

void GUI_LoadFonts();
void GUI_DestroyFonts();

void GUI_DrawMainTitle();
void GUI_DrawMap(Map *map, ALLEGRO_BITMAP *image);
void GUI_HideUnknownBackground(Map *map);

int GUI_GetScreenHeight();
int GUI_GetScreenWidth();
int GUI_GetTileSize();

ALLEGRO_BITMAP *GUI_GetImage(int index);
ALLEGRO_DISPLAY *GUI_GetDisplay();