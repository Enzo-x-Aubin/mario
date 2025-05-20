#include <SDL2/SDL_image.h>

#include "file.h"

// 
  Cette fonction permet de charger une image dans une SDL_Texture avec l'option SDL_TEXTUREACCESS_TARGET 
  pour pouvoir modifier la texture après coup. 
 /
SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer)
{
    SDL_Surface *surface = NULL; 
    SDL_Texture *tmp = NULL, *texture = NULL;

    // Chargement de l'image à partir du chemin spécifié
    surface = IMG_Load(path);
    if (NULL == surface) {
        // Si l'image n'a pas pu être chargée, afficher une erreur
        fprintf(stderr, "Erreur IMG_Load : %s", SDL_GetError());
        return NULL;
    }

    // Création de la texture temporaire à partir de la surface chargée
    tmp = SDL_CreateTextureFromSurface(renderer, surface);
    if (NULL == tmp) {
        // Si la création de la texture échoue, afficher une erreur
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }

    // Création de la texture cible qui pourra être modifiée
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
                            SDL_TEXTUREACCESS_TARGET, surface->w, surface->h); 
    if (texture == NULL) {
        // Si la création de la texture échoue, afficher une erreur
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }

    // Définir la texture comme cible de rendu pour pouvoir la modifier
    SDL_SetRenderTarget(renderer, texture);

    // Désactiver le mode de transparence pour la texture temporaire
    SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_NONE);

    // Copier la texture temporaire sur la texture cible
    SDL_RenderCopy(renderer, tmp, NULL, NULL);

    // Nettoyage : détruire la texture temporaire et libérer la surface
    SDL_DestroyTexture(tmp);
    SDL_FreeSurface(surface);

    // Rétablir le rendu sur le renderer principal
    SDL_SetRenderTarget(renderer, NULL);

    return texture; // Retourner la texture modifiable
}

// 
  Fonction qui charge les données du niveau 0 depuis un fichier et les stocke dans une structure Map.
 /
void LireLevel0(Map* map) {
    // Ouverture du fichier contenant les données du niveau
    FILE *fichier = fopen("level/niveau0.lvl", "r");
    if (fichier == NULL) {
        // Afficher une erreur si le fichier ne peut pas être ouvert
        perror("Erreur lors de l'ouverture du fichier niveau0.lvl");
        return;
    }

    char ligne[100];
    // Lire le nom du niveau (même s'il n'est pas utilisé ici)
    if (fgets(ligne, sizeof(ligne), fichier) == NULL) {
        fprintf(stderr, "Erreur lors de la lecture du nom du niveau\n");
        fclose(fichier);
        return;
    }

    int largeur, hauteur;
    // Lire les dimensions de la carte (largeur et hauteur)
    if (fscanf(fichier, "%d %d", &largeur, &hauteur) != 2) {
        fprintf(stderr, "Erreur lors de la lecture des dimensions de la map\n");
        fclose(fichier);
        return;
    }

    // Vérifier que les dimensions sont valides
    if (largeur <= 0 || hauteur <= 0) {
        fprintf(stderr, "Dimensions invalides : largeur = %d, hauteur = %d\n", largeur, hauteur);
        fclose(fichier);
        return;
    }

    // Affichage des dimensions de la map
    printf("Largeur : %d, Hauteur : %d\n", largeur, hauteur);

    // Affecter les dimensions à la structure Map
    map->width = largeur;
    map->height = hauteur;

    // Allocation dynamique du tableau 2D pour stocker les données de la map
    map->LoadedMap = malloc(map->height * sizeof(int*));
    if (map->LoadedMap == NULL) {
        fprintf(stderr, "Erreur d'allocation
