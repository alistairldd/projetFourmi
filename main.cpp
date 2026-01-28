#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <array>
using namespace std;

#include "doctest.h"
#include "coord.hpp"
#include "fourmis.hpp"
#include "place.hpp"
#include "affichage.hpp"
#include "colonie.hpp"

const int APPASUCRE = 20;



bool Cond1(Fourmi f, Place voisine, vector<Fourmi> tabF){
    if (voisine.contientFourmi()){
        int ind = voisine.getNumeroFourmi();
        int idCol = trouveFourmi(tabF, ind).getIdCol();   
        if (idCol != f.getIdCol()){return true;}
    }
    return false;
}


bool Cond2(Fourmi f, Place voisine){
    return f.chercheSucre() and voisine.contientSucre() and not voisine.contientNid();

}

bool Cond3(Fourmi f, Place voisine){
    return f.porteSucre() and voisine.contientNid(f.getIdCol());
}    

bool Cond4 (Fourmi f, Place voisine, Grille g){
    Place p1 = g.chargePlace(f.getCoord());
    return  f.porteSucre() and voisine.estVide() and voisine.estPlusProcheNid(p1, f.getIdCol());
}

bool Cond5(Fourmi f, Place voisine, Grille g){
    Place p1 = g.chargePlace(f.getCoord());
    return f.chercheSucre() and voisine.estSurUnePiste(f.getIdCol()) and voisine.estVide() and p1.estPlusProcheNid(voisine, f.getIdCol()) and p1.estSurUnePiste(f.getIdCol());

}

bool Cond6(Fourmi f, Place voisine){
    return f.chercheSucre() and voisine.estSurUnePiste(f.getIdCol()) and voisine.estVide();

}

bool Cond7(Fourmi f, Place voisine){
    return  f.chercheSucre() and voisine.estVide();
}


bool regle1(Fourmi f, EnsCoord voisins, Grille &g, vector<Fourmi> &tabF){
    for (Coord c : voisins.getTab()){
        Place p2 = g.chargePlace(c);
        if (Cond1(f, p2, tabF)){
            int idFT = trouveFourmiId(tabF, p2.getNumeroFourmi());
            tabF[idFT].setAlive(false);
            p2.enleveFourmi();
            g.rangePlace(p2);
            return true;
        }
    }
    return false;
}



bool regle2(Fourmi &f, EnsCoord voisins, Grille &g){

    Place p1 = g.chargePlace(f.getCoord());
    for (Coord c : voisins.getTab()) {
        Place p2 = g.chargePlace(c);
        if (Cond2 (f, p2)) {
            //cout << "Application règle 2" << endl;
            f.prendSucre(); 
            p2.enleveSucre();
            p1.posePheroSucre(f.getIdCol());
            g.rangePlace(p2);
            g.rangePlace(p1);
            return true;
        }
    }
    return false;
}


bool regle3(Fourmi &f, EnsCoord voisins, Grille &g, Colonie &col){
    for (Coord c : voisins.getTab()) {
        Place p2 = g.chargePlace(c);
        if (Cond3(f, p2)){
            //cout << "Application règle 3" << endl;
            p2.poseSucre();
            f.poseSucre();
            g.rangePlace(p2);
            col.ajouteSucre(1);
            return true;
        }
    }
    return false;
}


bool regle4(Fourmi &f, EnsCoord voisins, Grille &g){
    EnsCoord possibilites = {{}};
    for (Coord c : voisins.getTab()) {
        if (Cond4 (f, g.chargePlace(c), g)){
            //cout << "Application règle 4" << endl;
            possibilites.ajoute(c);
        }
    }
    
    if (possibilites.taille() == 0) {return false;}
    
    Coord co = possibilites.choixHasard();
    deplaceFourmiGrille(g, f, co);
    Place p2 = g.chargePlace(co);
    p2.posePheroSucre(f.getIdCol());
    g.rangePlace(p2);
    return true;
}



bool regle5(Fourmi &f, EnsCoord voisins, Grille &g){
    EnsCoord possibilites = {{}};
    for (Coord c : voisins.getTab()) {
        if (Cond5(f, g.chargePlace(c), g)){
            //cout << "Application règle 7"  << endl;
            possibilites.ajoute(c);
        }
    }
    if (possibilites.taille() == 0) {return false;}
    
    Coord co = possibilites.choixHasard();
    deplaceFourmiGrille(g, f, co);
    return true;
}

bool regle6(Fourmi &f, EnsCoord voisins, Grille &g){
    
    EnsCoord tabC{{}};
    for (Coord c : voisins.getTab()) {
        Place p2 = g.chargePlace(c); 
        if(p2.estSurUnePiste(f.getIdCol())){
            tabC.ajoute(p2.getCoord());
        }
    }
    if (tabC.getTab().size() < 2){
        return false;
    } 


    
    EnsCoord possibilites = {{}};
    for (Coord c : voisins.getTab()) {
        if (Cond6(f, g.chargePlace(c))){
            //cout << "Application règle 6"  << endl;
            possibilites.ajoute(c);
        }
    }
    if (possibilites.taille() == 0) {return false;}
    
    Coord co = possibilites.choixHasard();
    deplaceFourmiGrille(g, f, co);
    return true;

}

bool regle7(Fourmi &f, EnsCoord voisins, Grille &g){
    int k = 0;
    Coord c = voisins.choixHasard();
    do {
        k++;
        if (k == 10 or g.chargePlace(c).estVide()) break ;
        else c = voisins.choixHasard();
    } while (true);
    if (k < 10) {
        if (Cond7(f, g.chargePlace(c))) {
            //cout << "Application règle 7"  << endl;
            deplaceFourmiGrille(g, f, c);
            return true;
        }
    }

    return false;
}



void apparitionSucre(Grille &g){
    do {
        int x = rand() % TAILLEGRILLE;
        int y = rand() % TAILLEGRILLE;
        Place p = g.chargePlace(Coord{x,y});
        if (p.estVide()){
            p.poseSucre();
            p.poseSucre();
            p.poseSucre();
            g.rangePlace(p);
            return; 
        }
    } while(true);
}


void affichePheroNid(Grille g, int colonie){
    for (int i = 0; i < int(g.getGrille().size()); i++){
        for (int j = 0; j < int(g.getGrille().size()); j++){
            cout << setw(4) << g.getGrille()[i][j].getPheroNid()[colonie] << " ";
        }
        cout << endl;
    }
}

EnsCoord fourmiAutourNid(EnsCoord nid){
    vector<Coord> tab = nid.getTab();
    EnsCoord tabFourmis{{}};
    if (tab[0].getLig() > 0 and tab[0].getCol() > 0 and tab[3].getLig() < TAILLEGRILLE and tab[3].getCol() < TAILLEGRILLE){
        // ligne 1
        tabFourmis.ajoute(Coord{tab[0].getLig() - 1, tab[0].getCol()-1});
        tabFourmis.ajoute(Coord{tab[0].getLig() - 1, tab[0].getCol()});
        tabFourmis.ajoute(Coord{tab[0].getLig() - 1, tab[0].getCol()+1});
        tabFourmis.ajoute(Coord{tab[0].getLig() - 1, tab[0].getCol()+2});
        // ligne 2
        tabFourmis.ajoute(Coord{tab[0].getLig(), tab[0].getCol()-1});
        tabFourmis.ajoute(Coord{tab[0].getLig(), tab[0].getCol()+2});
        // ligne 3
        tabFourmis.ajoute(Coord{tab[0].getLig()+1, tab[0].getCol()-1});
        tabFourmis.ajoute(Coord{tab[0].getLig()+1, tab[0].getCol()+2});
        // ligne 4
        tabFourmis.ajoute(Coord{tab[0].getLig() +2, tab[0].getCol()-1});
        tabFourmis.ajoute(Coord{tab[0].getLig() +2, tab[0].getCol()});
        tabFourmis.ajoute(Coord{tab[0].getLig() +2, tab[0].getCol()+1});
        tabFourmis.ajoute(Coord{tab[0].getLig() +2, tab[0].getCol()+2});
    }
    return tabFourmis;
}




void apparitionFourmi(Colonie &col, vector<Fourmi> &tabF, Grille &g){
    int ind = 0;
    for(Fourmi f : tabF){
        if (f.getIdCol() == col.getId()){
            ind++;
            }
    }
    Coord spawn{col.getNid().getTab()[0].getLig() - 1, col.getNid().getTab()[0].getCol() - 1};
    Place pSpawn = g.chargePlace(spawn);
    if (pSpawn.estVide()){
        Fourmi f{ind+100*col.getId(), spawn, col.getId()};
        col.ajouteFourmi(f);
        pSpawn.poseFourmi(ind+100*col.getId());
        tabF.push_back(f);
    }
    g.rangePlace(pSpawn);
    return;
}




void sim(){
    EnsCoord s{{Coord{25,25}, Coord{42,10}, Coord{16,16}}};
    
    EnsCoord nid0{{Coord{12,12}, Coord{12,13}, Coord{13,13}, Coord{13,12}}}; 
    EnsCoord f0 = fourmiAutourNid(nid0);
    Colonie col0{f0, nid0, 0}; //En haut à gauche
    
    vector<Fourmi>tabF = col0.getTabFourmi();
    vector<Colonie> vCol = {{col0}};
    
    if (nbColonies >= 2) {
        EnsCoord nid1{{Coord{36,36}, Coord{36,37}, Coord{37,36}, Coord{37,37}}};
        EnsCoord f1 = fourmiAutourNid(nid1);
        Colonie col1{f1, nid1, 1}; //En bas à droite
        
        tabF = tabF + col1.getTabFourmi();
        vCol.push_back(col1); 
    }
    
    if (nbColonies >= 3){
        EnsCoord nid2{{Coord{12,36}, Coord{13,37}, Coord{12,37}, Coord{13,36}}}; 
        EnsCoord f2 = fourmiAutourNid(nid2);
        Colonie col2{f2, nid2, 2}; //En haut à droite
        
        tabF = tabF + col2.getTabFourmi(); 
        vCol.push_back(col2);
    }
    
    if (nbColonies >= 4){
        EnsCoord nid3{{Coord{36,12}, Coord{36,13}, Coord{37,12}, Coord{37,13}}};
        EnsCoord f3 = fourmiAutourNid(nid3);
        Colonie col3{f3, nid3, 3}; //En bas à gauche

        tabF = tabF + col3.getTabFourmi();
        vCol.push_back(col3);
    }



    Grille g = initialiseGrille(s, vCol);
    dessinerDamier(g, tabF);

    for (int i = 1; i < 100; i++) {
        if (i % APPASUCRE == 0) apparitionSucre(g);
        
        for (Colonie c : vCol){
            if (c.getQtSucre() >= 10){
                apparitionFourmi(c, tabF, g);
                c.retireSucre(10);
                vCol[c.getId()] = c;
            }
        }

        
        for (Fourmi fourmi: tabF){
            if (fourmi.estVivante()){
                EnsCoord vc = voisins(fourmi.getCoord());
                Colonie colF = vCol[fourmi.getIdCol()];
                if (not regle1(fourmi, vc, g, tabF)){
                    if (not regle2(fourmi, vc, g)){
                        if (not regle3(fourmi, vc, g, colF)){
                            if (not regle4(fourmi, vc, g)){
                                if (not regle5(fourmi, vc, g)){
                                    if (not regle6(fourmi, vc, g)){
                                        if (not regle7(fourmi, vc, g)){ 
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                vCol[colF.getId()] = colF;
                int idFT = trouveFourmiId(tabF, fourmi.getId());
                tabF[idFT] = fourmi;
                integrite(tabF, g);
                
            }
        }
        dessinerDamier(g, tabF);
        g.diminuePheroSucre();
    }
}




int main(int argc, const char **argv){
    srand(time(NULL));
    doctest::Context context(argc, argv);
    int test_result = context.run();
    if (context.shouldExit()) return test_result;
    sim();
}
