/*
   Copyright (c) 2015, Digi International Inc.

   Permission to use, copy, modify, and/or distribute this software for any
   purpose with or without fee is hereby granted, provided that the above
   copyright notice and this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
   OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
/*******************************************************************
	flowcontrol.c

	This program is used with RCM5400W series controllers
	and prototyping boards.

	Description
	===========
	This program demonstrates hardware flow control by sending a
	pattern of '*' characters out of TXC (serial port C) at
	115200 baud.

	One character at a time is received on RXC and is displayed.
	In this example RXD is configured as the CTS input, detecting
	a clear to send condition, and TXD is configured as the RTS
	output, signaling a ready condition. This demonstration can be
	performed with either one or two core module boards.

	Refer to the function description for serCflowcontrolOn()
	for a general description on how to set up flow control lines.
	(place cursor on function name and press CTRL+H)

	Prototyping Board Connections
	=============================

	On the RS232 connector, one or two boards
	-----------------------------------------

	   (CTS) RXD <-------> TXD (RTS)
	         RXC <-------> TXC
	         GND <-------> GND (two boards only)


	Instructions
	============
	1.	With two boards, run the program on the sending board and then
		disconnect and power it back up so that the program is running
		in stand-alone mode.
		Connect to the second board, which will be the receiver and
		run this same program normally.

	2.	A repeating triangular pattern should print out in
		the STDIO window. The program will periodically switch flow
		control on or off to	demonstrate the effect of no flow control
		for a very slow receiver.

	3.	You can also observe the signals with a scope to see flow
		control operating.

*******************************************************************/
#class auto

#define COUTBUFSIZE 31
#define CINBUFSIZE 15

//see serCflowcontrolOn() function description
#define SERC_RTS_PORT PCDR
#define SERC_RTS_SHADOW PCDRShadow
#define SERC_RTS_BIT 0
#define SERC_CTS_PORT PCDR
#define SERC_CTS_BIT 1

const long baud_rate = 115200L;

main()
{
	auto char send_buffer[128];
	auto int received;
	auto char fc_flag;
	auto int i;
	auto int j;

   serCopen(baud_rate);

	printf("Starting...\n");

	serCflowcontrolOn();
	fc_flag = 1;
	printf("Flow Control On\n");

	//prepare the pattern in the send buffer
	send_buffer[0] = 0;  //null terminator
	for (i = 0;i < 8;i++)
	{
		for (j=0; j <= i; j++)
		{
			strcat(send_buffer, "*");
		}
		strcat(send_buffer, "\r\n");
	}

	while (1)
	{
		costate
		{
			//send as fast as we can
			for(i = 0; i < 3;i++)
			{
				//do 3 rounds before switching
				//flow control
				waitfordone
				{
					cof_serCputs(send_buffer);
				}
			}

			//toggle flow control
			if (fc_flag)
			{
				serCflowcontrolOff();
				fc_flag = 0;
				printf("Flow Control Off\n");
			}
			else
			{
				serCflowcontrolOn();
				fc_flag = 1;
				printf("Flow Control On\n");
			}
		}
		costate
		{
			//receive characters in a leisurely fashion
			waitfordone
			{
				received = cof_serCgetc();
			}
			putchar(received);
	   }
	}
}


