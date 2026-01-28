#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <array>

#include "doctest.h"
#include "place.hpp"
#include "fourmis.hpp"
#include "coord.hpp"
#include "colonie.hpp"
using namespace std;


//*************CONSTRUCTEUR*******************
Place::Place(Coord c) : coord{c}, intensiteSucre{{}}, sucre{0}, intensiteNid{{}}, fourmi{-1}, elemNid{{}} {
    for (int i = 0; i < nbColonies; i++){
        elemNid.push_back(false);
        intensiteNid.push_back(0);
        intensiteSucre.push_back(0);
    }
}





//****************GETTERS PLACE**************

Coord Place::getCoord() const{
    return coord;
}


vector<int> Place::getPheroSucre() const{
    return intensiteSucre;
}

    
vector<float> Place::getPheroNid() const{
    return intensiteNid;
}


int Place::getNumeroFourmi() const{
    return fourmi;
}

int Place::getQuantiteSucre() const{
    return sucre;
}

vector<bool> Place::getElemNid() const{
    return elemNid;
}



//************METHODES PLACE***************


bool Place::contientFourmi() const{
    return fourmi != -1;
}


bool Place::contientSucre() const{
    return sucre > 0;
}

bool Place::contientNid(int colonie) const{
    return elemNid[colonie];
}

bool Place::contientNid() const{
    for (int i = 0; i < int(elemNid.size()); i++){
        if (elemNid[i]) return true;
    }
    return false;
}


bool Place::estVide() const{
    for(int i = 0; i < nbColonies; i++){
        if((*this).contientNid(i)) return false;
    }
    return not ((*this).contientFourmi() or (*this).contientSucre());
}



bool Place::estSurUnePiste(int colonie) const{
    return intensiteSucre[colonie] > 0;
}


void Place::poseSucre(){
    sucre += 1;
}


void Place::enleveSucre(){
    sucre -= 1 ;
}


void Place::poseNid(int colonie){
    elemNid[colonie] = true;
    intensiteNid[colonie] = 1;
}



void Place::poseFourmi(int id){
    fourmi = id;
}


void Place::enleveFourmi(){
    fourmi = -1;
}


void Place::posePheroNid(float quantite, int colonie){
    intensiteNid[colonie] = quantite;
}


void Place::posePheroSucre(int colonie){
    intensiteSucre[colonie] += 40;
}


void Place::diminuePheroSucre(int colonie){
    intensiteSucre[colonie] = max(intensiteSucre[colonie] - 2, 0);
}


bool Place::estPlusProcheNid(Place p2, int colonie) const{
    return (*this).getPheroNid()[colonie] > p2.getPheroNid()[colonie];
}



//*************FONCTIONS************
void deplaceFourmi(Fourmi &f, Place &d, Place &a){
    if (not d.contientFourmi()) throw runtime_error("Pas de fourmi dans la place de départ");
    if (d.getNumeroFourmi() != f.getId()) throw runtime_error("Fourmi pas dans la place de départ");
    if (a.contientFourmi()) throw runtime_error("Il y a déjà une fourmi dans la place d'arrivée");
    if (not voisins(d.getCoord()).contient(a.getCoord())) throw runtime_error("Les deux places ne sont pas à côtés");
    
    d.enleveFourmi();
    a.poseFourmi(f.getId());
    f.deplace(a.getCoord());
}

bool operator==(vector<int> t1, vector<int> t2){
    if(t1.size() != t2.size()) return false;
    for(int i = 0; i < int(t1.size()); i++){
        if(t1[i] != t2[i]) return false;
    }
    return true;
}

bool operator==(vector<float> t1, vector<float> t2){
    if(t1.size() != t2.size()) return false;
    for(int i = 0; i < int(t1.size()); i++){
        if(t1[i] != t2[i]) return false;
    }
    return true;
}




bool operator==(Place p1, Place p2){
    return p1.getCoord() == p2.getCoord() and p1.getPheroSucre() == p2.getPheroSucre() and p1.contientSucre() == p2.contientSucre() 
        and p1.getPheroNid() == p2.getPheroNid() and p1.getNumeroFourmi() == p2.getNumeroFourmi() and p1.contientNid(0) == p2.contientNid(0); // A RESOUDRE 
}

bool operator!=(Place p1, Place p2){
    return not (p1 == p2);
}



//****************CONSTRUCTEUR GRILLE**************

Grille::Grille() : grille{TAILLEGRILLE, vector<Place>(TAILLEGRILLE, Coord{0,0})} {
    for (int i = 0; i < TAILLEGRILLE; i++) {
        for (int j = 0; j < TAILLEGRILLE; j++) {
            grille[i][j] = Place{{i,j}} ;
        }
    }
}


//************METHODES GRILLE******************
vector<vector<Place>> Grille::getGrille(){
    return grille;
}


Place Grille::chargePlace(Coord c){
    return grille[c.getLig()][c.getCol()];
}

void Grille::rangePlace(Place p){
    Coord c = p.getCoord();
    grille[c.getLig()][c.getCol()] = p;
}


void Grille::diminuePheroSucre(){
    for (int i = 0; i < TAILLEGRILLE ; i++){
        for(int j = 0; j < TAILLEGRILLE; j++){
            for(int t =0; t < nbColonies; t++){
                grille[i][j].diminuePheroSucre(t);
            }
        }
    }
}

    


//************FONCTIONS******************

void placeNid(Grille &g, EnsCoord e, int colonie){
    vector<Coord> tab = e.getTab();
    for (Coord c : tab) {
        Place p = g.chargePlace(c);
        p.poseNid(colonie);
        g.rangePlace(p);
    }
}


void placeSucre(Grille &g, EnsCoord e){
    vector<Coord> tab = e.getTab();
    for (Coord c : tab) {
        Place p = g.chargePlace(c);
        p.poseSucre();
        p.poseSucre();
        p.poseSucre();
        g.rangePlace(p);
    }
}
        

void placeFourmis(Grille &g, vector<Fourmi> tab){
    for (Fourmi f : tab){
        Coord c = f.getCoord();
        Place p = g.chargePlace(c);
        p.poseFourmi(f.getId());
        g.rangePlace(p);
    }
}


Grille initialiseGrille(EnsCoord s, vector<Colonie> colonie){
    Grille g{};
    for(int i = 0; i < int(colonie.size()); i++){
        placeNid(g, colonie[i].getNid(), i);
        placeFourmis(g, colonie[i].getTabFourmi());
        lineariserPheroNid(g, i);
    }
    placeSucre(g, s);
    return g;
}



void lineariserPheroNid(Grille &g, int colonie){
    for (int k = 0; k < TAILLEGRILLE; k++){
        for (int i = 0; i < TAILLEGRILLE; i++){
            for (int j = 0; j < TAILLEGRILLE ; j++){
                Coord c{i, j};
                float m = g.chargePlace(c).getPheroNid()[colonie];
                if (m < 1){
                    EnsCoord v = voisins(c);
                    for (Coord cv : v.getTab()) {
                        m = max(m, g.chargePlace(cv).getPheroNid()[colonie]);
                    }
                    Place p = g.chargePlace(c);

                    p.posePheroNid(round((m - 1./TAILLEGRILLE)*100.0) /100.0, colonie);
                    g.rangePlace(p);
                }
            }
        }
    }
}


void deplaceFourmiGrille(Grille &g, Fourmi &f, Coord a){
    Place p1 = g.chargePlace(f.getCoord());
    Place p2 = g.chargePlace(a);
    deplaceFourmi(f, p1, p2);
    g.rangePlace(p1);
    g.rangePlace(p2);
}

                             

void integrite(vector<Fourmi> tab, Grille g){
    
    for (int i = 0; i < int(tab.size()); i ++){
        Fourmi f = tab[i];
        if (f.estVivante()) {
            //Cohérence fourmi-grille
            Coord c = f.getCoord();
            Place p = g.chargePlace(c);
            if (p.getNumeroFourmi() != f.getId()){
                cout << p.getCoord() <<  " Id de la fourmi dans la place " << p.getNumeroFourmi() << " Où croit être la fourmi : " << tab[i].getCoord() << endl;
                throw runtime_error("Cohérence fourmi-grille");
            }
        }
    }
    
    vector<vector<Place>> tabG = g.getGrille();
    for (int i = 0; i < TAILLEGRILLE; i++) {
        for (int j = 0; j < TAILLEGRILLE; j++){
            //Coordonnées des places 
            if (tabG[i][j].getCoord() != Coord{i,j}) throw runtime_error("Coordonnées des places");
            
            //Cohérence grille-fourmi
            if (tabG[i][j].contientFourmi()){
                int idf = tabG[i][j].getNumeroFourmi();
                Fourmi f = trouveFourmi(tab, idf);
                if (f.estVivante() and f.getCoord() != Coord{i,j}) throw runtime_error("Cohérence grille-fourmi");
            }
            
        }
    }
}
    
    
    
    

//*******TEST CASE**************



TEST_CASE("Place"){
    Place p{Coord{3,3}};
    SUBCASE("Getters"){
        CHECK(p.getCoord() == Coord{3,3});
        CHECK(p.getPheroSucre()[0] == 0);
        CHECK(p.getQuantiteSucre() == 0);
        CHECK(p.getPheroNid()[0] == 0);
        CHECK(p.getNumeroFourmi() == -1);
    }
    SUBCASE("contient"){
        CHECK_FALSE(p.contientFourmi());
        CHECK_FALSE(p.contientNid(0));
        CHECK_FALSE(p.contientSucre());
    }
    SUBCASE("estSurUnePiste, posePheroSucre, diminuePheroSucre"){
        CHECK_FALSE(p.estSurUnePiste(0));
        p.posePheroSucre(0);
        CHECK(p.estSurUnePiste(0));
        p.diminuePheroSucre(0);
        CHECK(p.estSurUnePiste(0));
        p.diminuePheroSucre(0);
        CHECK(p.estSurUnePiste(0));
    }
    SUBCASE("poseSucre, enleveSucre"){
        p.poseSucre();
        CHECK(p.contientSucre());
        p.enleveSucre();
        CHECK_FALSE(p.contientSucre());
    }
    SUBCASE("poseFourmi, enleveFourmi"){
        Fourmi a{0, Coord{3,3}, 0};
        p.poseFourmi(a.getId());
        CHECK(p.getNumeroFourmi() == 0);
        p.enleveFourmi();
        CHECK(p.getNumeroFourmi() == -1);
    }
    SUBCASE("posePheroNid"){
        p.posePheroNid(10,0);
        CHECK(p.getPheroNid()[0] == 10);
    }
    
    SUBCASE("deplaceFourmi"){
        Place p1{Coord{3,3}};
        Place p2{Coord{3,4}};
        Place p3{Coord{4,4}};
        Place p4{Coord{10,10}};
        Fourmi f1{0, Coord{3,3},0};
        Fourmi f2{1, Coord{4,4},0};
        p1.poseFourmi(f1.getId());
        p3.poseFourmi(f2.getId());
        CHECK_THROWS_WITH_AS(deplaceFourmi(f1, p1, p4), "Les deux places ne sont pas à côtés", runtime_error);
        CHECK_THROWS_WITH_AS(deplaceFourmi(f1, p2, p3), "Pas de fourmi dans la place de départ", runtime_error);
        CHECK_THROWS_WITH_AS(deplaceFourmi(f1, p3, p4), "Fourmi pas dans la place de départ", runtime_error);
        deplaceFourmi(f1, p1, p2);
        CHECK(f1.getCoord() == Coord{3,4});
        CHECK_FALSE(p1.contientFourmi());
        CHECK(p2.getNumeroFourmi() == 0);
        CHECK_THROWS_WITH_AS(deplaceFourmi(f1, p2, p3), "Il y a déjà une fourmi dans la place d'arrivée", runtime_error);
    }
    
    SUBCASE("estPlusProcheNid"){
        Place p1{Coord{3,3}};
        Place p2{Coord{3,4}};
        Place p4{Coord{10,10}};
        p2.posePheroNid(0.8,0);
        p4.posePheroNid(0.5,0);
        Place p5{Coord{19,19}};
        p1.poseNid(0);
        CHECK(p2.estPlusProcheNid(p4,0));
        CHECK_FALSE(p5.estPlusProcheNid(p4,0));
        CHECK(p1.estPlusProcheNid(p2,0));
    }
}



TEST_CASE("Grille"){
    SUBCASE("chargePlace"){
        Grille g{};
        CHECK( g.chargePlace(Coord{1,2}) == Place{Coord{1,2}} );
        CHECK_FALSE( g.chargePlace(Coord{1,2}) == Place{Coord{2,1}} );
    }
    
    SUBCASE("rangePlace"){
        Grille g{};
        Place p = g.chargePlace(Coord{1,2});
        p.poseSucre();
        g.rangePlace(p);
        CHECK(g.chargePlace(Coord{1,2}).contientSucre());
    }
    
    SUBCASE("placeNid"){
        Grille g{};
        EnsCoord e{{Coord{3,3}, Coord{3,4}, Coord{4,3}, Coord{4,4}}};
        placeNid(g, e, 0);
        Place p1 = g.chargePlace(Coord{3,3});
        Place p2 = g.chargePlace(Coord{3,4});
        Place p3 = g.chargePlace(Coord{4,3});
        Place p4 = g.chargePlace(Coord{4,4});
        Place p5 = g.chargePlace(Coord{4,5});
        CHECK(p1.contientNid(0));
        CHECK(p2.contientNid(0));
        CHECK(p3.contientNid(0));
        CHECK(p4.contientNid(0));
        CHECK_FALSE(p5.contientNid(0));
    }
    
    SUBCASE("placeSucre"){
        Grille g{};
        EnsCoord e{{Coord{5,5}, Coord{7,7}, Coord{10,3}, Coord{9,4}}};
        placeSucre(g, e);
        Place p1 = g.chargePlace(Coord{5,5});
        Place p2 = g.chargePlace(Coord{7,7});
        Place p3 = g.chargePlace(Coord{10,3});
        Place p4 = g.chargePlace(Coord{9,4});
        Place p5 = g.chargePlace(Coord{4,5});
        CHECK(p1.contientSucre());
        CHECK(p2.contientSucre());
        CHECK(p3.contientSucre());
        CHECK(p4.contientSucre());
        CHECK_FALSE(p5.contientSucre());
    }
    
    SUBCASE("placeFourmis"){
        Grille g{};
        EnsCoord e{{Coord{5,5}, Coord{7,7}, Coord{10,3}, Coord{9,4}}};
        Colonie col {e, e, 0};
        vector<Fourmi> tab = creeTabFourmis(e, 0);
        placeFourmis(g, tab);
        Place p1 = g.chargePlace(Coord{5,5});
        Place p2 = g.chargePlace(Coord{7,7});
        Place p3 = g.chargePlace(Coord{10,3});
        Place p4 = g.chargePlace(Coord{9,4});
        Place p5 = g.chargePlace(Coord{4,5});
        CHECK(p1.contientFourmi());
        CHECK(p2.contientFourmi());
        CHECK(p3.contientFourmi());
        CHECK(p4.contientFourmi());
        CHECK_FALSE(p5.contientFourmi());
    }
    
    
    SUBCASE("diminuePheroSucre"){
        Grille g{};
        Place p = g.chargePlace(Coord{1,1});
        p.posePheroSucre(0);
        p.diminuePheroSucre(0);
        g.rangePlace(p);
        CHECK(g.chargePlace(Coord{1,1}).estSurUnePiste(0));
        p.diminuePheroSucre(0);
        g.rangePlace(p);
        CHECK(g.chargePlace(Coord{1,1}).estSurUnePiste(0));
    }
    
    SUBCASE("lineariserPheroNid"){
        Grille g{};
        EnsCoord e{{Coord{3,3}, Coord{3,4}, Coord{4,3}, Coord{4,4}}};
        placeNid(g, e, 0);
        lineariserPheroNid(g,0);
        CHECK(g.chargePlace(Coord{2,2}).getPheroNid()[0] == float(0.98));
        CHECK(g.chargePlace(Coord{2,3}).getPheroNid()[0] == float(0.98));
        CHECK(g.chargePlace(Coord{1,1}).getPheroNid()[0] == float(0.96));
        CHECK(g.chargePlace(Coord{19,19}).getPheroNid()[0] == float(0.7));

    }
    
}


void afficheGrille(Grille g){
    for (int i = 0; i < TAILLEGRILLE; i++){
        for (int j = 0; j < TAILLEGRILLE; j++){
            cout << g.getGrille()[i][j].getCoord() << endl;
        }
    }
}

