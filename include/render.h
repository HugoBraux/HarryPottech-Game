#ifndef RENDER_H
#define RENDER_H

#include <3ds.h>
#include <citro2d.h>
#include "game.h"

void Render_Init(void);
void Render_Exit(void);

void Render_FormatNumber(long long num, char* buf, size_t size);

void Render_UpdateMoneyText(PlayerData* player);
void Render_UpdateShopText(PlayerData* player, int shopScroll);

void Render_DrawTitleScreen(void);
void Render_DrawMainScreen(PlayerData* player, int clickAnimTimer);
void Render_DrawShopScreen(PlayerData* player, int shopScroll);

#endif