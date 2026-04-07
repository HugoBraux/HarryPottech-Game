#include "render.h"
#include <stdio.h>

static C3D_RenderTarget* topScreen;
static C3D_RenderTarget* bottomScreen;

static C2D_SpriteSheet spriteSheet;
static C2D_Sprite sprites[9];
static bool spriteLoaded[9] = {false};

static C2D_TextBuf staticBuf;
static C2D_TextBuf dynamicBuf;
static C2D_Text startText, moneyText, shopText;

static long long last_argent_drawn = -1; 
static long long last_revenu_drawn = -1;
static long long last_influence_drawn = -1;

static u32 colorDeepPurple, colorGold, colorBurgundy, colorWhite;

void Render_Init(void) {
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    topScreen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottomScreen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    staticBuf = C2D_TextBufNew(4096);
    dynamicBuf = C2D_TextBufNew(4096);

    colorDeepPurple = C2D_Color32(45, 15, 65, 255);
    colorGold = C2D_Color32(255, 215, 0, 255);
    colorBurgundy = C2D_Color32(128, 0, 32, 255);
    colorWhite = C2D_Color32(255, 255, 255, 255);

    C2D_TextParse(&startText, staticBuf, "Cliquez ou Appuyez sur A");
    C2D_TextOptimize(&startText);

    spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    if (spriteSheet) {
        size_t numSprites = C2D_SpriteSheetCount(spriteSheet);
        for(size_t i = 0; i < numSprites && i < 9; i++) {
            C2D_SpriteFromSheet(&sprites[i], spriteSheet, i);
            spriteLoaded[i] = true;
        }
        
        if(spriteLoaded[0]) C2D_SpriteSetCenter(&sprites[0], 0.5f, 0.5f);
        if(spriteLoaded[4]) {
            C2D_SpriteSetCenter(&sprites[4], 0.5f, 0.5f);
            C2D_SpriteSetPos(&sprites[4], 200.0f, 80.0f);
        }
        if(spriteLoaded[5]) C2D_SpriteSetCenter(&sprites[5], 0.5f, 0.5f);
        if(spriteLoaded[6]) C2D_SpriteSetPos(&sprites[6], 270.0f, 5.0f);
        if(spriteLoaded[7]) {
            C2D_SpriteSetCenter(&sprites[7], 0.5f, 0.5f);
            C2D_SpriteSetPos(&sprites[7], 160.0f, 140.0f);
        }
        if(spriteLoaded[8]) {
            C2D_SpriteSetCenter(&sprites[8], 0.5f, 0.5f);
            C2D_SpriteSetPos(&sprites[8], 160.0f, 140.0f);
        }
    }
}

void Render_FormatNumber(long long num, char* buf, size_t size) {
    if (num >= 1000000000LL) snprintf(buf, size, "%.2ft", num / 1000000000.0);
    else if (num >= 1000000LL) snprintf(buf, size, "%.2fm", num / 1000000.0);
    else if (num >= 1000LL) snprintf(buf, size, "%.2fk", num / 1000.0);
    else snprintf(buf, size, "%lld", num);
}

void Render_UpdateMoneyText(PlayerData* player) {
    if (player->argent != last_argent_drawn || player->revenu_passif_sec != last_revenu_drawn || player->influence != last_influence_drawn) {
        C2D_TextBufClear(dynamicBuf);
        char moneyStr[256], aStr[32], cStr[32], pStr[32], iStr[32];
        
        Render_FormatNumber(player->argent, aStr, 32);
        Render_FormatNumber(player->click_power, cStr, 32);
        Render_FormatNumber(player->revenu_passif_sec, pStr, 32);
        Render_FormatNumber(player->influence, iStr, 32);
        
        snprintf(moneyStr, sizeof(moneyStr), "Banque: %se | Inf: %s\nClic: %s | Passif: %s/s", aStr, iStr, cStr, pStr);
        C2D_TextParse(&moneyText, dynamicBuf, moneyStr);
        C2D_TextOptimize(&moneyText);
        
        last_argent_drawn = player->argent;
        last_revenu_drawn = player->revenu_passif_sec;
        last_influence_drawn = player->influence;
    }
}

void Render_UpdateShopText(PlayerData* player, int shopScroll) {
    C2D_TextBufClear(staticBuf);
    char shopStr[1024];
    
    int offset = snprintf(shopStr, sizeof(shopStr), 
        "Boutique (B:Quitter | Haut/Bas:Defiler)\n\n");

    for (int i = 0; i < 4; i++) {
        int idx = shopScroll + i;
        if (idx < NB_ITEMS_BOUTIQUE) {
            char coutP[32];
            Render_FormatNumber(player->boutique[idx].cout_actuel, coutP, 32);
            
            char type_icon = '?';
            if (player->boutique[idx].type == ITEM_MEMBRE) type_icon = 'M';
            else if (player->boutique[idx].type == ITEM_GOODY) type_icon = 'G';
            else if (player->boutique[idx].type == ITEM_EVENT) type_icon = 'E';

            offset += snprintf(shopStr + offset, sizeof(shopStr) - offset, 
                "[%c] %s (Nv.%d) : %se\n\n", 
                type_icon, player->boutique[idx].nom, player->boutique[idx].niveau, coutP);
        }
    }
        
    C2D_TextParse(&shopText, staticBuf, shopStr);
    C2D_TextOptimize(&shopText);
    
    C2D_TextParse(&startText, staticBuf, "Cliquez ou Appuyez sur A"); 
}

void Render_DrawTitleScreen(void) {
    C2D_TargetClear(topScreen, colorBurgundy);
    C2D_SceneBegin(topScreen);
    if (spriteLoaded[4]) C2D_DrawSprite(&sprites[4]);
    if (spriteLoaded[0]) {
        C2D_SpriteSetPos(&sprites[0], 336.0f, 176.0f);
        C2D_DrawSprite(&sprites[0]);
    }
    if (spriteLoaded[5]) {
        C2D_SpriteSetPos(&sprites[5], 136.0f, 176.0f);
        C2D_DrawSprite(&sprites[5]);
    }

    C2D_TargetClear(bottomScreen, colorBurgundy);
    C2D_SceneBegin(bottomScreen);
    C2D_DrawText(&startText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, 160.0f, 120.0f, 0.5f, 0.7f, 0.7f, colorWhite);
}

void Render_DrawMainScreen(PlayerData* player, int clickAnimTimer) {
    C2D_TargetClear(topScreen, colorDeepPurple);
    C2D_SceneBegin(topScreen);
    if (spriteLoaded[1]) C2D_DrawSprite(&sprites[1]);
    C2D_DrawText(&moneyText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, 200.0f, 130.0f, 0.5f, 1.0f, 1.0f, colorGold);

    C2D_TargetClear(bottomScreen, colorDeepPurple);
    C2D_SceneBegin(bottomScreen);
    if (spriteLoaded[2]) C2D_DrawSprite(&sprites[2]);
    if (spriteLoaded[3]) C2D_DrawSprite(&sprites[3]);
    if (spriteLoaded[6]) C2D_DrawSprite(&sprites[6]);
    
    if (spriteLoaded[7] && spriteLoaded[8]) {
        if (clickAnimTimer > 0) C2D_DrawSprite(&sprites[8]);
        else C2D_DrawSprite(&sprites[7]);
    } else {
        C2D_DrawCircleSolid(160.0f, 140.0f, 0.0f, 45.0f, colorWhite);
    }
}

void Render_DrawShopScreen(PlayerData* player, int shopScroll) {
    C2D_TargetClear(topScreen, colorDeepPurple);
    C2D_SceneBegin(topScreen);
    C2D_DrawText(&moneyText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, 200.0f, 130.0f, 0.5f, 1.0f, 1.0f, colorGold);

    C2D_TargetClear(bottomScreen, colorBurgundy);
    C2D_SceneBegin(bottomScreen);
    C2D_DrawText(&shopText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, 160.0f, 25.0f, 0.5f, 0.6f, 0.6f, colorWhite);
}

void Render_Exit(void) {
    C2D_TextBufDelete(dynamicBuf);
    C2D_TextBufDelete(staticBuf);
    if (spriteSheet) C2D_SpriteSheetFree(spriteSheet);
    C2D_Fini();
    C3D_Fini();
}