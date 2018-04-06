#include "grman/grman.h"
#include <iostream>

#include "graphe.h"

int main()
{
    std::string nom;

    grman::init();
    grman::set_pictures_path("foto");
    std::cin >>nom;
    Graphe test;
    test.lecture(nom);
    test.sauvegarde(nom);

    while ( !key[KEY_ESC] )
    {
        test.update();
        grman::mettre_a_jour();
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();
