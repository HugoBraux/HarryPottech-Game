#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int main(int argc, char **argv) {
    gfxInitDefault();
    romfsInit();
    
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    C3D_RenderTarget* topScreen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottomScreen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    C2D_TextBuf staticBuf = C2D_TextBufNew(4096);
    C2D_TextBuf dynamicBuf = C2D_TextBufNew(4096);

    C2D_Text startText, moneyText;
    
    C2D_TextParse(&startText, staticBuf, "Cliquez pour commencer");
    C2D_TextParse(&moneyText, dynamicBuf, "Banque : 0 euros");
    
    C2D_TextOptimize(&startText);

    C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");

    C2D_Sprite presidentSprite;
    C2D_Sprite bgTopSprite;
    C2D_Sprite bgBottomSprite;
    C2D_Sprite uiRibbonSprite;
    C2D_Sprite logoSprite;
    C2D_Sprite textBubbleSprite;

    float baseRadius = 45.0f;
    float baseWidth = 0.0f;
    float baseHeight = 0.0f;
    bool hasBgTop = false;
    bool hasBgBottom = false;
    bool hasUiRibbon = false;
    bool hasLogo = false;
    bool hasTextBubble = false;
    size_t numSprites = 0;

    if (spriteSheet) {
        numSprites = C2D_SpriteSheetCount(spriteSheet);
        
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
    int lastArgent = 0;
    float scale = 1.0f;
    int gameState = 0;

    u32 colorDeepPurple = C2D_Color32(45, 15, 65, 255);
    u32 colorGold = C2D_Color32(255, 215, 0, 255);
    u32 colorBurgundy = C2D_Color32(128, 0, 32, 255);
    u32 colorWhite = C2D_Color32(255, 255, 255, 255);

    while (aptMainLoop()) {
        hidScanInput(); 
        u32 kDown = hidKeysDown();
        
        if (kDown & KEY_START) {
            break;
        }
        
        if (gameState == 0) {
            if (kDown & (KEY_TOUCH | KEY_A)) {
                gameState = 1;
            }
        } else if (gameState == 1) {
            if (kDown & KEY_TOUCH) {
                touchPosition touch;
                hidTouchRead(&touch);
                
                if (touch.px >= 110 && touch.px <= 210 &&
                    touch.py >= 90 && touch.py <= 190) {
                    argent += 1;
                    scale = 1.4f;
                }
            }

            if (scale > 1.0f) {
                scale -= 0.05f;
                if (scale < 1.0f) scale = 1.0f;
            }

            if (argent != lastArgent) {
                C2D_TextBufClear(dynamicBuf);
                char moneyStr[64];
                snprintf(moneyStr, sizeof(moneyStr), "Banque : %d euros", argent);
                C2D_TextParse(&moneyText, dynamicBuf, moneyStr);
                lastArgent = argent;
            }
        }

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        
        if (gameState == 0) {
            C2D_TargetClear(topScreen, colorBurgundy);
            C2D_SceneBegin(topScreen);
            
            if (hasLogo) {
                C2D_DrawSprite(&logoSprite);
            }
            
            if (spriteSheet && numSprites > 0) {
                presidentSprite.params.pos.w = baseWidth;
                presidentSprite.params.pos.h = baseHeight;
                presidentSprite.params.angle = 0.0f;
                C2D_SpriteSetPos(&presidentSprite, 336.0f, 176.0f);
                C2D_DrawSprite(&presidentSprite);
            }

            if (hasTextBubble) {
                C2D_SpriteSetPos(&textBubbleSprite, 136.0f, 176.0f);
                C2D_DrawSprite(&textBubbleSprite);
            }

            C2D_TargetClear(bottomScreen, colorBurgundy);
            C2D_SceneBegin(bottomScreen);
            C2D_DrawText(&startText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, 160.0f, 120.0f, 0.5f, 0.7f, 0.7f, colorWhite);

        } else if (gameState == 1) {
            C2D_TargetClear(topScreen, colorDeepPurple);
            C2D_SceneBegin(topScreen);
            
            if (hasBgTop) {
                C2D_DrawSprite(&bgTopSprite);
            }
            
            C2D_DrawText(&moneyText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, 200.0f, 130.0f, 0.5f, 1.0f, 1.0f, colorGold);

            C2D_TargetClear(bottomScreen, colorDeepPurple);
            C2D_SceneBegin(bottomScreen);
            
            if (hasBgBottom) {
                C2D_DrawSprite(&bgBottomSprite);
            }
            if (hasUiRibbon) {
                C2D_DrawSprite(&uiRibbonSprite);
            }
            
            C2D_DrawCircleSolid(160.0f, 140.0f, 0.0f, baseRadius * scale, colorWhite);
        }

        C3D_FrameEnd(0);
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