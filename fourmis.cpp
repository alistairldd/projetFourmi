#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <array>
using namespace std;

#include "doctest.h"
#include "coord.hpp"
#include "fourmis.hpp"

// ****************SETTER*****************

void Fourmi::setAlive(bool vie){
    alive = vie;
}

// ***************GETTERS***************
int Fourmi::getId() const{
    return indice;
}


Coord Fourmi::getCoord() const{
    return coord;
}

int Fourmi::getIdCol() const{
    return idCol;
}

// ***************METHODES**************

bool Fourmi::chercheSucre() const{
    return not sucre;
}

bool Fourmi::porteSucre() const{
    return sucre;
}


bool Fourmi::estVivante() const{
    return alive;
}


void Fourmi::poseSucre(){
    sucre = false;
}


void Fourmi::prendSucre(){
    sucre = true;
}


void Fourmi::deplace(Coord c){

    EnsCoord v = voisins((*this).getCoord());
    if (not v.contient(c)) throw runtime_error("Coordonnées invalides");  //Verifie qu'elle se déplace dans une case voisine
    coord = c;
}


// FONCTIONS


vector<Fourmi> creeTabFourmis(EnsCoord e, int idCol){
    vector<Fourmi> res;
    vector<Coord> tab = e.getTab();
    for (int i = 0; i < int(tab.size()); i++){
        res.push_back(Fourmi{100 * idCol + i, tab[i], idCol});
    }
    return res;
}

Fourmi trouveFourmi(vector<Fourmi> tabF, int idFourmi){
    for (int i = 0; i < int(tabF.size()); i++) {
        if (tabF[i].getId() == idFourmi) return tabF[i];
    }
    throw runtime_error("Pas de fourmi avec cet id dans le tableau fourmi");
}

int trouveFourmiId(vector<Fourmi> tabF, int idFourmi){
    for (int i = 0; i < int(tabF.size()); i++) {
        if (tabF[i].getId() == idFourmi) return i;
    }
    throw runtime_error("Trouve pas ce fourmi dans le tableau");
}


// Surcharge ==

bool operator==(Fourmi a, Fourmi b){
    return (a.getId() == b.getId() and a.getCoord() == b.getCoord() and a.porteSucre() == b.porteSucre() and a.estVivante() == b.estVivante() and a.getIdCol() == b.getIdCol());
}

// Surcharge !=

bool operator!=(Fourmi a, Fourmi b){
    return not(a == b);
}

vector<Fourmi> operator+(vector<Fourmi> tab1, vector<Fourmi> tab2){
    for (Fourmi f:tab2){
        tab1.push_back(f);
    }
    return tab1;
}


//  ************TEST CASE************
TEST_CASE("Fourmi"){
    Fourmi a{1, Coord{3,3},0};
    Fourmi b{2, Coord{49,49},0};
    SUBCASE("getId, getCoord, porteSucre, estVivante"){
        CHECK(a.getId() == 1);
        CHECK(a.getCoord() == Coord{3,3});
        CHECK(a.porteSucre() == false);
        CHECK(a.estVivante() == true);
        }
    
    
    SUBCASE("poseSucre, prendSucre"){
        a.prendSucre();
        CHECK(a.porteSucre() == true);
        a.poseSucre();
        CHECK(a.porteSucre() == false);
        a.poseSucre();
        CHECK(a.porteSucre() == false);
        }
    
    
    SUBCASE("deplace"){
        a.deplace(Coord{3,4});
        CHECK(a.getCoord() == Coord{3,4});
        a.deplace(Coord{4,4});
        CHECK(a.getCoord() == Coord{4,4});
        CHECK_THROWS_WITH_AS(a.deplace(Coord{10,10}), "Coordonnées invalides", runtime_error);
        CHECK_THROWS_WITH_AS(b.deplace(Coord{TAILLEGRILLE-1,TAILLEGRILLE}), "Coordonnées invalides", runtime_error);
    }
    
            
    SUBCASE("creeTabFourmis"){
        EnsCoord pos{{Coord{0,0}, Coord{1,1}, Coord{2,1}, Coord{3,5}}};
        vector<Fourmi> f = creeTabFourmis(pos, 0);
        CHECK( f[0] == Fourmi{0, Coord{0,0},0});
        CHECK( f[1] == Fourmi{1, Coord{1,1},0});
        CHECK( f[2] == Fourmi{2, Coord{2,1},0});
        CHECK( f[3] == Fourmi{3, Coord{3,5},0});

    }

}
    
