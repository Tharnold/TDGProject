#include "graphe.h"

SommetInterface::SommetInterface(int idx, int x, int y, std::string nom_foto, int foto_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (nom_foto!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(nom_foto);
        m_img.set_pic_idx(foto_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}

void Sommet::ajouter_var(double fertilite, double deces)
{
    m_fertilite=fertilite;
    m_deces_mois=deces;
}

void Sommet::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_valeur);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_valeur) );
}


void Sommet::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_valeur = m_interface->m_slider_value.get_value();
}

ArcInterface::ArcInterface(Sommet& from, Sommet& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating SommetInterface between vertices having no interface" << std::endl;
        throw "Bad ArcInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}

void Arc::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_poids);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_poids ) );
}

void Arc::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_poids = m_interface->m_slider_weight.get_value();
}

GrapheInterface::GrapheInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
}

void Graphe::lecture(std::string nom)
{

    std::string nf;// nom du fichier
    nf=nom;
    nf+=".txt";
    //VALEURS POUR STOCKER LES DONEES DU FICHIER
  //  Graphe tmp;
m_interface= std::make_shared<GrapheInterface>(50, 0, 750, 600);
    int idx,x,y; // pour les arretes x et y coresspondent respectivement a id_som1 et id_som2
    double val,fertilite,deces;// pour les arretes coresspond a poids
    std::string nom_foto;
    int nb_sommets;
    int nb_arretes;
    std::ifstream fichier(nf);
    if(fichier)
    {
        fichier >> nb_sommets;
        fichier >> nb_arretes;
        m_ordre=nb_sommets;
        m_nbarcs=nb_arretes;
        for(int i=0; i< nb_sommets; i++)
        {
            fichier >> idx;
            fichier >> val;
            fichier >> x;
            fichier >> y;
            fichier >> nom_foto;
            fichier >> fertilite;
            fichier >> deces;
            add_interfaced_sommet(idx,val,x,y,nom_foto);
        }
        for(int i=0; i< nb_arretes; i++)
        {
            fichier >> idx;

            fichier >> x;
            fichier >> y;
            fichier >> val;

            add_interfaced_arc(idx,x,y,val);
        }

       // return tmp;

    }
    else
    {

        std::cerr <<"l'ouverture du fichier est un echec cuisant"<< std::endl;
    }
}

void Graphe::sauvegarde(std::string nom)
{
    nom+=".txt";
    std::ofstream fichier(nom, std::ios::trunc);
    if(fichier)
    {
        fichier << m_ordre << std::endl;
        fichier << m_nbarcs << std::endl;
        for(auto &elt : m_sommets)
        {
            fichier << elt.m_index << " ";
            fichier << elt.m_valeur << " ";
            fichier << elt.m_interface->m_top_box.get_frame_pos().x << " ";
            fichier << elt.m_interface->m_top_box.get_frame_pos().y << " ";
            fichier << elt.m_interface->m_img.get_pic_name() << " ";
            fichier << elt.m_fertilite << " ";
            fichier << elt.m_deces_mois << std::endl;
        }
        for(auto &elt : m_arcs)
        {
            fichier << elt.m_indx << " ";
            fichier << elt.m_from << " ";
            fichier << elt.m_to << " ";
            fichier << elt.m_poids << std::endl;
        }
    }
}


void Graphe::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_sommets)
        //elt.second.pre_update();
        elt.pre_update();

    for (auto &elt : m_arcs)
        //elt.second.pre_update();
        elt.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_sommets)
        //elt.second.post_update();
        elt.post_update();

    for (auto &elt : m_arcs)
        //elt.second.post_update();
        elt.post_update();

}

void Graphe::add_interfaced_sommet(int idx, double valeur, int x, int y, std::string nom_foto, double fertilite, double deces_mois, int foto_idx )
{
    /*if ( m_sommets.find(idx)!=m_sommets.end() )
    {
        std::cerr << "Error adding sommet at idx=" << idx << " already used..." << std::endl;
        throw "Error adding sommet";
    }*/
    // Création d'une interface de sommet
    SommetInterface *vi = new SommetInterface(idx, x, y, nom_foto, foto_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    Sommet som_prov(valeur, vi, idx);
    som_prov.ajouter_var(fertilite,deces_mois);
    m_sommets.push_back(som_prov);
}

void Graphe::add_interfaced_arc(int idx, int id_som1, int id_som2, double poids)
{
    /*if ( m_arcs.find(idx)!=m_arcs.end() )
    {
        std::cerr << "Error adding arc at idx=" << idx << " already used..." << std::endl;
        throw "Error adding arc";
    }

    if ( m_sommets.find(id_som1)==m_sommets.end() || m_sommets.find(id_som2)==m_sommets.end() )
    {
        std::cerr << "Error adding arc idx=" << idx << " between sommets " << id_som1 << " and " << id_som2 << " not in m_sommets" << std::endl;
        throw "Error adding arc";
    }*/

    ArcInterface *ei = new ArcInterface(m_sommets[id_som1], m_sommets[id_som2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    Arc larc(poids, ei, idx);
    larc.m_from=id_som1;
    larc.m_to=id_som2;
    m_arcs.push_back(larc);
}
