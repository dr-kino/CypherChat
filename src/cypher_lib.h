#ifndef CYPHER_LIB_H
#define CYPHER_LIB_H

void DES_CBC ( int modo,
		       char * bloco_entrada,
		       char * chave,
               char * vetor,
		       char * bloco_saida);

#endif // CYPHER_LIB_H