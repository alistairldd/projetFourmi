#ifndef PLACE_HPP
#define PLACE_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <array>

#include "doctest.h"
#include "coord.hpp"
#include "fourmis.hpp"
#include "colonie.hpp"
using namespace std;


class Place{

    Coord coord;
    vector<int> intensiteSucre;
    int sucre;
    vector<float> intensiteNid; // vecteur intensité : indice = indice colonie
    int fourmi;
    vector<bool> elemNid; // vecteur de booléens, un indice = une colonnie

    public :
        // CONSTRUCTEUR
        Place(Coord c);
        
        // GETTERS
        Coord getCoord() const;
        vector<int> getPheroSucre() const;
        vector<float> getPheroNid() const;
        int getNumeroFourmi() const;
        int getQuantiteSucre() const;
        vector<bool> getElemNid() const;
    
        // METHODES
        bool contientFourmi() const; // true si contient une fourmi
        bool contientSucre() const; // true si la place contient du sucre
        bool contientNid() const; // true si la place contient un elem du nid
        bool contientNid(int colonie) const;
        bool estVide() const; // renvoie true si la place est vide
        bool estSurUnePiste(int colonie) const; // true si l'intensite des pheromones de sucre d'une colonie donnee est non nulle
        void poseSucre();
        void enleveSucre();
        void poseNid(int colonie);
        void poseFourmi(int id);
        void enleveFourmi();
        void posePheroNid(float quantite, int colonie); 
        void posePheroSucre(int colonie); // +10 par défaut
        void diminuePheroSucre(int colonie); //-5 par défaut
        bool estPlusProcheNid(Place p1, int colonie) const;

};

void deplaceFourmi(Fourmi &f, Place &d, Place &a);


class Grille{
    vector<vector<Place>> grille;
    
    
    public :
        
        //Constructeur
        Grille();
    
        //GETTERS
        vector<vector<Place>> getGrille(); 
        Place chargePlace(Coord c);
        void rangePlace(Place);
        

        void diminuePheroSucre();

};
        
        
bool operator==(Place p1, Place p2);
bool operator!=(Place p1, Place p2);

void afficheGrille(Grille g);
void placeNid(Grille &g, EnsCoord e, int colonie);
void placeSucre(Grille &g, EnsCoord e);
void placeFourmis(Grille &g, vector<Fourmi> tab);
Grille initialiseGrille(EnsCoord s, vector<Colonie> colonie);
void lineariserPheroNid(Grille &g, int Numcolonie);
void deplaceFourmiGrille(Grille &g, Fourmi &f, Coord a);
void integrite(vector<Fourmi> tab, Grille g);

#endif