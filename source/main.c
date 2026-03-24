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

    C2D_Text titleText, quitText;
    C2D_TextParse(&titleText, staticBuf, "HarryPottech - The Great Adventure");
    C2D_TextOptimize(&quitText);

    C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    if (!spriteSheet) {
        printf("\x1b[1;1HErreur de chargement du sprite\n");
    }

    C2D_Sprite presidentSprite;
    C2D_Sprite bgTopSprite;
    C2D_Sprite bgBottomSprite;
    float baseWidth = 0.0f;
    float baseHeight = 0.0f;
    bool hasBgTop = false;
    bool hasBgBottom = false;

    if (spriteSheet) {
        size_t numSprites = C2D_SpriteSheetCount(spriteSheet);
        
        if (numSprites > 0) {
            C2D_SpriteFromSheet(&presidentSprite, spriteSheet, 0);
            C2D_SpriteSetCenter(&presidentSprite, 0.5f, 0.5f);
            C2D_SpriteSetPos(&presidentSprite, 160.0f, 120.0f); 
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
    }

    int argent = 0;
    float scale = 1.0f;

    u32 colorTop = C2D_Color32(45, 15, 65, 255);
    u32 colorWhite = C2D_Color32(255, 255, 255, 255);
    u32 colorGold  = C2D_Color32(255, 215, 0, 255);

    while (aptMainLoop()) {
        hidScanInput(); 
        u32 kDown = hidKeysDown();
        
        if (kDown & KEY_START) {
            break;
        }

        if (kDown & KEY_TOUCH) {
            touchPosition touch;
            hidTouchRead(&touch);
            
            if (touch.px >= 160 - 50 && touch.px <= 160 + 50 &&
                touch.py >= 120 - 50 && touch.py <= 120 + 50) {
                argent += 10;
                scale = 1.5f;
            }
        }

        C2D_TextBufClear(dynamicBuf);
        char moneyStr[64];
        snprintf(moneyStr, sizeof(moneyStr), "Compte en banque : %d euros", argent);
        C2D_Text moneyText;
        C2D_TextParse(&moneyText, dynamicBuf, moneyStr);
        C2D_TextOptimize(&moneyText);

        if (scale > 1.0f) {
            scale -= 0.06f;
            if (scale < 1.0f) scale = 1.0f;
        }

        if (spriteSheet && C2D_SpriteSheetCount(spriteSheet) > 0) {
            presidentSprite.params.pos.w = baseWidth * scale;
            presidentSprite.params.pos.h = baseHeight * scale;

            float wiggle = sin(scale * 30.0f) * (scale - 1.0f) * 0.3f;
            presidentSprite.params.angle = wiggle;
        }

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        
        C2D_TargetClear(topScreen, colorTop);
        C2D_SceneBegin(topScreen);
        
        if (hasBgTop) {
            C2D_DrawSprite(&bgTopSprite);
        }
        
        C2D_DrawText(&titleText, C2D_WithColor, 20.0f, 20.0f, 0.5f, 0.6f, 0.6f, colorWhite);
        C2D_DrawText(&moneyText, C2D_WithColor, 20.0f, 80.0f, 0.5f, 0.8f, 0.8f, colorGold);
        C2D_DrawText(&quitText, C2D_WithColor, 20.0f, 210.0f, 0.5f, 0.5f, 0.5f, colorWhite);

        C2D_TargetClear(bottomScreen, colorTop);
        C2D_SceneBegin(bottomScreen);
        
        if (hasBgBottom) {
            C2D_DrawSprite(&bgBottomSprite);
        }
        if (spriteSheet && C2D_SpriteSheetCount(spriteSheet) > 0) {
            C2D_DrawSprite(&presidentSprite);
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