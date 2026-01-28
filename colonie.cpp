#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <array>
using namespace std;

#include "coord.hpp"
#include "fourmis.hpp"
#include "colonie.hpp"
#include "doctest.h"

// ****************CONSTRUCTEUR********************
Colonie::Colonie(EnsCoord f, EnsCoord nid, int ind) : tabF{creeTabFourmis(f, ind)}, nid{nid}, id{ind}, qtSucre{0} {}


// *****************GETTERS**********************
int Colonie::getId() const {
    return id;
}


EnsCoord Colonie::getNid() const{
    return nid;
}


vector<Fourmi> Colonie::getTabFourmi() const {
    return tabF;
}

int Colonie::getQtSucre() const {
    return qtSucre;
}


// *********************METHODES**********************

void Colonie::ajouteSucre(int qte){
    qtSucre += qte;
}

void Colonie::retireSucre(int quantite){
    qtSucre -= quantite;
}

void Colonie::ajouteFourmi(Fourmi f){
    tabF.push_back(f);
}




