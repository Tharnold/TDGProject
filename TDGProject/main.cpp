#include "grman/grman.h"
#include <iostream>

#include "graphe.h"

int main()
{
    grman::init();
    grman::set_pictures_path("foto");
    grman::fermer_allegro();


    return 0;
}
END_OF_MAIN();
