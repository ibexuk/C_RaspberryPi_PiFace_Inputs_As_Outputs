/*
Provided by IBEX UK LTD http://www.ibexuk.com
Electronic Product Design Specialists
RELEASED SOFTWARE

The MIT License (MIT)

Copyright (c) 2013, IBEX UK Ltd, http://ibexuk.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//Project Name:		Raspberry Pi - PiFace Expansion Board Driver



//##### THIS IS A SPECIALLY MODIFED VERSION OF THS PIFACE DRIVER WHICH USES INPUTS7:4 AS OUTPUTS INSTEAD OF INPUTS #####




//############################
//##### USING THE DRIVER #####
//############################
/*
//##### INITIALISE #####
	PifaceOpenSpiPort();

//##### CONTROLLING OUTPUTS ######
	//Setup individual pins:
	PIFACE_OUTPUT_0(1);
	PIFACE_OUTPUT_3(0);
	//or just write to register piface_outputs
	piface_set_outputs();		//Write piface_outputs to the PiFace


//##### READING INPUTS #####
	piface_read_inputs();		//Read piface_inputs from the PiFace
	if (PIFACE_INPUT_0)
	{
	}
	//or just read to register piface_inputs


//##### IF YOUR APPLICAITON EXITS #####
	PifaceCloseSpiPort();
*/


//*****************************
//*****************************
//********** DEFINES **********
//*****************************
//*****************************
#ifndef PIFACE_C_INIT		//Do only once the first time this file is used
#define	PIFACE_C_INIT



//-------------------
//----- OUTPUTS -----
//-------------------
#define	PIFACE_OUTPUT_0(state)	(state ? (piface_outputs |= 0x01) : (piface_outputs &= ~0x01))
#define	PIFACE_OUTPUT_1(state)	(state ? (piface_outputs |= 0x02) : (piface_outputs &= ~0x02))
#define	PIFACE_OUTPUT_2(state)	(state ? (piface_outputs |= 0x04) : (piface_outputs &= ~0x04))
#define	PIFACE_OUTPUT_3(state)	(state ? (piface_outputs |= 0x08) : (piface_outputs &= ~0x08))
#define	PIFACE_OUTPUT_4(state)	(state ? (piface_outputs |= 0x10) : (piface_outputs &= ~0x10))
#define	PIFACE_OUTPUT_5(state)	(state ? (piface_outputs |= 0x20) : (piface_outputs &= ~0x20))
#define	PIFACE_OUTPUT_6(state)	(state ? (piface_outputs |= 0x40) : (piface_outputs &= ~0x40))
#define	PIFACE_OUTPUT_7(state)	(state ? (piface_outputs |= 0x80) : (piface_outputs &= ~0x80))

//-------------------
//----- INPUTS -----
//-------------------
#define	PIFACE_INPUT_0			((piface_inputs & 0x01) ? 1 : 0)
#define	PIFACE_INPUT_1			((piface_inputs & 0x02) ? 1 : 0)
#define	PIFACE_INPUT_2			((piface_inputs & 0x04) ? 1 : 0)
#define	PIFACE_INPUT_3			((piface_inputs & 0x08) ? 1 : 0)
//#define	PIFACE_INPUT_4			((piface_inputs & 0x10) ? 1 : 0)
//#define	PIFACE_INPUT_5			((piface_inputs & 0x20) ? 1 : 0)
//#define	PIFACE_INPUT_6			((piface_inputs & 0x40) ? 1 : 0)
//#define	PIFACE_INPUT_7			((piface_inputs & 0x80) ? 1 : 0)





//----- SPI ACCESS SETUP -----
#define	PIFACE_SPI_DEVICE				0			//0=CS0

#define PIFACE_CMD_WRITE				0x40
#define PIFACE_CMD_READ					0x41

#define PIFACE_IODIRA					0x00	//I/O direction A
#define PIFACE_IODIRB					0x10	//I/O direction B
#define PIFACE_IOCON_BANK0				0x0A	//I/O config
#define PIFACE_IOCON					0x05	//I/O config
#define PIFACE_GPIOA					0x09	//Port A (PiFace uses as Output)
#define PIFACE_GPIOB					0x19	//Port B (PiFace uses as Input)
#define PIFACE_GPPUA					0x06	//Port A pullups
#define PIFACE_GPPUB					0x16	//Port B pullups


#endif


//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef PIFACE_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------
void piface_write_register (BYTE address, BYTE value);
int SpiWriteAndRead (int spi_device, unsigned char *data, int length);


//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
void PifaceOpenSpiPort (void);
void PifaceCloseSpiPort (void);
void piface_set_outputs (void);
void piface_read_inputs (void);
int piface_write_sequential_bytes_to_input_pins (unsigned char *data, int length);


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
extern void PifaceOpenSpiPort (void);
extern void PifaceCloseSpiPort (void);
extern void piface_set_outputs (void);
extern void piface_read_inputs (void);
extern int piface_write_sequential_bytes_to_input_pins (unsigned char *data, int length);



#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef PIFACE_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------
int spi_cs0_fd;				//file descriptor for the SPI device
int spi_cs1_fd;				//file descriptor for the SPI device
unsigned char spi_mode;
unsigned char spi_bitsPerWord;
unsigned int spi_speed;


//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)
BYTE piface_inputs = 0;
BYTE piface_outputs = 0;


#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------
extern BYTE piface_inputs;
extern BYTE piface_outputs;


#endif






