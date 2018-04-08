#ifndef GRAPHE_H_INCLUDED
#define GRAPHE_H_INCLUDED

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>

#include "grman/grman.h"

class SommetInterface
{
    //CLASSES AMIES
    friend class Sommet;
    friend class ArcInterface;
    friend class Graphe;

private:
    ///LISTE DES WIDGETS DU SOMMET
    //BOITE PRINCIPALE
    grman::WidgetBox m_top_box;
    //VALEUR DU SLIDER ET BOITE D'AFFICHAGE
    grman::WidgetVSlider m_slider_value;
    grman::WidgetText m_label_value;
    //POPULATION ET BOITE D'AFFICHAGE
    grman::WidgetText m_box_label_pop;
    grman::WidgetText m_label_pop;
    //WIDGET POUR L'IMAGE DU SOMMET
    grman::WidgetImage m_img;
    //NUMERO DU SOMMET ET BOITE D'AFFICHAGE
    grman::WidgetText m_label_idx;
    grman::WidgetText m_box_label_idx;
    //BOUTON DE SUPPRESSION DU SOMMET
    grman::WidgetButton m_bouton_delete;
    grman::WidgetText m_bouton_delete_label;
    //BOUTON D'AJOUT POUR LA CREATION D'ARRETE
    grman::WidgetButton m_bouton_link;
    grman::WidgetText m_bouton_link_label;

public:
    //CONSTRUCTEUR DE L'INTERFACE DU SOMMET
    SommetInterface(int idx, int x, int y, std::string nom_foto="", int foto_idx=0);

};

class Sommet
{
    //CLASSES AMIES
    friend class Graphe;
    friend class SommetInterface;
    friend class Arc;
    friend class ArcInterface;

private:
    ///VARIABLES
    //VECTEURS D4INDICES DES ARCS ENTRANT ET SORTANT
    std::vector <int> m_in;
    std::vector <int> m_out;
    //POPULATION NOMBRE DE NAISSANCE ET DE DECES PAR MOIS
    double m_valeur,m_fertilite,m_deces_mois;
    //NUMERO DU SOMMET
    int m_index;
    //NUMERO DE LA COMPOSANTE CONNEXE
    int m_cfc;
    //POINTEUR SUR L'INTERFACE DU SOMMET
    std::shared_ptr<SommetInterface> m_interface = nullptr;

public:
    //CONSTRUCTEUR DU SOMMET QUI APPELLE CELUI DE L'INTERFACE
    Sommet(double valeur=0, SommetInterface *interface=nullptr, int idx=0) :
            m_valeur(valeur), m_interface(interface),m_index(idx)  {  }
    //AJOUTE LES VARIABLES DE FERTILITE ET DE MORTALITE UNE FOIS LE SOMMET CREE
    void ajouter_var(double fertilite, double deces);
    //METHODES DE LIEN ENTRE LES VALEURS MODIFIABLES ET CELLES ENREGISTREES
    void pre_update();
    void post_update();
};

class ArcInterface
{
    //CLASSES AMIES
    friend class Arc;
    friend class Graphe;

private:
    ///LISTE DES WIDGETS DE L'ARC
    //FLECHE
    grman::WidgetEdge m_top_edge;
    //BOITE PRINCIPALE
    grman::WidgetBox m_box_edge;
    //SLIDER ET VALEUR
    grman::WidgetVSlider m_slider_weight;
    grman::WidgetText m_label_weight;
    //BOUTON DE SUPPRESSION DE L'ARRETE
    grman::WidgetButton m_bouton_delete;
    grman::WidgetText m_bouton_delete_label;

public:
    //CONSTRUCTEUR DE L'INTERFACE DE L'ARC
    ArcInterface(Sommet& from, Sommet& to);
};

class Arc
{
    //CLESSES AMIES
    friend class Graphe;
    friend class ArcInterface;

private:
    //SOMMET DE DEPART
    int m_from;
    //SOMMET D'ARRIVEE
    int m_to;
    //POIDS DE L'ARC
    double m_poids;
    //NUMERO DE L'ARC
    int m_indx;
    //POINTEUR SUR L'INTERFACE
    std::shared_ptr<ArcInterface> m_interface = nullptr;

public:
    //CONSTRUCTEUR
    Arc (double poids=0, ArcInterface *interface=nullptr, int idx=0,int from=0,int to=0) :
            m_poids(poids), m_interface(interface),m_indx(idx),m_from(from),m_to(to) {  }
    //METHODES DE LIEN ENTRE LES VALEURS MODIFIABLES ET CELLES ENREGISTREES
    void pre_update();
    void post_update();
};

class GrapheInterface
{
    //CLASSE AMIE
    friend class Graphe;

private:
    //BOITE PRINCIPALE
    grman::WidgetBox m_top_box;
    //BOITE CONTENANT LE GRAPHE
    grman::WidgetBox m_main_box;
    //BOITE AVEC LES BOUTONS
    grman::WidgetBox m_tool_box;
    //BOUTON D'AJOUT DU PREMIER SOMMET POUR LA CREATION D'UN ARC
    grman::WidgetButton m_bouton_ajout_sommet1;
    grman::WidgetText m_bouton_ajout_sommet1_label;
    //BOUTON D'AJOUT DU DEUXIEME SOMMET POUR LA CREATION D'UN ARC
    grman::WidgetButton m_bouton_ajout_sommet2;
    grman::WidgetText m_bouton_ajout_sommet2_label;
    //BOUTON DE POUR CREER UN ARC ENTRE LES SOMMET 1 ET 2 ENTRES
    grman::WidgetButton m_bouton_link;
    grman::WidgetText m_bouton_link_label;
    //BOUTON POUR AJOUTER UN SOMMET
    grman::WidgetButton m_ajouter_sommet;
    grman::WidgetText m_ajouter_sommet_label;
    //BOUTON POUR LANCER LA SIMULATION
    grman::WidgetButton m_lancer_simulation;
    grman::WidgetText m_lancer_simulation_label;
    //BOUTON POUR METTRE EN PAUSE LA SIMULATION
    grman::WidgetButton m_pause_simulation;
    grman::WidgetText m_pause_simulation_label;
    //BOUTON POUR CHARGER LE GRAPHE D'UN FICHIER
    grman::WidgetButton m_bouton_load;
    grman::WidgetText m_bouton_load_label;
    //BOUTON POUR SAUVEGARDER LE GRAPHE EN COURS D'UTILISATION
    grman::WidgetButton m_bouton_save;
    grman::WidgetText m_bouton_save_label;
     //BOUTON POUR METTRE EN SURBRILLANCE LES COMPOSANTES FORTEMENT CONNEXES
    grman::WidgetButton m_bouton_forteco;
    grman::WidgetText m_bouton_forteco_label;

public:
    //CONSTRUCTEUR DE L'INTERFACE DU GRAPHE
    GrapheInterface(int x, int y, int w, int h);
};

class Graphe
{
    //PAS DE CLASSES AMIES
private:
    //VECTEURS D'ARC ET DE SOMMETS DU GRAPHE
    std::vector<Arc> m_arcs;
    std::vector<Sommet> m_sommets;
    //ORDRE DU GRAPHE ET NOMBRE D'ARCS
    int m_ordre,m_nbarcs;
    //POINTEUR SUR L4iNTERFACE DU GRAPHE
    std::shared_ptr<GrapheInterface> m_interface = nullptr;
    //RETENUE D'UN SOMMET POUR SUPPRESSION OU CREATION D'UN ARC
    Sommet ss1;
    Sommet ss2;
    int bol;
    //BOOLEEN INDIQUANT SI LA SIMULATION TOURNE OU NON(GESTION DE LA PAUSE)
    int simu=0;


public:
    //CONSTRUCTEUR DU GRAPHE
    Graphe (GrapheInterface *interface=nullptr,int ordre=0, int nbarcs=0) :
            m_interface(interface),m_ordre(ordre),m_nbarcs(nbarcs)  {  }
    //METHODE D'AJOUT D'UN SOMMET
    void add_interfaced_sommet(int idx, double valeur, int x, int y, std::string nom_foto="", double fertilite=0.0, double deces_mois=0.0, int foto_idx=0);
    //METHODE D'AJOUT D'UN ARC
    void add_interfaced_arc(int idx, int som1, int som2, double poids=0);
    //METHODE DE RECUPERATION D'UN GRAPHE DANS UN FICHIER
    void lecture(std::string nom);
    //METHODE D'ENREGISTREMENT DANS UN FICHIER
    void sauvegarde(std::string nom);
    //ACTUALISATION DE L'AFFICHAGE
    void update();
    //SUPPRESSION D'UN SOMMET
    void suppression_sommet(int indice);
    //SUPPRESSION D'UN ARC
    void suppression_arc(int indice);
    //RECHERCHE D'UNE COMPOSANTE FORTEMENT CONNEXE
    std::vector<int> RechercheComposanteFortementConnexe(int s);
    //RECUPERATION DE TOUTES LES COMPOSANTES FORTEMENT CONNEXE
    std::vector< std::vector<int> > TouteLesComposantesFortementsConnexes();
    //METTRE EN SURBRILLANCE LES COMPOSANTES FORTEMENT CONNEXES
    void surbrillance(std::vector<std::vector<int>> tabc);
    //SIMULATION DE L'EVOLUTION DES POPULATIONS DANS LE TEMPS
    void simulation();
    //FONCTION DETERMINANT SI LES RESSOURCES SONT SUFFISANTES
    int ressources(int base,int ress);
};

#endif // GRAPHE_H_INCLUDED
