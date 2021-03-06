/*	Author: sflei002
 *  Partner(s) Name: 
 *	Lab Section:25
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *DEMO: https://drive.google.com/open?id=1OUl4dX-_IsgeV6h09MdcMBy8v7fQd_ic
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
        TCCR1B = 0x0B;

        OCR1A = 125;

        TIMSK1 = 0x02;

        TCNT1 = 0;

        _avr_timer_cntcurr = _avr_timer_M;

        SREG |= 0x80;
}

void TimerOff(){
        TCCR1B = 0x00;
}

void TimerISR(){
        TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
        _avr_timer_cntcurr--;
        if(_avr_timer_cntcurr == 0){
                TimerISR();
                _avr_timer_cntcurr = _avr_timer_M;
        }
}

void TimerSet(unsigned long M){
        _avr_timer_M = M;
        _avr_timer_cntcurr = _avr_timer_M;
}
//--------------------------------------------------------------------
enum States {led0, led1, led2, wait, on} state;
unsigned char A0;
unsigned char pre;

void Tick(){
	A0 = ~PINA & 0x01;
	switch(state){
		case led0:
			pre = 0;
			if(A0){
				state = wait;
			}
			else{
				state = led1;
			}
			break;
		case led1:
                        if(A0){
                                state = wait;
                        }
			else if(pre == 0){
				state = led2;
			}
                        else if(pre == 2){
                                state = led0;
                        }
			break;
		case led2:
			pre = 2;
                        if(A0){
                                state = wait;
                        }
                        else{
                                state = led1;
                        }
			break;
		case wait:
                        if(A0){
                                state = on;
                        }
                        else{
                                state = wait;
                        }
			break;
		case on:
			state = led1;
			break;
		default:
			state = led0;
			break;
	}

	switch(state){
                case led0:
			PORTB = 0x01;
                        break;
                case led1:
			PORTB = 0x02;
                        break;
                case led2:
			PORTB = 0x04;
                        break;
		case wait:
			break;
		case on:
			break;
                default:
                        break;
        }
}


int main(void) {
    /* Insert DDR and PORT initializations */
DDRA = 0x00;
PORTA = 0xFF;
DDRB = 0xFF;
PORTB = 0x00;
TimerSet(42);
TimerOn();
state = -1;
    /* Insert your solution below */
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
}
