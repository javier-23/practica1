#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);//Describe el comportamiento del agente
    int interact(Action accion, int valor); //Como interacciona este agente con otros (irrelevante para esta práctica)

  private:
  
  // Declarar aquí las variables de estado
  Action last_action;
  Orientacion brujula;

};

#endif
