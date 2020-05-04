/*	Author: sflei002
 *  Partner(s) Name: 
 *	Lab Section:25
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <timer.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {led0, led1, led2} state;

void Tick(){
	switch(state){
		case led0:
			break;
		case led1:
			break;
		case led2:
			break;
		default:
			state = led0;
			break;
	}

	switch(state){
                case led0:
			PORTB = 0x01;
			state = led1;
                        break;
                case led1:
			PORTB = 0x02;
                        state = led2;
                        break;
                case led2:
			PORTB = 0x04;
                        state = led0;
                        break;
                default:
                        break;
        }
}


int main(void) {
    /* Insert DDR and PORT initializations */
DDRB = 0xFF;
PORTB = 0x00;
TimerSet(125);
TimerOn();
state = -1;
    /* Insert your solution below */
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
}