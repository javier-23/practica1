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

	/*
	cout << mapaCiego.size() << endl;
	for(int i=0; i<mapaCiego.size(); i++){
		for(int j=0; j<mapaCiego.size(); j++){
			cout << mapaCiego[i][j] << " ";
		}
		cout << endl;
	}*/

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;

//Mover a la casilla que me interesa
if(!prioridad_accion){ //Si no he hecho anteriormente un movimiento con prioridad
	for(int i=1; i<sensores.terreno.size() && !encontrada_casilla; i++){
		if( sensores.terreno[i] == 'G' || sensores.terreno[i] == 'X' || sensores.terreno[i] == 'K' || sensores.terreno[i] == 'D'){
			prioridad_accion =true;
			if(i==1 || i ==4 || i==5 || i==9 || i==10 || i==11)
				accion = actTURN_SL;
			else if(i==2 || i ==6 || i==12)
				accion = actFORWARD;
			else
				accion = actTURN_SR;
			encontrada_casilla=true;
		}
		else
			prioridad_accion = false;
	}	
	encontrada_casilla=false;
}else{
	prioridad_accion=false;
}

	if ( (sensores.terreno[0]=='G' and !bien_situado) || sensores.nivel == 0){ 
		current_state.fil = sensores.posF;
		current_state.col= sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
		if(sensores.nivel !=0){ //Pintar lo descubierto previamente a caer en la casilla de posicionamiento
			copiarMatriz(mapaCiego,mapaResultado);
		}
	}

	if(sensores.terreno[0] == 'D'){
		tieneZapatillas=true;
		prioridad_accion=false;
	}

	if(sensores.terreno[0] == 'K'){
		tieneBikini=true;
		prioridad_accion=false;
	}

	if(sensores.terreno[0] == 'X'){
		if(sensores.bateria<4600){
			accion = actIDLE;
			prioridad_accion=true;
		}
		if(sensores.bateria==4600)
			prioridad_accion=false;
	}

	if(!bien_situado){
		PonerTerrenoEnMatriz(sensores.terreno, current_state, mapaCiego);
	}

	if(bien_situado){
		PonerTerrenoEnMatriz(sensores.terreno, current_state, mapaResultado);
	}

	// Decidir la nueva accion
	if(!prioridad_accion){
		if ((sensores.terreno[2] == 'T' || sensores.terreno[2] =='S' || sensores.terreno[2] == 'G' ||
	     	sensores.terreno[2] == 'X' || sensores.terreno[2] == 'D' || sensores.terreno[2] == 'K') && sensores.superficie[2]=='_'){
				accion = actFORWARD;
		}
		else if(sensores.terreno[2] == 'A' && tieneBikini){
				accion = actFORWARD;
		}
		else if(sensores.terreno[2] == 'B' && tieneZapatillas){
			accion = actFORWARD;
		}
	/*else if(sensores.terreno[2] == 'M'){
		if(!girar_derecha){
			accion = actTURN_SL;
			girar_derecha = (rand()%2 == 0);
		}
		else{
			accion = actTURN_SR;
			girar_derecha = (rand()%2 == 0);
		}
	}*/
		else if(girar == 0){
			accion = actTURN_SL;
			girar = rand()%4;
		}
		else if(girar == 1){
			accion = actTURN_SR;
			girar = rand()%4;
		}
		else if(girar==2){
			accion = actTURN_BL;
			girar = rand()%4;
		}
		else if(girar==3){
			accion = actTURN_BR;
			girar = rand()%4;
		}
	}

	// Determinar el efecto de la ultima accion enviada
	last_action = accion;
	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

void ComportamientoJugador::PonerTerrenoEnMatriz(const vector<unsigned char> &terreno, const state &st, vector< vector< unsigned char> > &matriz){
	matriz[st.fil][st.col] = terreno[0];
			switch (st.brujula){
			case norte:
				matriz[st.fil-1][st.col-1]=terreno[1];
				matriz[st.fil-1][st.col]=terreno[2];
				matriz[st.fil-1][st.col+1]=terreno[3];
				matriz[st.fil-2][st.col-2]=terreno[4];
				matriz[st.fil-2][st.col-1]=terreno[5];
				matriz[st.fil-2][st.col]=terreno[6];
				matriz[st.fil-2][st.col+1]=terreno[7];
				matriz[st.fil-2][st.col+2]=terreno[8];
				matriz[st.fil-3][st.col-3]=terreno[9];
				matriz[st.fil-3][st.col-2]=terreno[10];
				matriz[st.fil-3][st.col-1]=terreno[11];
				matriz[st.fil-3][st.col]=terreno[12];
				matriz[st.fil-3][st.col+1]=terreno[13];
				matriz[st.fil-3][st.col+2]=terreno[14];
				matriz[st.fil-3][st.col+3]=terreno[15];
				break;
			case noreste:
				matriz[st.fil-1][st.col]=terreno[1];
				matriz[st.fil-1][st.col+1]=terreno[2];
				matriz[st.fil][st.col+1]=terreno[3];
				matriz[st.fil-2][st.col]=terreno[4];
				matriz[st.fil-2][st.col+1]=terreno[5];
				matriz[st.fil-2][st.col+2]=terreno[6];
				matriz[st.fil-1][st.col+2]=terreno[7];
				matriz[st.fil][st.col+2]=terreno[8];
				matriz[st.fil-3][st.col]=terreno[9];
				matriz[st.fil-3][st.col+1]=terreno[10];
				matriz[st.fil-3][st.col+2]=terreno[11];
				matriz[st.fil-3][st.col+3]=terreno[12];
				matriz[st.fil-2][st.col+3]=terreno[13];
				matriz[st.fil-1][st.col+3]=terreno[14];
				matriz[st.fil][st.col+3]=terreno[15];
				break;
			case este:
				matriz[st.fil-1][st.col+1]=terreno[1];
				matriz[st.fil][st.col+1]=terreno[2];
				matriz[st.fil+1][st.col+1]=terreno[3];
				matriz[st.fil-2][st.col+2]=terreno[4];
				matriz[st.fil-1][st.col+2]=terreno[5];
				matriz[st.fil][st.col+2]=terreno[6];
				matriz[st.fil+1][st.col+2]=terreno[7];
				matriz[st.fil+2][st.col+2]=terreno[8];
				matriz[st.fil-3][st.col+3]=terreno[9];
				matriz[st.fil-2][st.col+3]=terreno[10];
				matriz[st.fil-1][st.col+3]=terreno[11];
				matriz[st.fil][st.col+3]=terreno[12];
				matriz[st.fil+1][st.col+3]=terreno[13];
				matriz[st.fil+2][st.col+3]=terreno[14];
				matriz[st.fil+3][st.col+3]=terreno[15];
				break;
			case sureste:
				matriz[st.fil][st.col+1]=terreno[1];
				matriz[st.fil+1][st.col+1]=terreno[2];
				matriz[st.fil+1][st.col]=terreno[3];
				matriz[st.fil][st.col+2]=terreno[4];
				matriz[st.fil+1][st.col+2]=terreno[5];
				matriz[st.fil+2][st.col+2]=terreno[6];
				matriz[st.fil+2][st.col+1]=terreno[7];
				matriz[st.fil+2][st.col]=terreno[8];
				matriz[st.fil][st.col+3]=terreno[9];
				matriz[st.fil+1][st.col+3]=terreno[10];
				matriz[st.fil+2][st.col+3]=terreno[11];
				matriz[st.fil+3][st.col+3]=terreno[12];
				matriz[st.fil+3][st.col+2]=terreno[13];
				matriz[st.fil+3][st.col+1]=terreno[14];
				matriz[st.fil+3][st.col]=terreno[15];
				break;
			case sur:
				matriz[st.fil+1][st.col+1]=terreno[1];
				matriz[st.fil+1][st.col]=terreno[2];
				matriz[st.fil+1][st.col-1]=terreno[3];
				matriz[st.fil+2][st.col+2]=terreno[4];
				matriz[st.fil+2][st.col+1]=terreno[5];
				matriz[st.fil+2][st.col]=terreno[6];
				matriz[st.fil+2][st.col-1]=terreno[7];
				matriz[st.fil+2][st.col-2]=terreno[8];
				matriz[st.fil+3][st.col+3]=terreno[9];
				matriz[st.fil+3][st.col+2]=terreno[10];
				matriz[st.fil+3][st.col+1]=terreno[11];
				matriz[st.fil+3][st.col]=terreno[12];
				matriz[st.fil+3][st.col-1]=terreno[13];
				matriz[st.fil+3][st.col-2]=terreno[14];
				matriz[st.fil+3][st.col-3]=terreno[15];
				break;
			case suroeste:
				matriz[st.fil+1][st.col]=terreno[1];
				matriz[st.fil+1][st.col-1]=terreno[2];
				matriz[st.fil][st.col-1]=terreno[3];
				matriz[st.fil+2][st.col]=terreno[4];
				matriz[st.fil+2][st.col-1]=terreno[5];
				matriz[st.fil+2][st.col-2]=terreno[6];
				matriz[st.fil+1][st.col-2]=terreno[7];
				matriz[st.fil][st.col-2]=terreno[8];
				matriz[st.fil+3][st.col]=terreno[9];
				matriz[st.fil+3][st.col-1]=terreno[10];
				matriz[st.fil+3][st.col-2]=terreno[11];
				matriz[st.fil+3][st.col-3]=terreno[12];
				matriz[st.fil+2][st.col-3]=terreno[13];
				matriz[st.fil+1][st.col-3]=terreno[14];
				matriz[st.fil][st.col-3]=terreno[15];
				break;
			case oeste:
				matriz[st.fil+1][st.col-1]=terreno[1];
				matriz[st.fil][st.col-1]=terreno[2];
				matriz[st.fil-1][st.col-1]=terreno[3];
				matriz[st.fil+2][st.col-2]=terreno[4];
				matriz[st.fil+1][st.col-2]=terreno[5];
				matriz[st.fil][st.col-2]=terreno[6];
				matriz[st.fil-1][st.col-2]=terreno[7];
				matriz[st.fil-2][st.col-2]=terreno[8];
				matriz[st.fil+3][st.col-3]=terreno[9];
				matriz[st.fil+2][st.col-3]=terreno[10];
				matriz[st.fil+1][st.col-3]=terreno[11];
				matriz[st.fil][st.col-3]=terreno[12];
				matriz[st.fil-1][st.col-3]=terreno[13];
				matriz[st.fil-2][st.col-3]=terreno[14];
				matriz[st.fil-3][st.col-3]=terreno[15];
				break;
			case noroeste:
				matriz[st.fil][st.col-1]=terreno[1];
				matriz[st.fil-1][st.col-1]=terreno[2];
				matriz[st.fil-1][st.col]=terreno[3];
				matriz[st.fil][st.col-2]=terreno[4];
				matriz[st.fil-1][st.col-2]=terreno[5];
				matriz[st.fil-2][st.col-2]=terreno[6];
				matriz[st.fil-2][st.col-1]=terreno[7];
				matriz[st.fil-2][st.col]=terreno[8];
				matriz[st.fil][st.col-3]=terreno[9];
				matriz[st.fil-1][st.col-3]=terreno[10];
				matriz[st.fil-2][st.col-3]=terreno[11];
				matriz[st.fil-3][st.col-3]=terreno[12];
				matriz[st.fil-3][st.col-2]=terreno[13];
				matriz[st.fil-3][st.col-1]=terreno[14];
				matriz[st.fil-3][st.col]=terreno[15];
				break;	
			}
}

void ComportamientoJugador::copiarMatriz(vector< vector< unsigned char> > &origen, vector< vector< unsigned char> > &destino){
	for(int i=0; i<destino.size();i++){
		for(int j=0; j<destino.size();j++){
			destino[i][j] = origen[i][j];
		}
	}
}
