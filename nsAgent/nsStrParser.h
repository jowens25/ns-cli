/*****************************************************************************
 * Project			: nsAgent
 * Author			: Bryan Wilcutt bwilcutt@yahoo.com
 * Date				: 5-13-18
 * System			: Nano PI
 * File				: nsStrParser.h
 *
 * Description		:
 *
 * This file contains support constructs for nsStrParser.c.  Definitions also
 * include parsing template strings for each support string type (1-10).
 *
 * Written for Novus Power.
 *
 * Copyright (c) Novus Power All Rights Reserved
 *****************************************************************************/
#ifndef NSSTRPARSER_H
#define NSSTRPARSER_H

#include <stdbool.h>

extern bool strParser(char *str);

int readFromShm(char* buff);

/*
	1. Identifier $GPNVS
	2. String ID 1
	3. Time (UTC) hhmmss
	4. Date mmddyy
	5. GPS 1 Lock (Valid) �A� = Valid, �V� = Not Valid, �N� = N/A
	6. GPS 2 Lock (Valid) �A� = Valid, �V� = Not Valid, �N� = N/A
	7. # of Sats in View (1) Greater of GPS or GNSS count, �N� = N/A
	8. # of Sats in View (2) Greater of GPS or GNSS count, �N� = N/A
	9. Channel Fault Byte 0x0000 to 0xFFFF (Hex OR�d value)
	10. Power Supply Fault Byte 0x00 to 0xFF (Hex OR�d value)
	11. Error Message Byte 0x00 to 0xFF (Hex OR�d value)
	12. Antenna 1 �0� = Ok, �1� = Error, �N� = N/A
	13. Antenna 2 �0� = Ok, �1� = Error, �N� = N/A
	14. NMEA Checksum *XX (xor�d value of bytes between $ and *)
*/

#define id1 "s6,s6,s1,s1,d,s1,x4,x4,x2,d,s1"

/*
	1. Identifier $GPNVS
	2. String ID 2
	3. Time (UTC) hhmmss
	4. Date mmddyy
	5. Channel 1 Vrms 0.00 to 3.30 [V]
	6. Channel 2 Vrms 0.00 to 3.30 [V]
	7. Channel 3 Vrms 0.00 to 3.30 [V]
	8. Channel 4 Vrms 0.00 to 3.30 [V]
	9. Channel 5 Vrms 0.00 to 3.30 [V]
	10. Channel 6 Vrms 0.00 to 3.30 [V]
	11. Channel 7 Vrms 0.00 to 3.30 [V]
	12. Channel 8 Vrms 0.00 to 3.30 [V]
	13. NMEA Checksum *XX (xor�d value of bytes between $ and *)
*/

#define id2 "s6,s6,f1.2,f1.2,f1.2,f1.2,f1.2,f1.2,f1.2,f1.2"

/*
	1. Identifier $GPNVS
	2. String ID 3
	3. Time (UTC) hhmmss
	4. Date mmddyy
	5. Power Supply 1 -30.0 to 30.0 [V]
	6. Power Supply 2 -30.0 to 30.0 [V]
	7. Power Supply 3 -30.0 to 30.0 [V]
	8. Power Supply 4 -30.0 to 30.0 [V]
	9. Power Supply 5 -30.0 to 30.0 [V]
	10. Power Supply 6 -30.0 to 30.0 [V]
	11. Power Supply 7 -30.0 to 30.0 [V]
	12. Power Supply 8 -30.0 to 30.0 [V]
	13. Built in Test (BIT) 0 = Ok, 1 = Fail
	14. Temperature (C) -40 to 99
	15. NMEA Checksum *XX (xor�d value of bytes between $ and *)

$GPNVS,3,043109,050818,11.9,0.28,0.00,0.00,-10.4,,,,1,25*4B
*/
#define id3 "s6,s6,s5,s5,s5,s5,s5,s5,s5,s5,s1,s3"

/*
	1. Identifier $GPNVS
	2. String ID 4
	3. Time (UTC) hhmmss
	4. Date mmddyy
	5. Channel 9 Vrms 0.00 to 3.30 [V]
	6. Channel 10 Vrms 0.00 to 3.30 [V]
	7. Channel 11 Vrms 0.00 to 3.30 [V]
	8. Channel 12 Vrms 0.00 to 3.30 [V]
	9. Channel 13 Vrms 0.00 to 3.30 [V]
	10. Channel 14 Vrms 0.00 to 3.30 [V]
	11. Channel 15 Vrms 0.00 to 3.30 [V]
	12. Channel 16 Vrms 0.00 to 3.30 [V]
	13. NMEA Checksum *XX (xor�d value of bytes between $ and *)

$GPNVS,4,044023,050818,0.20,0.20,0.20,0.20,0.20,0.20,0.20,0.20*41

*/

#define id4 "s6,s6,s4,s4,s4,s4,s4,s4,s4,s4"

/* 
	1. Identifier $GPNVS
	2. String ID 5
	3. Time (UTC) hhmmss
	4. Date mmddyy
	5. Potentiometer Value 0 to 63
	6. Fan PWM % 0 to 90
	7. Temperature -40 to 90 [C]
	8. NMEA Checksum *XX (xor�d value of bytes between $ and *)

$GPNVS,5,043629,050818,0,0,25,0000,0000,*4C
*/
#define id5 "s6,s6,s2,s2,s3,,,"


/*
	1. Identifier $GPNVS
	2. String ID 6
	3. Time (UTC) hhmmss
	4. Date mmddyy
	3. Active PCB Assembly 0 or 1
	4. GNSS Lock A = Locked, V = Unlocked
	5. Input Error 0 = Ok, 1 = A Error, 2 = B error
	6. Channel Status Word 0x0000 to 0x7FFF
	7. Primary PS Status 0x00 to 0Xff
	8. Secondary PS Status 0x00 to 0xFF
	9. Active PCB Status 0x00 to 0xFF
	10. Checksum Status 00 to 999
	11. Channel Fault Bin 0x0000 to 0x7FFF
	12. Primary PCB Amp Status 0x0000 to 0x7FFF
	13. Backup PCB Amp Status 0x0000 to 0x7FFF
	14. NMEA Checksum *XX (xor�d value of bytes between $ and *)
*/

#define id6 "s1,s1,s1,s6,s4,s4,s4,s3,s6,s6,s6"
/*
	1. Identifier $GPNVS
	2. String ID 7
	3. Time hhmmss
	4. Date mmddyy
	5. GPS Lock �A� = Valid, �V� = Not Valid
	6. # of Sats in View (1) Greater of GPS or GNSS count, �N� = N/A
	7. Error Byte 0x00 to 0xFF
	8. Freq Diff �999 (last count, clock cycles)
	9. PPS Diff �999 (last count, clock cycles)
	10. Freq Correction Slice �999 (DAC bits, per second)
	11. DAC Value Integer Representation, n x 1/(2^20)
	12. Power Supply Vdc
	13. Power Supply Vdc
	14. NMEA Checksum *XX (xor�d value of bytes between $ and *)
*/

#define id7 "s6,s6,s1,s2,s4,s3,s3,s3,s12,s6,s6"

/* 
	1. Identifier $GPNVS
	2. String ID 8
	3. Discipline Counter 0 = Off, 1 = Disciplined to Synthetic PPS
	4. User Enabled 0 = Off, 1 = On
	5. Event Enabled(System) 0 = Events Disabled, 1 = Events Enabled
	6. GPS Lock Achieved 0 = No Lock, 2 = Locked or previously locked
	7. Event Index 0-99, Current count of events in RAM
	8. Event Errors 0
	9. Event Time Alignmet 2 = LS applied, 1 = GPS, 0 = RTC
	10. Estimated Accuracy 0-999999 [ns]
	11. Edge Detect Direction 0 = Falling Edge, 1 = Rising Edge
	12. NMEA Checksum *XX (xor�d value of bytes between $ and *)
*/

#define id8 "s1,s1,s1,s2,s1,s1,s6,s1"

/*
	1. Identifier $GPNVS
	2. String ID 9
	5. Measured Frequency 9999900.000 to 10000100.000
	6. DAC Volts
	7. Freq
	8. multilength
	9. Antenna Voltage
	10. Output Vrms
	11. Batt Volts
	12. Temperature
	13. Heater Set Temperature
	14. NMEA Checksum *XX (xor�d value of bytes between $ and *)

	
*/
#define id9 "s12,s8,,,s5,s5,,s5,s5"

/*
	1. Identifier $GPNVS
	2. String ID 10
	3. PPS Stability Enabled 0 = Off, 1 = On
	4. PPS Disciplining to GPS 0 = Off, 1 = Actively Synchronized
	5. PPS Output Type 0 = Synthetic PPS, 1 = GPS PPS
	6. PPS Difference �999 (clock cycles)
	7. PPS pull Cal Factor 0.1 to 10.0
	8. PPS active Time Cal Factor 0 to 9
	9. Frequency Variance 0-999 (clock cycles per Loop period)
	10. NMEA Checksum *XX (xor�d value of bytes between $ and *)
*/

#define id10 "s1,s1,s1,s3,s4,s1,s3"

/*
	1. Identifier $GPNVS
	2. String ID 11
	3. Warmup remaining 0-99999
	4. Warmup time met 0=warmup not complete, 1= warmup complete
	5. Holdover Time elapsed 0-999999
	6. Holdover valid, 0=not valid, 1=valid
	7. Frequency valid, 0=not valid, 1=valid
	8. ,
	9. ,
	10. Holdover temperature -40.0 - 99.0
	11. NMEA Checksum *XX (xor�d value of bytes between $ and *)
*/
#define id11 "s5,s1,s6,s1,s1,,,s5"



/*
1. Identifier						$GPNVS
2. String ID						13
3. Priority Discipline Source		0 = GNSS, 1 = 10MHz input, 2 = Optical input
4. Current Discipline Source		0 = GNSS, 1 = 10MHz, 2 = Optical, 3 = Holdover
5. GNSS Lock						0 to 3, 0 = Unlocked, 3 = Fully Locked
6. RF Present						0 = No RF source, 1 = RF Source found
7. Opto Present						0 = No Optical source, 1 = Optical Source Found
8. Loop Lock						1 = Lock, 0 = Loop acquiring lock
9. Reserved
10. NMEA Checksum					*XX (xor’d value of bytes between $ and *)
*/
#define id13 "s1,s1,s1,s1,s1,s1"




/*
1. Identifier						$GPNVS
2. String ID						14
3. Rb Status						8=init,5=laserlock,0=full lock.
4. Rb Alarm							0=ok, 1=mwsignal, 4=lctemp, 5=abctemp, 8=lcabc, 9=allalarm
5. Rb Mode							0x%04
6. Rb steer							%04d
7. Rb discipline status				0=Disc Wait, 1=Disc Good, 2=Holdover
8. Rb last update second			%03u
9. Offset avg						s10
10. Rb used as holdover				0=Using GNSS, 1=Changing State, 2=Using Rb
11. NMEA Checksum					*XX (xor’d value of bytes between $ and *)
*/

#define id14 "s1,s1,s6,s4,s1,,,s1"



/*
1. Identifier						$GPNVS
2. String ID						15
3. Rb mro50 Status					0=init,1=full lock.
4. Rb mro50 loop state				0-7
5. Rb mro50 status word hex			0000 to FFFF
6. Rb fine tune						1600 to 3200
7. Rb fine tune Ref dbl				1600.00 to 3200.00
8. Rb coarse tune					%08x
9. TCXO control volts				float
10. laser control volts				float
11. laser volts						float
12. rb cell temp volts				float
13. diff between mro and gps		int
*/

#define id15 "s1,,s4,s4,,,,,,,,"


/*
	1. Identifier $GPNVS
	2. String ID 16
	3. Time (UTC) hhmmss
	4. Date mmddyy
	5. Channel 1 Vrms 0.00 to 3.30 [V]
	6. Channel 2 Vrms 0.00 to 3.30 [V]
	7. Channel 3 Vrms 0.00 to 3.30 [V]
	8. Channel 4 Vrms 0.00 to 3.30 [V]
	9. Channel 5 Vrms 0.00 to 3.30 [V]
	10. Channel 6 Vrms 0.00 to 3.30 [V]
	11. Channel 7 Vrms 0.00 to 3.30 [V]
	12. Channel 8 Vrms 0.00 to 3.30 [V]
	13. NMEA Checksum *XX (xor�d value of bytes between $ and *)
*/

#define id16 "s6,s6,f1.2,f1.2,f1.2,f1.2,f1.2,f1.2,f1.2,f1.2"


#endif
