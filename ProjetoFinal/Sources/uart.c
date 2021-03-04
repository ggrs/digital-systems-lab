/*
 *  uart.c
 *
 *  EA871 - Projeto Final
 *  Gustavo Graziosi Silva  RA: 169320
 * 
 */

#include "lcd.h"
#include "delay.h"
#include "uart.h"
#include "derivative.h" /* include peripheral declarations */
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))		// Macro que seta o bit idx do registrador reg

void init_UART0(void) {

	SET_BIT(SIM_SCGC4, 10);     // Habilita clock do UART0
	SET_BIT(SIM_SCGC5, 9);      // Habilita clock do PORTA

	// Configuracao do MUX
	PORTA_PCR1 &= 0xFFFFF8FF;	// Zera bits de MUX de PTA1
	PORTA_PCR2 &= 0xFFFFF8FF;	// Zera bits de MUX de PTA2

	// PTA1 - UART0_RX - ALT2
	SET_BIT(PORTA_PCR1, 9);		// Seta bit 1 do MUX de PTA1, assim os bits de MUX serao 010
	// PTA2 - UART0_TX - ALT2
	SET_BIT(PORTA_PCR2, 9);		// Seta bit 2 do MUX de PTA2, assim os bits de MUX serao 010

	// Configuracao do Clock
	SIM_SOPT2 &= 0xF9FFFFFF;	// Seta os bits 27-26 de SIM_SOPT2 para 01, assim a UART0 opera com o clock MCGFLLCLK
	SET_BIT(SIM_SOPT2, 26);
	
	UART0_BDH &= 0xC0;
	UART0_BDL = 0x0C;

	UART0_C1 &= 0xED;           // Configuracoes de bit de paridade e pacote de dados de 8 bits
	UART0_C2 |= 0x2C;           // Habilitacao do transmissor e receptor
	
	SET_BIT(NVIC_IPR3, 6);      // Define prioridade da interrupcao
	SET_BIT(NVIC_ISER, 12);     // Habilita reconhecimento de pedido de interrupcao
}

char getchar_UART0(void) {
	
	char c;
	
    if (UART0_S1 & UART0_S1_RDRF_MASK) { // verifica se existe um novo caractere na UART0 (Register Full)
        c = UART0_D;    // le o caractere
    } else {
    	
    }
    	
    return c;
    
}
	
void putchar_UART0(char c) {

    while( !(UART0_S1 & UART0_S1_TDRE_MASK)) ; 	// se nao esta vazio (Register Empty), espera 
    UART0_D = c;    // envia caracter a ser transmitido

}

void puts_UART0(char *sequencia) {

    int i = 0;

    while( *(sequencia+i) != '\0' ) {
        putchar_UART0( *(sequencia+i) );
        i++;
    }

}


