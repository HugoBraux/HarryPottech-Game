#include "game.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void Game_InitDefaultItems(PlayerData* player) {
    ItemBoutique m[NB_MEMBRES] = {
        {ITEM_MEMBRE, 1, "Quent'Incendio", 15LL, 1LL, "Rapporte 1/sec", 0, 15LL},
        {ITEM_MEMBRE, 2, "Loup Garou", 50LL, 3LL, "Rapporte 3/sec", 0, 50LL},
        {ITEM_MEMBRE, 3, "Doriengorgio", 150LL, 8LL, "Rapporte 8/sec", 0, 150LL},
        {ITEM_MEMBRE, 4, "Mattdragore", 400LL, 18LL, "Rapporte 18/sec", 0, 400LL},
        {ITEM_MEMBRE, 5, "Spero Bayonnum", 1000LL, 40LL, "Rapporte 40/sec", 0, 1000LL},
        {ITEM_MEMBRE, 6, "Hadrig", 2500LL, 90LL, "Rapporte 90/sec", 0, 2500LL},
        {ITEM_MEMBRE, 7, "Dobby’tille", 6000LL, 200LL, "Rapporte 200/sec", 0, 6000LL},
        {ITEM_MEMBRE, 8, "Elisazkaba", 15000LL, 450LL, "Rapporte 450/sec", 0, 15000LL},
        {ITEM_MEMBRE, 9, "EmmaLfoy", 35000LL, 1000LL, "Rapporte 1000/sec", 0, 35000LL},
        {ITEM_MEMBRE, 10, "Ilona Lovegood", 80000LL, 2100LL, "Rapporte 2100/sec", 0, 80000LL},
        {ITEM_MEMBRE, 11, "Juleviosa", 180000LL, 4500LL, "Rapporte 4500/sec", 0, 180000LL},
        {ITEM_MEMBRE, 12, "Loupin", 400000LL, 9500LL, "Rapporte 9500/sec", 0, 400000LL},
        {ITEM_MEMBRE, 13, "Wingardium Lucilosa", 900000LL, 20000LL, "Rapporte 20000/sec", 0, 900000LL},
        {ITEM_MEMBRE, 14, "Marry Potter", 2000000LL, 42000LL, "Rapporte 42000/sec", 0, 2000000LL},
        {ITEM_MEMBRE, 15, "Mathiserpentard", 4500000LL, 90000LL, "Rapporte 90000/sec", 0, 4500000LL},
        {ITEM_MEMBRE, 16, "Maléthis", 10000000LL, 190000LL, "Rapporte 190000/sec", 0, 10000000LL},
        {ITEM_MEMBRE, 17, "Nathangemort", 22000000LL, 400000LL, "Rapporte 400000/sec", 0, 22000000LL},
        {ITEM_MEMBRE, 18, "Noalbus Dumbledore", 50000000LL, 850000LL, "Rapporte 850000/sec", 0, 50000000LL},
        {ITEM_MEMBRE, 19, "Chabada-Kedavra", 110000000LL, 1800000LL, "Rapporte 1800000/sec", 0, 110000000LL},
        {ITEM_MEMBRE, 20, "Sar’pentard", 250000000LL, 3900000LL, "Rapporte 3900000/sec", 0, 250000000LL},
        {ITEM_MEMBRE, 21, "Tangryffondor", 550000000LL, 8000000LL, "Rapporte 8000000/sec", 0, 550000000LL},
        {ITEM_MEMBRE, 22, "BellaTriTrix", 900000000LL, 12500000LL, "Rapporte 12500000/sec", 0, 900000000LL},
        {ITEM_MEMBRE, 23, "TinLestrange", 1300000000LL, 17500000LL, "Rapporte 17500000/sec", 0, 1300000000LL},
        {ITEM_MEMBRE, 24, "BabavadaKedavra", 1600000000LL, 21000000LL, "Rapporte 21000000/sec", 0, 1600000000LL},
        {ITEM_MEMBRE, 25, "Azkabande", 1900000000LL, 24000000LL, "Rapporte 24000000/sec", 0, 1900000000LL},
        {ITEM_MEMBRE, 26, "Expecto Hugonum", 2100000000LL, 26000000LL, "Rapporte 26000000/sec", 0, 2100000000LL}
    };
    
    ItemBoutique g[NB_GOODIES] = {
        {ITEM_GOODY, 27, "Baguette magique", 50LL, 2LL, "Ameliore tes clics", 0, 50LL},
        {ITEM_GOODY, 28, "Gobelet", 500LL, 15LL, "Ameliore tes clics", 0, 500LL},
        {ITEM_GOODY, 29, "bracelet", 5000LL, 100LL, "Ameliore tes clics", 0, 5000LL},
        {ITEM_GOODY, 30, "Repas Nachos", 50000LL, 800LL, "Ameliore tes clics", 0, 50000LL},
        {ITEM_GOODY, 31, "Menu Burger King", 500000LL, 6000LL, "Ameliore tes clics", 0, 500000LL},
        {ITEM_GOODY, 32, "Pull HarryPottech", 5000000LL, 50000LL, "Ameliore tes clics", 0, 5000000LL}
    };
    
    ItemBoutique e[NB_EVENTS] = {
        {ITEM_EVENT, 33, "Hot-Rod", 1000LL, 50LL, "Boost d influence", 0, 1000LL},
        {ITEM_EVENT, 34, "Libération de Dobbi", 10000LL, 600LL, "Boost d influence", 0, 10000LL},
        {ITEM_EVENT, 35, "Serpent-Volant", 100000LL, 8000LL, "Boost d influence", 0, 100000LL},
        {ITEM_EVENT, 36, "Académide de la Bière", 1000000LL, 100000LL, "Boost d influence", 0, 1000000LL}
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
    
    player->vif.active = false;
    player->vif.next_spawn_time = osGetTime() + 10000 + (rand() % 15000);
    player->vif.popup_timer = 0;

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
        player->vif.active = false;
        player->vif.next_spawn_time = osGetTime() + 10000 + (rand() % 15000);
        player->vif.popup_timer = 0;

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

void Game_UpdateVifDOr(PlayerData* player) {
    u64 current_time = osGetTime();
    
    if (player->vif.popup_timer > 0) {
        player->vif.popup_timer--;
        player->vif.popup_y -= 0.5f;
    }

    if (!player->vif.active) {
        if (current_time >= player->vif.next_spawn_time) {
            player->vif.active = true;
            player->vif.y = 50.0f + (rand() % 140);
            
            if (rand() % 2 == 0) {
                player->vif.x = -20.0f;
                player->vif.speed_x = 2.0f + (rand() % 3);
            } else {
                player->vif.x = 340.0f;
                player->vif.speed_x = -(2.0f + (rand() % 3));
            }
            player->vif.speed_y = (rand() % 3) - 1.0f;
        }
    } else {
        player->vif.x += player->vif.speed_x;
        player->vif.y += player->vif.speed_y;
        
        if (player->vif.y < 40.0f || player->vif.y > 220.0f) {
            player->vif.speed_y = -player->vif.speed_y;
        }
        
        if (player->vif.x < -30.0f || player->vif.x > 350.0f) {
            player->vif.active = false;
            player->vif.next_spawn_time = current_time + 10000 + (rand() % 15000);
        }
    }
}

bool Game_ClickVifDOr(PlayerData* player, int tx, int ty) {
    if (player->vif.active) {
        if (tx >= player->vif.x - 10.0f && tx <= player->vif.x + 30.0f &&
            ty >= player->vif.y - 10.0f && ty <= player->vif.y + 30.0f) {
            
            player->vif.active = false;
            player->vif.next_spawn_time = osGetTime() + 10000 + (rand() % 15000);
            
            player->vif.popup_x = tx;
            player->vif.popup_y = ty - 15.0f;
            player->vif.popup_timer = 90;
            
            int reward = rand() % 3;
            if (reward == 0) {
                long long bonus = (long long)(player->argent * 0.4);
                player->argent += bonus;
                snprintf(player->vif.popup_text, sizeof(player->vif.popup_text), "+%lld e", bonus);
            } else if (reward == 1) {
                long long bonus = 100 + (rand() % 400);
                player->influence += bonus;
                snprintf(player->vif.popup_text, sizeof(player->vif.popup_text), "+%lld Inf", bonus);
            } else {
                long long bonus = player->revenu_passif_sec * 30;
                if (bonus == 0) bonus = 50;
                player->argent += bonus;
                snprintf(player->vif.popup_text, sizeof(player->vif.popup_text), "+%lld e", bonus);
            }
            return true;
        }
    }
    return false;
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