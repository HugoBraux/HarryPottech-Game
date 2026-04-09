# HarryPottech-Game

🧙 HarryPottech the great adventure est un jeu homebrew développé pour 3DS en langage C, qui met à l'honneur HarryPottech, liste BDE à Polytech Tours 2026. 🧙

# Description du jeu :

Il s'agit d'un jeu de type clicker où le but est de rapporter de l'argent à la liste, dans le but d'organiser des événements pour battre la liste adverse. L'objectif principal est donc d'avoir le plus d'influence.

# Equipe de développement :

Le jeu est développé par Hugo Braux et Baptiste Guilleux, étudiants en troisième année ISIE à Polytech Tours.

Baptiste est responsable de la gestion de projet ainsi que de l'environnement de développement et du développement.
Hugo s'occupe du game design, des textures ainsi que du développement.

## Installation et Compilation

### Pour les joueurs

**Sur une console de la famille 3DS (équipée d'un Custom Firmware comme Luma3DS) :**
* *Via le Homebrew Launcher (.3dsx)* : Placez le fichier `HarryPottech.3dsx` dans le dossier `3ds/` à la racine de votre carte SD. Lancez le Homebrew Launcher et sélectionnez le jeu.

**Sur Émulateur (PC, Mac, Linux, Android, iOS) :**
Téléchargez l'émulateur [Lime 3DS](https://lime3ds.github.io/). Lancez l'émulateur, faites **Fichier > Charger un fichier...** et sélectionnez `HarryPottech.3dsx`.

### Pour les développeurs (Compilation depuis les sources)
1. Installez l'environnement [devkitPro](https://devkitpro.org/) avec les paquets `devkitARM`, `libctru`, `citro2d` et `citro3d`.
2. Clonez ce dépôt.
3. Ouvrez un terminal à la racine du projet et lancez simplement la commande `make`.

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

# Fonctions principales (v.1.0.0) :

  Lancement du jeu :
  
  - Lancement du jeu et écran d'accueil avec le logo HarryPottech sur l'écran du haut. Sur l'écran du bas, bouton "démarrer la campagne".
  
  Jeu principal :
  - Sur l'écran du bas : zone de clique pour gagner de l'argent et boutons pour ouvrir la boutique / sprites animés à chaque fois qu'on achète un nouveau membre.
  - Sur l'écran du haut : affichage du compte en banque / du niveau d'influence / du niveau de clic / des statistiques de génération (argent par seconde)

  Boutique :
  - Recruter des membres de la liste
  - Acheter des goodies
  - Organiser des événements

  Boosts :
  - Membres de la liste : cliquent automatiquement à la place du joueur (certains rapportent plus que d'autres selon leur rareté). Le prix est évolutif à chaque membre recruté
  - Goodies : Améliorent la puissance de vos propres clics (clic manuel)
  - Events : les events rapportent de l'influence. Les events disponibles sont le petit déjeuner, le repas, l'afterwork et la grosse soirée

Evenements aléatoires :
  - Attrapez le vif d'or avant qu'il ne s'échappe pour gagner de l'argent.

  Sauvegarde :
  - Sauvegarde locale automatique sur la carte SD de la 3DS pour garder sa progression.

# Equilibrage et métriques

L'économie du jeu est pensée pour garder le joueur engagé et éviter qu'il ne s'ennuie :

- **Calcul des prix :** À chaque fois que vous achetez une amélioration (Membre, Goody ou Événement), son prix augmente selon la formule suivante :
  `Nouveau Prix = Ancien Prix × 1,6`
  Cela empêche d'acheter en boucle les objets les moins chers et force le joueur à économiser pour débloquer des choses plus puissantes.
- **Évolution du gameplay :** Le joueur commence par devoir cliquer beaucoup lui-même (puissance augmentée par les Goodies). Mais plus le jeu avance, plus il achète de Membres qui génèrent de l'argent tout seuls. Le jeu passe donc naturellement d'un "cliqueur frénétique" à un jeu de gestion passive.
- **Récompenses adaptatives :** L'argent gagné en attrapant le Vif d'Or n'est pas fixe. Il s'adapte à la richesse actuelle du joueur (par exemple, en donnant l'équivalent de 30 secondes de revenu passif). Grâce à cela, le Vif d'Or reste toujours très intéressant à attraper, même à la fin du jeu quand on est millionnaire.

# Fonctions additionnelles :

- Effets sonores de sorts magiques lors des clics et une musique en fond.
- Des évenements se déclenchent aléatoirement (par exemple perte d'influence car évenement de l'autre équipe, attraper le vif d'or sur l'écran)
- Si premier lancement du jeu, petite cinématique avec le choixpeau qui explique le principe du jeu
- Succès et trophées
