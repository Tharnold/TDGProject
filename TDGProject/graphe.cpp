#include "graphe.h"

SommetInterface::SommetInterface(int idx, int x, int y, std::string nom_foto, int foto_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
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
    //La croix pour suprimer un sommet
    m_top_box.add_child( m_bouton_delete );
    m_bouton_delete.set_frame(114,3,16,16);
    m_bouton_delete.set_bg_color(ROUGE);

    m_bouton_delete.add_child(m_bouton_delete_label);
    m_bouton_delete_label.set_message("X");
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
    m_box_edge.set_dim(24,75);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

    //Ajout u bouton supr
     m_box_edge.add_child( m_bouton_delete );
    m_bouton_delete.set_frame(6,50,16,16);
    m_bouton_delete.set_bg_color(ROUGE);

    m_bouton_delete.add_child(m_bouton_delete_label);
    m_bouton_delete_label.set_message("X");

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
    //Bouton ajout de sommet
    m_tool_box.add_child(m_bouton_ajout_sommet);
    m_bouton_ajout_sommet.set_frame(3,3,77,40);
    m_bouton_ajout_sommet.set_bg_color(VERTCLAIR);
    m_bouton_ajout_sommet.add_child(m_bouton_ajout_sommet_label);
    m_bouton_ajout_sommet_label.set_message("Add");
    //Bouton ajout d une arete de type 1
    m_tool_box.add_child(m_bouton_ajout_arete1);
    m_bouton_ajout_arete1.set_frame(3,43,77,40);
    m_bouton_ajout_arete1.set_bg_color(VERTCLAIR);
    m_bouton_ajout_arete1.add_child(m_bouton_ajout_arete1_label);
    m_bouton_ajout_arete1_label.set_message("Add");
    //Bouton pour une arte de type 2
    m_tool_box.add_child(m_bouton_ajout_arete2);
    m_bouton_ajout_arete2.set_frame(3,83,77,40);
    m_bouton_ajout_arete2.set_bg_color(VERTCLAIR);
    m_bouton_ajout_arete2.add_child(m_bouton_ajout_arete2_label);
    m_bouton_ajout_arete2_label.set_message("Add");
    // bouton pour load
     m_tool_box.add_child(m_bouton_load);
    m_bouton_load.set_frame(3,123,77,40);
    m_bouton_load.set_bg_color(VERTCLAIR);
    m_bouton_load.add_child(m_bouton_load_label);
    m_bouton_load_label.set_message("Load");
    //bouton pour save
     m_tool_box.add_child(m_bouton_save);
    m_bouton_save.set_frame(3,163,77,40);
    m_bouton_save.set_bg_color(VERTCLAIR);
    m_bouton_save.add_child(m_bouton_save_label);
    m_bouton_save_label.set_message("Save");


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
        std::cout << "recup ordre et tt" << std::endl;
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
            std::cout << "recup sommet : " << i << std::endl;
        }
        for(int i=0; i< nb_arretes; i++)
        {
            std::cout << "début arrete " << i << std::endl;
            fichier >> idx;
            std::cout << "recup idx" << std::endl;
            fichier >> x;
            std::cout << "recup x" << std::endl;
            fichier >> y;
            std::cout << "recup y" << std::endl;
            fichier >> val;
            std::cout << "recup val" << std::endl;

            add_interfaced_arc(idx,x,y,val);
            std::cout << "recup arc : " << i << std::endl;
        }

        // return tmp;
        //  std::cout << m_sommets[1].m_interface->m_top_box.get_posx();

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
//std::cout<<"test"<<std::endl;
    for (auto &elt : m_sommets)
        //elt.second.pre_update();
        elt.pre_update();
//std::cout<<"test2"<<std::endl;
    for (auto &elt : m_arcs)
        //elt.second.pre_update();
        elt.pre_update();
//std::cout<<"test3"<<std::endl;
    m_interface->m_top_box.update();
//std::cout<<"test4"<<std::endl;
    for (auto &elt : m_sommets)
        //elt.second.post_update();
        elt.post_update();
//std::cout<<"test5"<<std::endl;
    for (auto &elt : m_arcs)
        //elt.second.post_update();
        elt.post_update();

    for(int i=0;i<m_sommets.size();i++)
    {
        if(m_sommets[i].m_interface->m_bouton_delete.clicked())
        {
            suppression_sommet(m_sommets[i].m_index);
            m_ordre=m_ordre-1;
        }
    }

    for(int i=0;i<m_arcs.size();i++)
    {
        if(m_arcs[i].m_interface->m_bouton_delete.clicked())
        {
             m_interface->m_main_box.remove_child(m_arcs[i].m_interface->m_top_edge);
                m_arcs.erase(m_arcs.begin()+i);
                m_nbarcs=m_nbarcs-1;
        }
    }


}
void Graphe::suppression_sommet(int indice)
{
    Sommet tmp;
    Arc tmp2;
    int done2=0;
    int done=0;
    grman::WidgetBox * pt;
    //supression de toutes les arretes relie au somet

    while(done2==0)
    {   done2=1;
        for(int i=0;i<m_arcs.size();i++)
        {
            if(m_arcs[i].m_from==indice ||m_arcs[i].m_to==indice )
            {
                //on suprime l arete i
             //   std::cout<<"bitocul  "<<m_arcs[i].m_to<<"   " << i<<std::endl;
                //suppression_arc(i);
                done2=0;
                 m_interface->m_main_box.remove_child(m_arcs[i].m_interface->m_top_edge);
                m_arcs.erase(m_arcs.begin()+i);
                m_nbarcs=m_nbarcs-1;
            }
        }
    }
    //supression du sommet
    for(int i=0;i<m_sommets.size();i++)
    {
        if(m_sommets[i].m_index==indice && done ==0)
        {

          //  m_interface->m_top_box.remove_child(m_sommets[i].m_interface->m_top_box);
          //  delete &m_sommets[i].m_interface->m_top_box;
//          delete m_sommets[i].m_interface;
 //pt = &m_sommets[i].m_interface.m_main_box;//copie
            m_interface->m_main_box.remove_child(m_sommets[i].m_interface->m_top_box);//copie
           // m_interface->m_main_box.remove_child(pt);
            tmp=m_sommets[m_sommets.size()-1];

           m_sommets[m_sommets.size()-1] = m_sommets[i];
           m_sommets[i]=tmp;

           m_sommets.pop_back();

           done =1;
        }
    }



   // std::cout<<"test"<<std::endl;
}

void Graphe::suppression_arc(int indice)
{
    Arc tmp;
    int done=0;
    for(int i=0;i<m_arcs.size();i++)
    {
        if(m_arcs[i].m_indx==indice && done ==0)
        {

            //m_interface->m_top_box.remove_child(m_arcs[i].m_interface->m_top_edge);
           // delete &m_arcs[i].m_interface->m_top_edge;
//           delete m_arcs[i].m_interface;

 m_interface->m_main_box.remove_child(m_arcs[i].m_interface->m_top_edge);//copie
            //=m_arcs[m_arcs.size()-1];

          // m_arcs[m_arcs.size()-1] = m_arcs[i];
          // m_arcs[i]=tmp;

           //m_arcs.pop_back();
           std::cout<< "l arete a ete suprimee indice:"<< i<<std::endl;
           m_arcs.erase(m_arcs.begin()+i);


           done =1;
        }
    }
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
//    *ei.pls=&*ei.m_top_edge;
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_arcs.push_back(Arc(poids, ei, idx,id_som1,id_som2));
}
