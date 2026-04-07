#include <3ds.h>
#include "game.h"
#include "render.h"

int main(int argc, char **argv) {
    gfxInitDefault();
    romfsInit();
    
    Render_Init();

    PlayerData player;
    Game_Init(&player);

    int clickAnimTimer = 0;
    int shopScroll = 0;

    while (aptMainLoop()) {
        hidScanInput(); 
        u32 kDown = hidKeysDown();
        
        if (kDown & KEY_START) {
            break;
        }
        
        Game_UpdatePassiveIncome(&player);
        
        if (player.state == STATE_TITLE) {
            if (kDown & (KEY_TOUCH | KEY_A)) {
                player.state = STATE_MAIN;
            }
        } 
        else if (player.state == STATE_MAIN) {
            bool clicked = false;
            
            if (kDown & KEY_A) {
                clicked = true;
            } else if (kDown & KEY_TOUCH) {
                touchPosition touch;
                hidTouchRead(&touch);
                
                if (touch.px >= 260 && touch.px <= 320 && touch.py >= 0 && touch.py <= 50) {
                    player.state = STATE_SHOP;
                    Render_UpdateShopText(&player, shopScroll);
                } 
                else if (touch.px >= 110 && touch.px <= 210 && touch.py >= 90 && touch.py <= 190) {
                    clicked = true;
                }
            }

            if (clicked) {
                Game_Click(&player);
                clickAnimTimer = 8;
            }

            if (clickAnimTimer > 0) clickAnimTimer--;
        } 
        else if (player.state == STATE_SHOP) {
            bool ui_needs_update = false;

            if (kDown & KEY_B) {
                player.state = STATE_MAIN;
            }
            
            if (kDown & (KEY_DUP | KEY_CPAD_UP)) {
                if (shopScroll > 0) { shopScroll--; ui_needs_update = true; }
            }
            if (kDown & (KEY_DDOWN | KEY_CPAD_DOWN)) {
                if (shopScroll < NB_ITEMS_BOUTIQUE - 4) { shopScroll++; ui_needs_update = true; }
            }
            
            if (kDown & KEY_TOUCH) {
                touchPosition touch;
                hidTouchRead(&touch);
                
                if (touch.py >= 35) {
                    int uiIndex = (touch.py - 35) / 45;
                    if (uiIndex >= 0 && uiIndex < 4) {
                        if (Game_BuyItem(&player, shopScroll + uiIndex)) ui_needs_update = true;
                    }
                }
            }

            if (ui_needs_update) {
                Render_UpdateShopText(&player, shopScroll);
            }
        }
        
        Render_UpdateMoneyText(&player);

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        
        if (player.state == STATE_TITLE) {
            Render_DrawTitleScreen();
        } else if (player.state == STATE_MAIN) {
            Render_DrawMainScreen(&player, clickAnimTimer);
        } else if (player.state == STATE_SHOP) {
            Render_DrawShopScreen(&player, shopScroll);
        }

        C3D_FrameEnd(0);
    }

    Render_Exit();
    romfsExit();
    gfxExit();
    
    return 0;
}