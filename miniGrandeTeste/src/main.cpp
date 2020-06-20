#include "tiva/BoardImplementation.hpp"
/*
 * para mudar o led controlado deve se clicar sw1 sw2 sw1 sw2 em menos de 1s
 * a sequencia acima não deve alterar o periodo do led que esta sendo controlado
 * */

int stack[4] = {0, 0, 0, 0};
int stackHead;
int tickArr[5] = {20, 40, 60, 80, 100};
int tickControlGL = 2; // 0.6 default;
int tickControlRL = 2; // 0.6 default;
int tickCountProcessRL;
int tickCountProcessGL;
int tickCountSW1;
int tickCountSW2;
int tickCountBS;
int checkSW1;
int inSW1;
int checkSW2;
int inSW2;
int changed;
int *control;

void processRL() {
	if (tickCountProcessRL) tickCountProcessRL--;
	else {
		led_red.toggle();
		tickCountProcessRL = tickArr[tickControlRL];
	}
}

void processGL() {
	if (tickCountProcessGL) tickCountProcessGL--;
	else {
		led_green.toggle();
		tickCountProcessGL = tickArr[tickControlGL];
	}
}

void processSW1() {
	if (!sw1.read()) {
		checkSW1 = 1;
		tickCountSW1 = tickArr[0];
	}
	else {
		if (checkSW1) {
			if (!tickCountSW1 && (*control > 0)) {
				(*control)--;
				checkSW1 = 0;
				tickCountSW1 = tickArr[0];
			}
			tickCountSW1--;
		}
	}

}

void processSW2() {
	if (!sw2.read()) {
		checkSW2 = 1;
		tickCountSW2 = tickArr[0];
	}
	else {
		if (checkSW2) {
			if (!tickCountSW2 && (*control < 4)) {
				(*control)++;
				checkSW2 = 0;
				tickCountSW2 = tickArr[0];
			}
			tickCountSW2--;
		}
	}
}

void processBS() {
	if (tickCountBS) {
		if (!sw1.read() && inSW1 && stackHead < 4) {
			inSW1 = 0;
			stack[stackHead++] = 1;
		}
		if (!sw2.read() && inSW2 && stackHead < 4) {
			inSW2 = 0;
			stack[stackHead++] = 2;
		}
		tickCountBS--;
	}
	else {
		int i;
		int change = 0;
		if (stack[0] == 1 && stack[1] == 2) change++;
		if (stack[2] == 1 && stack[3] == 2) change++;
		for (i = 0; i < 4; i++) stack[i] = 0;
		if ((change == 2) && !changed) {
			control = &tickControlGL;
		}
		else {
			control = &tickControlRL;
		}
		stackHead = 0;
		tickCountBS = tickArr[4];
	}
	inSW1 = 1;
	inSW2 = 1;
}

void handleTimerOa() {
	timer0a.disableInterrupts();
	processSW1();
	processSW2();
	processRL();
	processGL();
	processBS();
	timer0a.enableInterrupts();
	timer0a.setPeriod(10000);
	timer0a.start();
}

int main() {
	board.init();
	timer0a.setCallback(handleTimerOa);
	timer0a.setPeriod(10000);
	timer0a.enableInterrupts();
	timer0a.start();
	tickCountProcessRL = tickArr[tickControlRL];
	tickCountProcessGL = tickArr[tickControlGL];
	control = &tickControlRL;
	tickCountSW1 = tickArr[0]; // 20
	tickCountSW2 = tickArr[0]; // 20
	tickCountBS = tickArr[4]; // 100
	checkSW1 = 0;
	inSW1 = 1;
	checkSW2 = 0;
	inSW2 = 1;
	stackHead = 0;
	changed = 0;
	while(1) {}
	return 0;
}
