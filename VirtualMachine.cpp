/*

Máquina Virtual

Tamanho do set de instruções: 32 bits

Códigos das instruções:

	ADD 0110 1110 -> "110d"
	SUB 0111 1000 -> "120d"
	LOAD 1000 0010 -> "130d"
	STORE 1000 1100 -> "140d"

Modelo das instruções de LOAD e STORE:

	(End. Mem.Dados)(End. Registrador)(Tipo de instrução)
	
	    16 bits           8 bits            8 bits
	    
     Exemplo:
     0b00000000000000010000000110000010 -> 0000000000000001 | 00000001 | 10000010
     Realiza o LOAD(10000010) do endereço de memória 0000000000000001 para o registrador 00000001
     
Modelo das instruções de ADD e SUB:

	(End. Registrador Destino)(End. Registrador A)(End. Registrador B)(Tipo de instrução)
	
			  8 bits				 8 bits				 8 bits			    8 bits

	 Exemplo:
	 0b000000011000000010000001001101110 -> 00000011 | 00000001 | 00000010 | 01101110
	 Realiza o ADD(01101110) do registrador A(00000001) com o registrador B(00000010) e armazena o resultado no registrador destino(00000011)
*/

#include <iostream>

using namespace std;

unsigned int memoriaDeDados[] = {1, 2, 0, 4, 8, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
unsigned int memoriaDePrograma[] = {0b00000000000000000000000010000010,
                                    0b00000000000000010000000110000010,
                                    0b00000010000000000000000101101110,
                                    0b00000000000000100000001010001100}; //Carrega os dois primeiros valores da memória de dados, soma e armazena na
                                                                         //terceira posição.
	
//Registradores
unsigned int PC;
unsigned int instrucao;
unsigned int tipoInstrucao;
unsigned int registradorA;
unsigned int registradorB;
unsigned int registradorDestino;
unsigned int registradorOrigem;
unsigned int posicaoMemoriaDados;
unsigned int registrador[256];

void decodificar(){
	
	tipoInstrucao = instrucao & 0b00000000000000000000000011111111;
	
	if(tipoInstrucao == 110 || tipoInstrucao == 120){
		//Somar, Subtrair
		registradorA = instrucao & 0b00000000111111110000000000000000;
		registradorA = registradorA >> 16;
		registradorB = instrucao & 0b00000000000000001111111100000000;
		registradorB = registradorB >> 8;
		registradorDestino = instrucao & 0b11111111000000000000000000000000;
		registradorDestino = registradorDestino >> 24;
	}
	else if(tipoInstrucao == 130){
		//Load
		registradorDestino = instrucao & 0b00000000000000001111111100000000;
		registradorDestino = registradorDestino >> 8;
		posicaoMemoriaDados = instrucao & 0b11111111111111110000000000000000;
		posicaoMemoriaDados = posicaoMemoriaDados >> 16;
	}
	else if(tipoInstrucao == 140){
		//Store
		registradorOrigem = instrucao & 0b00000000000000001111111100000000;
		registradorOrigem = registradorOrigem >> 8;
		posicaoMemoriaDados = instrucao & 0b11111111111111110000000000000000;
		posicaoMemoriaDados = posicaoMemoriaDados >> 16;
	}
}

void executar(){
	
	if(tipoInstrucao == 110){
		//Soma
		registrador[registradorDestino] = registrador[registradorA] + registrador[registradorB]; 
	}
	if(tipoInstrucao == 120){
		//Subtração
		registrador[registradorDestino] = registrador[registradorA] - registrador[registradorB];
	}
	if(tipoInstrucao == 130){
		//Load
		registrador[registradorDestino] = memoriaDeDados[posicaoMemoriaDados];
	}
	if(tipoInstrucao == 140){
		//Store
		memoriaDeDados[posicaoMemoriaDados] = registrador[registradorOrigem];
	}	
}

int main(){
	
	PC = 0;
	
	for (unsigned int i = 0; i < 256; i++){
		registrador[i] = 0;
	}
	
	while(PC < 4){
		instrucao = memoriaDePrograma[PC];
		PC = PC + 1;
		decodificar();
		executar();
	}
	
	cout << memoriaDeDados[2] << endl;
	
	return 0;	
}