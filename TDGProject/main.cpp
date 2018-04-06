#include "grman/grman.h"
#include <iostream>

#include "graphe.h"

int main()
{
    std::string nom;

grman::set_pictures_path("foto");
   std::cin >>nom;

    grman::init();
    Graphe test;
    test.lecture(nom);


    while ( !key[KEY_ESC] )
    {
        test.update();
        grman::mettre_a_jour();
    }

    grman::fermer_allegro();
    std::cout <<" nom du fichier stp pour sauvegarder on ecrira fancais plus tard"<<std::endl;
    std::cin >> nom;
        test.sauvegarde(nom);
    return 0;
}
END_OF_MAIN();
