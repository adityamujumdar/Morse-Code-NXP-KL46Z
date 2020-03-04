/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    Morse Code.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"

#define dotDelay 750120 //3000480 1s //750120		// 250ms
#define dashDelay 2250360 //9001440 3s //2250360		//750ms
#define interCharDelay 2250360 //9001440 3s //2250360	//750ms
#define interWordDelay 3000480 //15002400 5s //5sec//3000480	//1000ms

// Define a character pointer array consisting of all the morse code equivalents of symbols needed.
char* morseDefinitions[] = {
						".-",     //  0 = A
                       "-...",    //  1 = B
                       "-.-.",    //  2 = C
                       "-..",     //  3 = D
                       ".",       //  4 = E
                       "..-.",    //  5 = F
                       "--.",     //  6 = G
                       "....",    //  7 = H
                       "..",      //  8 = I
                       ".---",    //  9 = J
                       "-.-",     // 10 = K
                       ".-..",    // 11 = L
                       "--",      // 12 = M
                       "-.",      // 13 = N
                       "---",     // 14 = O
                       ".--.",    // 15 = P
                       "--.-",    // 16 = Q
                       ".-.",     // 17 = R
                       "...",     // 18 = S
                       "-",       // 19 = T
                       "..-",     // 20 = U
                       "...-",    // 21 = V
                       ".--",     // 22 = W
                       "-..-",    // 23 = X
                       "-.--",    // 24 = Y
                       "--..",    // 25 = Z
                       "-----",   // 26 = 0 (ASCII code 48)
                       ".----",   // 27 = 1
                       "..---",   // 28 = 2
                       "...--",   // 29 = 3
                       "....-",   // 30 = 4
                       ".....",   // 31 = 5
                       "-....",   // 32 = 6
                       "--...",   // 33 = 7
                       "---..",   // 33 = 8
                       "----.",   // 34 = 9
                       "--..--",  // 36 = ,
                       ".-.-.-"   // 37 = .
};

// function declaration
void displayMorse(char *message, int length);
void blinkGreen(char *morseDefinitions);

// function definition
/*
 * Function Description: This function takes in a char* message and its length.
 * It overlooks delays required between 2 consecutive letters and 2 consecutive words.
 */
void displayMorse(char *message, int length){
	long int k = 0;
	for(int i = 0; message[i] != '.'; i++){
		// check for the length equivalence and traverse till end of the string.
		if(strlen(message) <= length){
			// Capital Letters differentiator case
			if(message[i] >= 'A' && message[i] <= 'Z'){
				// ASCII value of A is 65
				blinkGreen(morseDefinitions[message[i] - 'A']);
				// After blinking the light, inter character delay of 750ms
				// Checks for next character being a space. If it isn't a space, delay for 750ms otherwise delay for 1000ms
				if(message[i] != ' '){
					// delay loop
					for(long int j = 0; j< interCharDelay; j++){
							k++;
					}
				}
			}
			// Numbers differentiator case.
			else if (message[i] >= '0' && message[i] <= '9'){
				// ASCII value of 0 is 48
				blinkGreen(morseDefinitions[message[i] - '0' + 26]);

				// for the last element. Distinguishing factor would be the length of the '.' character.
				if(message[i] != ' '){
					// busy wait loop
					for(long int j = 0; j< interCharDelay; j++){
							k++;
					}
				}
			}
			// Inter Word Delay state. OFF for 1000ms
			else if(message[i] == ' '){
				// delay loop
				for(long int j = 0; j< interWordDelay; j++){
						k++;
				}
			}
		}
	}
}

/*
 * Function Description: THis function gets passed a char*  to differentiate between
 * the individual '.' and '-' symbols that come with each word. The function is mainly
 * responsible for maintaining the led blink time. Checks if a specific char at a point
 * is a '.' or a '-'.
 */
void blinkGreen(char *morseDefinitions){
	long int k;
	// iterates till we reach the end of char*
	for(int i = 0; i < strlen(morseDefinitions); i++){
		// if it is a '.' turn LED on for dotDelay = 250ms
		if(morseDefinitions[i] == '.'){
			// Toggle GREEN_LED
			GPIOD->PTOR = 1U << 5U;
			for(long int j = 0; j< dotDelay; j++){
					k++;
			}
			// Toggle GREEN_LED
			GPIOD->PTOR = 1U << 5U;
			// check for what comes after the '.'
			if(morseDefinitions[i+1] != NULL){
				for(long int j = 0; j< dotDelay; j++){
				k++;
				}
			}

		}
		// if char is a '-', turn LED on for dashDelay = 750ms
		else if(morseDefinitions[i] == '-'){
			// Toggle GREEN_LED
			GPIOD->PTOR = 1U << 5U;
			for(long int j = 0; j< dashDelay; j++){
					k++;
			}
			// Toggle GREEN_LED
			GPIOD->PTOR = 1U << 5U;
			// check for what comes after the '-'
			if(morseDefinitions[i+1] != NULL){
				for(long int j = 0; j< dotDelay; j++){
				k++;
				}
			}

		}
	}

}
/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();


    // Set a pin to output

    // Enable SIM Clock gating
    // Look at system integration module in KL-46 reference
    SIM->SCGC5 |= 1<<12;
    // Configure PCR to GPIO
    // clear the state of the MUX and set everything to 000 for bits 10,9,8
    PORTD->PCR[5] &= ~0x700U;
    // Set MUX to 001
    PORTD->PCR[5] |= 1<<8;
    // Set Data Direction to Output
    GPIOD->PDDR |= (1 << 5);
    // Turn LED off after the GPIO pins turn the output to on and make it turn on.
    //LED_GREEN_OFF();
    GPIOD->PSOR = 1U << 5U;
    SIM->SCGC5 |= 0x2000U;

	PORTE->PCR[29] = ((1 << 8U) & 0x700U);
	PORTE->PCR[29] = PORT_PCR_MUX(1);
	GPIOE->PDDR |= (1 << 29);
	//LED_RED_OFF();
	GPIOE->PSOR = 1U << 29U;
	// give a message for the morse code generator to output
    char *givenMessage = "HE.";

    int i = 0, k = 0;
    /* Enter an infinite loop, just incrementing a counter. */
    while(i <= 3) {
    	// display the morse code for a message over an infinite loop
        displayMorse(givenMessage, 4);
        //LED_RED_TOGGLE();
        GPIOE->PTOR = 1U << 29U;
        for(long int j = 0; j< interWordDelay; j++){
        	k++;
		}
        //LED_RED_TOGGLE();
        GPIOE->PTOR = 1U << 29U;
        i++;
    }
    return 0 ;
}
