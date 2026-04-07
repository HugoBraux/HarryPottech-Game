#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

void format_number(long long num, char* buf, size_t size) {
    if (num >= 1000000000LL) {
        snprintf(buf, size, "%.2ft", num / 1000000000.0);
    } else if (num >= 1000000LL) {
        snprintf(buf, size, "%.2fm", num / 1000000.0);
    } else if (num >= 1000LL) {
        snprintf(buf, size, "%.2fk", num / 1000.0);
    } else {
        snprintf(buf, size, "%lld", num);
    }
}

struct Personnage {
    char nom[32];
    long long cout;
    long long revenu_sec;
    int niveau;
};

#define NB_PERSOS 12

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

    C2D_Text startText, moneyText, shopText;
    
    C2D_TextParse(&startText, staticBuf, "Cliquez ou Appuyez sur A");
    C2D_TextParse(&moneyText, dynamicBuf, "Banque : 0 euros");
    C2D_TextParse(&shopText, staticBuf, "Boutique");
    
    C2D_TextOptimize(&startText);
    C2D_TextOptimize(&shopText);

    C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");

    C2D_Sprite presidentSprite;
    C2D_Sprite bgTopSprite;
    C2D_Sprite bgBottomSprite;
    C2D_Sprite uiRibbonSprite;
    C2D_Sprite logoSprite;
    C2D_Sprite textBubbleSprite;
    C2D_Sprite shopSprite;
    C2D_Sprite melvin1Sprite;
    C2D_Sprite melvin2Sprite;

    float baseWidth = 0.0f;
    float baseHeight = 0.0f;
    float baseMelvinWidth = 0.0f;
    float baseMelvinHeight = 0.0f;
    
    bool hasBgTop = false;
    bool hasBgBottom = false;
    bool hasUiRibbon = false;
    bool hasLogo = false;
    bool hasTextBubble = false;
    bool hasShop = false;
    bool hasMelvin1 = false;
    bool hasMelvin2 = false;
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
        
        if (numSprites > 6) {
            C2D_SpriteFromSheet(&shopSprite, spriteSheet, 6);
            C2D_SpriteSetPos(&shopSprite, 270.0f, 5.0f);
            hasShop = true;
        }

        if (numSprites > 7) {
            C2D_SpriteFromSheet(&melvin1Sprite, spriteSheet, 7);
            C2D_SpriteSetCenter(&melvin1Sprite, 0.5f, 0.5f);
            C2D_SpriteSetPos(&melvin1Sprite, 160.0f, 140.0f);
            baseMelvinWidth = melvin1Sprite.params.pos.w;
            baseMelvinHeight = melvin1Sprite.params.pos.h;
            hasMelvin1 = true;
        }

        if (numSprites > 8) {
            C2D_SpriteFromSheet(&melvin2Sprite, spriteSheet, 8);
            C2D_SpriteSetCenter(&melvin2Sprite, 0.5f, 0.5f);
            C2D_SpriteSetPos(&melvin2Sprite, 160.0f, 140.0f);
            hasMelvin2 = true;
        }
    }

    long long argent = 0;
    long long lastArgent = -1;
    int gameState = 0;
    int clickAnimTimer = 0;
    int shopScroll = 0;
    
    long long clickPower = 1;
    long long clickUpgradeCost = 50;
    long long passiveIncomePerSecond = 0;
    
    struct Personnage persos[NB_PERSOS] = {
        {"Membre 1", 100LL, 2LL, 0},
        {"Membre 2", 500LL, 10LL, 0},
        {"Membre 3", 2500LL, 50LL, 0},
        {"Membre 4", 12500LL, 250LL, 0},
        {"Membre 5", 62500LL, 1250LL, 0},
        {"Membre 6", 312500LL, 6500LL, 0},
        {"Membre 7", 1562500LL, 35000LL, 0},
        {"Membre 8", 7812500LL, 180000LL, 0},
        {"Membre 9", 39062500LL, 900000LL, 0},
        {"Membre 10", 195312500LL, 4500000LL, 0},
        {"Membre 11", 976562500LL, 25000000LL, 0},
        {"Membre 12", 4882812500LL, 150000000LL, 0}
    };
    
    double fractionalMoney = 0.0;

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
        
        if (gameState > 0) {
            fractionalMoney += passiveIncomePerSecond / 60.0;
            if (fractionalMoney >= 1.0) {
                long long toAdd = (long long)fractionalMoney;
                argent += toAdd;
                fractionalMoney -= toAdd;
            }
        }
        
        if (gameState == 0) {
            if (kDown & (KEY_TOUCH | KEY_A)) {
                gameState = 1;
            }
        } else if (gameState == 1) {
            bool clicked = false;
            
            if (kDown & KEY_A) {
                clicked = true;
            } else if (kDown & KEY_TOUCH) {
                touchPosition touch;
                hidTouchRead(&touch);
                
                if (touch.px >= 260 && touch.px <= 320 && touch.py >= 0 && touch.py <= 50) {
                    gameState = 2;
                } else if (touch.px >= 110 && touch.px <= 210 && touch.py >= 90 && touch.py <= 190) {
                    clicked = true;
                }
            }

            if (clicked) {
                argent += clickPower;
                clickAnimTimer = 8;
            }

            if (clickAnimTimer > 0) {
                clickAnimTimer--;
            }

        } else if (gameState == 2) {
            if (kDown & KEY_B) {
                gameState = 1;
            }
            
            if (kDown & (KEY_DUP | KEY_CPAD_UP)) {
                if (shopScroll > 0) shopScroll--;
            }
            if (kDown & (KEY_DDOWN | KEY_CPAD_DOWN)) {
                if (shopScroll < NB_PERSOS - 4) shopScroll++;
            }
            
            if (kDown & KEY_TOUCH) {
                touchPosition touch;
                hidTouchRead(&touch);
                
                if (touch.py >= 30 && touch.py < 70) {
                    if (argent >= clickUpgradeCost) {
                        argent -= clickUpgradeCost;
                        clickPower += 1;
                        clickUpgradeCost = (long long)(clickUpgradeCost * 2.5);
                    }
                } else if (touch.py >= 70) {
                    int uiIndex = (touch.py - 70) / 42;
                    if (uiIndex >= 0 && uiIndex < 4) {
                        int realIndex = shopScroll + uiIndex;
                        if (realIndex < NB_PERSOS) {
                            if (argent >= persos[realIndex].cout) {
                                argent -= persos[realIndex].cout;
                                persos[realIndex].niveau += 1;
                                passiveIncomePerSecond += persos[realIndex].revenu_sec;
                                persos[realIndex].cout = (long long)(persos[realIndex].cout * 1.6);
                            }
                        }
                    }
                }
            }
        }
        
        if (argent != lastArgent || gameState == 2) {
            C2D_TextBufClear(dynamicBuf);
            char moneyStr[128];
            char aStr[32], cStr[32], pStr[32];
            
            format_number(argent, aStr, 32);
            format_number(clickPower, cStr, 32);
            format_number(passiveIncomePerSecond, pStr, 32);
            
            snprintf(moneyStr, sizeof(moneyStr), "Banque : %s e\nClic : %s | Passif : %s/s", aStr, cStr, pStr);
            C2D_TextParse(&moneyText, dynamicBuf, moneyStr);
            
            if (gameState == 2) {
                char shopStr[1024];
                char coutClic[32];
                format_number(clickUpgradeCost, coutClic, 32);
                
                int offset = snprintf(shopStr, sizeof(shopStr), 
                    "Boutique (B:Quitter | Haut/Bas:Defiler)\n\n"
                    "Clic (Nv.%lld) : %se\n\n", 
                    clickPower, coutClic);

                for (int i = 0; i < 4; i++) {
                    int idx = shopScroll + i;
                    if (idx < NB_PERSOS) {
                        char coutP[32];
                        format_number(persos[idx].cout, coutP, 32);
                        offset += snprintf(shopStr + offset, sizeof(shopStr) - offset, 
                            "%s (Nv.%d) : %se\n\n", 
                            persos[idx].nom, persos[idx].niveau, coutP);
                    }
                }
                    
                C2D_TextBufClear(staticBuf);
                C2D_TextParse(&shopText, staticBuf, shopStr);
                C2D_TextOptimize(&shopText);
            }
            lastArgent = argent;
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
            
            if (hasShop) {
                C2D_DrawSprite(&shopSprite);
            }
            
            if (hasMelvin1 && hasMelvin2) {
                if (clickAnimTimer > 0) {
                    melvin2Sprite.params.pos.w = baseMelvinWidth;
                    melvin2Sprite.params.pos.h = baseMelvinHeight;
                    C2D_DrawSprite(&melvin2Sprite);
                } else {
                    melvin1Sprite.params.pos.w = baseMelvinWidth;
                    melvin1Sprite.params.pos.h = baseMelvinHeight;
                    C2D_DrawSprite(&melvin1Sprite);
                }
            } else {
                C2D_DrawCircleSolid(160.0f, 140.0f, 0.0f, 45.0f, colorWhite);
            }

        } else if (gameState == 2) {
            C2D_TargetClear(topScreen, colorDeepPurple);
            C2D_SceneBegin(topScreen);
            C2D_DrawText(&moneyText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, 200.0f, 130.0f, 0.5f, 1.0f, 1.0f, colorGold);

            C2D_TargetClear(bottomScreen, colorBurgundy);
            C2D_SceneBegin(bottomScreen);
            C2D_DrawText(&shopText, C2D_WithColor | C2D_AtBaseline | C2D_AlignCenter, 160.0f, 25.0f, 0.5f, 0.6f, 0.6f, colorWhite);
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