/*
	EA871 - Projeto Final
	Gustavo Graziosi Silva	RA: 169320

*/

#include "lcd.h"
#include "delay.h"
#include "derivative.h" /* include peripheral declarations */
#define SET_BIT(reg, idx)	reg = (reg | (1 << idx))		// Macro que seta o bit idx do registrador reg

void init_GPIO_LCD(void) {

    SET_BIT(SIM_SCGC5, 11);     // Habilita clock do PORTC (System Clock Gating Control)

    PORTC_PCR0 &= 0xFFFFF8FF;	// Zera bits de MUX de PTC0
    PORTC_PCR1 &= 0xFFFFF8FF;	// Zera bits de MUX de PTC1
    PORTC_PCR2 &= 0xFFFFF8FF;	// Zera bits de MUX de PTC2
    PORTC_PCR3 &= 0xFFFFF8FF;	// Zera bits de MUX de PTC3
    PORTC_PCR4 &= 0xFFFFF8FF;	// Zera bits de MUX de PTC4
    PORTC_PCR5 &= 0xFFFFF8FF;	// Zera bits de MUX de PTC5
    PORTC_PCR6 &= 0xFFFFF8FF;	// Zera bits de MUX de PTC6
    PORTC_PCR7 &= 0xFFFFF8FF;	// Zera bits de MUX de PTC7
    PORTC_PCR8 &= 0xFFFFF8FF;	// Zera bits de MUX de PTC8
    PORTC_PCR9 &= 0xFFFFF8FF;	// Zera bits de MUX de PTC9

    SET_BIT(PORTC_PCR0, 8);		// Seta bit 0 do MUX de PTC0, assim os bits de MUX serao 001
    SET_BIT(PORTC_PCR1, 8);		// Seta bit 1 do MUX de PTC1, assim os bits de MUX serao 001
    SET_BIT(PORTC_PCR2, 8);		// Seta bit 2 do MUX de PTC2, assim os bits de MUX serao 001
    SET_BIT(PORTC_PCR3, 8);		// Seta bit 3 do MUX de PTC3, assim os bits de MUX serao 001
    SET_BIT(PORTC_PCR4, 8);		// Seta bit 4 do MUX de PTC4, assim os bits de MUX serao 001
    SET_BIT(PORTC_PCR5, 8);		// Seta bit 5 do MUX de PTC5, assim os bits de MUX serao 001
    SET_BIT(PORTC_PCR6, 8);		// Seta bit 6 do MUX de PTC6, assim os bits de MUX serao 001
    SET_BIT(PORTC_PCR7, 8);		// Seta bit 7 do MUX de PTC7, assim os bits de MUX serao 001
    SET_BIT(PORTC_PCR8, 8);		// Seta bit 8 do MUX de PTC8, assim os bits de MUX serao 001
    SET_BIT(PORTC_PCR9, 8);		// Seta bit 9 do MUX de PTC9, assim os bits de MUX serao 001
 
    GPIOC_PDDR = 0x000003FF;  // Configura bits 0-9 do PORTC como saidas
    GPIOC_PDOR = 0;     // Garante nivel 0 em todas as saidas do PORTC

}

void init_LCD(void) {

	delay(40000);				// Espera 40 ms
    putcmd_LCD(0x03C, 5000);	// Liga o display e configura para duas linhas
    putcmd_LCD(0x00C, 5000);	// Configura o controle On/Off do display
    putcmd_LCD(0x001, 5000);	// Limpa a tela do display
    putcmd_LCD(0x006, 5000);	// Ativa o modo de entrada

}

void clear_LCD(void) { 
	
    putcmd_LCD(0x01, 5000);
    
}

void putchar_LCD(unsigned char caracter) {

	delay(50);
    SET_BIT(GPIOC_PDOR, 8);				// Seta R/S = 1 (receber comandos)
    delay(50);
    SET_BIT(GPIOC_PDOR, 9);            // Ativa o sinal E = 1
    delay(50);
    GPIOC_PDOR = (GPIOC_PDOR & 0xffffff00) | caracter; // Insere o caracter no barramento de dados (D0-D7)
    delay(50);
    SET_BIT(GPIOC_PTOR, 9);            // Desativa o sinal E (E = 0)
    delay(50);

}

void putcmd_LCD(unsigned char car_cmd, unsigned int tempo) {

	delay(5000);
	GPIOC_PDOR = 0x00;
    delay(5000);
    SET_BIT(GPIOC_PDOR, 9);           // Ativa o sinal E (E = 1)
    delay(5000);
    GPIOC_PDOR = (GPIOC_PDOR & 0xffffff00) | car_cmd;	// Escreve o caracter no barramento de dados
    delay(tempo);
    GPIOC_PDOR = GPIOC_PDOR & 0xfffffdff;           // Desativa o sinal E (E = 0)
    delay(5000);
    GPIOC_PDOR = 0x00;
    delay(5000);
}

void puts_LCD(unsigned char *sequencia) {

    int i = 0;
    int k = 16;
    char vetor[53];
    
    while ( *(sequencia+i ) != '\0') {       
    	if (i == 16) {
    	    setpos_LCD(2,0);
    	}
    	if (i < 32) {
        	putchar_LCD( *(sequencia+i) );
        }
        vetor[i] = *(sequencia+i);
        if (i >= 32) {
        	setpos_LCD(1,k);
        	putchar_LCD(vetor[k]);
        	putcmd_LCD(0x07,5000);
        	setpos_LCD(2,k);
        	putchar_LCD( *(sequencia+i) );
        	putcmd_LCD(0x06,5000);
        	k++;
        }   
        i++;
    }
}

void setpos_LCD(unsigned int linha, unsigned int posicao) {

    int t = 40;         // Define o tempo do comando de setar posicao da DDRAM
    int cte = 0x80;
    int cmd = posicao + cte;
    if (linha == 2) {
        cmd += 64;      // Se for linha = 2, adiciona 16 posicoes ao cursor
    }
    cmd = (char) cmd;
    putcmd_LCD (cmd, t);
}


