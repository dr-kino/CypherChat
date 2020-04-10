#ifndef CYPHER_LIB_H
#define CYPHER_LIB_H



#ifdef __cplusplus
extern "C" {
#endif

void DES_CBC ( int modo,
		       char * bloco_entrada,
		       char * chave,
               char * vetor,
		       char * bloco_saida);
			   
#ifdef __cplusplus
}
#endif

#endif // CYPHER_LIB_H