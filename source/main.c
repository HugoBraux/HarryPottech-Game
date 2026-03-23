#include <3ds.h>
#include <stdio.h>

int main(int argc, char **argv) {
    // Initialisation du système graphique
    gfxInitDefault();

    // Création de deux objets pour gérer le texte séparément
    PrintConsole topScreen, bottomScreen;

    // Assigner la console texte à l'écran du haut et du bas
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);

    int argent = 0;

    // Boucle principale du jeu
    while (aptMainLoop()) {
        gspWaitForVBlank();
        
        // Lire les boutons et l'écran tactile
        hidScanInput(); 
        u32 kDown = hidKeysDown();
        
        // Quitter si START est pressé
        if (kDown & KEY_START) {
            break;
        }

        // Ajouter de l'argent si on touche l'écran
        if (kDown & KEY_TOUCH) {
            argent += 10;
        }

        // --- GESTION DE L'ÉCRAN DU HAUT ---
        consoleSelect(&topScreen);
        // \x1b[Y;XH permet de positionner le texte à la ligne Y et colonne X
        printf("\x1b[2;2H=== HARRYPOTTECH - LISTE BDE ===");
        printf("\x1b[5;2HArgent de la liste : %d euros   ", argent);
        printf("\x1b[28;2HAppuyez sur START pour quitter.");

        // --- GESTION DE L'ÉCRAN DU BAS (TACTILE) ---
        consoleSelect(&bottomScreen);
        printf("\x1b[2;2H--- ECRAN TACTILE ---");
        printf("\x1b[5;2HTapotez ici pour gagner de l'argent !");

        // Afficher les coordonnées exactes du stylet/souris
        if (kDown & KEY_TOUCH) {
            touchPosition touch;
            hidTouchRead(&touch);
            printf("\x1b[8;2HDernier clic en X:%03d Y:%03d", touch.px, touch.py);
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
    }

    // Fermeture propre
    gfxExit();
    return 0;
}