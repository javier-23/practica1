#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;



Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;
	int a;

	switch (last_action)
    {
      case actTURN_SR: //mantenerse en la misma casilla y girar 45º a la derecha
        a = current_state.brujula;
		a = (a+1)%8;
		current_state.brujula = static_cast<Orientacion>(a);
      break;
      
      case actTURN_SL:
        a = current_state.brujula;
		a = (a+7)%8;
		current_state.brujula = static_cast<Orientacion>(a);
      break;

      case actTURN_BR: //girar 135º a la derecha
        a = current_state.brujula;
		a = (a+3)%8;
		current_state.brujula = static_cast<Orientacion>(a);
      break;
    
      case actTURN_BL:
        a = current_state.brujula;
		a = (a+13)%8;
		current_state.brujula = static_cast<Orientacion>(a);
      break;  

      case actFORWARD: 

		switch (current_state.brujula)
		{
		case norte:
			current_state.fil--;
			break;
		case noreste:
			current_state.fil--;
			current_state.col++;
			break;
		case este:
			current_state.col++;
			break;
		case sureste:
			current_state.fil++;
			current_state.col++;
			break;
		case sur:
			current_state.fil++;
			break;
		case suroeste:
			current_state.fil++;
			current_state.col--;
			break;
		case oeste:
			current_state.col--;
			break;
		case noroeste:
			current_state.col--;
			current_state.fil--;
			break;	
		}
      break;
	}

	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch(sensores.sentido){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Noreste" << endl; break;
		case 2: cout << "Este" << endl; break;
		case 3: cout << "Sureste" << endl; break;
		case 4: cout << "Sur " << endl; break;
		case 5: cout << "Suroeste" << endl; break;
		case 6: cout << "Oeste" << endl; break;
		case 7: cout << "Noroeste" << endl; break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;


	if (sensores.terreno[0]=='G' and !bien_situado){ 
		current_state.fil = sensores.posF;
		current_state.col= sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
	}

	if(bien_situado){
		mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
			switch (current_state.brujula)
			{
			case norte:
				mapaResultado[current_state.fil-1][current_state.col-1]=sensores.terreno[1];
				mapaResultado[current_state.fil-1][current_state.col]=sensores.terreno[2];
				mapaResultado[current_state.fil-1][current_state.col+1]=sensores.terreno[3];
				mapaResultado[current_state.fil-2][current_state.col-2]=sensores.terreno[4];
				mapaResultado[current_state.fil-2][current_state.col-1]=sensores.terreno[5];
				mapaResultado[current_state.fil-2][current_state.col]=sensores.terreno[6];
				mapaResultado[current_state.fil-2][current_state.col+1]=sensores.terreno[7];
				mapaResultado[current_state.fil-2][current_state.col+2]=sensores.terreno[8];
				mapaResultado[current_state.fil-3][current_state.col-3]=sensores.terreno[9];
				mapaResultado[current_state.fil-3][current_state.col-2]=sensores.terreno[10];
				mapaResultado[current_state.fil-3][current_state.col-1]=sensores.terreno[11];
				mapaResultado[current_state.fil-3][current_state.col]=sensores.terreno[12];
				mapaResultado[current_state.fil-3][current_state.col+1]=sensores.terreno[13];
				mapaResultado[current_state.fil-3][current_state.col+2]=sensores.terreno[14];
				mapaResultado[current_state.fil-3][current_state.col+3]=sensores.terreno[15];
				break;
			case noreste:
				mapaResultado[current_state.fil-1][current_state.col]=sensores.terreno[1];
				mapaResultado[current_state.fil-1][current_state.col+1]=sensores.terreno[2];
				mapaResultado[current_state.fil][current_state.col+1]=sensores.terreno[3];
				mapaResultado[current_state.fil-2][current_state.col]=sensores.terreno[4];
				mapaResultado[current_state.fil-2][current_state.col+1]=sensores.terreno[5];
				mapaResultado[current_state.fil-2][current_state.col+2]=sensores.terreno[6];
				mapaResultado[current_state.fil-1][current_state.col+2]=sensores.terreno[7];
				mapaResultado[current_state.fil][current_state.col+2]=sensores.terreno[8];
				mapaResultado[current_state.fil-3][current_state.col]=sensores.terreno[9];
				mapaResultado[current_state.fil-3][current_state.col+1]=sensores.terreno[10];
				mapaResultado[current_state.fil-3][current_state.col+2]=sensores.terreno[11];
				mapaResultado[current_state.fil-3][current_state.col+3]=sensores.terreno[12];
				mapaResultado[current_state.fil-2][current_state.col+3]=sensores.terreno[13];
				mapaResultado[current_state.fil-1][current_state.col+3]=sensores.terreno[14];
				mapaResultado[current_state.fil][current_state.col+3]=sensores.terreno[15];
				break;
			case este:
				mapaResultado[current_state.fil-1][current_state.col+1]=sensores.terreno[1];
				mapaResultado[current_state.fil][current_state.col+1]=sensores.terreno[2];
				mapaResultado[current_state.fil+1][current_state.col+1]=sensores.terreno[3];
				mapaResultado[current_state.fil-2][current_state.col+2]=sensores.terreno[4];
				mapaResultado[current_state.fil-1][current_state.col+2]=sensores.terreno[5];
				mapaResultado[current_state.fil][current_state.col+2]=sensores.terreno[6];
				mapaResultado[current_state.fil+1][current_state.col+2]=sensores.terreno[7];
				mapaResultado[current_state.fil+2][current_state.col+2]=sensores.terreno[8];
				mapaResultado[current_state.fil-3][current_state.col+3]=sensores.terreno[9];
				mapaResultado[current_state.fil-2][current_state.col+3]=sensores.terreno[10];
				mapaResultado[current_state.fil-1][current_state.col+3]=sensores.terreno[11];
				mapaResultado[current_state.fil][current_state.col+3]=sensores.terreno[12];
				mapaResultado[current_state.fil+1][current_state.col+3]=sensores.terreno[13];
				mapaResultado[current_state.fil+2][current_state.col+3]=sensores.terreno[14];
				mapaResultado[current_state.fil+3][current_state.col+3]=sensores.terreno[15];
				break;
			case sureste:
				mapaResultado[current_state.fil][current_state.col+1]=sensores.terreno[1];
				mapaResultado[current_state.fil+1][current_state.col+1]=sensores.terreno[2];
				mapaResultado[current_state.fil+1][current_state.col]=sensores.terreno[3];
				mapaResultado[current_state.fil][current_state.col+2]=sensores.terreno[4];
				mapaResultado[current_state.fil+1][current_state.col+2]=sensores.terreno[5];
				mapaResultado[current_state.fil+2][current_state.col+2]=sensores.terreno[6];
				mapaResultado[current_state.fil+2][current_state.col+1]=sensores.terreno[7];
				mapaResultado[current_state.fil+2][current_state.col+2]=sensores.terreno[8];
				mapaResultado[current_state.fil][current_state.col+3]=sensores.terreno[9];
				mapaResultado[current_state.fil+1][current_state.col+3]=sensores.terreno[10];
				mapaResultado[current_state.fil+2][current_state.col+3]=sensores.terreno[11];
				mapaResultado[current_state.fil+3][current_state.col+3]=sensores.terreno[12];
				mapaResultado[current_state.fil+3][current_state.col+2]=sensores.terreno[13];
				mapaResultado[current_state.fil+3][current_state.col+1]=sensores.terreno[14];
				mapaResultado[current_state.fil+3][current_state.col]=sensores.terreno[15];
				break;
			case sur:
				mapaResultado[current_state.fil+1][current_state.col+1]=sensores.terreno[1];
				mapaResultado[current_state.fil+1][current_state.col]=sensores.terreno[2];
				mapaResultado[current_state.fil+1][current_state.col-1]=sensores.terreno[3];
				mapaResultado[current_state.fil+2][current_state.col+2]=sensores.terreno[4];
				mapaResultado[current_state.fil+2][current_state.col+1]=sensores.terreno[5];
				mapaResultado[current_state.fil+2][current_state.col]=sensores.terreno[6];
				mapaResultado[current_state.fil+2][current_state.col-1]=sensores.terreno[7];
				mapaResultado[current_state.fil+2][current_state.col-2]=sensores.terreno[8];
				mapaResultado[current_state.fil+3][current_state.col+3]=sensores.terreno[9];
				mapaResultado[current_state.fil+3][current_state.col+2]=sensores.terreno[10];
				mapaResultado[current_state.fil+3][current_state.col+1]=sensores.terreno[11];
				mapaResultado[current_state.fil+3][current_state.col]=sensores.terreno[12];
				mapaResultado[current_state.fil+3][current_state.col-1]=sensores.terreno[13];
				mapaResultado[current_state.fil+3][current_state.col-2]=sensores.terreno[14];
				mapaResultado[current_state.fil+3][current_state.col-3]=sensores.terreno[15];
				break;
			case suroeste:
				mapaResultado[current_state.fil+1][current_state.col]=sensores.terreno[1];
				mapaResultado[current_state.fil+1][current_state.col-1]=sensores.terreno[2];
				mapaResultado[current_state.fil][current_state.col-1]=sensores.terreno[3];
				mapaResultado[current_state.fil+2][current_state.col]=sensores.terreno[4];
				mapaResultado[current_state.fil+2][current_state.col-1]=sensores.terreno[5];
				mapaResultado[current_state.fil+2][current_state.col-2]=sensores.terreno[6];
				mapaResultado[current_state.fil+1][current_state.col-2]=sensores.terreno[7];
				mapaResultado[current_state.fil][current_state.col-2]=sensores.terreno[8];
				mapaResultado[current_state.fil+3][current_state.col]=sensores.terreno[9];
				mapaResultado[current_state.fil+3][current_state.col-1]=sensores.terreno[10];
				mapaResultado[current_state.fil+3][current_state.col-2]=sensores.terreno[11];
				mapaResultado[current_state.fil+3][current_state.col-2]=sensores.terreno[12];
				mapaResultado[current_state.fil+2][current_state.col-3]=sensores.terreno[13];
				mapaResultado[current_state.fil+1][current_state.col-3]=sensores.terreno[14];
				mapaResultado[current_state.fil][current_state.col-3]=sensores.terreno[15];
				break;
			case oeste:
				mapaResultado[current_state.fil+1][current_state.col-1]=sensores.terreno[1];
				mapaResultado[current_state.fil][current_state.col-1]=sensores.terreno[2];
				mapaResultado[current_state.fil-1][current_state.col-1]=sensores.terreno[3];
				mapaResultado[current_state.fil+2][current_state.col-2]=sensores.terreno[4];
				mapaResultado[current_state.fil+1][current_state.col-2]=sensores.terreno[5];
				mapaResultado[current_state.fil][current_state.col-2]=sensores.terreno[6];
				mapaResultado[current_state.fil-1][current_state.col-2]=sensores.terreno[7];
				mapaResultado[current_state.fil-2][current_state.col-2]=sensores.terreno[8];
				mapaResultado[current_state.fil+3][current_state.col-3]=sensores.terreno[9];
				mapaResultado[current_state.fil+2][current_state.col-3]=sensores.terreno[10];
				mapaResultado[current_state.fil+1][current_state.col-3]=sensores.terreno[11];
				mapaResultado[current_state.fil][current_state.col-3]=sensores.terreno[12];
				mapaResultado[current_state.fil-1][current_state.col-3]=sensores.terreno[13];
				mapaResultado[current_state.fil-2][current_state.col-3]=sensores.terreno[14];
				mapaResultado[current_state.fil-3][current_state.col-3]=sensores.terreno[15];
				break;
			case noroeste:
				mapaResultado[current_state.fil][current_state.col-1]=sensores.terreno[1];
				mapaResultado[current_state.fil-1][current_state.col-1]=sensores.terreno[2];
				mapaResultado[current_state.fil-1][current_state.col]=sensores.terreno[3];
				mapaResultado[current_state.fil][current_state.col-2]=sensores.terreno[4];
				mapaResultado[current_state.fil-1][current_state.col-2]=sensores.terreno[5];
				mapaResultado[current_state.fil-2][current_state.col-2]=sensores.terreno[6];
				mapaResultado[current_state.fil-2][current_state.col-1]=sensores.terreno[7];
				mapaResultado[current_state.fil-2][current_state.col]=sensores.terreno[8];
				mapaResultado[current_state.fil][current_state.col-3]=sensores.terreno[9];
				mapaResultado[current_state.fil-1][current_state.col-3]=sensores.terreno[10];
				mapaResultado[current_state.fil-2][current_state.col-3]=sensores.terreno[11];
				mapaResultado[current_state.fil-3][current_state.col-3]=sensores.terreno[12];
				mapaResultado[current_state.fil-3][current_state.col-2]=sensores.terreno[13];
				mapaResultado[current_state.fil-3][current_state.col-1]=sensores.terreno[14];
				mapaResultado[current_state.fil-3][current_state.col]=sensores.terreno[15];
				break;	
			}
		
	}

	// Decidir la nueva accion
	if ((sensores.terreno[2] == 'T' || sensores.terreno[2] =='S' || sensores.terreno[2] == 'G' || sensores.terreno[2]== 'A' || sensores.terreno[2] == 'K' ||
		 sensores.terreno[2] == 'B' || sensores.terreno[2] == 'D'|| sensores.terreno[2] == 'X') && sensores.superficie[2]=='_'){
		accion = actFORWARD;
	}
	else if(!girar_derecha){
		accion = actTURN_SL;
		girar_derecha = (rand()%2 == 0);
	}
	else{
		accion = actTURN_SR;
		girar_derecha = (rand()%2 == 0);
	}

	// Determinar el efecto de la ultima accion enviada
	last_action = accion;
	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

