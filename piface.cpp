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



#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )
#define PIFACE_C
#include "piface.h"


#include <fcntl.h>				//Needed for SPI port
#include <sys/ioctl.h>			//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>




//******************************************
//******************************************
//********** PIFACE SPI OPEN PORT **********
//******************************************
//******************************************
void PifaceOpenSpiPort (void)
{
	int status_value = -1;
    int *spi_cs_fd;


    //----- SET SPI MODE -----
    //SPI_MODE_0 (0,0) 	CPOL=0 (Clock Idle low level), CPHA=0 (SDO transmit/change edge active to idle)
    //SPI_MODE_1 (0,1) 	CPOL=0 (Clock Idle low level), CPHA=1 (SDO transmit/change edge idle to active)
    //SPI_MODE_2 (1,0) 	CPOL=1 (Clock Idle high level), CPHA=0 (SDO transmit/change edge active to idle)
    //SPI_MODE_3 (1,1) 	CPOL=1 (Clock Idle high level), CPHA=1 (SDO transmit/change edge idle to active)
    spi_mode = SPI_MODE_0;

    //----- SET BITS PER WORD -----
    spi_bitsPerWord = 8;

    //----- SET SPI BUS SPEED -----
    spi_speed = 1000000;		//Max speed PiFace 10MHz (1000000 gives 1MHz = 1uS per bit (measured))


    if (PIFACE_SPI_DEVICE)
    	spi_cs_fd = &spi_cs1_fd;
    else
    	spi_cs_fd = &spi_cs0_fd;


    if (PIFACE_SPI_DEVICE)
    	*spi_cs_fd = open(std::string("/dev/spidev0.1").c_str(), O_RDWR);
    else
    	*spi_cs_fd = open(std::string("/dev/spidev0.0").c_str(), O_RDWR);

    if (*spi_cs_fd < 0)
    {
        perror("Error - Could not open SPI device");
        exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MODE, &spi_mode);
    if(status_value < 0)
    {
        perror("Could not set SPIMode (WR)...ioctl fail");
        exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MODE, &spi_mode);
    if(status_value < 0)
    {
      perror("Could not set SPIMode (RD)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    {
      perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    {
      perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    {
      perror("Could not set SPI speed (WR)...ioctl fail");
      exit(1);
    }

    status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    {
      perror("Could not set SPI speed (RD)...ioctl fail");
      exit(1);
    }


    //----- SETUP THE PIFACE -----
    piface_write_register(PIFACE_IOCON_BANK0, 0xa8);	//Enable hardware addressing <<<<<SEQOP=1=sequential auto increment mode disabled and BANK=1
    piface_write_register(PIFACE_GPIOA, 0x00);	//Initialise all outputs to 0
    piface_write_register(PIFACE_GPIOB, 0x00);	//Initialise all outputs to 0
    piface_write_register(PIFACE_IODIRA, 0);	//Set port A as outputs
    piface_write_register(PIFACE_IODIRB, 0x0F);	//Set port B as outputs:inputs		<<<SET WHICH PINS TO BE USED AS OUTPUTS.  0xFF = all inputs.  Set bit low for output.<<<<<<<<<<<<<<<<
    piface_write_register(PIFACE_GPPUB, 0x0F);	//Turn on port B pullups			<<<SET WHICH PINS TO BE USED AS OUTPUTS.  0xFF = all inputs  Set bit low for output. <<<<<<<<<<<<<<<<


    return;
}



//*******************************************
//*******************************************
//********** PIFACE SPI CLOSE PORT **********
//*******************************************
//*******************************************
void PifaceCloseSpiPort (void)
{
	int status_value = -1;
    int *spi_cs_fd;

    if (PIFACE_SPI_DEVICE)
    	spi_cs_fd = &spi_cs1_fd;
    else
    	spi_cs_fd = &spi_cs0_fd;


    status_value = close(*spi_cs_fd);
    if(status_value < 0)
    {
    	perror("Error - Could not close SPI device");
    	exit(1);
    }
    return;
}



//****************************************
//****************************************
//********** PIFACE SET OUTPUTS **********
//****************************************
//****************************************
void piface_set_outputs (void)
{
	piface_write_register(PIFACE_GPIOA, piface_outputs);
}



//****************************************
//****************************************
//********** PIFACE READ INPUTS **********
//****************************************
//****************************************
void piface_read_inputs (void)
{
    BYTE data_buffer[] = {PIFACE_CMD_READ, PIFACE_GPIOB, 0};
    SpiWriteAndRead(PIFACE_SPI_DEVICE, data_buffer, 3);
    piface_inputs = ~data_buffer[2];
}




//********************************************
//********************************************
//********** PIFACE WRITE REGISTER ***********
//********************************************
//********************************************
void piface_write_register (BYTE address, BYTE value)
{
    BYTE data_buffer[] = {PIFACE_CMD_WRITE, address, value};
    SpiWriteAndRead(PIFACE_SPI_DEVICE, data_buffer, 3);
}



//*******************************************************************************
//*******************************************************************************
//********** PIFACE WRITE ONE OR MORE SEQUENTIAL BYTES TO THE OUTPUTS ***********
//*******************************************************************************
//*******************************************************************************
//This fucntion uses an array allowing you to pass sequential bytes to the pins replicating a serial bus if you want to.
//If not call with an array of 1 byte and length=1
//port = PIFACE_GPIOA or PIFACE_GPIOB (if using pins as outputs)
int piface_write_sequential_bytes_to_ouputs (unsigned char port, unsigned char *data, int length)
{
	length += 2;	//Add the command and address bytes to the length
    
    
	struct spi_ioc_transfer spi[length];
	int i = 0;
	int data_index;
	int retVal = -1;
    int *spi_cs_fd;
    unsigned char data_command_address[2] = {PIFACE_CMD_WRITE, port};
    

    if (PIFACE_SPI_DEVICE)
    	spi_cs_fd = &spi_cs1_fd;
    else
    	spi_cs_fd = &spi_cs0_fd;

    //Command and address first
    i = 0;
	spi[i].tx_buf        = (unsigned long)(&data_command_address[0]); // transmit from "data"
	spi[i].rx_buf        = (unsigned long)(&data_command_address[0]) ; // receive into "data"
	spi[i].len           = sizeof(*(data_command_address + 0));
	spi[i].delay_usecs   = 0 ;
	spi[i].speed_hz      = spi_speed ;
	spi[i].bits_per_word = spi_bitsPerWord ;
	spi[i].cs_change = 0;
	
	i++;
	spi[i].tx_buf        = (unsigned long)(&data_command_address[1]); // transmit from "data"
	spi[i].rx_buf        = (unsigned long)(&data_command_address[1]) ; // receive into "data"
	spi[i].len           = sizeof(*(data_command_address + 1));
	spi[i].delay_usecs   = 0 ;
	spi[i].speed_hz      = spi_speed ;
	spi[i].bits_per_word = spi_bitsPerWord ;
	spi[i].cs_change = 0;
	
    
	//One spi transfer for each byte
	data_index = 0;
	for (i = 2; i < length; i++)
	{
		spi[i].tx_buf        = (unsigned long)(data + data_index); // transmit from "data"
		spi[i].rx_buf        = (unsigned long)(data + data_index) ; // receive into "data"
		spi[i].len           = sizeof(*(data + data_index)) ;
		spi[i].delay_usecs   = 0 ;
		spi[i].speed_hz      = spi_speed ;
		spi[i].bits_per_word = spi_bitsPerWord ;
		spi[i].cs_change = 0;
		
		data_index++;
	}

	retVal = ioctl(*spi_cs_fd, SPI_IOC_MESSAGE(length), &spi) ;

	if(retVal < 0)
	{
		perror("Error - Problem transmitting spi data..ioctl");
		exit(1);
	}

	return retVal;
}





//*******************************************
//*******************************************
//********** SPI WRITE & READ DATA **********
//*******************************************
//*******************************************
//data		Bytes to write.  Contents is overwritten with bytes read.
int SpiWriteAndRead (int spi_device, unsigned char *data, int length)
{
	struct spi_ioc_transfer spi[length];
	int i = 0;
	int retVal = -1;
    int *spi_cs_fd;

    if (spi_device)
    	spi_cs_fd = &spi_cs1_fd;
    else
    	spi_cs_fd = &spi_cs0_fd;

	//One spi transfer for each byte
	for (i = 0 ; i < length ; i++)
	{
		spi[i].tx_buf        = (unsigned long)(data + i); // transmit from "data"
		spi[i].rx_buf        = (unsigned long)(data + i) ; // receive into "data"
		spi[i].len           = sizeof(*(data + i)) ;
		spi[i].delay_usecs   = 0 ;
		spi[i].speed_hz      = spi_speed ;
		spi[i].bits_per_word = spi_bitsPerWord ;
		spi[i].cs_change = 0;
	}

	retVal = ioctl(*spi_cs_fd, SPI_IOC_MESSAGE(length), &spi) ;

	if(retVal < 0)
	{
		perror("Error - Problem transmitting spi data..ioctl");
		exit(1);
	}

	return retVal;
}



