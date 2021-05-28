#include <stdio.h>

//Declaração de variáveis não global
char memoria[154],
         ir,
         ro0,
         ro1;
int mbr;
short int mar,
          imm,
          pc,
          reg[4];
char e,
     l,
     g;
int posicao = 0x00,
        temp = 0x00,
        c = 0;
int main(int argc, char const *argv[])
{
    //Zera a memória
    for (int cont = 0; cont < 154; cont++)
        memoria[cont] = 0;
    //Recebe do usuário e adiciona na memória
    while(c == 0)
    {
        printf("Insira o endereco de memoria e o conteudo\n-> (Aqui se digita dois numeros em hexadecimal separados por um espaco)\n->");
        scanf("%x %x", &posicao, &temp);
        if(posicao == 0xF01 && temp == 0xF01)
            break;
        posicao = posicao;
        temp = temp;
        memoria[posicao] = temp;
    }

    //Passa o opcode para o IR
    pc = 0;
    mbr = memoria[pc];
    ir = mbr >> 3;
    //verifica qual é o mnemônico
    if(ir >= 0x01 && ir <= 0x09 ){
        pc++;
        mbr = mbr << 8;
        mbr = mbr || memoria[pc];
        ro0 = (mbr << 21) >> 30;
        ro1 = (mbr << 23) >> 30;
        printf("\nsou uma intrucao de 16 bits");
    }else if(ir >= 0xB && ir <= 0x1A){
        pc++;
        mbr = mbr << 8;
        mbr = mbr || memoria[pc];
        pc++;
        mbr = mbr << 8;
        mbr = mbr || memoria[pc];
        printf("\nsou uma instrucao de 24 bits");
        if (ir >= 0x0B && ir <= 0x11){
            mar = mbr;
            printf("\nestou entre 11 e 17");
        }else{
            ro0 = (mbr << 13) >> 30;
            printf("\nestou entre 18 e 26");
        }
    }else if (ir == 0x00 || ir == 0x0A){
        if (ir == 0){
            printf("halt");
        }else{
            ro0 = (mbr << 29) >> 30;
            printf("not");
        }
        printf("\nsou uma instrucao de 8 bits");
    }
    printf("\nIR: %i\n", ir);
    printMemoria(memoria);
    system("PAUSE");
}

//Imprime na tela todos os espaços de memória
void printMemoria(char *memoria[]){
    int pular = 14;
    for (int cont = 0; cont < 154; cont++){
        if(cont == pular){
            printf("\n");
            pular = pular + 14;
        }
        if(cont <= 15)
            printf("0");
        if(memoria[cont] <= 15 )
            printf("%x:   0x0%x | ", cont, memoria[cont]);
        else    
            printf("%x:   0x%x | ", cont, memoria[cont]);
    }
}