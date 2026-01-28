#include <iostream>     // pour cout
#include <iomanip>      // pour setfill, setw
#include <sstream>      // pour ostringstream
#include <fstream>      // pour ofstream
#include <string>

#include "coord.hpp"
#include "fourmis.hpp"
#include "colonie.hpp"
#include "place.hpp"
#include "affichage.hpp"


using namespace std;
// variable globale permettant de creer des noms de fichiers differents
int compteurFichier = 0;
// action dessinant un damier

void dessinerDamier(Grille grille, vector<Fourmi> tabF){
    int i,j;
    int r = 0,g = 0,b = 0;
    vector<vector<Place>> tabG = grille.getGrille();
    ostringstream filename;
    // creation d'un nouveau nom de fichier de la forme img347.ppm
    filename << "img" << setfill('0') << setw(3) << compteurFichier << ".ppm";
    compteurFichier++;
    cout << "Ecriture dans le fichier : " << filename.str() << endl;
    // ouverture du fichier
    ofstream fic(filename.str(), ios::out | ios::trunc);
    // ecriture de l'entete
    fic << "P3" << endl
        << TAILLEGRILLE << " " << TAILLEGRILLE << " " << endl
        << 255 << " " << endl;
    // ecriture des pixels
    for (i = 0; i < TAILLEGRILLE; i++){
        for (j = 0; j < TAILLEGRILLE; j++){
            // calcul de la couleur
            
            if (tabG[i][j].contientNid()) {
                vector<bool> tabNid = tabG[i][j].getElemNid();
                if (nbColonies == 1) {r = 255 ; g = 0 ; b = 0;}
                else if(nbColonies == 2){
                    if (tabNid[0]){ r = 255 ; g = 0 ; b = 0;}
                    else{ r = 0 ; g = 255 ; b = 0;}
                } else if(nbColonies == 3){
                    if (tabNid[0]){ r = 255 ; g = 0 ; b = 0;} 
                    else if (tabNid[1]){ r = 0 ; g = 255 ; b = 0;}
                    else {r = 0 ; g = 0 ; b = 255;}
                } else {
                    if (tabNid[0]){ r = 255 ; g = 0 ; b = 0;} 
                    else if (tabNid[1]){ r = 0 ; g = 255 ; b = 0;}
                    else if (tabNid[2]){ r = 0 ; g = 0 ; b = 255;}
                    else {r = 255 ; g = 255 ; b = 0;}
                }
            }
            
            
            else if (tabG[i][j].contientFourmi()) {
                int idF = tabG[i][j].getNumeroFourmi();
                int idCol = trouveFourmi(tabF, idF).getIdCol();
                if (nbColonies == 1) {r = 255 ; g = 0 ; b = 0;}
                else if(nbColonies == 2){
                    if (idCol == 0){ r = 255 ; g = 0 ; b = 0;}
                    else{ r = 0 ; g = 255 ; b = 0;}
                } else if(nbColonies == 3){
                    if (idCol == 0){ r = 255 ; g = 0 ; b = 0;} 
                    else if (idCol == 1){ r = 0 ; g = 255 ; b = 0;}
                    else {r = 0 ; g = 0 ; b = 255;}
                } else {
                    if (idCol == 0){ r = 255 ; g = 0 ; b = 0;} 
                    else if (idCol == 1){ r = 0 ; g = 255 ; b = 0;}
                    else if (idCol == 2){ r = 0 ; g = 0 ; b = 255;}
                    else { r = 255 ; g = 255 ; b = 0;}
                }
            }
            
            
            else if (tabG[i][j].estVide()) {
                r = 0; g = 0 ; b = 0;
                vector<int> qt = tabG[i][j].getPheroSucre(); 
                if (nbColonies == 1) {r = max(0, qt[0] * 5) ; g = 0 ; b = 0;}
                else if(nbColonies == 2){
                    if (qt[0] > 0) r += max(0, min(200, qt[0] * 5));
                    if (qt[1] > 0) g += max(0, min(200, qt[1] * 5));
                } else if(nbColonies == 3){
                    if (qt[0] > 0) r += max(0, min(200, qt[0] * 5));
                    if (qt[1] > 0) g += max(0, min(200, qt[1] * 5));
                    if (qt[2] > 0) b += max(0, min(200, qt[2] * 5));
                } else {
                    if (qt[0] > 0) r += max(0, min(200, qt[0] * 5));
                    if (qt[1] > 0) g += max(0, min(200, qt[1] * 5));
                    if (qt[2] > 0) b += max(0, min(200, qt[2] * 5));
                    if (qt[3] > 0){
                        r += max(0, min(200, qt[3] * 5));
                        g += max(0, min(200, qt[3] * 5));
                    }
                }

            }
            
            else if (tabG[i][j].contientSucre()) { 
                r = 255; g = 255; b = 255; // case blanche
            }
            
            


           
        // ecriture de la couleur dans le fichier
        fic << r << " " << g << " " << b << "    ";
      }
    // fin de ligne dans l'image
    fic << endl;
  }
  // fermeture du fichier
  fic.close();
}

