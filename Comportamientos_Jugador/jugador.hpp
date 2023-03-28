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
      current_state.fil = current_state.col = 99;
      last_action = actIDLE;
      current_state.brujula = norte;
      girar_derecha = false;
      bien_situado = false;
      tamMapa = size;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor); 

  private:
  
  // Declarar aquí las variables de estado
  Action last_action;
  state current_state; // Para recordar donde estoy y hacia donde voy a dar el siguiente paso
  bool girar_derecha;
  bool bien_situado; //Si la variable actual refleja la posicion correcta
  int tamMapa;

};

#endif
