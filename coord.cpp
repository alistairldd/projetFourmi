#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <array>
using namespace std;

#include "doctest.h"
#include "coord.hpp"

// ****** COORD ****** //


// ***********CONSTRUCTEUR*********
Coord::Coord(int x, int y) : lig{x}, col{y} {
    if(x>TAILLEGRILLE or y > TAILLEGRILLE or x < 0 or y < 0){
        throw runtime_error("Coordonnées incorrectes");
    }
}

// *************GETTERS**********

int Coord::getLig(){
    return lig;
}

int Coord::getCol(){
       return col;
}


// Surcharge << Coord

ostream& operator<<(ostream &out, Coord a){
    out << "(" << a.getLig() << "," << a.getCol() << ")";
    return out;
}

// Surcharge == Coord

bool operator==(Coord a, Coord b){
    return (a.getLig() == b.getLig()
            and a.getCol() == b.getCol());
}

// Surcharge != Coord

bool operator!=(Coord a, Coord b){
    return not (a == b);
}



// ****** ENSCOORD  ******//

// ********CONSTRUCTEUR*********
EnsCoord::EnsCoord(vector<Coord> Vc) : tabCoord{{}} {
    for(int i = 0; i < int(Vc.size()); i++){
        tabCoord.push_back(Vc[i]);
    }
}


// ********METHODES***********
int EnsCoord::position(Coord pos) const{
    for(int i = 0; i < int(tabCoord.size()); i++){
        if (tabCoord[i] == pos){
            return i;
        }
    }
    return -1;
}


bool EnsCoord::contient(Coord pos) const{
    return (*this).position(pos) != -1;
}


void EnsCoord::ajoute(Coord pos){
    if(not (*this).contient(pos)){
        tabCoord.push_back(pos);
    }
}

void EnsCoord::supprime(Coord pos){
    if ((*this).contient(pos) == false){
        throw runtime_error("coord pas contenu dans ensemble");
    }
    
    if ((*this).estVide()){
        throw runtime_error("Ensemble vide");
    }
    Coord annexe = tabCoord[(*this).position(pos)];
    
    tabCoord[(*this).position(pos)] = tabCoord[int(tabCoord.size())-1];
    
    tabCoord[int(tabCoord.size())-1] = annexe;
        
    tabCoord.pop_back();
}


bool EnsCoord::estVide() const{
    return (int(tabCoord.size()) == 0);
}


int EnsCoord::taille() const{
    return int(tabCoord.size());
}


Coord EnsCoord::ieme(int n) const{
    if( n >= int(tabCoord.size()) or n < 0){
        throw runtime_error("n invalide");
    }
    return tabCoord[n];
}

vector<Coord> EnsCoord::getTab(){
    return tabCoord;
}


Coord EnsCoord::choixHasard() const{
    int a = rand() % tabCoord.size();
    return tabCoord[a];
}

// *******FONCTIONS********

EnsCoord voisins(Coord c){
    EnsCoord ens{{}};
    int lig = c.getLig();
    int col = c.getCol();
    for(int i = max(lig - 1, 0) ; i <= min(lig + 1, TAILLEGRILLE - 1); i++){
        for(int j = max(col - 1, 0) ; j <= min(col +1, TAILLEGRILLE - 1); j++){
            if (Coord{i,j} != c){
                ens.ajoute(Coord{i,j});
                
            }
        }
    }
    return ens;
}

// Surcharge << EnsCoord

ostream& operator<<(ostream &out, EnsCoord c1){
    for(int i = 0; i < int(c1.getTab().size()); i++){
        out << c1.getTab()[i] << " ";
    }
    out << endl;
    return out;
}

// Surcharge == EnsCoord

bool operator==(EnsCoord a, EnsCoord b){
    if (a.getTab().size() != b.getTab().size()){return false;}
    
    for (int i = 0; i < int(a.getTab().size()); i++){
        if (not b.contient(a.getTab()[i])){
            return false;
        }
    }
    return true;            
}

// TEST CASES 


// COORD




TEST_CASE("Coord"){
    SUBCASE("get lig col"){
        Coord co{1,2};
        CHECK(co.getLig() == 1);
        CHECK(co.getCol() == 2);

        Coord co1{0,0};
        CHECK(co1.getLig() == 0);
        CHECK(co1.getCol() == 0);

        Coord co2{0,2};
        CHECK(co2.getLig() == 0);
        CHECK(co2.getCol() == 2);

        Coord co3{1,1};
        CHECK(co3.getLig() == 1);
        CHECK(co3.getCol() == 1);
    }
    SUBCASE("operator =="){
    CHECK(Coord{1,2} == Coord{1,2});
    CHECK(Coord{0,0} == Coord{0,0});
    CHECK(Coord{3,1} == Coord{3,1});
    CHECK(Coord{0,1} == Coord{0,1});
    CHECK_FALSE(Coord{1,2}  == Coord{2,3});
    CHECK_FALSE(Coord{1,2}  == Coord{2,3});
    CHECK_FALSE(Coord{1,2}  == Coord{2,3});
    CHECK_FALSE(Coord{1,2}  == Coord{2,3}); 
    
    }
    SUBCASE("operator !="){
    CHECK(Coord{1,2}  != Coord{0,0});
    CHECK(Coord{2,2}  != Coord{0,0});
    CHECK(Coord{4,1}  != Coord{3,1});
    CHECK(Coord{0,1}  != Coord{1,0});
    CHECK(Coord{1,0}  != Coord{0,0});
    CHECK(Coord{0,1}  != Coord{0,0});
    CHECK(Coord{1,1}  != Coord{0,0});
    CHECK(Coord{1,0}  != Coord{0,1});
    CHECK_FALSE(Coord{1,2}  != Coord{1,2});
    }
}

// ENSCOORD

TEST_CASE(" EnsCoord "){
    SUBCASE("operator =="){
    CHECK(EnsCoord{{Coord{1,1},Coord{2,2},Coord{1,0}}} 
          == EnsCoord{{Coord{1,0},Coord{2,2},Coord{1,1}}});
    CHECK(EnsCoord{{Coord{1,1},Coord{2,2}}} 
          == EnsCoord{{Coord{2,2},Coord{1,1}}});
    CHECK(EnsCoord{{Coord{1,1},Coord{2,2}}} 
      == EnsCoord{{Coord{2,2},Coord{1,1}}});
    CHECK(EnsCoord{{Coord{0,0},Coord{10,10}}} 
          == EnsCoord{{Coord{0,0},Coord{10,10}}});
    }
    SUBCASE("position"){
        EnsCoord pos{{Coord{0,0}, Coord{1,1}, Coord{2,1}, Coord{3,5}}};
        CHECK(pos.position(Coord{1,1}) == 1);
        CHECK(pos.position(Coord{2,1}) == 2);
        CHECK(pos.position(Coord{0,0}) == 0);
        CHECK(pos.position(Coord{3,5}) == 3);
        CHECK(pos.position(Coord{4,5}) == -1);
        CHECK(pos.position(Coord{4,7}) == -1);
    }
    SUBCASE("contient"){
        EnsCoord pos{{Coord{0,0}, Coord{1,1}, Coord{2,1}, Coord{3,5}}};
        CHECK(pos.contient(Coord{1,1}));
        CHECK(pos.contient(Coord{2,1}));
        CHECK(pos.contient(Coord{0,0}));
        CHECK(pos.contient(Coord{3,5}));
        CHECK_FALSE(pos.contient(Coord{4,5}));
        CHECK_FALSE(pos.contient(Coord{4,7}));
    }
    SUBCASE("ajoute"){
        EnsCoord pos{{Coord{1,1}}};
        CHECK(pos.contient(Coord{1,1}));
        CHECK_FALSE(pos.contient(Coord{2,1}));
        pos.ajoute(Coord{2,1});
        CHECK(pos.contient(Coord{2,1}));
        CHECK_FALSE(pos.contient(Coord{0,0}));
        pos.ajoute(Coord{0,0});
        CHECK(pos.contient(Coord{0,0}));
    }
    SUBCASE("supprime"){
        EnsCoord pos{{Coord{0,0}, Coord{1,1}, Coord{2,1}, Coord{3,5}}};
        CHECK(pos.contient(Coord{1,1}));
        pos.supprime(Coord{1,1});
        CHECK_FALSE(pos.contient(Coord{1,1}));
        CHECK(pos.contient(Coord{2,1}));
        pos.supprime(Coord{2,1});
        CHECK_FALSE(pos.contient(Coord{2,1}));
        pos.supprime(Coord{0,0});
        CHECK_FALSE(pos.contient(Coord{0,0}));
        pos.ajoute(Coord{0,0});
        CHECK(pos.contient(Coord{0,0}));
    }
    SUBCASE("estVide"){
        EnsCoord pos{{Coord{0,0}, Coord{1,1}}};
        CHECK_FALSE(pos.estVide());
        pos.supprime(Coord{0,0});
        CHECK_FALSE(pos.estVide());
        pos.supprime(Coord{1,1});
        CHECK(pos.estVide());
    }
    SUBCASE("taille"){
        EnsCoord pos{{Coord{1,1}}};
        CHECK(pos.taille() == 1);
        pos.ajoute(Coord{0,0});
        CHECK(pos.taille() == 2);
        pos.ajoute(Coord{1,0});
        CHECK(pos.taille() == 3);
        pos.supprime(Coord{0,0});
        CHECK(pos.taille() == 2);
    }
    SUBCASE("ieme"){
        EnsCoord pos{{Coord{0,0}, Coord{1,1}, Coord{2,1}, Coord{3,5}}};
        CHECK(pos.ieme(2) == Coord{2,1});
        pos.supprime(Coord{2,1});
        CHECK(pos.ieme(2) == Coord{3,5});
        CHECK(pos.ieme(0) == Coord{0,0});
    }
    SUBCASE("choix hasard"){
        EnsCoord ens{{Coord{2,3}, Coord{2,4}, Coord{2,5}, Coord{3,3}, Coord{3,5}, Coord{4,3}, Coord{4,4},Coord{4,5}}};
        for(int i = 0; i < 5;i++){
            CHECK(ens.contient(ens.choixHasard()));
        }
    }
    SUBCASE("voisins"){
        CHECK(voisins(Coord{0,0}) == EnsCoord{{Coord{0,1}, Coord{1,0}, Coord{1,1}}});
        CHECK(voisins(Coord{3,4}) == EnsCoord{{Coord{2,3}, Coord{2,4}, Coord{2,5}, Coord{3,3}, Coord{3,5}, Coord{4,3}, Coord{4,4},Coord{4,5}}});
        CHECK(voisins(Coord{11,12}) == EnsCoord{{Coord{10,11}, Coord{10,12}, Coord{10,13}, Coord{11,11}, Coord{11,13}, Coord{12,11}, Coord{12,12},Coord{12,13}}});
        CHECK(voisins(Coord{0,19}) == EnsCoord{{Coord{0,18}, Coord{0,20}, Coord{1,18}, Coord{1,19},Coord{1,20}}});
        CHECK(voisins(Coord{10,0}) == EnsCoord{{Coord{9,0}, Coord{9,1}, Coord{10,1}, Coord{11,0}, Coord{11,1}}});
        CHECK(voisins(Coord{19,19}) == EnsCoord{{Coord{18,18}, Coord{18,19}, Coord{18,20}, Coord{19,18},Coord{19,20}, Coord{20,18}, Coord{20,19}, Coord{20,20}}});

    }
}

