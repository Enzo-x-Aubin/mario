#include "event.h"      // Inclut les fonctions liées aux événements
#include "charactere.h"  // Inclut les fonctions et structures liées au personnage

// Déclaration des fonctions utilisées
int statique(Personnage *mario);               // Fonction pour obtenir l'animation statique de Mario
void saut(Personnage *mario, int *img);       // Fonction pour gérer le saut de Mario
void afficher_mario(Personnage *mario, int *img); // Fonction pour afficher l'animation de Mario en fonction de son état

// Fonction principale de gestion des événements du jeu
int event(Personnage *mario, SDL_Renderer *renderer, SDL_Event event, int *img){
    switch (event.type){ // Switch sur le type d'événement
        case SDL_QUIT: // Si l'événement est la fermeture de la fenêtre
            return 0;   // Quitter le programme
            break;
        case SDL_KEYDOWN: // Si une touche est pressée
            switch (event.key.keysym.sym){ // Switch sur la touche pressée
                case SDLK_RIGHT: // Si la touche droite est pressée
                    mario->direction = 1; // Mario se déplace vers la droite
                    mario->dernieredirection = mario->direction; // Met à jour la dernière direction
                    break;
                case SDLK_LEFT: // Si la touche gauche est pressée
                    mario->direction = 2; // Mario se déplace vers la gauche
                    mario->dernieredirection = mario->direction; // Met à jour la dernière direction
                    break;
                case SDLK_UP: // Si la touche haut est pressée (saut)
                    mario->jump = 1; // Déclenche le saut
                    break;
                case SDLK_ESCAPE: // Si la touche Échap est pressée
                    return 0; // Quitter le programme
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP: // Si une touche est relâchée
            switch(event.key.keysym.sym){ // Switch sur la touche relâchée
                case SDLK_RIGHT:
                case SDLK_LEFT:
                    mario->direction = 0; // Arrêter le mouvement
                    *img = statique(mario); // Mettre l'image à l'état statique du personnage
                    break;
                default:
                    break;                    
            }
        default:
            break;
    }
    return 1; // Retourner 1 pour indiquer que le programme continue
}

// Fonction pour déplacer Mario en fonction de sa direction
void deplacer(Personnage *mario, Map map, Sprites *sprites){
    if(mario->direction == 1){ // Si Mario se déplace vers la droite
        if(collision(*mario, map, sprites) == 0){ // Vérifie la collision
            mario->position.x += 2; // Déplace Mario vers la droite
        } else {
            return; // Si collision, ne pas déplacer Mario
        }
    } else if(mario->direction == 2){ // Si Mario se déplace vers la gauche
        if(collision(*mario, map, sprites) == 0){ // Vérifie la collision
            mario->position.x -= 2; // Déplace Mario vers la gauche
        } else {
            return; // Si collision, ne pas déplacer Mario
        }
    }
}

// Fonction pour obtenir l'animation statique de Mario en fonction de sa dernière direction
int statique(Personnage *mario){
    if(mario->dernieredirection == 1){ // Si la dernière direction est vers la droite
        return MARIO_DROITE; // Retourne l'animation "Mario regarde à droite"
    } else if(mario->dernieredirection == 2){ // Si la dernière direction est vers la gauche
        return MARIO_GAUCHE; // Retourne l'animation "Mario regarde à gauche"
    }
}

// Fonction pour gérer le saut de Mario
void saut(Personnage *mario, int *img){
    if(mario->jump == 1){ // Si Mario est en train de sauter
        mario->position.y -= 2; // Monte Mario
        mario->jumptime += 2; // Augmente le temps de saut
    }
    if(mario->jumptime >= 70){ // Quand Mario atteint la hauteur maximale du saut
        mario->jump = 0; // Arrêter le saut
    }
    if(mario->jumptime > 0 && mario->jump == 0){ // Si Mario commence à redescendre après le saut
        mario->position.y += 2; // Descend Mario
        mario->jumptime -= 2; // Diminue le temps de saut
    }
    if(mario->jump == 0 && mario->jumptime == 0){ // Si Mario a fini son saut
        *img = statique(mario); // Afficher l'animation statique du personnage
    }
}

// Fonction pour afficher l'animation de Mario en fonction de sa direction et de son état
void afficher_mario(Personnage *mario, int *img){
    if(mario->direction == 1){ // Si Mario se déplace vers la droite
        if(mario->jumptime > 0){ // Si Mario est en l'air (en train de sauter)
            *img = MARIO_DROITE_SAUT; // Afficher l'animation de Mario sautant vers la droite
        } else {
            *img = MARIO_DROITE_COURS; // Afficher l'animation de Mario courant vers la droite
        }
    } else if(mario->direction == 2){ // Si Mario se déplace vers la gauche
        if(mario->jumptime > 0){ // Si Mario est en l'air
            *img = MARIO_GAUCHE_SAUT; // Afficher l'animation de Mario sautant vers la gauche
        } else {
            *img = MARIO_GAUCHE_COURS; // Afficher l'animation de Mario courant vers la gauche
        }
    } else if(mario->jumptime > 0){ // Si Mario ne se déplace pas mais est en l'air
        if(mario->dernieredirection == 1){ // Si la dernière direction était vers la droite
            *img = MARIO_DROITE_SAUT; // Afficher l'animation de Mario sautant vers la droite
        } else if(mario->dernieredirection == 2){ // Si la dernière direction était vers la gauche
            *img = MARIO_GAUCHE_SAUT; // Afficher l'animation de Mario sautant vers la gauche
        }
    }
}

// Fonction pour vérifier les collisions entre Mario et les éléments de la carte
int collision(Personnage mario, Map map, Sprites *sprites){
    SDL_Rect temp = mario.position; // Rectangle représentant la position actuelle de Mario
    SDL_Rect block = {(mario.position.x/Size_Sprite)*Size_Sprite, (mario.position.y/Size_Sprite)*Size_Sprite, Size_Sprite, Size_Sprite}; // Calcul d'un bloc de la carte en fonction de la position de Mario

    if(mario.direction == 1){ // Si Mario se déplace vers la droite
        int spriteNum = map.LoadedMap[(mario.position.y/Size_Sprite)][(mario.position.x/Size_Sprite)+1]; // Numéro du sprite à droite de Mario
        temp.x += 2; // Déplace temporairement Mario vers la droite
        if(sprites[spriteNum].traverser == 1){ // Si le sprite est traversable
            if(SDL_HasIntersection(&temp, &block)){ // Si Mario entre en collision avec un bloc
                return 1; // Retourne 1 pour indiquer une collision
            } else {
                return 0; // Pas de collision
            }
        }
    } else if(mario.direction == 2){ // Si Mario se déplace vers la gauche
        int spriteNum = map.LoadedMap[(mario.position.y/Size_Sprite)][(mario.position.x/Size_Sprite)-1]; // Numéro du sprite à gauche de Mario
        temp.x -= 2; // Déplace temporairement Mario vers la gauche
        if(sprites[spriteNum].traverser == 1){ // Si le sprite est traversable
            if(SDL_HasIntersection(&temp, &block)){ // Si Mario entre en collision avec un bloc
                return 1; // Retourne 1 pour indiquer une collision
            } else {
                return 0; // Pas de collision
            }
        }
    }
}
