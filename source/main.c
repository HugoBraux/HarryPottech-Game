#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int main(int argc, char **argv) {
    romfsInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    C3D_RenderTarget* topScreen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottomScreen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    C2D_TextBuf staticBuf = C2D_TextBufNew(4096);
    C2D_TextBuf dynamicBuf = C2D_TextBufNew(4096);

    C2D_Text titleText, quitText, introText, startText;
    
    C2D_TextParse(&startText, staticBuf, "Appuyez sur un bouton");
    
    C2D_TextOptimize(&titleText);
    C2D_TextOptimize(&quitText);
    C2D_TextOptimize(&introText);
    C2D_TextOptimize(&startText);

    C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    if (!spriteSheet) {
        printf("\x1b[1;1HErreur de chargement des spritesheet\n");
    }

    C2D_Sprite presidentSprite;
    C2D_Sprite bgTopSprite;
    C2D_Sprite bgBottomSprite;
    C2D_Sprite uiRibbonSprite;
    C2D_Sprite logoSprite;
    C2D_Sprite textBubbleSprite;

    float baseWidth = 0.0f;
    float baseHeight = 0.0f;
    bool hasBgTop = false;
    bool hasBgBottom = false;
    bool hasUiRibbon = false;
    bool hasLogo = false;
    bool hasTextBubble = false;

    if (spriteSheet) {
        size_t numSprites = C2D_SpriteSheetCount(spriteSheet);
        
        if (numSprites > 0) {
            C2D_SpriteFromSheet(&presidentSprite, spriteSheet, 0);
            C2D_SpriteSetCenter(&presidentSprite, 0.5f, 0.5f);
            baseWidth = presidentSprite.params.pos.w;
            baseHeight = presidentSprite.params.pos.h;
        }

        if (numSprites > 1) {
            C2D_SpriteFromSheet(&bgTopSprite, spriteSheet, 1);
            C2D_SpriteSetPos(&bgTopSprite, 0.0f, 0.0f);
            hasBgTop = true;
        }

        if (numSprites > 2) {
            C2D_SpriteFromSheet(&bgBottomSprite, spriteSheet, 2);
            C2D_SpriteSetPos(&bgBottomSprite, 0.0f, 0.0f);
            hasBgBottom = true;
        }
        
        if (numSprites > 3) {
            C2D_SpriteFromSheet(&uiRibbonSprite, spriteSheet, 3);
            C2D_SpriteSetPos(&uiRibbonSprite, 0.0f, 0.0f);
            hasUiRibbon = true;
        }

        if (numSprites > 4) {
            C2D_SpriteFromSheet(&logoSprite, spriteSheet, 4);
            C2D_SpriteSetCenter(&logoSprite, 0.5f, 0.5f);
            C2D_SpriteSetPos(&logoSprite, 200.0f, 80.0f);
            hasLogo = true;
        }

        if (numSprites > 5) {
            C2D_SpriteFromSheet(&textBubbleSprite, spriteSheet, 5);
            C2D_SpriteSetCenter(&textBubbleSprite, 0.5f, 0.5f);
            hasTextBubble = true;
        }
    }

    int argent = 0;
    int totalClicks = 0;
    float scale = 1.0f;
    u64 startTime = 0;
    int gameState = 0;

    u32 colorDeepPurple = C2D_Color32(45, 15, 65, 255);
    u32 colorCream = C2D_Color32(255, 253, 208, 255);
    u32 colorGold = C2D_Color32(255, 215, 0, 255);
    u32 colorLightGray = C2D_Color32(200, 200, 200, 255);
    u32 colorBurgundy = C2D_Color32(128, 0, 32, 255);
    u32 colorWhite = C2D_Color32(255, 255, 255, 255);

    while (aptMainLoop()) {
        hidScanInput(); 
        u32 kDown = hidKeysDown();
        
        if (kDown & KEY_START) {
            break;
        }

        if (gameState == 0) {
            if (kDown && !(kDown & KEY_TOUCH)) {
                gameState = 1;
                startTime = osGetTime();
            }

            C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
            
            C2D_TargetClear(topScreen, colorBurgundy);
            C2D_SceneBegin(topScreen);
            
            if (hasLogo) {
                C2D_DrawSprite(&logoSprite);
            }
            
            if (spriteSheet && C2D_SpriteSheetCount(spriteSheet) > 0) {
                presidentSprite.params.pos.w = baseWidth;
                presidentSprite.params.pos.h = baseHeight;
                C2D_SpriteSetPos(&presidentSprite, 336.0f, 176.0f);
                C2D_DrawSprite(&presidentSprite);
            }

            if (hasTextBubble) {
                C2D_SpriteSetPos(&textBubbleSprite, 136.0f, 176.0f);
                C2D_DrawSprite(&textBubbleSprite);
                C2D_DrawText(&introText, C2D_WithColor | C2D_AlignCenter, 136.0f, 176.0f, 0.5f, 0.6f, 0.6f, colorDeepPurple);
            }

            C2D_TargetClear(bottomScreen, colorBurgundy);
            C2D_SceneBegin(bottomScreen);
            
            C2D_DrawText(&startText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, 160.0f, 120.0f, 0.5f, 0.7f, 0.7f, colorWhite);

            C3D_FrameEnd(0);

        } else if (gameState == 1) {
            
            if (kDown & KEY_TOUCH) {
                touchPosition touch;
                hidTouchRead(&touch);
                
                if (touch.px >= 160 - 50 && touch.px <= 160 + 50 &&
                    touch.py >= 140 - 50 && touch.py <= 140 + 50) {
                    argent += 10;
                    totalClicks++;
                    scale = 1.4f;
                }
            }

            u64 currentTime = osGetTime();
            double elapsedMinutes = (currentTime - startTime) / 60000.0;
            int cpm = 0;
            if (elapsedMinutes > 0.0) {
                cpm = (int)(totalClicks / elapsedMinutes);
            }

            C2D_TextBufClear(dynamicBuf);

            char moneyStr[64];
            snprintf(moneyStr, sizeof(moneyStr), "REVENUS: %d\x80", argent);
            C2D_Text moneyText;
            C2D_TextParse(&moneyText, dynamicBuf, moneyStr);
            C2D_TextOptimize(&moneyText);

            char cpmStr[64];
            snprintf(cpmStr, sizeof(cpmStr), "CPM: %d", cpm);
            C2D_Text cpmText;
            C2D_TextParse(&cpmText, dynamicBuf, cpmStr);
            C2D_TextOptimize(&cpmText);

            if (scale > 1.0f) {
                scale -= 0.05f;
                if (scale < 1.0f) scale = 1.0f;
            }

            if (spriteSheet && C2D_SpriteSheetCount(spriteSheet) > 0) {
                presidentSprite.params.pos.w = baseWidth * scale;
                presidentSprite.params.pos.h = baseHeight * scale;
                C2D_SpriteSetPos(&presidentSprite, 160.0f, 140.0f);

                float wiggle = sin(scale * 20.0f) * (scale - 1.0f) * 0.2f;
                presidentSprite.params.angle = wiggle;
            }

            C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
            
            C2D_TargetClear(topScreen, colorDeepPurple);
            C2D_SceneBegin(topScreen);
            
            if (hasBgTop) {
                C2D_DrawSprite(&bgTopSprite);
            }
            
            C2D_DrawText(&titleText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, 200.0f, 40.0f, 0.5f, 0.7f, 0.7f, colorCream);
            C2D_DrawText(&moneyText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, 200.0f, 130.0f, 0.5f, 1.0f, 1.0f, colorGold);
            C2D_DrawText(&quitText, C2D_WithColor | C2D_AtBaseline | C2D_AlignRight, 380.0f, 220.0f, 0.5f, 0.5f, 0.5f, colorLightGray);

            C2D_TargetClear(bottomScreen, colorDeepPurple);
            C2D_SceneBegin(bottomScreen);
            
            if (hasBgBottom) {
                C2D_DrawSprite(&bgBottomSprite);
            }
            
            if (hasUiRibbon) {
                C2D_DrawSprite(&uiRibbonSprite);
            }
            
            if (spriteSheet && C2D_SpriteSheetCount(spriteSheet) > 0) {
                C2D_DrawSprite(&presidentSprite);
            }

            C2D_DrawText(&cpmText, C2D_WithColor, 15.0f, 15.0f, 0.5f, 0.7f, 0.7f, colorGold);

            C3D_FrameEnd(0);
        }
    }

    C2D_TextBufDelete(dynamicBuf);
    C2D_TextBufDelete(staticBuf);
    
    if (spriteSheet) {
        C2D_SpriteSheetFree(spriteSheet);
    }
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    romfsExit();
    
    return 0;
}