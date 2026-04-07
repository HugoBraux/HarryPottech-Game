#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <3ds.h>

#define NB_MEMBRES 26
#define NB_GOODIES 6
#define NB_EVENTS 4
#define INFLUENCE_MAX 1000000LL

typedef enum {
    STATE_TITLE = 0,
    STATE_MAIN = 1,
    STATE_SHOP = 2
} GameState;

typedef enum {
    ITEM_MEMBRE = 0,
    ITEM_GOODY = 1,
    ITEM_EVENT = 2
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
    float render_x;
    float render_y;
    int visual_type;
} ItemBoutique;

typedef struct {
    bool active;
    float x;
    float y;
    float speed_x;
    float speed_y;
    u64 next_spawn_time;
    char popup_text[32];
    int popup_timer;
    float popup_x;
    float popup_y;
} VifDOr;

typedef struct {
    long long argent;
    double fraction_argent; 
    long long click_power;
    long long revenu_passif_sec;
    long long influence;
    GameState state;
    u64 last_update_time;
    ItemBoutique membres[NB_MEMBRES];
    ItemBoutique goodies[NB_GOODIES];
    ItemBoutique events[NB_EVENTS];
    VifDOr vif;
} PlayerData;

void Game_Init(PlayerData* player);
void Game_Save(PlayerData* player);
void Game_Load(PlayerData* player);
void Game_UpdatePassiveIncome(PlayerData* player);
void Game_Click(PlayerData* player);
bool Game_BuyItem(PlayerData* player, ItemType type, int index);
void Game_InitDefaultItems(PlayerData* player);
void Game_UpdateVifDOr(PlayerData* player);
bool Game_ClickVifDOr(PlayerData* player, int tx, int ty);

#endif