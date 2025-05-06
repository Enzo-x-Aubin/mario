#include "game.h"
#include "file.h"
#include "charactere.h"
#include "event.h"

int jouer(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255,255,255,120);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
	//charger image et personnage. 
    SDL_Rect position_initiale = {400, 400, WIDTH_MARIO, HEIGHT_MARIO};
    Personnage mario;
    init_mario(&mario, renderer, position_initiale);
    int img_mario = MARIO_DROITE;

    int continuer = 1; //a utiliser pour savoir si on continue la boucle du jeu ou si on arrête. 
    SDL_Event events;
    
    while(continuer){ //coeur du jeu ici, les actions seront repété pour faire le déplacement des différentes images, ...
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, mario.image[img_mario], NULL, &mario.position);
        SDL_RenderPresent(renderer);
        SDL_PollEvent(&events);
        continuer = event(&mario, renderer, events, &img_mario);
        deplacer(&mario);
        
    }
    
	//a vous de compléter, au fur et à mesure, les deux fonctions en dessous pour bien faire le nettoyage. 
    //LibererMap(map, sprites);
    //freePersonnage(mario, goomba, nbGoomba);

    return continuer;
}

void AfficherMapAvecSprites(Map* map, Sprites* sprites, SDL_Renderer* renderer) {
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            int spriteIndex = map->LoadedMap[i][j];
            if (spriteIndex >= 0 && spriteIndex < NbSprites) {
                SDL_Rect dest = { j * Size_Sprite, i * Size_Sprite, Size_Sprite, Size_Sprite };
                SDL_RenderCopy(renderer, sprites[spriteIndex].sprite, NULL, &dest);
            }
        }
    }
}

int jouer(SDL_Renderer* renderer) {
    // Charger l'image et le personnage
    
    // Initialisation de la map
    Map map = {0};
    LireLevel0(&map);

    // Initialisation des sprites
    Sprites sprites[NbSprites];
    InitialiserSprites(sprites, renderer);

    // Initialisation des variables
    int continuer = 1; // À utiliser pour savoir si on continue la boucle du jeu ou si on arrête
    SDL_Event event;   // Déclaration de l'événement

    // Configuration de la couleur de fond (blanc)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

    while (continuer) { // Coeur du jeu ici
        SDL_RenderClear(renderer);

        // Afficher la map avec les sprites
        AfficherMapAvecSprites(&map, sprites, renderer);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {  // Boucle qui gère les événements
            switch (event.type) {
                case SDL_QUIT:  // Si l'utilisateur ferme la fenêtre
                    continuer = 0;
                    break;

                case SDL_KEYDOWN:  // Si une touche est pressée
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:  // Si la touche échappe est pressée
                            continuer = 0;
                            break;

                        case SDLK_1:  // Si la touche '1' est pressée
                            AfficherMapAvecSprites(&map, sprites, renderer); // Afficher la map avec les sprites
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }
    }

    // Libération des ressources une fois le jeu terminé
    for (int i = 0; i < map.height; i++) {
        free(map.LoadedMap[i]); // Libération de chaque ligne du tableau
    }
    free(map.LoadedMap); // Libération du tableau principal

    for (int i = 0; i < NbSprites; i++) {
        SDL_DestroyTexture(sprites[i].sprite); // Libération des textures des sprites
    }

    return continuer;  // Retourne la valeur de "continuer" pour quitter ou non
}

void InitialiserSprites(Sprites* sprites, SDL_Renderer* renderer) {
    sprites[0].sprite = loadImage("img/sky.png", renderer);
    sprites[0].traverser = 1; // Le ciel est traversable

    sprites[1].sprite = loadImage("img/sol.png", renderer);
    sprites[1].traverser = 0; // Le sol n'est pas traversable

    sprites[2].sprite = loadImage("img/block.png", renderer);
    sprites[2].traverser = 0; // Le bloc de pierre n'est pas traversable

    sprites[3].sprite = loadImage("img/boite.png", renderer);
    sprites[3].traverser = 0; // La boîte n'est pas traversable

    sprites[4].sprite = loadImage("img/tuyau1.png", renderer);
    sprites[4].traverser = 0; // Tuyau partie 1

    sprites[5].sprite = loadImage("img/tuyau2.png", renderer);
    sprites[5].traverser = 0; // Tuyau partie 2

    sprites[6].sprite = loadImage("img/tuyau3.png", renderer);
    sprites[6].traverser = 0; // Tuyau partie 3

    sprites[7].sprite = loadImage("img/tuyau4.png", renderer);
    sprites[7].traverser = 0; // Tuyau partie 4
}