#include "grman/grman.h"
#include <iostream>

#include "graphe.h"

int main()
{
    //DEFINITION DU DOSSIER POUR LES PHOTOS
    grman::set_pictures_path("foto");
    //VARIABLE DE RECUPERATION
    std::string nom;
    std::cout << "Quel graphe voulez vous ?  ";
    //RECUPERATION DU NOM DU FICHIER
    std::cin >>nom;
    //INITIALISATION DE L'AFFICHAGE
    grman::init();
    //CREATION D'UN GRAPHE
    Graphe test;
    //RECUPERATION DU GRAPHE DANS LE FICHER
    test.lecture(nom);
    //DEBUT DE LA BOUCLE D'ACTION
    while ( !key[KEY_ESC] )
    {
        //MISE A JOUR DU GRAPHE
        test.update();
        //MISA A JOUR DE L'AFFICHAGE
        grman::mettre_a_jour();
    }
    //FERMETURE DE l4AFFICHAGE
    grman::fermer_allegro();
    return 0;
}
END_OF_MAIN();
