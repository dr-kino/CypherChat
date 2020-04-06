#include "gmock/gmock.h"
#include "../../src/cypher_lib.h"

TEST(TestCypherLib, TestCaseOne) {
	char bloco_entrada[101];
  	char bloco_saida[101];
  	char chave[9];
  	char vetor_inicial[9];
  	int modo;
    bool test = false;

  	strcpy(bloco_entrada,"!!cypher lib test!!");

  	strcpy(chave,"12345678");
  	strcpy(vetor_inicial,"initvect");
  
  	// Encrypt 
  	modo = 0;
  	DES_CBC(modo,bloco_entrada,chave,vetor_inicial,bloco_saida);
   
  	//printf("Encrypting\n");
  	//printf("Input Block (128 bits ou 16 bytes)...........: %s\n",bloco_entrada);
  	//printf("Cypher Key (64 bits ou 8 bytes)..............: %s\n",chave);
  	//printf("Output Block (128 bits ou 16 bytes)..........: %s\n",bloco_saida);

  	// Decrypt
  	modo = 1;
	strcpy(vetor_inicial,"initvect");
	DES_CBC(modo,bloco_saida,chave,vetor_inicial,bloco_entrada);

  	//printf("\nDecripting\n");
  	//printf("Input Block (128 bits ou 16 bytes)...........: %s\n",bloco_saida);
  	//printf("Cypher Key (64 bits ou 8 bytes)..............: %s\n",chave);
  	//printf("Output Block (128 bits ou 16 bytes)..........: %s\n",bloco_entrada);

    if ((bloco_entrada[0] == '!') && (bloco_entrada[1] == '!') && (bloco_entrada[2] == 'c') &&
        (bloco_entrada[3] == 'y') && (bloco_entrada[4] == 'p') && (bloco_entrada[5] == 'h') &&
        (bloco_entrada[6] == 'e') && (bloco_entrada[7] == 'r') && (bloco_entrada[8] == ' ') &&
        (bloco_entrada[9] == 'l') && (bloco_entrada[10] == 'i') && (bloco_entrada[11] == 'b') &&
        (bloco_entrada[12] == ' ') && (bloco_entrada[13] == 't') && (bloco_entrada[14] == 'e') &&
        (bloco_entrada[15] == 's') && (bloco_entrada[16] == 't') && (bloco_entrada[17] == '!') &&
        (bloco_entrada[18] == '!')) 
    {
        test = true;
    }
    else {
        test = false;
    }

    ASSERT_TRUE(test);
}