#ifndef FOURMIS_HPP
#define FOURMIS_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <array>
#include "coord.hpp"

using namespace std;

class Fourmi{
    
    bool alive;
    Coord coord;
    bool sucre;
    int indice;
    int idCol;

    public :
    
        // CONSTRUCTEUR
        Fourmi(int indice, Coord c, int idCol) : alive{true}, coord{c}, sucre{false}, indice{indice}, idCol{idCol} {}
        
        // SETTER
        void setAlive(bool vie);
         
        // GETTERS

        int getId() const;
        Coord getCoord() const ;
        int getIdCol() const;
    
        // METHODES
        bool chercheSucre() const;
        bool porteSucre() const;
        bool estVivante() const; // si la fourmi est vivante
        void poseSucre(); // enleve une charge de sucre sur la fourmi
        void prendSucre(); // ajoute une charge de sucre sur la fourmi
        void deplace(Coord c);
        void libererFourmi();

};

vector<Fourmi> creeTabFourmis(EnsCoord e, int idCol); //Pour chaque coordonnée, crée une fourmi avec pour indice celui du vector
Fourmi trouveFourmi(vector<Fourmi> tabF, int idFourmi); //Renvoie la fourmi selon son id dans le tableau de fourmi
int trouveFourmiId(vector<Fourmi> tabF, int idFourmi); //Renvoie l'id de la fourmi dans le tableau selon l'id de la fourmi
vector<Fourmi> operator+(vector<Fourmi> tab1, vector<Fourmi> tab2);
bool operator!=(Fourmi a, Fourmi b);
bool operator==(Fourmi a, Fourmi b);


#endif
