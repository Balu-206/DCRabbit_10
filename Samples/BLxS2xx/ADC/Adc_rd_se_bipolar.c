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
/***************************************************************************
	adc_rd_se_bipolar.c

	This sample program is for the BLxS2xx series controllers.

	Description:
	============
   Reads and displays the voltage of all single-ended analog input
   channels. The voltage is calculated from coefficients read from
   the reserved eeprom storage device.

   !!!Important!!! "Power supply outputs must be floating for the
                    following steps to be correct"

   Connections for bipolar mode of operation, +-10V
   ------------------------------------------------
   1. Remove all jumpers from JP4 for voltage measurements.
   2. Connect the positive power supply lead to an input ADC channel.
	3.	Connect the negative power supply lead to AGND on the controller.

   To obtain negative voltage values for the ADC to read, reverse the
   power leads as follows:
   1. Connect the negative power supply lead of to an input ADC channel.
	2.	Connect the positive power supply lead to AGND on the controller.

	Instructions:
	=============
	1. Compile and run this program.
	2. Follow the prompted directions of this program during execution.
	3. Voltage will be continuously displayed for all channels.

***************************************************************************/
#class auto	 // Change local var storage default to "auto"

// include BLxS2xx series library
#use "BLxS2xx.lib"

// set the STDIO cursor location and display a string
void DispStr(int x, int y, char *s)
{
   x += 0x20;
   y += 0x20;
   printf ("\x1B=%c%c%s", x, y, s);
}

// blank the stdio screen
void  blankScreen(void)
{
   printf("\x1Bt");
}

void printrange( void )
{
	printf("\n\n");
   printf("Gain code\tVoltage range\n");
	printf("---------\t-------------\n");
	printf("    0    \t +- 10.0v\n");
	printf("    1    \t +- 5.00v\n");
	printf("    2    \t +- 2.50v\n");
	printf("    3    \t +- 2.00v\n");
	printf("    4    \t +- 1.25v\n");
	printf("    5    \t +- 1.00v\n");
}


void main ()
{
	auto int channel, keypress;
	auto int key;
	auto int gaincode;
	auto float voltage;
   auto int mode;
   auto char s[128];

   // Initialize the controller
	brdInit();

	// Configure channels 0 to 7 for Single-Ended bipolar mode of operation.
   // (Max voltage range is �10V)
   for (channel = 0; channel < BL_ANALOG_IN; ++channel)
   {
      anaInConfig(channel, SE1_MODE);
   }

	while (1)
	{
     	printrange();
		printf(" Choose gain code (0-5) =  ");
		do
		{
      	while(!kbhit()); // wait for key hit
			key = getchar() - '0';
		} while (key < 0 || key > 5);
		gaincode = key;
		printf("%d", gaincode);

      blankScreen();
		DispStr(1, 2,  "A/D input voltage for channels 0 - 7");
		DispStr(1, 3,  "------------------------------------");
   	DispStr(1, 14, "Press key to select another gain option.");

   	while(1)
      {
			for (channel = 0; channel < BL_ANALOG_IN; ++channel)
			{
      		voltage = anaInVolts(channel, gaincode);
            if (voltage > BL_ERRCODESTART)
            {
            	// valid read
         		sprintf(s, "Channel = %2d Voltage = %.3fV              ",
               			channel, voltage);
         	}
            else if (voltage == BL_NOT_CAL)
            {
               sprintf(s, "Channel = %2d Voltage = Not Calibrated     ",
                        channel);
            }
            else
            {
               sprintf(s, "Channel = %2d Voltage = Exceeded Range     ",
                        channel);
            }
            DispStr(1,channel + 4, s);
			}
         if (kbhit())
			{
				getchar();
            blankScreen();
            while (kbhit()) { getchar(); }
            break;
			}
      }
   }
}	//end main

