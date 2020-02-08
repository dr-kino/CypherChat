#include "cypher_lib.h"

int main(int argc, char *argv[]) { 
	char bloco_entrada[101];
  	char bloco_saida[101];
  	char chave[9];
  	char vetor_inicial[9];
  	int modo;

  	strcpy(bloco_entrada,"!!cypher lib test!!");

  	strcpy(chave,"12345678");
  	strcpy(vetor_inicial,"initvect");
  
  	// Encrypt 
  	modo = 0;
  	DES_CBC(modo,bloco_entrada,chave,vetor_inicial,bloco_saida);
   
  	printf("Encrypting\n");
  	printf("Input Block (128 bits ou 16 bytes)...........: %s\n",bloco_entrada);
  	printf("Cypher Key (64 bits ou 8 bytes)..............: %s\n",chave);
  	printf("Output Block (128 bits ou 16 bytes)..........: %s\n",bloco_saida);

  	// Decrypt
  	modo = 1;
	strcpy(vetor_inicial,"initvect");
  	DES_CBC(modo,"zǝfM@pwc0n&<",chave,vetor_inicial,bloco_entrada);
	//DES_CBC(modo,bloco_saida,chave,vetor_inicial,bloco_entrada);

  	printf("\nDecripting\n");
  	printf("Input Block (128 bits ou 16 bytes)...........: %s\n",bloco_saida);
  	printf("Cypher Key (64 bits ou 8 bytes)..............: %s\n",chave);
  	printf("Output Block (128 bits ou 16 bytes)..........: %s\n",bloco_entrada);

  	return 0;
}