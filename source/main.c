#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char **argv) {
    // Initialisation du système graphique et RomFS
    romfsInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    // Initialisation des cibles de rendu 2D pour les DEUX écrans
    C3D_RenderTarget* topScreen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottomScreen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    // Initialisation des buffers pour le texte graphique (Citro2D)
    C2D_TextBuf staticBuf = C2D_TextBufNew(4096);
    C2D_TextBuf dynamicBuf = C2D_TextBufNew(4096);

    // Préparation des textes statiques
    C2D_Text titleText, quitText;
    C2D_TextParse(&titleText, staticBuf, "HarryPottech - The Great Adventure");
    C2D_TextOptimize(&quitText);

    // Chargement de la feuille de sprites générée depuis sprites.t3x
    C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    if (!spriteSheet) {
        printf("\x1b[1;1HErreur de chargement du sprite\n");
    }

    // Initialisation du sprite du président
    C2D_Sprite presidentSprite;
    float baseWidth = 0.0f;
    float baseHeight = 0.0f;

    if (spriteSheet) {
        C2D_SpriteFromSheet(&presidentSprite, spriteSheet, 0);
        // On définit le centre de l'image comme point de pivot (pour rotation/scale)
        C2D_SpriteSetCenter(&presidentSprite, 0.5f, 0.5f);
        // Position au centre de l'écran du bas (320x240)
        C2D_SpriteSetPos(&presidentSprite, 160.0f, 120.0f); 

        // On sauvegarde la taille d'origine pour calculer le scale correctement
        baseWidth = presidentSprite.params.pos.w;
        baseHeight = presidentSprite.params.pos.h;
    }

    int argent = 0;
    float scale = 1.0f;

    // Couleurs pour le fond dégradé (Haut: Violet sombre, Bas: Violet magique clair)
    u32 colorTop = C2D_Color32(45, 15, 65, 255);
    u32 colorBot = C2D_Color32(148, 0, 211, 255);
    // Couleurs pour le texte
    u32 colorWhite = C2D_Color32(255, 255, 255, 255);
    u32 colorGold  = C2D_Color32(255, 215, 0, 255);

    // Boucle principale du jeu
    while (aptMainLoop()) {
        hidScanInput(); 
        u32 kDown = hidKeysDown();
        
        // Quitter si START est pressé
        if (kDown & KEY_START) {
            break;
        }

        // Action de clic
        if (kDown & KEY_TOUCH) {
            touchPosition touch;
            hidTouchRead(&touch);
            
            // Bounding box (zone de clic de +/- 50 pixels autour du centre)
            if (touch.px >= 160 - 50 && touch.px <= 160 + 50 &&
                touch.py >= 120 - 50 && touch.py <= 120 + 50) {
                argent += 10;
                scale = 1.5f; // Déclenche le "pop" (agrandissement max)
            }
        }

        // --- GESTION DU TEXTE DYNAMIQUE ---
        // On vide le buffer dynamique et on recrée le texte de l'argent à chaque frame
        C2D_TextBufClear(dynamicBuf);
        char moneyStr[64];
        snprintf(moneyStr, sizeof(moneyStr), "Compte en banque : %d euros", argent);
        C2D_Text moneyText;
        C2D_TextParse(&moneyText, dynamicBuf, moneyStr);
        C2D_TextOptimize(&moneyText);

        // --- GESTION DE L'ÉCRAN DU BAS (Animation Sprite) ---
        // Réduction progressive de la taille (retour élastique)
        if (scale > 1.0f) {
            scale -= 0.06f;
            if (scale < 1.0f) scale = 1.0f;
        }

        if (spriteSheet) {
            // Modification manuelle des paramètres du sprite
            presidentSprite.params.pos.w = baseWidth * scale;
            presidentSprite.params.pos.h = baseHeight * scale;

            // Effet de tremblement (wiggle) mathématique basé sur le scale
            float wiggle = sin(scale * 30.0f) * (scale - 1.0f) * 0.3f;
            presidentSprite.params.angle = wiggle;
        }

        // --- DÉBUT DU RENDU ---
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        
        // 1. RENDU ÉCRAN DU HAUT (400x240)
        C2D_TargetClear(topScreen, colorTop);
        C2D_SceneBegin(topScreen);
        
        // Fond dégradé (Attention : 400 de largeur ici !)
        C2D_DrawRectangle(0, 0, 0, 400, 240, colorTop, colorTop, colorBot, colorBot);
        
        // Dessin des textes (x, y, z, scaleX, scaleY, color)
        C2D_DrawText(&titleText, C2D_WithColor, 20.0f, 20.0f, 0.5f, 0.6f, 0.6f, colorWhite);
        C2D_DrawText(&moneyText, C2D_WithColor, 20.0f, 80.0f, 0.5f, 0.8f, 0.8f, colorGold);
        C2D_DrawText(&quitText, C2D_WithColor, 20.0f, 210.0f, 0.5f, 0.5f, 0.5f, colorWhite);

        // 2. RENDU ÉCRAN DU BAS (320x240)
        C2D_TargetClear(bottomScreen, colorTop);
        C2D_SceneBegin(bottomScreen);
        
        // Fond dégradé (320 de largeur)
        C2D_DrawRectangle(0, 0, 0, 320, 240, colorTop, colorTop, colorBot, colorBot);

        // Dessiner le sprite par-dessus
        if (spriteSheet) {
            C2D_DrawSprite(&presidentSprite);
        }

        C3D_FrameEnd(0);
    }

    // Libération de la mémoire et fermeture propre
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