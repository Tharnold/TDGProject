#include "graphe.h"
#include <chrono>
#include <thread>

SommetInterface::SommetInterface(int idx, int x, int y, std::string nom_foto, int foto_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 1.0);  // Valeurs arbitraires, � adapter...
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

    // label de visualisation de la population
    m_top_box.add_child( m_box_label_pop );
    m_box_label_pop.set_frame(33,89,40,12);
    m_box_label_pop.set_bg_color(BLANC);

    m_box_label_pop.add_child( m_label_pop );
    //La croix pour suprimer un sommet
    m_top_box.add_child( m_bouton_delete );
    m_bouton_delete.set_frame(114,3,16,16);
    m_bouton_delete.set_bg_color(ROUGE);

    m_bouton_delete.add_child(m_bouton_delete_label);
    m_bouton_delete_label.set_message("X");
    //bouton pour relier deux sommets
    m_top_box.add_child( m_bouton_link );
    m_bouton_link.set_frame(114,23,16,16);
    m_bouton_link.set_bg_color(JAUNE);

    m_bouton_link.add_child(m_bouton_link_label);
    m_bouton_link_label.set_message("+");
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

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_fertilite);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_fertilite) );

    m_interface->m_label_pop.set_message(std::to_string((int)m_valeur));
}


void Sommet::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_fertilite = m_interface->m_slider_value.get_value();
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

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,75);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, � adapter...
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

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_poids);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (double)m_poids ) );
}

void Arc::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_poids = m_interface->m_slider_weight.get_value();
}

GrapheInterface::GrapheInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_top_box.set_bg_color(BLANCJAUNE);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(0xFFF8DC);
    //Bouton ajout de sommet1
    m_tool_box.add_child(m_bouton_ajout_sommet1);
    m_bouton_ajout_sommet1.set_frame(3,3,77,40);
    m_bouton_ajout_sommet1.set_bg_color(0x749A6F);
    m_bouton_ajout_sommet1.add_child(m_bouton_ajout_sommet1_label);
    m_bouton_ajout_sommet1_label.set_message("Add s1");
    //Bouton ajout de sommeet 2
    m_tool_box.add_child(m_bouton_ajout_sommet2);
    m_bouton_ajout_sommet2.set_frame(3,43,77,40);
    m_bouton_ajout_sommet2.set_bg_color(0xB7CA79);
    m_bouton_ajout_sommet2.add_child(m_bouton_ajout_sommet2_label);
    m_bouton_ajout_sommet2_label.set_message("Add s2");
    //Bouton link des deux sommets
    m_tool_box.add_child(m_bouton_link);
    m_bouton_link.set_frame(3,83,77,40);
    m_bouton_link.set_bg_color(0x749A6F);
    m_bouton_link.add_child(m_bouton_link_label);
    m_bouton_link_label.set_message("link");
    //bouton pour ajouter un sommet
    m_tool_box.add_child(m_ajouter_sommet);
    m_ajouter_sommet.set_frame(3,123,77,40);
    m_ajouter_sommet.set_bg_color(0xB7CA79);
    m_ajouter_sommet.add_child(m_ajouter_sommet_label);
    m_ajouter_sommet_label.set_message("+ sommet");
    //bouton pour lancer la simulation
    m_tool_box.add_child(m_lancer_simulation);
    m_lancer_simulation.set_frame(3,163,77,40);
    m_lancer_simulation.set_bg_color(0x749A6F);
    m_lancer_simulation.add_child(m_lancer_simulation_label);
    m_lancer_simulation_label.set_message("Play");
    //bouton pour pauser la simulation
    m_tool_box.add_child(m_pause_simulation);
    m_pause_simulation.set_frame(3,203,77,40);
    m_pause_simulation.set_bg_color(0xB7CA79);
    m_pause_simulation.add_child(m_pause_simulation_label);
    m_pause_simulation_label.set_message("Pause");
    // bouton pour load
    m_tool_box.add_child(m_bouton_load);
    m_bouton_load.set_frame(3,243,77,40);
    m_bouton_load.set_bg_color(0x749A6F);
    m_bouton_load.add_child(m_bouton_load_label);
    m_bouton_load_label.set_message("Load");
    //bouton pour save
    m_tool_box.add_child(m_bouton_save);
    m_bouton_save.set_frame(3,283,77,40);
    m_bouton_save.set_bg_color(0xB7CA79);
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
    m_link=0;
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

void Graphe::simulation()
{
    double popu=0;
    double eaten=0;
    double eat=0;
    int som=0;
    int rarc=0;
    double pds=0;
    for (int i=0; i<m_sommets.size(); i++)
    {
        popu=m_sommets[i].m_valeur;
        popu+=m_sommets[i].m_valeur*(m_sommets[i].m_fertilite-m_sommets[i].m_deces_mois);
        for(int j=0; j<m_sommets[i].m_in.size(); j++)
        {
            for(int k=0; k<m_sommets.size(); k++)
            {
                for(int l=0; l<m_arcs.size(); l++)
                {
                    if(m_arcs[l].m_indx==j)
                        rarc=m_arcs[l].m_from;
                    pds=m_arcs[l].m_poids;
                }
                if(m_sommets[k].m_index==rarc)
                    som=k;
            }
            eaten+=pds*m_sommets[som].m_valeur;
        }

        for(int j=0; j<m_sommets[i].m_out.size(); j++)
        {
            for(int k=0; k<m_sommets.size(); k++)
            {
                for(int l=0; l<m_arcs.size(); l++)
                {
                    if(m_arcs[l].m_indx==j)
                        rarc=m_arcs[l].m_to;
                    pds=m_arcs[l].m_poids;
                }
                if(m_sommets[k].m_index==rarc)
                    som=k;
            }
            eat+=pds*m_sommets[som].m_valeur;
        }
        popu-=eaten;
        popu=ressources(popu,eat);
        if(popu<0)
            popu=0;
        m_sommets[i].m_valeur=popu;
        std::cout << "popu : " << popu << " /   i :   " << i << std::endl;
    }
}

double Graphe::ressources(double base,double ress)
{
    if(base>ress)
        return base-(base-ress)*0.75;
    else
        return base;
}


std::vector<int> Graphe::RechercheComposanteFortementConnexe(int s)
{
    //VARIABLES
    std::vector< std::vector<int> > matrice_adjacence;
    std::vector<int> tmp;
    std::vector <int> c1;
    std::vector <int> c2;
    std::vector <int> c;
    std::vector <int> jean;
    std::vector <int> marques;
    //int x;
    int a,b;
    //int y;
    int ajoute =1;
  //  int s;
    int blind=0;

    //RECUPERATION DU SOMMET DE REFERENCE

    //INITIALISATION DE LA MATRICE D ADJACENCE
    for(int j=0; j<m_ordre; j++)
    {
        tmp.push_back(0);
    }
    for(int i=0; i<m_ordre; i++)
    {
        matrice_adjacence.push_back(tmp);
    }
    //REMPLISSAGE DE LA MATRICE D ADJACENCE.
    for(int i=0; i<m_arcs.size(); i++)
    {
        for(int j=0; j<m_sommets.size(); j++)
        {
            if(m_sommets[j].m_index==m_arcs[i].m_from)
            {
                a=j;
            }
        }
        for(int t=0; t<m_sommets.size(); t++)
        {
            if(m_sommets[t].m_index==m_arcs[i].m_to)
            {
                b=t;
            }
        }
        matrice_adjacence[a][b]=1;
        //   matrice_adjacence[b][a]=1;
    }

    //INTIALISATIONS DES VECTEURS
    for(int i=0; i<m_sommets.size(); i++)
    {
        c1.push_back(0);
        c2.push_back(0);
        c.push_back(0);
        jean.push_back(0);
        marques.push_back(0);
    }
    //LE SOMMET S DEVIENT CONNEXE
    for(int i=0; i<m_ordre; i++)
    {
        if(m_sommets[i].m_index==s)
        {
            c1[i]=1;
            c2[i]=1;
        }
    }


    //RECHERCHE C1
    while(ajoute==1)
    {
        ajoute=0;


        for(int x=0; x<m_ordre; x++)
        {
            if(marques[x]==0 && c1[x]==1)
            {
                marques[x]=1;
                for(int y=0; y<m_ordre; y++)
                {
                    if(matrice_adjacence[x][y]==1 && marques[y]==0)
                    {
                        c1[y]=1;
                        ajoute=1;
                    }
                }
            }
        }
    }
    //RECHERCHE C2
    for(int i=0; i<m_ordre; i++)
    {
        //RESET DE MARQUES et jean

        for(int t=0; t<m_ordre; t++)
        {
            marques[t]=0;
            jean[t]=0;
        }
         //LE SOMMET S DEVIENT CONNEXE
    for(int t=0; t<m_ordre; t++)
    {
        if(m_sommets[t].m_index==m_sommets[i].m_index)
        {
            jean[t]=1;
        }
    }
        //REBAIL PTDRLOLXD
        ajoute=1;
        while(ajoute==1)
        {
            ajoute=0;


            for(int x=0; x<m_ordre; x++)
            {
                if(marques[x]==0 && jean[x]==1)
                {
                    marques[x]=1;
                    for(int y=0; y<m_ordre; y++)
                    {
                        if(matrice_adjacence[x][y]==1 && marques[y]==0)
                        {
                            jean[y]=1;
                            ajoute=1;
                        }
                    }
                }
            }
        }
        //REGARDER SI CE TRUC PEUT ALLER A S
        for(int g=0; g<m_ordre; g++)  //PARCOURS DE TMP
        {
            if(m_sommets[g].m_index==s)
            {
                if(jean[g]==1)
                {
                    c2[i]=1;
                }
            }
        }

    }
    //CREATION DE LA COMPOSANTE FORTEMENT CONNEXE
    for(int i=0; i<m_ordre; i++)
    {
        c[i]=c1[i]&c2[i];
    }


    return c;
}

std::vector< std::vector<int> > Graphe::TouteLesComposantesFortementsConnexes()
{
     std::vector< std::vector<int> > matrice_adjacence;
      std::vector< std::vector<int> > tabc;
       std::vector<int> marques;
       std::vector<int> tmp;
       int a,b;
      //INITIALISATION DE LA MATRICE D ADJACENCE + TABC + MARQUES
    for(int j=0; j<m_ordre; j++)
    {
        tmp.push_back(0);
        marques.push_back(0);
    }
    for(int i=0; i<m_ordre; i++)
    {
        matrice_adjacence.push_back(tmp);
        tabc.push_back(tmp);
    }
    //REMPLISSAGE DE LA MATRICE D ADJACENCE.
    for(int i=0; i<m_arcs.size(); i++)
    {
        for(int j=0; j<m_sommets.size(); j++)
        {
            if(m_sommets[j].m_index==m_arcs[i].m_from)
            {
                a=j;
            }
        }
        for(int t=0; t<m_sommets.size(); t++)
        {
            if(m_sommets[t].m_index==m_arcs[i].m_to)
            {
                b=t;
            }
        }
        matrice_adjacence[a][b]=1;
        //   matrice_adjacence[b][a]=1;
    }
    for(int x=0;x<m_ordre;x++)
    {
        if(marques[x]==0)
        {
            tabc[x]=RechercheComposanteFortementConnexe(m_sommets[x].m_index);
            marques[x]=1;
            for(int y=0;y<m_ordre;y++)
            {
                if(tabc[x][y]==1 && marques[y]==0)
                {
                    marques[y]=1;
                }
            }
        }
    }
     for(int x=0;x<m_ordre;x++)
     {
         std::cout<<m_sommets[x].m_index<<" ";
     }
     std::cout<<std::endl;
    return tabc;

}
void Graphe::update()
{

    int val_ind=0;
    int b;
    std::string nom;
    double fertilite;
    double deces;
    int population;


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


    if(m_interface->m_bouton_ajout_sommet1.clicked()&&bol!=1)
    {
        bol=1;
    }
    if(m_interface->m_bouton_ajout_sommet2.clicked()&&bol!=2)
    {
        bol=2;
    }

    for(int i=0; i<m_sommets.size(); i++)
    {

        if(m_sommets[i].m_interface->m_bouton_delete.clicked())
        {
            suppression_sommet(m_sommets[i].m_index);
        }
        if(m_sommets[i].m_interface->m_bouton_link.clicked())
        {
            switch(bol)
            {
            case 1:
                ss1=m_sommets[i];
                rest;
            case 2:
                ss2=m_sommets[i];
                rest;
            }
        }
    }

    if(m_interface->m_bouton_link.clicked() && ss1.m_index!=ss2.m_index)
    {
        b=0;
        while (b==0)
        {
            for(int i=0; i<m_arcs.size(); i++ )
            {
                if(val_ind==m_arcs[i].m_indx)
                {
                    val_ind++;
                }
                else
                {
                    b=1;
                }


            }
        }
        add_interfaced_arc(val_ind,ss1.m_index,ss2.m_index);
        m_nbarcs++;
    }


    if (m_interface->m_bouton_save.clicked())
    {
        std::cout << "Dans quel fichier enregistrer votre graphe ?" << std::endl;
        std::cin >> nom;
        sauvegarde(nom);
    }

    if (m_interface->m_bouton_load.clicked())
    {
        std::cout << "Quel fichier voulez vous load ?" << std::endl;
        std::cin >> nom;

        while (m_arcs.size()>0)
        {
            m_arcs.pop_back();
        }
        while (m_sommets.size()>0)
        {
            m_sommets.pop_back();
        }
        lecture(nom);
    }
    if(m_interface->m_ajouter_sommet.clicked())
    {
        std::cout << std::endl << "----------AJJOUT SOMMET----------" << std::endl << std::endl;
        std::cout << "Quel animal ou fruit voulez vous ?  ";
        std::cin >> nom;
        nom+=".png";
        std::cout << "Combien en faut-il ?  ";
        std::cin >> population;
        std::cout << "Quelle fertilité pour l'espece ? (double)  ";
        std::cin >> fertilite;
        std::cout << "Quelle mortalité naturelle ? (double)  ";
        std::cin >> deces;

        b=0;
        while (b==0)
        {
            for(int i=0; i<m_sommets.size(); i++ )
            {
                if(val_ind==m_sommets[i].m_index)
                {
                    val_ind++;
                }
                else
                {
                    b=1;
                }
            }
        }
        add_interfaced_sommet(val_ind,population,300,300,nom,fertilite,deces);
        m_ordre++;
    }

    if(m_interface->m_lancer_simulation.clicked())
        simu=1;
    if(m_interface->m_pause_simulation.clicked())
        simu=0;
    if(simu==1)
        simulation();


    // supression des arcs
    for(int i=0; i<m_arcs.size(); i++)
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
    {
        done2=1;
        for(int i=0; i<m_arcs.size(); i++)
        {
            if(m_arcs[i].m_from==indice ||m_arcs[i].m_to==indice )
            {
                //on suprime l arete i
                //   std::cout<<"bitocul  "<<m_arcs[i].m_to<<"   " << i<<std::endl;
                //suppression_arc(i);
                done2=0;
                m_interface->m_main_box.remove_child(m_arcs[i].m_interface->m_top_edge);
                m_arcs.erase(m_arcs.begin()+i);
                m_nbarcs--;
            }
        }
    }
    //supression du sommet
    for(int i=0; i<m_sommets.size(); i++)
    {
        if(m_sommets[i].m_index==indice && done ==0)
        {


            m_interface->m_main_box.remove_child(m_sommets[i].m_interface->m_top_box);//copie

            tmp=m_sommets[m_sommets.size()-1];

            m_sommets[m_sommets.size()-1] = m_sommets[i];
            m_sommets[i]=tmp;

            m_sommets.pop_back();
            m_ordre--;

            done =1;
        }
    }



    // std::cout<<"test"<<std::endl;
}

void Graphe::suppression_arc(int indice)
{
    Arc tmp;
    int done=0;
    for(int i=0; i<m_arcs.size(); i++)
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
    // Creation d'une interface de sommet
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

    Sommet som1,som2;
    for (int i=0; i<m_sommets.size(); i++)
    {
        if(m_sommets[i].m_index==id_som1)
            som1=m_sommets[i];
        if(m_sommets[i].m_index==id_som2)
            som2=m_sommets[i];
    }

    ArcInterface *ei = new ArcInterface(som1,som2);
//    *ei.pls=&*ei.m_top_edge;

    m_interface->m_main_box.add_child(ei->m_top_edge);
    Arc larc(poids, ei, idx,id_som1,id_som2);
    m_arcs.push_back(larc);

    for (int i=0; i<m_sommets.size(); i++)
    {
        if(m_sommets[i].m_index==id_som1)
            m_sommets[i].m_out.push_back(idx);
        if(m_sommets[i].m_index==id_som2)
            m_sommets[i].m_in.push_back(idx);
    }
}
void Graphe::surbrillance(std::vector<std::vector<int>> tabc)
{
    std::vector <int> rep;
    std::vector <int> idxe;//cotient les indice des sommets appartenant a uen composante fortement connexe
    std::vector<int> colou;
    int cola=0;
    colou.push_back(ROUGE);
    colou.push_back(ROSE);
    colou.push_back(VIOLET);

    colou.push_back(VERT);
    colou.push_back(CYAN);
    colou.push_back(BLEU);
    colou.push_back(ORANGE);
    colou.push_back(JAUNE);
    colou.push_back(NOIR);
    colou.push_back(VERTSOMBRE);

    for(int i=0;i<tabc.size();i++)
    {
        for(int j=0;j<tabc[i].size();j++)
        {
            if(tabc[i][j]==1 && tabc[i][j+1]==1)
            {
                rep.push_back(i);
            }
        }
    }
    //breh breh
    for(int i=0;i<rep.size();i++)
    {
        //FOUTRE UN RESERT DE IDXE§!!!!!!!
       for(int j=0;j<tabc[rep[i]].size();j++)
       {
           if(tabc[rep[i]][j]==1)
           {
               idxe.push_back(m_sommets[j].m_index);
           }
       }
       //PARCOURS DES ARRETES
       for(int a=0;a<m_arcs.size();a++)
       {
           //parcours des from
           for(int f=0;f<idxe.size();f++)
           {
               if(m_arcs[a].m_from==idxe[f])
               {
                   //parcours des to
                   for(int t=0;t<idxe.size();t++)
                   {
                       if(m_arcs[a].m_to==idxe[t])
                       {

                           m_arcs[a].m_interface->m_top_edge.set_color(colou[cola]);

                       }
                       //cola++;
                   }


               }

           }


       }
    }
}
