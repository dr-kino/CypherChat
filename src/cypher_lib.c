#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#define DATA_LENGTH     8
#define KEY_LENGTH      8
#define L_LENGTH        4
#define R_LENGTH        4
#define K_LENGTH        6
#define f_LENGTH        4
#define C_D_LENGTH      7
#define PC_2_LENGTH     6*8
#define E_LENGTH        6*8
#define P_LENGTH        4*8
#define IP_LENGTH       8*8
#define PC_1_LENGTH     7*8
#define mask_1          0xfc;
#define mask_2          0x03;
#define mask_3          0xf0;
#define mask_4          0x0f;
#define mask_5          0xc0;
#define mask_6          0x3f;
#define mask_7          0x1e;
#define mask_8          0x20;
#define mask_9          0x01;
#define ON              1
#define OFF             0
#define DES_BLOCO       8  

void desnbs(int codop, unsigned char *inp_c, unsigned char * chave, unsigned char *scratch_buffer);
static void bits (unsigned char *INP,int INP_LEN,unsigned char *OUT,int OUT_LEN, unsigned char *TABLE,int TABLE_LENGTH);
static void shift_left_bits (unsigned char *INP,int INP_LENGTH,int LOOP_NO, unsigned char *SHIFT_LEFT_TABLE);
static  void Xoriza ( char *orig1, char *orig2_dst, int tam_origem );
static void SUBSTRING ( char *str,char *dst, int inic, int fim);
void INSERIR ( char *str_dst,char *str_org,int inic_dst,int inic_org,int fim);
static  void descompacta_decimaliza( unsigned char *origem, char *destino, int tamanho_da_origem );

void SUBSTRING ( char *str,char *dst, int inic, int fim) {
    int i,k=0;

    for (i=inic-1; i < fim+inic-1; i++)
        *(dst+k++)= *(str+i);
	
    dst[k]='\0';
}

void desnbs(int codop, unsigned char *inp_c, unsigned char * chave, unsigned char *scratch_buffer) {
/*int codop;        0 - cifra; 1 - decifra
unsigned char *inp_c;            /* Dado (8 bytes)
unsigned char *chave;            /* Chave ( 8bytes)
unsigned char *scratch_buffer;   /* Resultado (8 bytes) */

    /*** TABELA PARA A PERMUTACAO INICIAL "IP" *********************/
    static unsigned char
    IP_fips[] =  {58,50,42,34,26,18,10,02,60,52,44,36,28,20,12,04,
		62,54,46,38,30,22,14,06,64,56,48,40,32,24,16, 8,
		57,49,41,33,25,17, 9,01,59,51,43,35,27,19,11,03,
		61,53,45,37,29,21,13,05,63,55,47,39,31,23,15,07};


    /*** TABELA PARA A PERMUTACAO INVERSA "IP-1" *******************/
    static unsigned char
    IP_1_fips[] ={40, 8,48,16,56,24,64,32,39,07,47,15,55,23,63,31,
		38,06,46,14,54,22,62,30,37,05,45,13,53,21,61,29,
		36,04,44,12,52,20,60,28,35,03,43,11,51,19,59,27,
		34,02,42,10,50,18,58,26,33,01,41, 9,49,17,57,25};


    /*** TABELA "PC-2" *********************************************/
    static unsigned char
    PC_2_fips[] ={14,17,11,24,01,05,03,28,15,06,21,10,23,19,12,04,
		26, 8,16,07,27,20,13,02,41,52,31,37,47,55,30,40,
		51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};


    /*** TABELA "SELECT E" *****************************************/
    static unsigned char
    E_fips[]   = {32,01,02,03,04,05,04,05,06,07, 8, 9, 8, 9,10,11,
		12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,
		22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,01};


    /*** TABELA DE PERMUTACAO "P" **********************************/
    static unsigned char
    P_fips[]   = {16,07,20,21,29,12,28,17,01,15,23,26,05,18,31,10,
		02, 8,24,14,32,27,03, 9,19,13,30,06,22,11,04,25};


    /*** TABELA DE ESCOLHA DE PERMUTACOES "PC-1" *******************/
    static unsigned char
    PC_1_fips[] = {57,49,41,33,25,17, 9,01,58,50,42,34,26,18,
		 10,02,59,51,43,35,27,19,11,03,60,52,44,36,
		 63,55,47,39,31,23,15,07,62,54,46,38,30,22,
		 14,06,61,53,45,37,29,21,13,05,28,20,12,04};


    /*** TABELA DE FUNCOES "Si" ************************************/
    static unsigned char
    S_fips[]   = {14,04,13,01,02,15,11, 8,03,10,06,12,05, 9,00,07,
		00,15,07,04,14,02,13,01,10,06,12,11, 9,05,03, 8,
    /* s1 */    04,01,14, 8,13,06,02,11,15,12, 9,07,03,10,05,00,
		15,12, 8,02,04, 9,01,07,05,11,03,14,10,00,06,13,

		15,01, 8,14,06,11,03,04, 9,07,02,13,12,00,05,10,
		03,13,04,07,15,02, 8,14,12,00,01,10,06, 9,11,05,
    /* s2 */    00,14,07,11,10,04,13,01,05, 8,12,06, 9,03,02,15,
		13, 8,10,01,03,15,04,02,11,06,07,12,00,05,14, 9,

		10,00, 9,14,06,03,15,05,01,13,12,07,11,04,02, 8,
		13,07,00, 9,03,04,06,10,02, 8,05,14,12,11,15,01,
    /* s3 */    13,06,04, 9, 8,15,03,00,11,01,02,12,05,10,14,07,
		01,10,13,00,06, 9, 8,07,04,15,14,03,11,05,02,12,

		07,13,14,03,00,06, 9,10,01,02, 8,05,11,12,04,15,
		13, 8,11,05,06,15,00,03,04,07,02,12,01,10,14, 9,
    /* s4 */    10,06, 9,00,12,11,07,13,15,01,03,14,05,02, 8,04,
		03,15,00,06,10,01,13, 8, 9,04,05,11,12,07,02,14,

		02,12,04,01,07,10,11,06, 8,05,03,15,13,00,14, 9,
		14,11,02,12,04,07,13,01,05,00,15,10,03, 9, 8,06,
    /* s5 */    04,02,01,11,10,13,07, 8,15, 9,12,05,06,03,00,14,
		11, 8,12,07,01,14,02,13,06,15,00, 9,10,04,05,03,

		12,01,10,15, 9,02,06, 8,00,13,03,04,14,07,05,11,
		10,15,04,02,07,12, 9,05,06,01,13,14,00,11,03, 8,
    /* s6 */     9,14,15,05,02, 8,12,03,07,00,04,10,01,13,11,06,
		04,03,02,12, 9,05,15,10,11,14,01,07,06,00, 8,13,

		04,11,02,14,15,00, 8,13,03,12, 9,07,05,10,06,01,
		13,00,11,07,04, 9,01,10,14,03,05,12,02,15, 8,06,
    /* s7 */    01,04,11,13,12,03,07,14,10,15,06, 8,00,05, 9,02,
		06,11,13, 8,01,04,10,07, 9,05,00,15,14,02,03,12,

		13,02, 8,04,06,15,11,01,10, 9,03,14,05,00,12,07,
		01,15,13, 8,10,03,07,04,12,05,06,11,00,14, 9,02,
    /* s8 */    07,11,04,01, 9,12,14,02,00,06,10,13,15,03,05, 8,
		02,01,14,07,04,10, 8,13,15,12, 9,00,03,05,06,11};

    /*** TABELA DE DESLOCAMENTOS A ESQUERDA ***********************/
    static unsigned char SHIFT_LEFT_TAB[] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
    int i,
    j,
    k,
    l,
    m,
    n,
    w,
    loop_no,
    row,
    column,
    flag;
    register unsigned char aux_1, aux_2;
    unsigned char
	aux_3,
	aux_4,
	* p_1,
	* p_2,
	* p_3,
	* p_4,
	* p_w,
	* pointer;
    unsigned char K [K_LENGTH],
	K_scratch [K_LENGTH],
	K_i [16 * K_LENGTH],
	L_line[L_LENGTH],
	R_line[R_LENGTH],
	C_D [C_D_LENGTH],
	f_scratch[f_LENGTH],
	f[f_LENGTH];
    union {
        unsigned char data [DATA_LENGTH];
        struct metades {
	        unsigned char block_L[L_LENGTH],
	        block_R[R_LENGTH];
        } partial;
    } inp;

    inp.partial= *(struct metades *)inp_c;
    flag = OFF;

    /* executa a permutacao do bloco de entrada, de acordo com a          */
    /* TABELA DE PERMUTACAO "IP"                                          */
    bits (inp.data,DATA_LENGTH,scratch_buffer,DATA_LENGTH,IP_fips,IP_LENGTH);
    p_1 = scratch_buffer;
    for(p_2 = inp.data; p_2 < (inp.data+DATA_LENGTH);p_2++)
      * p_2 = * p_1 ++;

    /* obtendo os blocos "Co" e "Do" a partir da chave de entrada         */
    bits (chave,KEY_LENGTH,C_D,C_D_LENGTH,PC_1_fips,PC_1_LENGTH);
    for (loop_no=1;loop_no<=16;++loop_no) {
        /* executando o deslocamento rotativo a esquerda dos blocos "Ci"    */
        /* e "Di" de acordo com o numero da interacao corrente              */
        pointer = C_D;
        i = C_D_LENGTH;
        shift_left_bits (pointer,C_D_LENGTH,loop_no,SHIFT_LEFT_TAB);
        /* obtendo o bloco "Ki"     */
        bits (C_D,C_D_LENGTH,K,K_LENGTH,PC_2_fips,PC_2_LENGTH);
        p_1 = K;
        p_2 = K_i + ( 6 * (loop_no - 1));
        for (w=0;w<K_LENGTH;++w)
            * p_2 ++ = * p_1 ++;
    }

    for (loop_no=1;loop_no<=16;++loop_no) {
        flag = OFF;
        /* iniciando o calculo da funcao "f" de criptografia. Aplicando     */
        /* "E" no bloco "R" corrente, e colocando o resultado em "K_scratch"*/

        bits (inp.partial.block_R,R_LENGTH,K_scratch,K_LENGTH,E_fips,E_LENGTH);

        /* executando a soma modulo 2, e armazenando em "K_scratch"         */
        p_1 = K_scratch;

        p_2 = K_i + (6 * ((codop)?(16-loop_no):(loop_no - 1)));

        for (i=0;i<K_LENGTH;++i)
            * p_1 ++ ^= * p_2 ++;
    
        /* aplicando as funcoes "Si" */
        flag = OFF;
        pointer = K_scratch;
        p_1 = S_fips;
        p_2 = f_scratch;
        l = 1;
        for (j=0;j<(K_LENGTH/3);++j) {
            for (i=0;i<4;++i) {
	            switch (i) {
	                case 0: {
	                    aux_1 = * pointer & mask_1;
	                    aux_2 = aux_1 >> 2;
	                    break;
	                }
	                case 1: {
	                    aux_1 = * pointer & mask_2;
	                    aux_2 = aux_1 << 4;
	                    ++ pointer;
	                    aux_1 = * pointer & mask_3;
	                    aux_2 |= aux_1 >> 4;
	                    break;
	                }
	                case 2: {
	                    aux_1 = * pointer & mask_4;
	                    aux_2 = aux_1 << 2;
	                    ++ pointer;
	                    aux_1 = * pointer & mask_5;
	                    aux_2 |= aux_1 >> 6;
	                    break;
	                }
	                case 3:
	                    aux_2 = * pointer & mask_6;
	                    ++ pointer;
	                    break;
	            }
	            aux_1 = aux_2 & mask_7;
	            column = aux_1 >> 1; /* valor decimal correspondente aos 4 bits centrais do bloco de 6 bits  */
	            row = aux_2 & mask_9;
	            aux_1 = aux_2 & mask_8;
	            row |= (aux_1 >> 4); /* valor decimal correspondente aos 2 bits extremos do bloco de 6 bits  */
	            /* determinando o indice de acesso a matriz "Si" */
	            p_1 += row * 16 + column;
	            if (flag == OFF) {
	                aux_3 = * p_1;
	                flag = ON;
	            }
	            else {
	                aux_4 = * p_1;
	                * p_2 ++ = aux_4 | (aux_3 << 4);
	                flag = OFF;
	            }
	            p_1 = S_fips + l ++ * 64;
            }
        }
        ++ pointer;
        /* aplicando a tabela "P" no bloco "f_scratch", para gerar o bloco "f" de saida da funcao */
        bits (f_scratch,f_LENGTH,f,f_LENGTH,P_fips,P_LENGTH);
        /* salva em "f_scratch" o conteudo atual do "block_L"        */
        p_1 = f_scratch;
        p_2 = inp.partial.block_L;
        p_3 = inp.partial.block_R;
        for (k=0;k<f_LENGTH;++k) {
          * p_1 ++ = * p_2;
          * p_2 ++ = * p_3 ++;
        }
        pointer = f_scratch;
        /* executando a soma modulo 2 de "Ln-1" E "f(Rn-1,Kn)" e armazenando em "Rn"        */
        p_3 = f;
        p_4 = inp.partial.block_R;
        for (k=0;k<f_LENGTH;++k)
            * p_4 ++ = (* p_3 ++) ^ (* pointer ++);
    }
    /* permuta o bloco "R" pelo bloco "L", e vice-versa */
    p_1 = inp.partial.block_R;
    p_2 = inp.partial.block_L;
    for (i=0;i<L_LENGTH;++i) {
        aux_1 = * p_1;
        * p_1 ++ = * p_2;
        * p_2 ++ = aux_1;
    }

    /* executa a permutacao do bloco de saida, de acordo com a TABELA DE PERMUTACAO "IP-1" */
    bits (inp.data,DATA_LENGTH,scratch_buffer,DATA_LENGTH,IP_1_fips,IP_LENGTH);

}          /** FIM DES_NBS **/


/*** bit *****************************************************************/
/*                                                                       */
/*************************************************************************/

static void bits (unsigned char *INP,int INP_LEN,unsigned char *OUT,int OUT_LEN, unsigned char *TABLE,int TABLE_LENGTH) {
/* unsigned char * INP,
	       * OUT,
	       * TABLE;
 int INP_LEN,
     OUT_LEN,
     TABLE_LENGTH;*/
static unsigned char maskNBS[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    int i;
    register int j;
    unsigned char x=0x80;
    unsigned char * p_1,
		* p_2,
		* p_3,
		* p_4;
    p_1 = OUT;

    for (i=0;i<OUT_LEN;++i)
        * p_1 ++ = 0;
    
    p_1 = OUT;
    p_4 = TABLE+TABLE_LENGTH;
    for ( p_2=TABLE ; p_2<p_4 ; p_2++) {
        j = (*p_2 - 1)/8;
        if ( INP[j] & maskNBS[*p_2-(8*j)-1] ) {
            * p_1 |= x;
        }

        if (!(x>>=1)) { 
            x = 0x80; ++p_1; 
        }
    }
 }

/*** shift_left_bits *****************************************************/
/*                                                                       */
/*************************************************************************/

static void shift_left_bits (unsigned char *INP,int INP_LENGTH,int LOOP_NO, unsigned char *SHIFT_LEFT_TABLE) {
/* unsigned char * INP;
 int INP_LENGTH,
     LOOP_NO;
 unsigned char *SHIFT_LEFT_TABLE;*/

    unsigned char * aux_1,
		* aux_2,
		* aux_3,
		* aux_4;
    typedef unsigned char * ptruc;
    register ptruc pointer;
    unsigned char c_1,
		c_2,
		d_1,
		d_2,
		x_1,
		x_2;
    int i, j;

    pointer = INP;
    switch (SHIFT_LEFT_TABLE[LOOP_NO-1]) {
        case 1: {
            c_1 = (* pointer & 0x80) >> 3;
            pointer += 3;
            d_1 = (* pointer & 0x08) >> 3;
            pointer = INP;
            for(j=0;j<(INP_LENGTH-1);++j) {
	            * pointer ++ <<= 1;
	            x_1 = (* pointer --  & 0x80) >> 7;
	            * pointer ++ |= x_1;
            }
            * pointer=(((*pointer)<<1)&0xfe)|d_1;
            pointer -= 3;
            * pointer &= 0xef;
            * pointer |= c_1;
            break;
        }
        case 2: {
            c_1 = (* pointer & 0x80) >> 2;
            c_2 = (* pointer & 0x40) >> 2;
            pointer += 3;
            d_1 = (* pointer & 0x08) >> 2;
            d_2 = (* pointer & 0x04) >> 2;
            pointer = INP;
            for(j=0;j<(INP_LENGTH-1);++j) {
	            * pointer ++ <<= 2;
	            x_1 = (* pointer & 0x80) >> 6;
	            x_2 = (* pointer -- & 0x40) >> 6;
	            * pointer |= x_1;
	            * pointer ++ |= x_2;
            }      
            * pointer <<= 2;
            * pointer = (((*pointer & 0xfd) | d_1) & 0xfe) | d_2;
            pointer -= 3;
            * pointer = (((*pointer & 0xdf) | c_1) & 0xef)|c_2;
        }
    }
 }

static  void compacta( char *origem, unsigned char *destino, int tamanho_da_origem ) {
    unsigned char c, d, c1, d1;
    int i;

    for ( i = 0, tamanho_da_origem /= 2; i < tamanho_da_origem; i++ ) {  
        c = toupper( origem[i*2] );
        d = toupper( origem[i*2+1] );
        c1 = ( c & 0x0f ) << 4;
        d1 = d & 0x0f;
        destino[i] = d1 | c1;
    }
}

static  void Xoriza ( char *orig1, char *orig2_dst, int tam_origem ) { 
    int i;
    char c1;
    char c2;
    for ( i = 0 ; i < tam_origem ; i++ ) { 
        c1 = orig1[i];
        c2 = orig2_dst[i];
        orig2_dst[i] = c1 ^ c2;
    }
}

static  void Expandir( char *origem, char *destino, int tamanho_da_origem ) { 
    int i,j;
    char c;

    for ( i = 0, j = 0; i < tamanho_da_origem ; i++ ) {
        c = origem[i];
        c = (c & 0xF0) >> 4;
        if ( c>=0 && c<=9 )
	        c |= '0';
        else
	        c += ( 0x37);
    
        destino[j++] = c;
   
	    c = (origem[i] & 0x0F);
        if ( c>=0 && c<=9 )
	        c |= '0';
        else
	        c += (0x37);
        destino[j++] = c;
    }
    destino[j]=0;
}

static void Compactar( char *origem, char *destino, int tamanho_da_origem ) {
    unsigned char caracter, c, d;
    int i;

    for ( i = 0, tamanho_da_origem /= 2; i < tamanho_da_origem; i++ ) {
        c =  origem[i*2];
	    d =  origem[i*2+1];
	    if ((c >= 48) && (c <= 57 ))
		    caracter = ( c & 0x0f ) << 4;
	    else {   
            c = c & 0xDF; // tornando as letras Maiusculas;
		    caracter = ( c - 'A' + 0x0A ) << 4;
	    }
	 
        if (( d >= 48) && ( d <= 57))
            destino[i] = ( d & 0x0f ) | caracter;
        else { 
            d = d & 0xDF; // tornando as letras Maiusculas;
	        destino[i] = ( d - 'A' + 0x0A ) | caracter;
        }
    }
    destino[i]=0;
}

void INSERIR ( char *str_dst, char *str_org, int inic_dst, int inic_org, int fim) { 
    int i;
    inic_dst--;
    for (i=inic_org-1; i < fim+inic_org-1; i++)
	    *(str_dst+inic_dst++)= *(str_org+i);
}

void DES_CBC ( int modo, char * bloco_entrada, char * chave, char * vetor, char * bloco_saida) {
    int i;
    char entrada[9];
    char saida[9];
    int tam_entrada;
  
    tam_entrada = strlen(bloco_entrada);

    if (modo == 0) // criptografar
        for (i=0; i < tam_entrada; i+=8){ 
            INSERIR(entrada,bloco_entrada,1,i+1,8);
            Xoriza(entrada,vetor,8); 
	        desnbs(0,vetor,chave,saida);
            INSERIR(vetor,saida,1,1,8);
            INSERIR(bloco_saida,saida,i+1,1,8);
        }
    else
        for (i=0; i < tam_entrada; i+=8) { 
            INSERIR(entrada,bloco_entrada,1,i+1,8);
            desnbs(1,entrada,chave,saida);
	        Xoriza(saida,vetor,8);
            INSERIR(bloco_saida,vetor,i+1,1,8);
	        INSERIR(vetor,bloco_entrada,1,i+1,8);
        }
}
