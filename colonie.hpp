#ifndef COLONIE_HPP
#define COLONIE_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <array>
using namespace std;

#include "coord.hpp"
#include "fourmis.hpp"

const int nbColonies = 4; //Nombre de colonies, pouvant aller de 1 à 4

class Colonie{
    vector<Fourmi> tabF;
    EnsCoord nid;
    int id;
    int qtSucre;

    
    public:
        // CONSTRUCTEUR
        Colonie(EnsCoord f, EnsCoord nid, int ind);
    
        // GETTERS
        int getId() const;
        EnsCoord getNid() const;
        vector<Fourmi> getTabFourmi() const;
        int getQtSucre() const;
    
        // METHODES
        void ajouteSucre(int quantite);
        void retireSucre(int quantite);
        void ajouteFourmi(Fourmi f);
    
};



#endif