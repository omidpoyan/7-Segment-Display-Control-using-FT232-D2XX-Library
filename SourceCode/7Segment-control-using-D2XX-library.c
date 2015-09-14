	
	//====================================================================================================//
	//    Program to control the 7 Segment Display serially using PC and FT232 (asynchronous mode)        //
	//====================================================================================================//
	
	//====================================================================================================//
	// www.xanthium.in										                                              //
	// Copyright (C) 2015 Rahul.S                                                                         //
	//====================================================================================================//
	
	//====================================================================================================//
	// The Program puts the FT232 chip in Asynchronous Bit Bang Mode using D2xx Library ( 8 bit parallal  //
	// port )and controls the 7 segment display connected to the port.                                    //
	//                                                                                                    //
	//====================================================================================================//
	// Hardware Used:-                                                                                    //
	// USB2SERIAL Converter - FT232 based USB to Serial/RS232/RS485 Converter                             //
	//----------------------------------------------------------------------------------------------------//
	// Pin Configuration       |                                                                          // 
	//-------------------------+                                                                          //
    //  				    +------------+                          LT543                                 //
	//      PC   		    | [FT232RL]  |      +------+       +------------+                             //
	// 	+--------+			|  TXD -> D0 |----->|  7   |------>|a           |                             //                                                    //
	// 	|+------+| 			|  RXD -> D1 |----->|  4   |------>|b  +======+ |                             //                                                 //
	// 	||	    ||		    | ~RTS -> D2 |----->|  L   |------>|c  ||    || |                             //                                                 //
 	// 	|| 	    ||	  USB   | ~CTS -> D3 |----->|  S   |------>|d  ||    || |                             //                      //
	// 	|+------+|=========>| ~DTR -> D4 |----->|  2   |------>|e  +======+ |                             //                                                 //
	// 	/ [] [] 0\		    | ~DSR -> D5 |----->|  4   |------>|f  ||    || |                             //                                               //
	// /[] [] [] 0\		    | ~DCD -> D6 |----->|  4   |------>|g  ||    || |----+                        //                                                     //
	//+------------+ 		| ~RI  -> D7 |----->|      |------>|dp +======+ |    |                        //                                                        //
	//  				    |            |      +------+       +------------+    |                        //
	//  				    +------------+                                       \/                       //
	//                         USB2SERIAL      OCTAL BUFFER     7 SEGMENT DISPLAY                         //
	//                                                          (COMMON CATHODE)                          //
	//====================================================================================================//
	// Compiler  :	gcc 4.8.1 (MinGW)                                                                     //
	// Library   :  ftd2xx.lib                                                                            //
	// Commands  :  gcc -o 7SegctrlD2xx 7Segment-control-using-D2XX-library.c ftd2xx.lib                  //
	// OS        :	Windows(Windows 7)                                                                    //
	// Programmer:	Rahul.S                                                                               //
	// Date	     :	19-june-2015                                                                         //
	//====================================================================================================//
	
	//======================================================================================//
	//  IF YOU ARE USING Archive CDM 2.08.30 WHQL Certified.zip FROM FTDI WEBSITE           //
	//======================================================================================//
	// ftd2xx.h   - CDM 2.08.30 WHQL Certified\CDM v2.08.30 WHQL Certified\ftd2xx.h         //
	// ftd2xx.lib - CDM 2.08.30 WHQL Certified\CDM v2.08.30 WHQL Certified\i386\ftd2xx.lib  //
	//======================================================================================//
	//--------------------------------------------------------------------------------------//
	
	
	//==============================================================================//
	//                               Character codes                                //
	//==============================================================================//
    //   +-----------------------------+	+------------------------------+        //
  	//   |  7 segments of the Display  |    | Hexa decimal characters 0-F  |        //
  	//   +-----------------------------+    +--------------+---------------+        //
  	//   |	0x00  // No Segments ON    |    |  0x3F  // 0  |  0x77 // A    |        //
  	//   |  0x01  // Segment a  ON     |    |  0x06  // 1  |  0x7C // b    |        //
 	//	 |  0x02  // Segment b  ON     |    |  0x5B  // 2  |  0x39 // C    |        //
  	//	 |  0x04  // Segment c  ON     |    |  0x4F  // 3  |  0x5E // d    |        //
  	//	 |  0x08  // Segment d  ON     |    |  0x66  // 4  |  0x79 // E    |        //
  	//	 |  0x10  // Segment e  ON     |    |  0x6D  // 5  |  0x71 // F    |        //
    //	 |  0x20  // Segment f  ON     |    |  0x7D  // 6  |               |        //
    //	 |  0x40  // Segment g  ON     |    |  0x07  // 7  |               |        //
    //	 |  0x80  // Segment dp ON     |    |  0x7F  // 8  |               |        //
  	//   |  0xFF  // All Segments ON   |    |  0x6F  // 9  |               |        //
    //   +-----------------------------+    +--------------+---------------+        //
	//	                                                                            //
  	//   +-------------------+                                                      //
 	//	 |English Characters |	                                                    //
  	//   +-------------------+                                                      //
  	//	 |	0x76 // H        |                                                      //
    //	 |  0x38 // L        |                                                      //
    //	 |  0x54 // n        |                                                      //
    //   |  0x73 // P        |                                                      //
  	//   |  0x78 // t        |                                                      //
  	//   |  0x3E // U        |                                                      //
  	//	 |  0x72 // Y        |                                                      //
	//   +-------------------+                                                      //
	//==============================================================================//
	
	#include <stdio.h>
	#include <windows.h>  // for windows specific keywords in ftd2xx.h
	#include "ftd2xx.h"   // Header file for ftd2xx.lib 
	int main(void)
		{
		
			void Heading(void);
			void delay(void);
			FT_HANDLE FT_handle;    // handle to FT 232 chip
			FT_STATUS FT_status;    // status of the FT 232 chip

			UCHAR Mask;             // for selecting which pins are input/output
			UCHAR Mode;             // Selects the mode of operation for the chip
			DWORD BaudRate;         // for selecting the Baud Rate
			
			DWORD BytesWritten = 0; // No of bytes written by FT_Write()
			UCHAR Buffer = 0x0;
			
			UCHAR Segment_Characters[]     = {0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0xFF};
			
			UCHAR Hexadecimal_Characters[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
			UCHAR English_Characters[]     = {0x76,0x38,0x54,0x73,0x78,0x3E,0x72};
			
			Heading();
			FT_status = FT_Open(0,&FT_handle); // Open  a connection to FT232RL
		
			if(FT_status == FT_OK)             // Error Checking
				printf("\n\n\t  Connection to FT232 opened ");
			else 
				printf("\n\t  ERROR! in Opening connection");
			
			Mode = 0x01;              // Select Chip mode as Asynchronous Bit Bang
			Mask = 0xff;              // Direction Mask,8 bit port all output

			FT_status = FT_SetBitMode(FT_handle,Mask,Mode); //sets Asynchronous BBM
		
			if(FT_status == FT_OK)             // Error Checking
				{
					printf("\n\n\t  Mode = 0x%x, Asynchronous Bit Bang Mode Selected",Mode);
					printf("  \n\t  Mask = 0x%x, Direction = All 8 bits output",Mask);
				}
			else 
				printf("\n\t  ERROR! in setting Asynchronous Bit Bang Mode");
			
			BaudRate = 9600;       // Setting the Baud rate to 9600

			FT_status = FT_SetBaudRate(FT_handle,BaudRate); //Sets FT232 to 9600 baud
        
			if(FT_status == FT_OK)             // Error Checking
				{
					printf("\n\n\t  Baudrate Set at %d bps",BaudRate);
				}
			else 
				printf("\n\t  ERROR! in setting Baudrate");
				
			printf("\n\n\t+-------------------------------------------------+");
		    printf("\n\t|Controlling 7 Segment Display using D2XX library |");
		    printf("\n\t+-------------------------------------------------+");
			
			// +-------------------------------------------------------+ //
			// |        Writing to 7 Segment LED Driver from PC        | //
			// +-------------------------------------------------------+ //
			
			//----------------- Writing to individual segments ----------------//
			
			printf("\n\n\t  controlling individual segments ");
			printf("\n\n\t");
			int i =0;
			for (i = 0;i<sizeof(Segment_Characters);i++)
			{
			    Buffer = Segment_Characters[i];
				
				FT_status = FT_Write( FT_handle,       // handle to the chip
								      &Buffer,         // address of the data
                                      sizeof(Buffer),  // Size of the Buffer
                                      &BytesWritten    // Number of bytes written 
                                    );
									
			    if(FT_status == FT_OK)             // Error Checking
					{
						//printf("\n\n\t  0x%x data written",Segment_Characters[i]);
						printf(" * ");
						
					}
				else 
					printf("\n\t  ERROR! in Writing to Port");
					//getchar();
				delay();
			}
			
			//----------------- Writing Hexa decimal characters ----------------//
			
			printf("\n\n\t  Writing Hexadecimal characters ");
			printf("\n\n\t");
			for (i = 0;i<sizeof(Hexadecimal_Characters);i++)
			{
			    Buffer = Hexadecimal_Characters[i];
				
				FT_status = FT_Write( FT_handle,       // handle to the chip
								      &Buffer,         // address of the data
                                      sizeof(Buffer),  // Size of the Buffer
                                      &BytesWritten    // Number of bytes written 
                                    );
									
			    if(FT_status == FT_OK)             // Error Checking
					{
						//printf("\n\n\t  0x%x data written",Hexadecimal_Characters[i]);
						printf(" * ");
					}
				else 
					printf("\n\t  ERROR! in Writing to Port");
					//getchar();
				delay();
			}
			
			//-----------------------------------------------------------------------------//
			//                     Exit from Asynchronous Mode                             //
			//-----------------------------------------------------------------------------//
			
			printf("\n\n\t  Press any key to exit from Asynchronous Bit Bang Mode");		
			getchar();           // Press any key to reset the chip 
		
			Mode = 0x00;         // Mask = 0x00 will reset the chip and-
                             // -get it out of Asynchronous mode
		
			FT_status = FT_SetBitMode(FT_handle,Mask,Mode); // Reset the chip 
		
			if(FT_status == FT_OK)             // Error Checking
				{
					printf("\n\n\t  FT232 exited from Asynchronous Bit Bang Mode");
					printf("\n\n\t  Press any key to exit");
					printf("\n\t+-------------------------------------------------+");
				}
				
			FT_Close(FT_handle); // Close the Serial port connection
			getchar();           // Press any key  to exit
		}

	void Heading(void)
	{
		printf("\n\t+-------------------------------------------------+");
		printf("\n\t|  Asynchronous Bit Bang Mode using D2XX library  |");
		printf("\n\t+-------------------------------------------------+");

	}
	
	void delay()
	{
		int i,j;
		for(i =0; i<999999; i++)
			for(j=0;j<240;j++);
	}