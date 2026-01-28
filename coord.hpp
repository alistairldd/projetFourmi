#ifndef COORD_HPP
#define COORD_HPP


#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <array>
using namespace std;

const int TAILLEGRILLE = 50;

class Coord{

    int lig;
    int col;

    public:
        // CONSTRUCTEUR
        Coord(int x, int y);
        
        // GETTERS
        int getLig();
        int getCol();
        
};


class EnsCoord{
    vector<Coord> tabCoord;
        
    public:
        // CONSTRUCTEUR
        EnsCoord(vector<Coord>);
        
        // GETTER
        vector<Coord> getTab();
    
        // METHODES
        int position(Coord pos) const;
        bool contient(Coord pos) const;
        void ajoute(Coord pos);
        void supprime(Coord pos);
        bool estVide() const;
        int taille() const;
        Coord ieme(int n) const; // retourne l’élément qui est à la position n dans tabCoord
        Coord choixHasard() const; // retourne une coordonnée au hasard parmi un ensemble de tabCoord
};

EnsCoord voisins(Coord c); // retourne l'ensemble des voisins de c
// SURCHARGES COORD
ostream& operator<<(ostream &out, Coord a);
bool operator==(Coord a, Coord b);
bool operator!=(Coord a, Coord b);

// SURCHARGES ENSCOORD
ostream& operator<<(ostream &out, EnsCoord c1);
bool operator==(EnsCoord a, EnsCoord b);


#endif 

