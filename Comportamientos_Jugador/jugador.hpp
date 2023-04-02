#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

struct state{
  int fil;
  int col;
  Orientacion brujula;
};

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      current_state.fil = current_state.col = size-1;
      last_action = actIDLE;
      current_state.brujula = norte;
      girar = 0;
      bien_situado = false;
      tamMapa = size;
      tieneBikini = tieneZapatillas = false;
      vector<vector<unsigned char> >aux((size*2)-1, vector<unsigned char> ((size*2)-1,'?'));
      mapaCiego = aux;
      prioridad_accion= encontrada_casilla = false;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    void PonerTerrenoEnMatriz(const vector<unsigned char> &terreno, const state &st, vector< vector< unsigned char> > &matriz);
    void copiarMatriz(vector< vector< unsigned char> > &origen, vector< vector< unsigned char> > &destino);

    Action think(Sensores sensores);
    int interact(Action accion, int valor); 

  private:
  
  // Declarar aquí las variables de estado
  Action last_action;
  state current_state; // Para recordar donde estoy y hacia donde voy a dar el siguiente paso
  int girar;
  bool bien_situado; //Si la variable actual refleja la posicion correcta
  int tamMapa;
  bool tieneBikini, tieneZapatillas;
  vector<vector<unsigned char> >mapaCiego;
  bool prioridad_accion, encontrada_casilla;

};

#endif
