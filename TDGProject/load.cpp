#include <fstream>
#include <iostream>
#include "load.h"
//#include "graphe.h"
Graphe load(std::string nom)
{

    std::string nf;// nom du fichier
    nf=nom;
    nf+=".txt";
    //VALEURS POUR STOCKER LES DONEES DU FICHIER
    Graphe tmp;
tmp.m_interface= std::make_shared<GrapheInterface>(50, 0, 750, 600);
    int idx,x,y; // pour les arretes x et y coresspondent respectivement a id_som1 et id_som2
    double val;// pour les arretes coresspond a poids
    std::string nom_foto;
    int nb_sommets;
    int nb_arretes;
    std::ifstream fichier(nf);
    if(fichier)
    {
        fichier >> nb_sommets;
        fichier >> nb_arretes;
        for(int i=0; i< nb_sommets; i++)
        {
            fichier >> idx;
            fichier >> val;
            fichier >> x;
            fichier >> y;
            fichier >> nom_foto;
            tmp.add_interfaced_sommet(idx,val,x,y,nom_foto);
        }
        for(int i=0; i< nb_sommets; i++)
        {
            fichier >> idx;

            fichier >> x;
            fichier >> y;
            fichier >> val;

            tmp.add_interfaced_arc(idx,x,y,val);
        }

        return tmp;

    }
    else
    {

        std::cerr <<"l'ouverture du fichier est un echec cuisant"<< std::endl;
    }
    // return matrice_adjacence;
}

