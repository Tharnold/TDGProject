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
    friend class Sommet;
    friend class ArcInterface;
    friend class Graphe;
private:
    grman::WidgetBox m_top_box;
    grman::WidgetVSlider m_slider_value;
    grman::WidgetText m_label_value;
    grman::WidgetImage m_img;
    grman::WidgetText m_label_idx;
    grman::WidgetText m_box_label_idx;

public:
    SommetInterface(int idx, int x, int y, std::string nom_foto="", int foto_idx=0);

};

class Sommet
{
    friend class Graphe;
    friend class SommetInterface;
    friend class Arc;
    friend class ArcInterface;

private:
    std::vector <int> m_in;
    std::vector <int> m_out;
    double m_valeur,m_fertilite,m_deces_mois;
    int m_index;
    std::shared_ptr<SommetInterface> m_interface = nullptr;

public:
    Sommet(double valeur=0, SommetInterface *interface=nullptr, int idx=0) :
            m_valeur(valeur), m_interface(interface),m_index(idx)  {  }
    void ajouter_var(double fertilite, double deces);

    void pre_update();
    void post_update();
};

class ArcInterface
{
    friend class Arc;
    friend class Graphe;

private:
    grman::WidgetEdge m_top_edge;
    grman::WidgetBox m_box_edge;
    grman::WidgetVSlider m_slider_weight;
    grman::WidgetText m_label_weight;

public:
    ArcInterface(Sommet& from, Sommet& to);

};

class Arc
{
    friend class Graphe;
    friend class ArcInterface;

private:
    int m_from;
    int m_to;
    double m_poids;
    int m_indx;
    std::shared_ptr<ArcInterface> m_interface = nullptr;

public:
    Arc (double poids=0, ArcInterface *interface=nullptr, int idx=0) :
            m_poids(poids), m_interface(interface),m_indx(idx) {  }

    void pre_update();
    void post_update();

};

class GrapheInterface
{
    friend class Graphe;

private:
    grman::WidgetBox m_top_box;
    grman::WidgetBox m_main_box;
    grman::WidgetBox m_tool_box;

public:
    GrapheInterface(int x, int y, int w, int h);

};

class Graphe
{
   // friend class GrapheInterface;
private:
    std::vector<Arc> m_arcs;
    std::vector<Sommet> m_sommets;
    int m_ordre,m_nbarcs;
    std::shared_ptr<GrapheInterface> m_interface = nullptr;

public:
    Graphe (GrapheInterface *interface=nullptr,int ordre=0, int nbarcs=0) :
            m_interface(interface),m_ordre(ordre),m_nbarcs(nbarcs)  {  }

        void add_interfaced_sommet(int idx, double valeur, int x, int y, std::string nom_foto="", double fertilite=0.0, double deces_mois=0.0, int foto_idx=0);
        void add_interfaced_arc(int idx, int som1, int som2, double poids=0);

        void lecture(std::string nom);
        void sauvegarde(std::string nom);
        void update();

};



#endif // GRAPHE_H_INCLUDED
