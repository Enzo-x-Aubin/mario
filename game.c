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