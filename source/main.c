#include <3ds.h>
#include "game.h"
#include "render.h"

#define UI_SHOP_X_MIN 260
#define UI_SHOP_X_MAX 320
#define UI_SHOP_Y_MIN 0
#define UI_SHOP_Y_MAX 50

#define UI_CLICK_X_MIN 110
#define UI_CLICK_X_MAX 210
#define UI_CLICK_Y_MIN 90
#define UI_CLICK_Y_MAX 190

#define UI_SAVE_X_MIN 0
#define UI_SAVE_X_MAX 60
#define UI_SAVE_Y_MIN 0
#define UI_SAVE_Y_MAX 50

int main(int argc, char **argv) {
    gfxInitDefault();
    romfsInit();
    
    Render_Init();

    PlayerData player;
    Game_Init(&player);
    Game_Load(&player);

    int clickAnimTimer = 0;
    int shopScroll = 0;
    int shopCategory = 0; 

    while (aptMainLoop()) {
        hidScanInput(); 
        u32 kDown = hidKeysDown();
        
        if (kDown & KEY_START) {
            Game_Save(&player);
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
            Game_UpdateVifDOr(&player);
            
            if (kDown & KEY_A) {
                clicked = true;
            } else if (kDown & KEY_TOUCH) {
                touchPosition touch;
                hidTouchRead(&touch);
                
                if (Game_ClickVifDOr(&player, touch.px, touch.py)) {
                    
                }
                else if (touch.px >= UI_SHOP_X_MIN && touch.px <= UI_SHOP_X_MAX && touch.py >= UI_SHOP_Y_MIN && touch.py <= UI_SHOP_Y_MAX) {
                    player.state = STATE_SHOP;
                    Render_UpdateShopText(&player, shopScroll, shopCategory);
                } 
                else if (touch.px >= UI_CLICK_X_MIN && touch.px <= UI_CLICK_X_MAX && touch.py >= UI_CLICK_Y_MIN && touch.py <= UI_CLICK_Y_MAX) {
                    clicked = true;
                }
                else if (touch.px >= UI_SAVE_X_MIN && touch.px <= UI_SAVE_X_MAX && touch.py >= UI_SAVE_Y_MIN && touch.py <= UI_SAVE_Y_MAX) {
                    Game_Save(&player);
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

            if (kDown & KEY_L) {
                shopCategory--;
                if (shopCategory < 0) shopCategory = 2;
                shopScroll = 0;
                ui_needs_update = true;
            }
            if (kDown & KEY_R) {
                shopCategory++;
                if (shopCategory > 2) shopCategory = 0;
                shopScroll = 0;
                ui_needs_update = true;
            }
            
            int currentMaxItems = 0;
            if (shopCategory == 0) currentMaxItems = NB_MEMBRES;
            else if (shopCategory == 1) currentMaxItems = NB_GOODIES;
            else if (shopCategory == 2) currentMaxItems = NB_EVENTS;

            if (kDown & (KEY_DUP | KEY_CPAD_UP)) {
                if (shopScroll > 0) { shopScroll--; ui_needs_update = true; }
            }
            if (kDown & (KEY_DDOWN | KEY_CPAD_DOWN)) {
                if (shopScroll < currentMaxItems - 4 && currentMaxItems > 4) { 
                    shopScroll++; ui_needs_update = true; 
                }
            }
            
            if (kDown & KEY_TOUCH) {
                touchPosition touch;
                hidTouchRead(&touch);
                
                if (touch.py >= 35) {
                    int uiIndex = (touch.py - 35) / 45;
                    if (uiIndex >= 0 && uiIndex < 4) {
                        if (Game_BuyItem(&player, (ItemType)shopCategory, shopScroll + uiIndex)) {
                            ui_needs_update = true;
                        }
                    }
                }
            }

            if (ui_needs_update) {
                Render_UpdateShopText(&player, shopScroll, shopCategory);
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