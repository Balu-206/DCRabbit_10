/********************************************************************

	simple5wire.c
	Digi International, Copyright � 2008.  All rights reserved.

	This sample program is for the BL4S1xx series SBCs.

   Description:
	============
	This program demonstrates basic initialization for a
	simple RS232 5-wire loopback displayed in STDIO window.

	Here's the typical connections for a 5 wire interface that
   would be made between controllers.

		 TX <---> RX
       RX <---> TX
		RTS <---> CTS
      CTS <---> RTS
		Gnd <---> Gnd

	Connections:
	============
	1. Connect TX/1-W to RX located on J5.
   2. Connect RTS to CTS located on J5.

	Instructions:
	=============
	1.  Compile and run this program.
	2.  To test flow control, disconnect RTS from CTS while
		 running	this program.  Characters should stop printing
		 in STDIO window and continue when RTS and CTS are
		 connected again.

********************************************************************/
#class auto	 // Change local var storage default to "auto"

// include BL4S1xx series library
#use "BL4S1xx.lib"

// serial buffer size
#define DINBUFSIZE  15
#define DOUTBUFSIZE 15

// serial baud rate
#ifndef _232BAUD
#define _232BAUD 115200
#endif


main()
{
   int input, output;

   // initialize board
   brdInit();

    // open serial port
   serDopen(_232BAUD);

   // Must use serial mode 1 for 5 wire RS232 operation and
   // serMode must be executed after the serXopen function(s).
	serMode(1);

   // Clear serial data buffers
   serDwrFlush();
	serDrdFlush();

   // infinite loop sending and receiving on serial port
   while (1)
	{
		for (output='a'; output<='z'; ++output)
		{
			printf("Sending char: %c\t", (char) output);
			serDputc (output);								//	Send Byte
			while ((input=serDgetc()) == -1);			// Wait to receive
			printf("Received char: %c\n", input);
		}
	}
}








