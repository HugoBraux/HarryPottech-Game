#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define NB_ITEMS_BOUTIQUE 36

typedef enum {
    STATE_TITLE = 0,
    STATE_MAIN = 1,
    STATE_SHOP = 2
} GameState;

typedef enum {
    ITEM_MEMBRE,
    ITEM_GOODY,
    ITEM_EVENT
} ItemType;

typedef struct {
    ItemType type;
    int id;
    char nom[32];
    long long cout_base;
    long long revenu;
    char description[64];
    int niveau;
    long long cout_actuel;
} ItemBoutique;

typedef struct {
    long long argent;
    double fraction_argent; 
    long long click_power;
    long long revenu_passif_sec;
    long long influence;
    GameState state;
    
    ItemBoutique boutique[NB_ITEMS_BOUTIQUE];
} PlayerData;

void Game_Init(PlayerData* player);
void Game_UpdatePassiveIncome(PlayerData* player);
void Game_Click(PlayerData* player);
bool Game_BuyItem(PlayerData* player, int item_index);

#endif