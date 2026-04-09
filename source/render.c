#include "render.h"
#include <stdio.h>

static C3D_RenderTarget* topScreen;
static C3D_RenderTarget* bottomScreen;

static C2D_SpriteSheet spriteSheet;
static C2D_Sprite sprites[15];
static bool spriteLoaded[15] = {false};

static C2D_TextBuf staticBuf;
static C2D_TextBuf dynamicBuf;
static C2D_TextBuf popupBuf;
static C2D_Text startText, moneyText, statsText, shopText, infText;

static long long last_argent_drawn = -1; 
static long long last_revenu_drawn = -1;
static long long last_influence_drawn = -1;

static u32 colorDeepPurple, colorGold, colorBurgundy, colorWhite, colorGray;

void Render_Init(void) {
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    topScreen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottomScreen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    staticBuf = C2D_TextBufNew(4096);
    dynamicBuf = C2D_TextBufNew(4096);
    popupBuf = C2D_TextBufNew(256);

    colorDeepPurple = C2D_Color32(45, 15, 65, 255);
    colorGold = C2D_Color32(255, 215, 0, 255);
    colorBurgundy = C2D_Color32(128, 0, 32, 255);
    colorWhite = C2D_Color32(255, 255, 255, 255);
    colorGray = C2D_Color32(50, 50, 50, 200);

    C2D_TextParse(&startText, staticBuf, "Cliquez ou Appuyez sur A");
    C2D_TextOptimize(&startText);

    spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    if (spriteSheet) {
        size_t numSprites = C2D_SpriteSheetCount(spriteSheet);
        for(size_t i = 0; i < numSprites && i < 15; i++) {
            C2D_SpriteFromSheet(&sprites[i], spriteSheet, i);
            spriteLoaded[i] = true;
        }
        
        if(spriteLoaded[0]) C2D_SpriteSetCenter(&sprites[0], 0.5f, 0.5f);
        if(spriteLoaded[4]) {
            C2D_SpriteSetCenter(&sprites[4], 0.5f, 0.5f);
            C2D_SpriteSetPos(&sprites[4], 200.0f, 80.0f);
        }
        if(spriteLoaded[5]) C2D_SpriteSetCenter(&sprites[5], 0.5f, 0.5f);
        if(spriteLoaded[7]) {
            C2D_SpriteSetCenter(&sprites[7], 0.5f, 0.5f);
            C2D_SpriteSetPos(&sprites[7], 160.0f, 140.0f);
        }
        if(spriteLoaded[8]) {
            C2D_SpriteSetCenter(&sprites[8], 0.5f, 0.5f);
            C2D_SpriteSetPos(&sprites[8], 160.0f, 140.0f);
        }
        for (int i = 9; i <= 13; i++) {
            if (spriteLoaded[i]) C2D_SpriteSetCenter(&sprites[i], 0.5f, 0.5f);
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
        char moneyStr[64], statsStr[128], infStr[128], aStr[32], cStr[32], pStr[32], iStr[32], mStr[32];
        
        Render_FormatNumber(player->argent, aStr, 32);
        Render_FormatNumber(player->click_power, cStr, 32);
        Render_FormatNumber(player->revenu_passif_sec, pStr, 32);
        Render_FormatNumber(player->influence, iStr, 32);
        Render_FormatNumber(INFLUENCE_MAX, mStr, 32);
        
        snprintf(moneyStr, sizeof(moneyStr), "%s \u20AC", aStr);
        C2D_TextParse(&moneyText, dynamicBuf, moneyStr);
        C2D_TextOptimize(&moneyText);

        snprintf(statsStr, sizeof(statsStr), "Clic: %s | Passif: %s/s", cStr, pStr);
        C2D_TextParse(&statsText, dynamicBuf, statsStr);
        C2D_TextOptimize(&statsText);

        snprintf(infStr, sizeof(infStr), "Influence : %s / %s", iStr, mStr);
        C2D_TextParse(&infText, dynamicBuf, infStr);
        C2D_TextOptimize(&infText);
        
        last_argent_drawn = player->argent;
        last_revenu_drawn = player->revenu_passif_sec;
        last_influence_drawn = player->influence;
    }
}

void Render_UpdateShopText(PlayerData* player, int shopScroll, int shopCategory) {
    C2D_TextBufClear(staticBuf);
    char shopStr[1024];
    
    const char* tabName = "";
    ItemBoutique* currentArray = NULL;
    int maxItems = 0;

    if (shopCategory == 0) {
        tabName = "Membres";
        currentArray = player->membres;
        maxItems = NB_MEMBRES;
    } else if (shopCategory == 1) {
        tabName = "Goodies";
        currentArray = player->goodies;
        maxItems = NB_GOODIES;
    } else {
        tabName = "Evenements";
        currentArray = player->events;
        maxItems = NB_EVENTS;
    }
    
    int offset = snprintf(shopStr, sizeof(shopStr), 
        "--- %s ---\n(L/R: Naviguer | H/B: Defiler)\n\n", tabName);

    for (int i = 0; i < 5; i++) {
        int idx = shopScroll + i;
        if (idx < maxItems) {
            char coutP[32];
            Render_FormatNumber(currentArray[idx].cout_actuel, coutP, 32);
            
            offset += snprintf(shopStr + offset, sizeof(shopStr) - offset, 
                "%s (Nv.%d) : %s\u20AC\n\n", 
                currentArray[idx].nom, currentArray[idx].niveau, coutP);
        }
    }
        
    C2D_TextParse(&shopText, staticBuf, shopStr);
    C2D_TextOptimize(&shopText);
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

    float fillRatio = (float)player->influence / (float)INFLUENCE_MAX;
    if (fillRatio > 1.0f) fillRatio = 1.0f;
    float barWidth = 300.0f;
    float currentWidth = barWidth * fillRatio;

    C2D_DrawRectSolid(50.0f, 15.0f, 0.5f, barWidth, 20.0f, colorGray);
    C2D_DrawRectSolid(50.0f, 15.0f, 0.5f, currentWidth, 20.0f, colorBurgundy);
    C2D_DrawText(&infText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, 200.0f, 30.0f, 0.5f, 0.6f, 0.6f, colorWhite);

    if (spriteLoaded[14]) {
        C2D_SpriteSetPos(&sprites[14], 5.0f, 185.0f);
        C2D_DrawSprite(&sprites[14]);
    }
    
    float mScaleX = 0.50f;
    float mScaleY = 0.65f;
    if (clickAnimTimer > 0) {
        mScaleX = 0.55f;
        mScaleY = 0.70f;
    }
    
    C2D_DrawText(&moneyText, C2D_WithColor | C2D_AtBaseline | C2D_AlignLeft, 50.0f, 218.0f, 0.5f, mScaleX, mScaleY, colorGold);
    C2D_DrawText(&statsText, C2D_WithColor | C2D_AtBaseline | C2D_AlignRight, 390.0f, 225.0f, 0.5f, 0.6f, 0.6f, colorWhite);

    C2D_TargetClear(bottomScreen, colorDeepPurple);
    C2D_SceneBegin(bottomScreen);
    if (spriteLoaded[2]) C2D_DrawSprite(&sprites[2]);
    if (spriteLoaded[3]) C2D_DrawSprite(&sprites[3]);
    
    if (spriteLoaded[7] && spriteLoaded[8]) {
        if (clickAnimTimer > 0) C2D_DrawSprite(&sprites[8]);
        else C2D_DrawSprite(&sprites[7]);
    } else {
        C2D_DrawCircleSolid(160.0f, 140.0f, 0.0f, 45.0f, colorWhite);
    }

    int active_indices[NB_MEMBRES];
    int active_count = 0;
    
    for (int i = 0; i < NB_MEMBRES; i++) {
        if (player->membres[i].niveau > 0 && player->membres[i].render_x >= 0.0f) {
            active_indices[active_count++] = i;
        }
    }
    
    for (int i = 0; i < active_count - 1; i++) {
        for (int j = 0; j < active_count - i - 1; j++) {
            if (player->membres[active_indices[j]].render_y > player->membres[active_indices[j+1]].render_y) {
                int temp = active_indices[j];
                active_indices[j] = active_indices[j+1];
                active_indices[j+1] = temp;
            }
        }
    }

    u64 timeMs = osGetTime();
    for (int i = 0; i < active_count; i++) {
        int idx = active_indices[i];
        int speed = 1000 / (player->membres[idx].revenu > 0 ? player->membres[idx].revenu : 1);
        if (speed < 100) speed = 100;
        if (speed > 1000) speed = 1000;
        
        int frame = (timeMs / speed) % 2;
        int spriteIndex = (player->membres[idx].visual_type == 0) ? (9 + frame) : (11 + frame);
        
        if (spriteLoaded[spriteIndex]) {
            C2D_SpriteSetPos(&sprites[spriteIndex], player->membres[idx].render_x, player->membres[idx].render_y);
            C2D_DrawSprite(&sprites[spriteIndex]);
        }
    }

    if (player->vif.active) {
        if (spriteLoaded[13]) {
            C2D_SpriteSetPos(&sprites[13], player->vif.x, player->vif.y);
            C2D_DrawSprite(&sprites[13]);
        } else {
            C2D_DrawRectSolid(player->vif.x - 10.0f, player->vif.y - 10.0f, 0.5f, 20.0f, 20.0f, colorGold);
        }
    }

    if (player->vif.popup_timer > 0) {
        C2D_TextBufClear(popupBuf);
        C2D_Text popupText;
        C2D_TextParse(&popupText, popupBuf, player->vif.popup_text);
        C2D_TextOptimize(&popupText);
        C2D_DrawText(&popupText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, player->vif.popup_x, player->vif.popup_y, 0.5f, 0.6f, 0.6f, colorGold);
    }
}

void Render_DrawShopScreen(PlayerData* player, int shopScroll) {
    C2D_TargetClear(topScreen, colorDeepPurple);
    C2D_SceneBegin(topScreen);

    float fillRatio = (float)player->influence / (float)INFLUENCE_MAX;
    if (fillRatio > 1.0f) fillRatio = 1.0f;
    float barWidth = 300.0f;
    float currentWidth = barWidth * fillRatio;

    C2D_DrawRectSolid(50.0f, 15.0f, 0.5f, barWidth, 20.0f, colorGray);
    C2D_DrawRectSolid(50.0f, 15.0f, 0.5f, currentWidth, 20.0f, colorBurgundy);
    C2D_DrawText(&infText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, 200.0f, 30.0f, 0.5f, 0.6f, 0.6f, colorWhite);

    if (spriteLoaded[14]) {
        C2D_SpriteSetPos(&sprites[14], 5.0f, 185.0f);
        C2D_DrawSprite(&sprites[14]);
    }
    
    C2D_DrawText(&moneyText, C2D_WithColor | C2D_AtBaseline | C2D_AlignLeft, 50.0f, 218.0f, 0.5f, 0.50f, 0.65f, colorGold);
    C2D_DrawText(&statsText, C2D_WithColor | C2D_AtBaseline | C2D_AlignRight, 390.0f, 225.0f, 0.5f, 0.6f, 0.6f, colorWhite);

    C2D_TargetClear(bottomScreen, colorBurgundy);
    C2D_SceneBegin(bottomScreen);
    C2D_DrawText(&shopText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, 160.0f, 20.0f, 0.5f, 0.5f, 0.5f, colorWhite);
}

void Render_Exit(void) {
    C2D_TextBufDelete(popupBuf);
    C2D_TextBufDelete(dynamicBuf);
    C2D_TextBufDelete(staticBuf);
    if (spriteSheet) C2D_SpriteSheetFree(spriteSheet);
    C2D_Fini();
    C3D_Fini();
}