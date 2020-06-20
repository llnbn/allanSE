#include "tiva/BoardImplementation.hpp"
#include <string.h>

int countTimer;
char buffer[40];

void UART_OutChar(char data) {
	while((UART0_FR_R&UART_FR_TXFF) != 0);
	UART0_DR_R = data;
}

void printMessage(const char *message) {
	while (*message != '\0') {
		UART_OutChar(*message);
		message++;
	}
}

void handleTimerSw1() {
	timer0a.disableInterrupts();
	led_red.off();
	led_green.on();
	sw1.enableInterrupts();
}

void handleTimerSw2() {
	timer1a.disableInterrupts();
	countTimer++;
	if (countTimer == 5) {
		countTimer = 0;
		sw2.enableInterrupts();
		led_blue.off();
		led_green.on();
	}
	else {
		led_blue.toggle();
		timer1a.enableInterrupts();
		timer1a.setPeriod(1000000);
		timer1a.start();
	}
}

void handleSw1() {
	strcpy(&buffer[0], "\r\nSW1 ativado!");
	printMessage(&buffer[0]);
	sw1.disableInterrupts();
	sw2.enableInterrupts();
	led_green.off();
	led_blue.off();
	led_red.on();
	timer0a.enableInterrupts();
	timer0a.start();
}

void handleSw2() {
	strcpy(&buffer[0], "\r\nSW2 ativado!");
	printMessage(&buffer[0]);
	sw2.disableInterrupts();
	sw1.enableInterrupts();
	led_green.off();
	led_red.off();
	led_blue.on();
	countTimer = 0;
	timer1a.enableInterrupts();
	timer1a.start();
}

int main(void) {
	board.init();
	sw1.setCallback(handleSw1);
	sw1.enableInterrupts();
	sw2.setCallback(handleSw2);
	sw2.enableInterrupts();
	timer0a.setCallback(handleTimerSw1);
	timer0a.setPeriod(7000000);
	timer1a.setCallback(handleTimerSw2);
	timer1a.setPeriod(1000000);
//	sw3.setCallback(handleSw3);
//	sw3.enableInterrupts();
	led_green.on();
	while(1){
	}
}

