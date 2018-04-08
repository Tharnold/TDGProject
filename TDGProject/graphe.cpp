#include "graphe.h"
#include <chrono>
#include <thread>
#include <queue>
SommetInterface::SommetInterface(int idx, int x, int y, std::string nom_foto, int foto_idx)
{
    //LA BOITE ENGLOBANTE
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    //LE SLIDER DE REGLAGE DE VALEUR
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 1.0);
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    //LABEL DE VISUALISATION DE VALEUR
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    //UNE ILLUSTRATION
    if (nom_foto!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(nom_foto);
        m_img.set_pic_idx(foto_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    //LABEL DE VISUALISATION D INDEX DU SOMMET DANS UNE BOITE
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);
    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );

    //LABEL DE VISUALISATION DE LA POPULATION
    m_top_box.add_child( m_box_label_pop );
    m_box_label_pop.set_frame(33,89,40,12);
    m_box_label_pop.set_bg_color(BLANC);
    m_box_label_pop.add_child( m_label_pop );

    //BOUTON POUR LA SUPPRESSION DU SOMMET
    m_top_box.add_child( m_bouton_delete );
    m_bouton_delete.set_frame(114,3,16,16);
    m_bouton_delete.set_bg_color(ROUGE);
    m_bouton_delete.add_child(m_bouton_delete_label);
    m_bouton_delete_label.set_message("X");

    //BOUTON POUR SELECTIONNER LE SOMMET LORS DE L AJOUT D UNE ARRETE
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

    //MAJ DE LA VALEUR DU SLIDER
    m_interface->m_slider_value.set_value(m_fertilite);

    //MAJ DE L AFFICHAGE DE LA VALEUR DU SLIDER
    m_interface->m_label_value.set_message( std::to_string( m_fertilite) );
    m_interface->m_label_pop.set_message(std::to_string((int)m_valeur));
}


void Sommet::post_update()
{
    if (!m_interface)
        return;

    //MAJ DE LA VALEUR ASSOCIEE AU SLIDER
    m_fertilite = m_interface->m_slider_value.get_value();
}

ArcInterface::ArcInterface(Sommet& from, Sommet& to)
{
    //WIDGETEDGE DE L INTERFACE DE L ARC
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating SommetInterface between vertices having no interface" << std::endl;
        throw "Bad ArcInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    //BOITE DE REGLAGE DE L ARC
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,75);
    m_box_edge.set_bg_color(BLANCBLEU);

    //SLIDER DE L ARC
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    //LABEL DE VISUALISATION DE LA VALEUR ASSOCIEE AU SLIDER
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

    //BOUTON POUR SUPPRIMER UNE ARETE
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

    //COPIER LA VALEUR LOCALE DU POIDS VERS LE SLIDER
    m_interface->m_slider_weight.set_value(m_poids);

    //COPIER LA VALEUR LOCALE DU POIDS VERS LE LABEL SOUS LE SLIDER
    m_interface->m_label_weight.set_message( std::to_string( (double)m_poids ) );
}

void Arc::post_update()
{
    if (!m_interface)
        return;

    //REPRENDRE LA VALEUR DU SLIDER DANS LA DONNEE
    m_poids = m_interface->m_slider_weight.get_value();
}

GrapheInterface::GrapheInterface(int x, int y, int w, int h)
{
    //INITIALISATION DE LA BOITE ENGLOBANT TOUT L INTERFACE
    m_top_box.set_dim(790,590);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(90,590);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(700,590);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(0xFFF8DC);

    //BOUTON POUR SELECTIONNER LE SOMMET 1
    m_tool_box.add_child(m_bouton_ajout_sommet1);
    m_bouton_ajout_sommet1.set_frame(3,3,77,40);
    m_bouton_ajout_sommet1.set_bg_color(0x749A6F);
    m_bouton_ajout_sommet1.add_child(m_bouton_ajout_sommet1_label);
    m_bouton_ajout_sommet1_label.set_message("Add s1");

    //BOUTON POUR SELECTIONNER LE SOMMET 2
    m_tool_box.add_child(m_bouton_ajout_sommet2);
    m_bouton_ajout_sommet2.set_frame(3,43,77,40);
    m_bouton_ajout_sommet2.set_bg_color(0xB7CA79);
    m_bouton_ajout_sommet2.add_child(m_bouton_ajout_sommet2_label);
    m_bouton_ajout_sommet2_label.set_message("Add s2");

    //BOUTON POUR CREER UNE ARETE ENTRE LES SOMMETS 1 ET 2
    m_tool_box.add_child(m_bouton_link);
    m_bouton_link.set_frame(3,83,77,40);
    m_bouton_link.set_bg_color(0x749A6F);
    m_bouton_link.add_child(m_bouton_link_label);
    m_bouton_link_label.set_message("link");

    //BOUTON POUR AJOUTER UN SOMMET
    m_tool_box.add_child(m_ajouter_sommet);
    m_ajouter_sommet.set_frame(3,123,77,40);
    m_ajouter_sommet.set_bg_color(0xB7CA79);
    m_ajouter_sommet.add_child(m_ajouter_sommet_label);
    m_ajouter_sommet_label.set_message("+ sommet");

    //BOUTON POUR LANCER LA SIMULATION
    m_tool_box.add_child(m_lancer_simulation);
    m_lancer_simulation.set_frame(3,163,77,40);
    m_lancer_simulation.set_bg_color(0x749A6F);
    m_lancer_simulation.add_child(m_lancer_simulation_label);
    m_lancer_simulation_label.set_message("Play");

    //BOUTON POUR METTRE EN PAUSE LA SIMULATION
    m_tool_box.add_child(m_pause_simulation);
    m_pause_simulation.set_frame(3,203,77,40);
    m_pause_simulation.set_bg_color(0xB7CA79);
    m_pause_simulation.add_child(m_pause_simulation_label);
    m_pause_simulation_label.set_message("Pause");

    //BOUTON POUR CHARGER UN GRAPHE DANS UN FICHIER
    m_tool_box.add_child(m_bouton_load);
    m_bouton_load.set_frame(3,243,77,40);
    m_bouton_load.set_bg_color(0x749A6F);
    m_bouton_load.add_child(m_bouton_load_label);
    m_bouton_load_label.set_message("Load");

    //BOUTON POUR SAUVEGARDER LE GRAPHE EN COURS D UTILISATION DANS UN FICHIER
    m_tool_box.add_child(m_bouton_save);
    m_bouton_save.set_frame(3,283,77,40);
    m_bouton_save.set_bg_color(0xB7CA79);
    m_bouton_save.add_child(m_bouton_save_label);
    m_bouton_save_label.set_message("Save");

    //BOUTON POUR AFFICHER LES COMPOSANTES FORTEMENTS CONNEXES DU GRAPHE ACTIF
    m_tool_box.add_child(m_bouton_forteco);
    m_bouton_forteco.set_frame(3,323,77,40);
    m_bouton_forteco.set_bg_color(0x749A6F);
    m_bouton_forteco.add_child(m_bouton_forteco_label);
    m_bouton_forteco_label.set_message("elt.cnx");

    //BOUTON POUR RESET LES COULEURS DES ARCS
    m_tool_box.add_child(m_bouton_resetcol);
    m_bouton_resetcol.set_frame(3,363,77,40);
    m_bouton_resetcol.set_bg_color(0xB7CA79);
    m_bouton_resetcol.add_child(m_bouton_resetcol_label);
    m_bouton_resetcol_label.set_message("Reset");

    //BOUTON POUR AFFIVHER L ELEMENT FAIBLE
    m_tool_box.add_child(m_bouton_kco);
    m_bouton_kco.set_frame(3,403,77,40);
    m_bouton_kco.set_bg_color(0x749A6F);
    m_bouton_kco.add_child(m_bouton_kco_label);
    m_bouton_kco_label.set_message("k-cnx");

    //BOUTON QUITTER
    m_tool_box.add_child(m_bouton_quit);
    m_bouton_quit.set_frame(3,443,77,40);
    m_bouton_quit.set_bg_color(0xB7CA79);
    m_bouton_quit.add_child(m_bouton_quit_label);
    m_bouton_quit_label.set_message("Quit");


}
void Graphe::resetcol()
{
    for (int i=0; i<m_arcs.size(); i++)
    {
        m_arcs[i].m_interface->m_top_edge.set_color(GRISSOMBRE);
    }
}

void Graphe::lecture(std::string nom)
{

    m_nfo=nom;

    std::string nf;
    nf=nom;
    nf+=".txt";


    //VALEURS POUR STOCKER LES DONEES DU FICHIER
    m_interface= std::make_shared<GrapheInterface>(50, 0, 750, 600);
    int idx,x,y;
    double val,fertilite,deces;
    std::string nom_foto;
    int nb_sommets;
    int nb_arretes;

    //OUVERTURE DU FICHER
    std::ifstream fichier(nf);
    if(fichier)
    {
        fichier >> nb_sommets;
        fichier >> nb_arretes;
        // std::cout << "recup ordre et tt" << std::endl;
        m_ordre=nb_sommets;
        m_nbarcs=nb_arretes;

        //RECUPERATION DES VALEURS DES SOMMETS
        for(int i=0; i< nb_sommets; i++)
        {
            fichier >> idx;
            fichier >> val;
            fichier >> x;
            fichier >> y;
            fichier >> nom_foto;
            fichier >> fertilite;
            fichier >> deces;
            add_interfaced_sommet(idx,val,x,y,nom_foto,fertilite,deces);
        }

        //RECUPERATION DES VALEURS DES ARCS
        for(int i=0; i< nb_arretes; i++)
        {
            fichier >> idx;
            fichier >> x;
            fichier >> y;
            fichier >> val;
            add_interfaced_arc(idx,x,y,val);
        }
    }
    else
    {

        std::cerr <<"l'ouverture du fichier est un echec cuisant"<< std::endl;
    }
}

void Graphe::sauvegarde(std::string nom)
{
    //OUVERTURE DU FICHIER
    nom+=".txt";
    std::ofstream fichier(nom, std::ios::trunc);
    if(fichier)
    {
        //ECRITURE DANS LE FICHIER
        fichier << m_ordre << std::endl;
        fichier << m_nbarcs << std::endl;
        for(auto &elt : m_sommets)
        {
            //ECRITURE DES VALEURS DES SOMMETS
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
            //ECRITURE DES VALEURS DES ARETES
            fichier << elt.m_indx << " ";
            fichier << elt.m_from << " ";
            fichier << elt.m_to << " ";
            fichier << elt.m_poids << std::endl;
        }
    }
}

void Graphe::simulation()
{
    //POPULATION PENDANT LA MISE A JOUR
    int popu=0;
    //NOMBRE D'INDIVIDUS MORTS DE PREDATEURS
    int eaten=0;
    //NOMBRE DE RESSOURCES
    int eat=0;
    int som=0;
    int rarc=0;
    double pds=0;

    //PARCOURS DE TOUS LES SOMMETS POUR LEUR MISE A JOUR
    for (int i=0; i<m_sommets.size(); i++)
    {
        //RAZ
        popu=0;
        eaten=0;
        eat=0;
        som=0;
        rarc=0;
        pds=0;
        //CALCUL DE LA POPULATION AU TEMPS T+1
        popu=m_sommets[i].m_valeur;
        popu+=m_sommets[i].m_valeur*(m_sommets[i].m_fertilite-m_sommets[i].m_deces_mois);
        std::cout << "FERTILITE ===   " << m_sommets[i].m_fertilite << std::endl;
        std::cout << "MORT ===   " << m_sommets[i].m_deces_mois << std::endl;
        std::cout << "foisveleur   " << m_sommets[i].m_valeur*(m_sommets[i].m_fertilite-m_sommets[i].m_deces_mois) << std::endl;
        ///RECUPERATION DES ARCS INCIDENTS POUR AVOIR LA QUANTITE DE PREDATEURS ET DE PROIES
        //RECUPERATION DES PREDATEURS
        for(int j=0; j<m_sommets[i].m_in.size(); j++)
        {
            for(int k=0; k<m_sommets.size(); k++)
            {
                for(int l=0; l<m_arcs.size(); l++)
                {
                    if(m_arcs[l].m_indx==m_sommets[i].m_in[j])
                    {
                        rarc=m_arcs[l].m_from;
                        pds=m_arcs[l].m_poids;
                    }
                }
                if(m_sommets[k].m_index==rarc)
                {
                    som=k;
                }
            }
            eaten+=pds*m_sommets[som].m_valeur;
        }
        pds=0;
        eat=0;
        //RECUPERATION DES PROIES(RESSOURCES)
        for(int j=0; j<m_sommets[i].m_out.size(); j++)
        {
            for(int k=0; k<m_sommets.size(); k++)
            {
                for(int l=0; l<m_arcs.size(); l++)
                {
                    if(m_arcs[l].m_indx==m_sommets[i].m_out[j])
                    {
                        rarc=m_arcs[l].m_to;
                        pds=m_arcs[l].m_poids;
                    }
                }
                if(m_sommets[k].m_index==rarc)
                {
                    som=k;
                }
            }
            if(pds!=0)
                eat+=m_sommets[som].m_valeur/pds;
            else
                eat+=10000000;
        }
        popu-=eaten;
        //VERIFICATION DES MORTS PAR MANQUE DE NOURRITURE
        if(eat>=0)
            popu=ressources(popu,eat);
        if(popu<0)
            popu=0;
        m_sommets[i].m_valeur=popu;
        std::cout << "popu : " << popu << " /   i :   " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

    }
}

//VERIFICATION SI LES RESSOURCES SONT SUFFISANTES
int Graphe::ressources(int base,int ress)
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
    std::vector<int> tmp;   //VECTEUR TAMPON
    std::vector <int> c1;  //COMPOSANTE CONNEXE DIRECTES PARTANTS DE S
    std::vector <int> c2;  //COMPOSANTE CONNEXE INDIRECTS ARRIVANT VERS S
    std::vector <int> c;    //COMPOSANTE FORTEMENT CONNEXE QUE L ON VA RETOURNER
    std::vector <int> jean;  //COMPOSANTE PARTANT D UN SOMMET POUR VERIFIER S IL PEUT ALLER JUSQU A S POUR L AJOUTER A C2
    std::vector <int> marques;  //TABLEAU INDIQUANTS SI LES SOMMETS SONT MARQUES
    int a,b;    //ENTIERS POUR REMPLIR LA MATRICE D ADJACENCE
    int ajoute =1;  //BOOLEEN INDIQUANT SI ON AJOUTE UNE NOUVELLE COMPOSANTE CONNEXE

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
        //matrice_adjacence[b][a]=1;
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

    //RECHERCHE DES COMPOSANTES CONNEXES PARTANT DE S
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
                        //AJOUT DE LA NOUVELLE COMPOSANTE CONNEXE
                        c1[y]=1;
                        ajoute=1;
                    }
                }
            }
        }
    }

    //RECHERCHE DES COMPOSANTES CONNEXES ARRIVANT A S
    for(int i=0; i<m_ordre; i++)
    {

        //RESET DE MARQUES ET JEAN
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

        //RECHERCHE DES COMPOSANTES CONNEXE PARTANT DE I
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

        //ON REGARDE SI LA COMOSANTE ARRIVE JUSQU A S
        for(int g=0; g<m_ordre; g++)
        {
            if(m_sommets[g].m_index==s)
            {
                if(jean[g]==1)
                {
                    //AJOUT DE LA COMPOSANTE CONNEXE
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

    //ON RETURN LA COMPOSANTE FORTEMENT CONNEXE
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

    }

    //RECHERCHE DE LA COMPOSANTE FORTEMENT CONNEXE DE TOUT LES SOMMETS
    for(int x=0; x<m_ordre; x++)
    {
        if(marques[x]==0)
        {
            tabc[x]=RechercheComposanteFortementConnexe(m_sommets[x].m_index);
            marques[x]=1;

            for(int y=0; y<m_ordre; y++)
            {
                if(tabc[x][y]==1 && marques[y]==0)
                {
                    marques[y]=1;
                }
            }
        }
    }

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

    //AFFICHAGE DES COMPOSANTES FORTEMENTS CONNEXES
    if(m_interface->m_bouton_forteco.clicked())
    {
        surbrillance(TouteLesComposantesFortementsConnexes());
    }
    //SI ON APUIE SUR C-CNX

    if(m_interface->m_bouton_kco.clicked())
    {
        algodekco();
        for(int i=0; i<m_ordre; i++)
        {
            for(int j=0; j<m_combif.size(); j++)
            {
                if(m_combif[j]==i)
                {
                    m_sommets[i].m_interface->m_top_box.set_color(ROUGE);
                }
            }
        }
    }
    //INDICATION DE SI L ON VEUT SELECTIONNER LE SOMMETS DE DEPART OU D ARRIVEE POUR LA CREATION D UN ARC
    if(m_interface->m_bouton_ajout_sommet1.clicked()&&bol!=1)
    {
        bol=1;
    }
    if(m_interface->m_bouton_ajout_sommet2.clicked()&&bol!=2)
    {
        bol=2;
    }

    //PARCOURS DES SOMMETS POUR LES BOUTONS CLICK
    for(int i=0; i<m_sommets.size(); i++)
    {
        //SI ON CLIQUE SUR LE BOUTON SUPPRIMER
        if(m_sommets[i].m_interface->m_bouton_delete.clicked())
        {
            //ON SUPPRIME LE SOMMET
            suppression_sommet(m_sommets[i].m_index);
        }

        //SI ON CLIQUE SUR LE BONTON "+"
        if(m_sommets[i].m_interface->m_bouton_link.clicked())
        {
            switch(bol)
            {
            //SI ON A SELECTIONE QU ON VOULAIT SELECTIONNER LE SOMMET DE DEPART
            case 1:

                //ON STOCK LE SOMMET EN QUESTION
                ss1=m_sommets[i];
                rest;

            //SI ON A SELECTIONE QU ON VOULAIT SELECTIONNER LE SOMMET D ARRIVEE
            case 2:

                //ON STOCK LE SOMMET EN QUESTION
                ss2=m_sommets[i];
                rest;
            }
        }
    }

    //SI ON APPUIE SUR LE BOUTON LINK ET QUE LES SOMMETS DE DEPART ET ARRIVEE SONT DIFFERENTS
    if(m_interface->m_bouton_link.clicked() && ss1.m_index!=ss2.m_index)
    {
        b=0;

        //ON CHERCHE UN INDICE A ATTRIBUER A L ARC QUE L ON VA AJOUTER
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

        //ON CREE L ARC
        add_interfaced_arc(val_ind,ss1.m_index,ss2.m_index);
        m_nbarcs++;
    }

    //SI ON CLICK SUR LE BOUTON SAVE
    if (m_interface->m_bouton_save.clicked())
    {
        //APPEL DE LA FONCTION DE SAUVEGARDE
        std::cout << "Dans quel fichier enregistrer votre graphe ?" << std::endl;
        std::cin >> nom;
        sauvegarde(nom);
    }

    //SI ON CLIQUE SUR LE BOUTON LOAD
    if (m_interface->m_bouton_load.clicked())
    {
        std::cout << "Quel fichier voulez vous load ?" << std::endl;
        std::cin >> nom;

        //ON SUPPRIME TOUTE LES DONEES DU GRAPHE ACTIF
        while (m_arcs.size()>0)
        {
            m_arcs.pop_back();
        }
        while (m_sommets.size()>0)
        {
            m_sommets.pop_back();
        }

        //ON CHARGE LE NOUVEAU GRAPHE
        lecture(nom);
    }
    //SI ON CLIQUE SUR LE BOUTON RESET
    if(m_interface->m_bouton_resetcol.clicked())
    {
        resetcol();
    }

    //SI ON CLIQUE SUR LE BOUTON ADD
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

        //ON CHERCHE UN INDICE A ATTRIBUER AU NOUVEAU SOMMET
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

        //AJOUT DU NOUVEAU SOMMET
        add_interfaced_sommet(val_ind,population,300,300,nom,fertilite,deces);
        m_ordre++;
    }

    //SI ON CLIQUE SUR LE BOUTON PLAY
    if(m_interface->m_lancer_simulation.clicked())
        simu=1; //ON LANCE LA SIMULATION
    if(m_interface->m_pause_simulation.clicked())
        simu=0; //ON MET LA SIMULATION EN PAUSE
    if(simu==1)
        simulation();


    //ON PARCOURS LES ARCS
    for(int i=0; i<m_arcs.size(); i++)
    {
        //SI ON CLIQUE SUR LE BOUTON POUR SUPPRIMER L ARC
        if(m_arcs[i].m_interface->m_bouton_delete.clicked())
        {
            //ON SUPPRIME L ARC
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

    //SUPPRESSION DE TOUTES LES ARRETES RELIEES AU SOMMET
    while(done2==0)
    {
        done2=1;
        for(int i=0; i<m_arcs.size(); i++)
        {
            if(m_arcs[i].m_from==indice ||m_arcs[i].m_to==indice )
            {
                //ON SUPPRIME L ARETE I SI ELLE EST RELIEE AU SOMMET
                done2=0;
                m_interface->m_main_box.remove_child(m_arcs[i].m_interface->m_top_edge);
                m_arcs.erase(m_arcs.begin()+i);
                m_nbarcs--;
            }
        }
    }

    //SUPPRESSION DU SOMMET
    for(int i=0; i<m_sommets.size(); i++)
    {
        if(m_sommets[i].m_index==indice && done ==0)
        {
            //SUPRESSION DE L INTERfACE DU SOMMET
            m_interface->m_main_box.remove_child(m_sommets[i].m_interface->m_top_box);//copie
            //SUPRESSION DES DONNEE DU SOMMET
            tmp=m_sommets[m_sommets.size()-1];
            m_sommets[m_sommets.size()-1] = m_sommets[i];
            m_sommets[i]=tmp;
            m_sommets.pop_back();
            m_ordre--;

            //ON INDIQUE QUE LE SOMMET A ETE SUPPRIME
            done =1;
        }
    }
}



void Graphe::add_interfaced_sommet(int idx, double valeur, int x, int y, std::string nom_foto, double fertilite, double deces_mois, int foto_idx )
{

    //CREATION D UNE INTERFACE
    SommetInterface *vi = new SommetInterface(idx, x, y, nom_foto, foto_idx);

    //AJOUT DE LA TOPBOX
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    Sommet som_prov(valeur, vi, idx);
    som_prov.ajouter_var(fertilite,deces_mois);
    m_sommets.push_back(som_prov);

}

void Graphe::add_interfaced_arc(int idx, int id_som1, int id_som2, double poids)
{
    //RECUPERATION DES SOMMETS POUR AVOIR LEURS INDICES
    Sommet som1,som2;
    for (int i=0; i<m_sommets.size(); i++)
    {
        if(m_sommets[i].m_index==id_som1)
            som1=m_sommets[i];
        if(m_sommets[i].m_index==id_som2)
            som2=m_sommets[i];
    }

    //CREATION DE L INTERFACE
    ArcInterface *ei = new ArcInterface(som1,som2);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    Arc larc(poids, ei, idx,id_som1,id_som2);
    m_arcs.push_back(larc);

    //AJOUT DANS LES VECTEURS D ARCS DES SOMMETS
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
    std::vector <int> idxe;//CONTIENT LES INDICES DES SOMMETS APPARTENANT A UNE COMPOSANTE FORTEMENT CONNEXE
    std::vector<int> colou;
    int cola=0;
    int mf,mt;
    colou.push_back(ROUGE);
    colou.push_back(ROSE);
    colou.push_back(VIOLET);
    colou.push_back(VERT);
    colou.push_back(CYAN);
    colou.push_back(BLEU);
    colou.push_back(ORANGE);
    colou.push_back(JAUNE);
    colou.push_back(NOIR);
    colou.push_back(VERTFLUOSOMBRE);
    //ON RESET LES COULEURS DES ARCS
    resetcol();

    //ON INDIQUE A QUELLE COMPOSANTE FROTEMENT CONNEXE APPARTIENT CHAQUE SOMMET
    for(int i=0; i<tabc.size(); i++)
    {
        for(int j=0; j<tabc[i].size(); j++)
        {
            if(tabc[i][j]==1)
            {
                m_sommets[j].m_cfc=i;
            }
        }
    }

    //ON PARCOURS LES ARCS
    for(int i=0; i<m_arcs.size(); i++)
    {
        //ON CHERCHE A QUELLE COMPOSANTE FORTEMENT CONNECTE APPARTIENT LE SOMMET DE DEPART DE L ARC
        for(int f=0; f<m_sommets.size(); f++)
        {
            if(m_sommets[f].m_index==m_arcs[i].m_from)
            {
                mf=m_sommets[f].m_cfc;


            }
        }

        //ON CHERCHE A QUELLE COMPOSANTE FORTEMENT CONNECTE APPARTIENT LE SOMMET D ARRIVEE DE L ARC
        for(int f=0; f<m_sommets.size(); f++)
        {
            if(m_sommets[f].m_index==m_arcs[i].m_to)
            {
                mt=m_sommets[f].m_cfc;

            }
        }

        //SI L ARC RELIE DEUX SOMMETS DE LA MEME COMPOSANTES FORTEMENT CONNEXE
        if(mf==mt)
        {
            //ON LE COLORIE DE LA MEME COULEUR QUE LES ARCS RELIANT DES SOMMETS APPARTENANT A LA MEME COMPOSANTE FORTEMENT CONNEXE ENTRE EUX
            m_arcs[i].m_interface->m_top_edge.set_color(colou[mf]);

        }
    }
}

void Graphe::algodekco()
{
    m_combi_done=0;
    int t=0;
    for(int i=1; i<m_ordre; i++)
    {

        while(m_tmp.size()>0)
        {
            m_tmp.pop_back();
        }
        while(m_combi.size()>0)
        {
            m_combi.pop_back();
        }

        kco(i);
    }
}

void Graphe::kco(int k) // equivalent de son main
{

    for(int i=0; i<m_ordre; i++)
    {
        m_tmp.push_back(i);
    }
    recu(0,k);
}

void Graphe::recu(int offset,int k)
{


    if(k==0)
    {
        testconnexite(m_combi);
        return;
    }
    for(int i=offset; i<m_tmp.size(); i++)
    {
        m_combi.push_back(m_tmp[i]);
        recu(i+1,k-1);
        m_combi.pop_back();
    }




}

void Graphe::testconnexite(std::vector<int> v)
{
    std::vector<std::vector<int>> matrice_adjacence;
    std::vector<int>jve;
    std::vector<int>tmp;
    std::vector<int>pred;
    std::vector<int> marques;
    std::queue<int> file;
    int a,b;
    int x=0;

    for(int i=0; i<v.size(); i++)
    {
        jve.push_back(m_sommets[v[i]].m_index);
    }
    for(int i=0; i<jve.size(); i++)
    {
        suppression_sommet(jve[i]);

    }
    //BFS
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
        matrice_adjacence[b][a]=1;
    }
    //initialisation de pred
    for (int i=0; i<m_ordre; i++)
    {
        pred.push_back(0);
    }
    // c pour du beure
    for(int i=0; i<matrice_adjacence.size(); i++)
    {
        for(int j=0; j<matrice_adjacence[i].size(); j++)
        {
            if(matrice_adjacence[i][j]==1)
            {
                x=i;
                i=matrice_adjacence.size()-1;
                j=i;
            }
        }

    }

    //INIT MARQUES
    for(int i=0; i<m_ordre; i++)
    {
        marques.push_back(0);
    }
    //SELECTION SUN SOMMET INITIAL
    file.push(x);
    marques[x]=1;
    //
    while(!file.empty())
    {
        x=file.front();
        file.pop();
        for(int y=0; y<m_ordre; y++)
        {
            if( (matrice_adjacence[x][y]==1|| matrice_adjacence[y][x]==1 )  && marques[y]==0)
            {
                marques[y]=1;
                file.push(y);
                pred[y]=x;
            }
        }
    }

    for(int i=0; i<marques.size(); i++)
    {
        if(marques[i]==0 && m_combi_done==0)
        {
            m_combif=v;
            m_combi_done=1;
        }
    }



    /*bail daffichage de toute les possibilite
    static int count = 0;
    std::cout << "combination no " << (++count) << ": [ ";
    for (int i = 0; i < v.size(); ++i)
    {
        std::cout << v[i] << " ";
    }
    std::cout << "] " << std::endl;
     */
    while (m_arcs.size()>0)
    {
        m_arcs.pop_back();
    }
    while (m_sommets.size()>0)
    {
        m_sommets.pop_back();
    }

    lecture(m_nfo);
}

int Graphe::QUIT()
{
    if(m_interface->m_bouton_quit.clicked())
        return 1;
}
///FIN
