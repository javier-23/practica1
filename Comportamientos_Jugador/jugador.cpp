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
		a = (a+5)%8;
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

	
	if(sensores.reset){
		tieneBikini = false;
		tieneZapatillas=false;
		bien_situado=false;
		current_state.fil= current_state.col = tamanio;
		current_state.brujula = norte;
		VaciarMatriz(mapaCiego); //Vaciar la matriz grande para luego volver a hacer la copia
		descubriendo = permiso_paso =false;
	}

	bool no_pases = false;
	for(int i=1; i<sensores.terreno.size(); i++){
		if(sensores.terreno[2] == 'M' || sensores.terreno[1] == 'M' || sensores.terreno[3] == 'M'){
			no_pases=true;
		}
	}

//Mover a la casilla que me interesa
	if(!prioridad_accion && !no_pases){ //Si no he hecho anteriormente un movimiento con prioridad
		for(int i=1; i<sensores.terreno.size() && !encontrada_casilla; i++){ //Recorre la visión del jugador
			//Si hay alguna casilla que me interesa
			if( (sensores.terreno[i] == 'G' && !bien_situado) || (sensores.terreno[i] == 'X' && sensores.bateria<4000) || (sensores.terreno[i] == 'K' && !tieneBikini) ||
			    (sensores.terreno[i] == 'D' && !tieneZapatillas)){ 																			
				
				prioridad_accion =true;
				if(i==1 || i ==4 || i==5 || i==9 || i==10 || i==11){
					accion = actTURN_SL;
					if(sensores.terreno[i] == 'K' || sensores.terreno[i] == 'D')
						permiso_paso = true;
				}
				else if(i==2 || i ==6 || i==12){
					if((sensores.terreno[2] == 'B' && !tieneZapatillas) || (sensores.terreno[2] == 'A' && !tieneBikini)){
						accion = hacer_giro();
					}
					else{
						if(sensores.superficie[2] != 'a' && sensores.superficie[2] != 'l')
							accion = actFORWARD;
						else
							accion = hacer_giro();
					}
				}
				else{
					accion = actTURN_SR;
					if((sensores.terreno[i] == 'K' || sensores.terreno[i] == 'D'))
						permiso_paso = true;
				}
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
		fil_ciega=current_state.fil; //Fila de la casilla de posicionamiento de la matriz ciega
		col_ciega = current_state.col; //Columna de la casilla de posicionamiento de la matriz ciega
		current_state.fil = sensores.posF;
		current_state.col = sensores.posC;
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
		if(sensores.bateria<4400){
			accion = actIDLE;
			prioridad_accion=true;
		}
		if(sensores.bateria==4400 || (sensores.vida<1000 && sensores.bateria>=1500))
			prioridad_accion=false;
	}

	if(!bien_situado){
		PonerTerrenoEnMatriz(sensores.terreno, current_state, mapaCiego);
		permiso_paso = ComprobarPermiso(sensores.terreno, current_state);  //Comprobar si esta rodeado de agua o de bosque
	}

	if(bien_situado){
		PonerTerrenoEnMatriz(sensores.terreno, current_state, mapaResultado);
		permiso_paso = ComprobarPermiso(sensores.terreno, current_state); //Comprobar si esta rodeado de agua o de bosque
		matrizPasos[current_state.fil][current_state.col]++;
	}


	if(((sensores.terreno[3] == 'M' && sensores.terreno[7] != 'M' && sensores.terreno[13] == 'M') || (sensores.terreno[1] == 'M' && sensores.terreno[5]!='M' && sensores.terreno[11] == 'M')) 
		&& sensores.terreno[2] !='M'){ //Si esta paralelo a un muro con un hueco
		prioridad_muro = true;
	}

	bool continua=false;
	if(((sensores.terreno[3] == 'M' && sensores.terreno[7] == 'M' && sensores.terreno[13] == 'M') || (sensores.terreno[1] == 'M' && sensores.terreno[5]=='M' && sensores.terreno[11] == 'M')) 
		&& sensores.terreno[2] !='M'){
		continua=true;
	}

	//Para que vaya descubriendo mapa
	if(!prioridad_accion && bien_situado && tieneBikini && tieneZapatillas){
		//Action auxiliar;
		if(!descubriendo && matrizPasos[current_state.fil][current_state.col]==3 && sensores.bateria>2800){
			descubriendo = true;
			accion = hacer_giro();
		}
		else{
			descubriendo=false;
		}
	}
	else if(!prioridad_accion && bien_situado && (tieneBikini || tieneZapatillas)){
		if(!descubriendo && matrizPasos[current_state.fil][current_state.col]==4 && sensores.bateria>2800){
			descubriendo = true;
			accion = hacer_giro();
		}
		else{
			descubriendo=false;
		}
	}


	// Decidir la nueva accion
	if(!prioridad_accion && !descubriendo){
		if(sensores.superficie[2] != 'a' && sensores.superficie[2] != 'l'){
			if((sensores.terreno[1] == 'M' || sensores.terreno[3] == 'M') && sensores.terreno[6] == 'P'){
				if(sensores.terreno[1] == 'M')
					if(rand()%2==0)
						accion=actTURN_BR;
					else
						accion=actTURN_SR;
				else
					 if(rand()%2==0)
						accion=actTURN_BL;
					else
						accion=actTURN_SL;
			}
			else if ((sensores.terreno[2] == 'T' || sensores.terreno[2] =='S' || sensores.terreno[2] == 'G' ||
	     		sensores.terreno[2] == 'X' || sensores.terreno[2] == 'D' || sensores.terreno[2] == 'K')){
					accion = actFORWARD;
					permiso_paso = false;
			}
			else if((sensores.terreno[2] == 'A' && tieneBikini) || (sensores.terreno[2] == 'A' && permiso_paso)){
					accion = actFORWARD;
			}
			else if((sensores.terreno[2] == 'B' && tieneZapatillas) || (sensores.terreno[2] == 'B' && permiso_paso)){
				accion = actFORWARD;
			}
			else if(ComprobacionMuro(sensores.terreno)){
				if(aleatoriedad_muro)
					accion = actTURN_SR;
				else
					accion = actTURN_SL;
			}
			else{
				accion = hacer_giro();
			}
		}
		else if(sensores.superficie[2] == 'a' || sensores.superficie[2] == 'l'){ //Si en la siguiente casilla hay un aldeano
			accion = hacer_giro();
		}
	}

	if(salir){ //Para que en el siguiete movimiento salga (apunte al hueco y en el siguiente movimiento vaya alli)
		if(sensores.terreno[3] != 'M' && sensores.terreno[7]=='M'){
			accion = actTURN_SR;
		}
		else{
			accion = actTURN_SL;
		}
		salir = false;
		prioridad_muro = false;
		aleatoriedad_muro = rand()%2;
	}

	if(prioridad_muro)
		salir = true;

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
	for(int i=0; i<destino.size(); i++){
		for(int j=0; j< destino.size(); j++){
			if(destino[i][j]== '?' && origen[i+(fil_ciega-current_state.fil)][j+(col_ciega-current_state.col)] != '?') // Si en el mapaResueltado no esta pintado que pinte del mapaCiego
				destino[i][j] = origen[i+(fil_ciega-current_state.fil)][j+(col_ciega-current_state.col)];
		}
	}
}

void ComportamientoJugador::VaciarMatriz(vector< vector< unsigned char> > &matriz){
	for (int i = 0; i< matriz.size();i++){
		for (int j = 0; j < matriz.size(); j++){
			matriz[i][j] = '?';
		}
	}
	
}

Action ComportamientoJugador::hacer_giro(){
	Action aux;
	girar = rand()%4;
	switch (girar)
	{
	case 0:
		aux = actTURN_SL;
		break;
	case 1:
		aux = actTURN_SR;
		break;
	case 2:
		aux = actTURN_BL;
		break;
	case 3:
		aux = actTURN_BL;
		break;
	}
	return aux;
}

bool ComportamientoJugador::ComprobarPermiso(const vector<unsigned char> &terreno,const state &st){ //Funcion para saber si estoy encerrado por agua, bosque y/o precipicios
	
	bool saliendo=false;
	if(!cerrojo){
		pos_x = st.fil; pos_y = st.col;
		cerrojo=true;
	}

	switch (st.brujula)
	{
	case norte:
		if((terreno[2] =='B' || terreno[2] =='A' || terreno[2] == 'P' ) && !asegurado[0]){ 
			if(st.fil==pos_x && st.col==pos_y){
				contador_permiso++;
				asegurado[0] = true;
			}
			else{
				saliendo=true;
			}
		}
		break;
	case noreste:
		if((terreno[2] =='B' || terreno[2] =='A' || terreno[2] == 'P' )&& !asegurado[1]){
			if(st.fil==pos_x && st.col==pos_y){
				contador_permiso++;
				asegurado[1] = true;
			}
			else{
				saliendo=true;
			}
		}
		break;
	case este:
		if((terreno[2] =='B' || terreno[2] =='A' || terreno[2] == 'P' ) && !asegurado[2]){
			if(st.fil==pos_x && st.col==pos_y){
				contador_permiso++;
				asegurado[2] = true;
			}
			else{
				saliendo=true;
			}
		}
		break;
	case sureste:
		if((terreno[2] =='B' || terreno[2] =='A' || terreno[2] == 'P' ) && !asegurado[3]){
			if(st.fil==pos_x && st.col==pos_y){
				contador_permiso++;
				asegurado[3] = true;
			}
			else{
				saliendo=true;
			}
		}
		break;
	case sur:
		if((terreno[2] =='B' || terreno[2] =='A' || terreno[2] == 'P' ) && !asegurado[4]){
			if(st.fil==pos_x && st.col==pos_y){
				contador_permiso++;
				asegurado[4] = true;
			}
			else{
				saliendo=true;
			}
		}
		break;
	case suroeste:
		if((terreno[2] =='B' || terreno[2] =='A' || terreno[2] == 'P' ) && !asegurado[5]){
			if(st.fil==pos_x && st.col==pos_y){
				contador_permiso++;
				asegurado[5] = true;
			}
			else{
				saliendo=true;
			}
		}
		break;
	case oeste:
		if((terreno[2] =='B' || terreno[2] =='A' || terreno[2] == 'P' )&& !asegurado[6]){
			if(st.fil==pos_x && st.col==pos_y){
				contador_permiso++;
				asegurado[6] = true;
			}
			else{
				saliendo=true;
			}
		}
		break;
	case noroeste:
		if((terreno[2] =='B' || terreno[2] =='A' || terreno[2] == 'P' ) && !asegurado[7]){
			if(st.fil==pos_x && st.col==pos_y){
				contador_permiso++;
				asegurado[7] = true;
			}
			else{
				saliendo=true;
			}
		}
		break;
	}
	if(saliendo){
		contador_permiso=0;
		vector<bool> copia(8,false);
		asegurado = copia;
		cerrojo=false;
	}
	if(contador_permiso == 8){
		contador_permiso=0;
		vector<bool> copia(8,false);
		asegurado = copia;
		cerrojo=false;
		return true;
	}
	return false;
}

 bool ComportamientoJugador::ComprobacionMuro(const vector<unsigned char> &terreno){
	bool hay_muro = false;
	if(terreno[1] == 'M' && terreno[2] == 'M' && terreno[3]=='M'){
		hay_muro = true;
	}else if(terreno[1]!= 'M' && terreno[2] == 'M' && terreno[3]=='M'){
		hay_muro=true;
	}else if(terreno[1]=='M' && terreno[2] == 'M' && terreno[3]!='M'){
		hay_muro=true;
	}else if(terreno[2] == 'M' && terreno[1] != 'M' && terreno[3] != 'M'){
		hay_muro=true;
	}
	return hay_muro;
 }