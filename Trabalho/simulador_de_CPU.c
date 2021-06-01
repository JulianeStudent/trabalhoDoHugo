#include <stdio.h>
#include <conio.h>

//Declaração de variáveis
char memoria[154], ir, ro0, ro1;
int mbr;
short int mar, imm, pc, reg[4];
char e, l, g, finalizar;
unsigned int posicao, temp;

int main(int argc, char const *argv[])
{
    //Zera a memória
    for (int cont = 0; cont < 154; cont++)
        memoria[cont] = 0;
    //Recebe do usuário e adiciona na memória
    while(1)
    {
        printf("Insira o endereco de memoria e o conteudo\n-> (Aqui se digita dois numeros em hexadecimal separados por um espaco)\n->");
        scanf("%x %x", &posicao, &temp);
        if(posicao == 0xF01 && temp == 0xF01)
            break;
        memoria[posicao] = temp;
    }
    system("cls");
    pc = 0;
    while(1){
        mbr = 0;
        ro0 = 0;
        ro1 = 0;
        mbr = memoria[pc] & 0xff;
        ir = mbr >> 3;
        //Verifica qual é o opcode
        if (ir == 0x00 || ir == 0x0A){//halt ou  not, 8 bits
            if (ir == 0){
                printEstadoCpuMemoria(memoria, ir, ro0, ro1, mbr, mar, imm, pc, reg);
                printf("\nTrabalho finalizado");
                break;
            }else{
                ro0 = (mbr << 29) >> 30;
                reg[ro0] = reg[ro0];
            }
        }else if(ir >= 0x01 && ir <= 0x09 ){//16 bits da instrução (1 a 9)
            pc++;
            mbr = mbr << 8;
            mbr = mbr | memoria[pc] & 0xff;
            ro0 = (mbr << 21) >> 30;
            ro1 = (mbr << 23) >> 30;
            //add regX, regY; regX recebe o seu valor mais o valor regY
            if ( ir == 0x01){
                reg[ro0] = (reg[ro0] + reg[ro1]) & 0xffff;
                pc++;
	        }
            //sub regX, regY; regX recebe o seu valor menos o valor regY
            if ( ir == 0x2){
                reg[ro0] = (reg[ro0] - reg[ro1]) & 0xffff;
                pc++;
	        }
            //mul regX, regY; regX recebe o seu valor multiplicado por regY
            if ( ir == 0x03){
                reg[ro0] = (reg[ro0] * reg[ro1]) & 0xffff;
                pc++;
	        }
            //div regX, regY; regX recebe o seu valor dividido por regY
        	if ( ir == 0x04){
                reg[ro0] = (reg[ro0] / reg[ro1]) & 0xffff;
                pc++;
	        }
            //comp regX, regY;
        	if ( ir == 0x05){
                if (reg[ro0] == reg[ro1]){
                    e = 1;
                }else{
                    e = 0;
                }
                if (reg[ro0] < reg[ro1]){
                    l = 1;
                }else{
                    l = 0;
                }
                if (reg[ro0] > reg[ro1]){
                    g = 1;
                }else{
                    g = 0;
                }
                pc++;
	        }
            //movr
            if ( ir == 0x06){
                reg[ro0] = reg[ro1] & 0xffff;
                pc++;
	        }
            //and
	        if ( ir == 0x07){
                reg[ro0] = (reg[ro0] & reg[ro1]) & 0xffff;
                pc++;
	        }
            //or
	        if ( ir == 0x08){
                reg[ro0] = (reg[ro0] | reg[ro1]) & 0xffff;
                pc++;
	        }
            //xor
            if ( ir == 0x09){
                reg[ro0] = (reg[ro0] ^ reg[ro1]) & 0xffff;
                pc++;
            }
            
        }else if(ir >= 0xB && ir <= 0x1A){//24 bits da instrução 11 a 26
            pc++;
            mbr = mbr << 8;
            mbr = mbr | memoria[pc] & 0xffff;
            pc++;
            mbr = mbr << 8;
            mbr = mbr | memoria[pc] & 0xffffff;
            if (ir >= 0x0B && ir <= 0x11){
                //je
                if ( ir == 0x0B){
                    mar = mbr & 0xffff;
                    if ( e == 1 ) 
                        pc = mar;
                    else
                        pc++;
                }
                //jne
                if ( ir == 0x0C){
                    mar = mbr;
                    if ( e == 0 ) 
                        pc = mar;
                    else
                        pc++;
                }
                //jl M[X]; muda o PC para X se L = 1
                if(ir == 0x0D && l == 1){
                    mar = mbr;
                    if(l == 1)
                        pc = mar;
                    else
                        pc++;
                }
                //jle M[X]; muda o PC para X se E = 1 ou L = 1
                else if (ir == 0x0E){
                    mar = mbr;
                    if(e == 1 || l == 1)
                        pc = mar;
                    else
                        pc++;
                }
                //jg M[X]; muda o PC para X se G = 1
                else if(ir == 0x0F){
                    mar = mbr;
                    if(g == 1)
                        pc = mar;
                    else
                        pc++;
                }
                //jge M[X]; muda o PC para X se E = 1 ou G = 1
                else if(ir == 0x10){
                    mar = mbr;
                    if(e == 1 || g == 1)
                        pc = mar;
                    else
                        pc++;
                }
                //jmp M[X]; muda o PC para X
                else{
                    mar = mbr;
                    pc = mar;
                }
            }else{
                ro0 = (mbr << 13) >> 30;
                //ld regX, M[Y]; carega para regX a palavra que está em Y
                if(ir == 0x12){
                    mar = mbr;
                    mbr =  memoria[mar++] & 0xffff;
                    mbr = mbr << 8;
                    mbr = mbr | memoria[mar] & 0xffff;
                    reg[ro0] = mbr & 0xffff;
                    pc++;
                }
                //st regX, M[Y]; armazena na memória, começando em Y, a palavra de 16 bits que está em regX
                else if(ir == 0x13){
                    mar = mbr & 0xffff;
                    mbr = reg[ro0] & 0xffff;
                    memoria[mar] = mbr >> 8;
                    mar++;
                    memoria[mar] = mbr & 0xff;
                    pc++;
                }
                //movi regX, imm; regX recebe o valor que está no IMM
                else if(ir == 0x14){
                    imm = mbr & 0xffff;
                    reg[ro0] = imm & 0xffff;
                    pc++;
                }
                //addi regX, imm; regX recebe o seu valor mais o IMM
                else if(ir == 0x15){
                    imm = mbr & 0xffff;
                    reg[ro0] = (reg[ro0] + imm) & 0xffff;
                    pc++;
                }
                //subi regX, imm; regX recebe seu valor menos IMM
                else if(ir == 0x16){
                    imm = mbr & 0xffff;
                    reg[ro0] = (reg[ro0] - imm) & 0xffff;
                    pc++;
                }
                //muli regX, imm; regX recebe seu valor multiplicado pelo IMM
                else if(ir == 0x17){
                    imm = mbr & 0xffff;
                    reg[ro0] = (reg[ro0] * imm) & 0xffff;
                    pc++;
                }
                //divi regX, imm; regX recebe seu valor dividido pelo IMM
                else if(ir == 0x18){
                    imm = mbr & 0xffff;
                    reg[ro0] = (reg[ro0] / imm) & 0xffff;
                    pc++;
                }
                //lsh regX, imm; desloca regX em IMM bits para esquerda
                else if(ir == 0x19){
                    imm = mbr & 0xffff;
                    reg[ro0] = reg[ro0] << imm;
                    pc++;
                }
                //rsh regX, imm; desloca regX em IMM bits para direita
                else if(ir == 0x19){
                    imm = mbr & 0xffff;
                    reg[ro0] = reg[ro0] >> imm;
                    pc++;
                }
            }
        }
        printEstadoCpuMemoria(memoria, ir, ro0, ro1, mbr, mar, imm, pc, reg);
        printf("\nPrecione uma tecla para iniciar o proximo ciclo de maquina ou aperte 'c' para finalizar o trabalho.");
        finalizar = getch();
        int valorAscii;
        valorAscii = finalizar;
        if(valorAscii == 67 || valorAscii == 99){
            printf("\nTrabalho finalizado");
            break;
        }else{
            system("cls");
        }
    }
}

void printEstadoCpuMemoria(char memoria[],char ir,char ro0,char ro1, int mbr, short int mar, short int imm, short int pc, short int reg[]){
    int pular = 12;
    printf("CPU:\n");
    printf("R0:  0x%x   R1:  0x%x     R2:  0x%x     R3: 0x%x\n", reg[0], reg[1], reg[2], reg[3]);
    printf("MBR: 0x%x   MAR: 0x%x     IMM: 0x%x     PC: 0x%x\n", mbr, mar, imm, pc);
    printf("IR:  0x%x   RO0: 0x%x     RO1: 0x%x\n", ir, ro0, ro1);
    printf("MEMORIA:\n");
    for (int cont = 0; cont < 154; cont++){
        if(cont == pular){
            printf("\n");
            pular = pular + 12;
        }
        if(cont <= 15)
            printf("0");
        printf("%x:  0x%x   | ", cont, memoria[cont] & 0xff);
    }
}