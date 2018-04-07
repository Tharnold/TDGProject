#include "grman/grman.h"
#include <iostream>

#include "graphe.h"

int main()
{
    std::string nom;

    grman::set_pictures_path("foto");
    std::vector <std::vector<int>>v;
   std::cin >>nom;

    grman::init();
    Graphe test;
    test.lecture(nom);
   // v=test.RechercheComposanteFortementConnexe();
   // for(int i=0;i<v.size();i++)
   // {
   //     std::cout<<v[i]<<" ";
   // }
   v=test.TouteLesComposantesFortementsConnexes();
   for(int i=0;i<v.size();i++)
   {
       for(int j=0;j<v[i].size();j++)
       {
           std::cout<<v[i][j]<<" ";
       }
       std::cout<<std::endl;
   }
   test.surbrillance(v);


    while ( !key[KEY_ESC] )
    {
        test.update();
        grman::mettre_a_jour();
    }

    grman::fermer_allegro();
    return 0;
}
END_OF_MAIN();
