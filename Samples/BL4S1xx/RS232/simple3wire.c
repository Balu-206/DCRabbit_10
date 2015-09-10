/**********************************************************

	simple3wire.c
	Digi International, Copyright � 2008.  All rights reserved.

	This sample program is for the BL4S1xx series SBCs.

   Description:
	============
	This program demonstrates basic initialization for a
	simple RS232 3-wire loopback displayed in STDIO window.

   Here's the typical connections for a 3 wire interface that
   would be made between controllers.

       TX <---> RX
       RX <---> TX
		Gnd <---> Gnd

	Connections:
	============
	1. Connect TX/1-W to CTS (CTS is RXF) located on J5.
   2. Connect RX to RTS (RTS is TXF) located on J5.

	Instructions:
	=============
	1. Compile and run this program.
 	2. View STDIO window for sample program results.

**********************************************************/
#class auto	 // Change local var storage default to "auto"

// include BL4S1xx series library
#use "BL4S1xx.lib"

// serial buffer size
#define DINBUFSIZE  15
#define DOUTBUFSIZE 15
#define FINBUFSIZE  15
#define FOUTBUFSIZE 15

// serial baud rate
#ifndef _232BAUD
#define _232BAUD 115200
#endif

main()
{
   int input, output, output2;

   // initialize board (including serial ports)
   brdInit();

    // open serial ports
   serDopen(_232BAUD);
   serFopen(_232BAUD);

   // Must use serial mode 0 for 3 wire RS232 operation and
   // serMode must be executed after the serXopen function(s).
	serMode(0);

   // Clear serial data buffers
   serDwrFlush();
	serDrdFlush();
   serFwrFlush();
	serFrdFlush();

   // infinite loop sending and receiving on serial port
   while (1)
	{
		for (output='a'; output<='z'; ++output)
		{
			// TXD -> RXF
			printf("TXD: %c\t", (char) output);
			serDputc (output);								//	Send Byte
			while ((input=serFgetc()) == -1);			// Wait to receive
			printf("RXF: %c\t", (char) input);
         // TXF->RXD
			output2 = toupper(input);
			printf("TXF: %c\t", (char) output2);
			serFputc (output2);								//	Send Byte
			while ((input=serDgetc()) == -1);			// Wait to receive
			printf("RXD: %c\n", (char) input);
		}
	}
}

