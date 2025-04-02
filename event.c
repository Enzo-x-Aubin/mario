#include "event.h"
#include "charactere.h"

void deplacer(Personnage *mario);
int statique(Personnage *mario);

int event(Personnage *mario, SDL_Renderer *renderer, SDL_Event event, int *img){
    switch (event.type){
        case SDL_QUIT:
            return 0;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym){
                case SDLK_RIGHT:
                    mario->direction = 1;
                    *img = MARIO_DROITE_COURS;
                    break;
                case SDLK_LEFT:
                    mario->direction = 2;
                    *img = MARIO_GAUCHE_COURS;
                    break;
                case SDLK_ESCAPE:
                    return 0;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.sym){
                case SDLK_RIGHT:
                case SDLK_LEFT:
                    mario->dernieredirection = mario->direction;
                    mario->direction = 0;
                    *img = statique(mario);
                    break;
                default:
                    break;                    
            }
        default:
            break;
    }
    return 1;
}

void deplacer(Personnage *mario){
    if(mario->direction == 1){
        mario->position.x += 10;
    } else if(mario->direction == 2){
        mario->position.x -= 10;
    } else {
        return;
    }
}

int statique(Personnage *mario){
    if(mario->dernieredirection == 1){
        return MARIO_DROITE;
    } else if(mario->dernieredirection == 2){
        return MARIO_GAUCHE;
    }
}