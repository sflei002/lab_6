/*	Author: sflei002
 *  Partner(s) Name: 
 *	Lab Section:25
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
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
enum States {init, wait, inc, dec, inc2, dec2, reset} state;
unsigned char A0 = 0x00;
unsigned char A1 = 0x00;
unsigned char cnt = 0x00;

void Tick(){
	A0 = ~PINA & 0x01;
	A1 = ~PINA & 0x02;
	switch(state){
		case init:
			state = init;
			break;
		case wait:
			if(!A0 && !A1){
				state = wait;
				break;
			}
			else if(A0 && !A1){
				state = inc;
				break;
			}
			else if(!A0 && A1){
                                state = dec;
                                break;
                        }
			else if(A0 && A1){
                                state = reset;
                                break;
                        }
		case inc:
			if(A0 && !A1){
                                state = inc2;
                                break;
                        }
                        else if(!A0 && !A1){
                                state = wait;
                                break;
                        }
                        else if(A0 && A1){
                                state = reset;
                                break;
                        }
		case inc2:
			if(A0 && !A1){
                                state = inc2;
                                break;
                        }
                        else if(!A0 && !A1){
                                state = wait;
                                break;
                        }
                        else if(A0 && A1){
                                state = reset;
                                break;
                        }
		case dec:
			if(!A0 && A1){
                                state = dec2;
                                break;
                        }
                        else if(!A0 && !A1){
                                state = wait;
                                break;
                        }
                        else if(A0 && A1){
                                state = reset;
                                break;
                        }
		case dec2:
			if(!A0 && A1){
                                state = dec2;
                                break;
                        }
                        else if(!A0 && !A1){
                                state = wait;
                                break;
                        }
                        else if(A0 && A1){
                                state = reset;
                                break;
                        }
		case reset:
			break;
		default:
			state = init;
			break;
	}

	switch(state){
                case init:
			PORTB = 0x07;
			state = wait;
                        break;
                case wait:
                        break;
                case inc:
			if(PORTB >= 9){
				state = wait;
				break;
			}
			else{
				PORTB = PORTB + 0x01;
				state = inc;
				break;
			}
		case inc2:
			if(PORTB >= 9){
                                state = wait;
                                break;
			}
			if(cnt < 10){
				state = inc2;
				cnt = cnt + 0x01;
				break;
			}
			else if(cnt >= 10){
				PORTB = PORTB + 0x01;
				cnt = 0x00;
				break;
			}
		case dec:
			if(PORTB <= 0){
                                state = wait;
                                break;
                        }
                        else{
                                PORTB = PORTB - 0x01;
                                state = dec;
                                break;
                        }
		case dec2:
			if(PORTB <= 0){
                                state = wait;
                                break;
                        }
                        if(cnt < 10){
                                state = dec2;
                                cnt = cnt + 0x01;
                                break;
                        }
                        else if(cnt >= 10){
                                PORTB = PORTB - 0x01;
                                cnt = 0x00;
                                break;
                        }
		case reset:
			PORTB = 0x00;
			cnt = 0x00;
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
TimerSet(10);
TimerOn();
state = -1;
    /* Insert your solution below */
    while (1) {
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;
    }
}
