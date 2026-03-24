# HarryPottech-Game

🧙 HarryPottech the great adventure est un jeu homebrew développé pour 3DS en langage C, qui met à l'honneur HarryPottech, liste BDE à Polytech Tours 2026. 🧙

# Description du jeu :

Il s'agit d'un jeu de type clicker où le but est de rapporter de l'argent à la liste, dans le but d'organiser des événements battre la liste adverse. L'objectif principal est donc d'avoir le plus d'influence.

# V1 du jeu :

  <u>Lancement du jeu :</u>
  
  - Lancement du jeu et écran d'acceuil avec le logo HarryPottech - The Great Adventure sur l'écran du haut. Sur l'écran du bas, bouton "démarrer la campagne" ou "reprendre la sauvegarde"
  
  <u>Jeu principal :</u>
  - Sur l'écran du bas : zone de clique pour gagner de l'argent (La tête du Président avec des lunettes rondes et un éclair) et boutons pour ouvrir la boutique, les événements, et sauvegarder.
  - Sur l'écran du haut : affichage du compte en banque / du niveau d'influence vs La Révolutionaliste / des statistiques de génération (argent par seconde) / historique des derniers évenements

  <u>Boutique :</u>
  - Recruter des membres de la liste
  - Organiser des événements

  <u>Boosts :</u>
  - Membres de la liste : cliquent automatiquement à la place du joueur (certains rapportent plus que d'autres selon leur rareté). Le prix est évolutif à chaque membre recruté
  - Goodies : Améliorent la puissance de vos propres clics (clic manuel)
  - Events : les events rapportent de l'influence. Les events disponibles sont le petit déjeuner, le repas, l'afterwort et la grosse soirée

  <u>Sauvegarde :</u>
  - Sauvegarde locale sur la carte SD de la 3DS pour garder sa progression.

# Fonctions additionnelles :

- Effets sonores de sorts magiques lors des clics et une musique en fond.
- Des évenements se déclenchent aléatoirement (par exemple perte d'influence car évenement de l'autre équipe, attraper le vif d'or sur l'écran)
- Si premier lancement du jeu, petite cinématique avec le choixpeau qui explique le principe du jeu
- Succès et trophées

# Equipe de développement :

Le jeu est développé par Hugo Braux et Baptiste Guilleux, étudiants en troisième année ISIE à Polytech Tours.

Baptiste s'occupe de la gestion de projet ainsi que du développement général.
Hugo s'occupe du game design ainsi que du développement.

# Technologies utilisées :

Langages : C

Bibliothèques système et graphiques :

libctru : Bibliothèque standard pour l'accès aux fonctionnalités matérielles de la 3DS.

citro2d : Moteur de rendu 2D utilisé pour l'affichage des textes, des fonds et du sprite du président.

citro3d : Bibliothèque 3D, initialisée comme base matérielle pour le rendu.

Math : Bibliothèque mathématique standard de C utilisée pour gérer les calculs d'animation des sprites.

Chaîne de compilation (Toolchain) :

devkitARM (devkitPro) : Compilateur et environnement de développement ciblant l'architecture ARM de la 3DS.

Make : Outil d'automatisation pour la construction du projet.

Outils de traitement d'assets :

tex3ds : Outil de conversion permettant de transformer les images et les fichiers .t3s en textures lisibles par la 3DS (.t3x).

smdhtool : Outil utilisé pour empaqueter l'icône, le titre, l'auteur et la description dans le fichier .smdh.

Émulation et Tests :

Lime 3DS : Émulateur 3DS utilisé pour exécuter et tester le jeu pendant le développement.
