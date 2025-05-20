#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "file.h"    // Inclut le fichier file.h, probablement pour des fonctions de gestion des fichiers
#include "game.h"    // Inclut le fichier game.h, probablement pour des fonctions liées au jeu

// Ligne de compilation pour générer le programme :
// gcc main.c file.c game.c event.c charactere.c -o mario -lSDL2main -lSDL2_image -lSDL2

// Déclaration des fonctions utilisées dans le code
void init(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture, int w, int h); // Fonction d'initialisation
void end_game(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture);             // Fonction de nettoyage en fin de jeu

int main(int argc, char *argv[])
{
    // Déclaration des variables pour la fenêtre, le renderer et la texture du menu
    SDL_Window *menu = NULL; // Pointeur vers la fenêtre du menu
    SDL_Renderer *renderer_menu = NULL; // Pointeur vers le renderer pour le menu
    SDL_Texture *img_menu = NULL; // Pointeur vers la texture du menu
    SDL_Event event; // Structure pour gérer les événements
    bool done = false; // Variable de contrôle pour sortir de la boucle principale

    // Initialisation de la fenêtre, du renderer et de la texture
    init(&menu, &renderer_menu, &img_menu, LARGEUR_FENETRE, HAUTEUR_FENETRE);

    // Boucle principale du programme, attend les événements et réagit en fonction
    while((!done) && SDL_WaitEvent(&event)){
        switch(event.type){ // Switch sur le type d'événement
            case SDL_QUIT: // Si l'événement est une fermeture de la fenêtre
                done = true; // Terminer la boucle
                break;
            case SDL_KEYDOWN: // Si une touche est pressée
                switch(event.key.keysym.sym){ // Switch sur la touche pressée
                    case SDLK_1: // Si la touche '1' est pressée
                    case SDLK_KP_1: // Si la touche '1' du pavé numérique est pressée
                        SDL_Log("1 Pressé"); // Log de l'action
                        SDL_DestroyTexture(img_menu); // Détruire la texture du menu
                        jouer(renderer_menu); // Lancer la fonction jouer() (probablement pour démarrer le jeu)
                        done = true; // Quitter la boucle
                        break;
                    case SDLK_2: // Si la touche '2' est pressée
                    case SDLK_KP_2: // Si la touche '2' du pavé numérique est pressée
                        SDL_Log("2 Pressé"); // Log de l'action
                        SDL_RenderClear(renderer_menu); // Effacer le renderer du menu
                        // Charger et afficher une nouvelle texture (probablement les crédits)
                        SDL_Texture *texture = loadImage("./img/credit.png", renderer_menu);
                        SDL_RenderCopy(renderer_menu, texture, NULL, NULL); // Afficher la texture sur le renderer
                        SDL_RenderPresent(renderer_menu); // Mettre à jour le rendu
                        break;
                    case SDLK_ESCAPE: // Si la touche 'Échap' est pressée
                        done = true; // Quitter la boucle
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
    }

    // Fonction de nettoyage en fin de programme
    end_game(menu, renderer_menu, img_menu);

    return 0; // Fin du programme principal
}

// Fonction d'initialisation : crée la fenêtre, le renderer et charge l'image de fond
void init(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture, int w, int h){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){ // Initialisation de la SDL pour la vidéo
        SDL_Log("Erreur initialisation SDL: ");
        exit(0); // Si erreur, quitter le programme
    }
    // Création de la fenêtre avec le titre "Menu"
    *window = SDL_CreateWindow("Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    if(window == NULL){ // Si l'ouverture de la fenêtre échoue
        SDL_Log("Erreur initialisation menu: ");
        SDL_Quit(); // Quitter SDL
        exit(0); // Quitter le programme
    }
    // Création du renderer pour la fenêtre créée
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL){ // Si l'ouverture du renderer échoue
        SDL_Log("Erreur loadimage: ");
        SDL_DestroyWindow(*window); // Détruire la fenêtre
        SDL_Quit(); // Quitter SDL
        exit(0); // Quitter le programme
    }
    // Chargement de l'image du menu et affichage de celle-ci
    *texture = loadImage("./img/menu.jpg", *renderer);
    SDL_RenderCopy(*renderer, *texture, NULL, NULL); // Copier la texture sur le renderer
    SDL_RenderPresent(*renderer); // Mettre à jour le rendu de la fenêtre
}

// Fonction de nettoyage : libère les ressources allouées pour la fenêtre, le renderer et la texture
void end_game(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture){
    SDL_DestroyTexture(texture); // Détruire la texture
    SDL_DestroyRenderer(renderer); // Détruire le renderer
    SDL_DestroyWindow(window); // Détruire la fenêtre
    SDL_Quit(); // Quitter la SDL
}
