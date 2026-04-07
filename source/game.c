#include "game.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void Game_InitDefaultItems(PlayerData* player) {
    ItemBoutique m[NB_MEMBRES] = {
        {ITEM_MEMBRE, 1, "Membre 1", 15LL, 1LL, "Rapporte 1/sec", 0, 15LL},
        {ITEM_MEMBRE, 2, "Membre 2", 50LL, 3LL, "Rapporte 3/sec", 0, 50LL},
        {ITEM_MEMBRE, 3, "Membre 3", 150LL, 8LL, "Rapporte 8/sec", 0, 150LL},
        {ITEM_MEMBRE, 4, "Membre 4", 400LL, 18LL, "Rapporte 18/sec", 0, 400LL},
        {ITEM_MEMBRE, 5, "Membre 5", 1000LL, 40LL, "Rapporte 40/sec", 0, 1000LL},
        {ITEM_MEMBRE, 6, "Membre 6", 2500LL, 90LL, "Rapporte 90/sec", 0, 2500LL},
        {ITEM_MEMBRE, 7, "Membre 7", 6000LL, 200LL, "Rapporte 200/sec", 0, 6000LL},
        {ITEM_MEMBRE, 8, "Membre 8", 15000LL, 450LL, "Rapporte 450/sec", 0, 15000LL},
        {ITEM_MEMBRE, 9, "Membre 9", 35000LL, 1000LL, "Rapporte 1000/sec", 0, 35000LL},
        {ITEM_MEMBRE, 10, "Membre 10", 80000LL, 2100LL, "Rapporte 2100/sec", 0, 80000LL},
        {ITEM_MEMBRE, 11, "Membre 11", 180000LL, 4500LL, "Rapporte 4500/sec", 0, 180000LL},
        {ITEM_MEMBRE, 12, "Membre 12", 400000LL, 9500LL, "Rapporte 9500/sec", 0, 400000LL},
        {ITEM_MEMBRE, 13, "Membre 13", 900000LL, 20000LL, "Rapporte 20000/sec", 0, 900000LL},
        {ITEM_MEMBRE, 14, "Membre 14", 2000000LL, 42000LL, "Rapporte 42000/sec", 0, 2000000LL},
        {ITEM_MEMBRE, 15, "Membre 15", 4500000LL, 90000LL, "Rapporte 90000/sec", 0, 4500000LL},
        {ITEM_MEMBRE, 16, "Membre 16", 10000000LL, 190000LL, "Rapporte 190000/sec", 0, 10000000LL},
        {ITEM_MEMBRE, 17, "Membre 17", 22000000LL, 400000LL, "Rapporte 400000/sec", 0, 22000000LL},
        {ITEM_MEMBRE, 18, "Membre 18", 50000000LL, 850000LL, "Rapporte 850000/sec", 0, 50000000LL},
        {ITEM_MEMBRE, 19, "Membre 19", 110000000LL, 1800000LL, "Rapporte 1800000/sec", 0, 110000000LL},
        {ITEM_MEMBRE, 20, "Membre 20", 250000000LL, 3900000LL, "Rapporte 3900000/sec", 0, 250000000LL},
        {ITEM_MEMBRE, 21, "Membre 21", 550000000LL, 8000000LL, "Rapporte 8000000/sec", 0, 550000000LL},
        {ITEM_MEMBRE, 22, "Membre 22", 900000000LL, 12500000LL, "Rapporte 12500000/sec", 0, 900000000LL},
        {ITEM_MEMBRE, 23, "Membre 23", 1300000000LL, 17500000LL, "Rapporte 17500000/sec", 0, 1300000000LL},
        {ITEM_MEMBRE, 24, "Membre 24", 1600000000LL, 21000000LL, "Rapporte 21000000/sec", 0, 1600000000LL},
        {ITEM_MEMBRE, 25, "Membre 25", 1900000000LL, 24000000LL, "Rapporte 24000000/sec", 0, 1900000000LL},
        {ITEM_MEMBRE, 26, "Membre 26", 2100000000LL, 26000000LL, "Rapporte 26000000/sec", 0, 2100000000LL}
    };
    
    ItemBoutique g[NB_GOODIES] = {
        {ITEM_GOODY, 27, "Goody 1", 50LL, 2LL, "Ameliore tes clics", 0, 50LL},
        {ITEM_GOODY, 28, "Goody 2", 500LL, 15LL, "Ameliore tes clics", 0, 500LL},
        {ITEM_GOODY, 29, "Goody 3", 5000LL, 100LL, "Ameliore tes clics", 0, 5000LL},
        {ITEM_GOODY, 30, "Goody 4", 50000LL, 800LL, "Ameliore tes clics", 0, 50000LL},
        {ITEM_GOODY, 31, "Goody 5", 500000LL, 6000LL, "Ameliore tes clics", 0, 500000LL},
        {ITEM_GOODY, 32, "Goody 6", 5000000LL, 50000LL, "Ameliore tes clics", 0, 5000000LL}
    };
    
    ItemBoutique e[NB_EVENTS] = {
        {ITEM_EVENT, 33, "Event 1", 1000LL, 50LL, "Boost d influence", 0, 1000LL},
        {ITEM_EVENT, 34, "Event 2", 10000LL, 600LL, "Boost d influence", 0, 10000LL},
        {ITEM_EVENT, 35, "Event 3", 100000LL, 8000LL, "Boost d influence", 0, 100000LL},
        {ITEM_EVENT, 36, "Event 4", 1000000LL, 100000LL, "Boost d influence", 0, 1000000LL}
    };

    memcpy(player->membres, m, sizeof(m));
    memcpy(player->goodies, g, sizeof(g));
    memcpy(player->events, e, sizeof(e));

    for(int i = 0; i < NB_MEMBRES; i++) {
        player->membres[i].render_x = -1.0f;
        player->membres[i].render_y = -1.0f;
        player->membres[i].visual_type = i % 2;
    }
}

void Game_Init(PlayerData* player) {
    srand(osGetTime());
    player->argent = 0;
    player->fraction_argent = 0.0;
    player->click_power = 1;
    player->revenu_passif_sec = 0;
    player->influence = 0;
    player->state = STATE_TITLE;
    player->last_update_time = osGetTime();
    Game_InitDefaultItems(player);
}

void Game_Save(PlayerData* player) {
    FILE* f = fopen("sdmc:/harrypottech_save.bin", "wb");
    if (f) {
        fwrite(player, sizeof(PlayerData), 1, f);
        fclose(f);
    }
}

void Game_Load(PlayerData* player) {
    FILE* f = fopen("sdmc:/harrypottech_save.bin", "rb");
    if (f) {
        fread(player, sizeof(PlayerData), 1, f);
        fclose(f);
        player->last_update_time = osGetTime();

        for(int i = 0; i < NB_MEMBRES; i++) {
            if (player->membres[i].niveau > 0 && player->membres[i].render_x == -1.0f) {
                do {
                    player->membres[i].render_x = 20.0f + (rand() % 280);
                    player->membres[i].render_y = 60.0f + (rand() % 140);
                } while ((player->membres[i].render_x - 160.0f) * (player->membres[i].render_x - 160.0f) + 
                         (player->membres[i].render_y - 140.0f) * (player->membres[i].render_y - 140.0f) < 3600.0f);
            }
        }
    }
}

void Game_UpdatePassiveIncome(PlayerData* player) {
    u64 current_time = osGetTime();
    u64 delta_ms = current_time - player->last_update_time;
    player->last_update_time = current_time;

    if (player->state > 0 && player->revenu_passif_sec > 0) {
        player->fraction_argent += (player->revenu_passif_sec * delta_ms) / 1000.0;
        if (player->fraction_argent >= 1.0) {
            long long toAdd = (long long)player->fraction_argent;
            player->argent += toAdd;
            player->fraction_argent -= toAdd;
        }
    }
}

void Game_Click(PlayerData* player) {
    player->argent += player->click_power;
}

bool Game_BuyItem(PlayerData* player, ItemType type, int item_index) {
    ItemBoutique* item = NULL;

    if (type == ITEM_MEMBRE && item_index >= 0 && item_index < NB_MEMBRES) {
        item = &player->membres[item_index];
    } else if (type == ITEM_GOODY && item_index >= 0 && item_index < NB_GOODIES) {
        item = &player->goodies[item_index];
    } else if (type == ITEM_EVENT && item_index >= 0 && item_index < NB_EVENTS) {
        item = &player->events[item_index];
    }

    if (item && player->argent >= item->cout_actuel) {
        if (type == ITEM_MEMBRE && item->niveau == 0) {
            do {
                item->render_x = 20.0f + (rand() % 280);
                item->render_y = 60.0f + (rand() % 140);
            } while ((item->render_x - 160.0f) * (item->render_x - 160.0f) + 
                     (item->render_y - 140.0f) * (item->render_y - 140.0f) < 3600.0f);
        }

        player->argent -= item->cout_actuel;
        item->niveau++;

        if (type == ITEM_MEMBRE) {
            player->revenu_passif_sec += item->revenu;
        } else if (type == ITEM_GOODY) {
            player->click_power += item->revenu;
        } else if (type == ITEM_EVENT) {
            player->influence += item->revenu;
        }

        item->cout_actuel = (long long)(item->cout_actuel * 1.6);
        return true;
    }
    return false;
}