/*********************************************************************************\
*
* MMMMMMMMMMMM   SSSSSSSSSSSS   WW   WW   WW   MECHATRONIK
* MM   MM   MM   SS             WW   WW   WW   SCHULE
* MM   MM   MM   SSSSSSSSSSSS   WW   WW   WW   WINTERTHUR
* MM   MM   MM             SS   WW   WW   WW
* MM   MM   MM   SSSSSSSSSSSS   WWWWWWWWWWWW   www.msw.ch
*
*
* Dateiname: main.c
*
* Projekt  : Muster
* Hardware : uC-Board, ATmega2560v von Atmel
*
* Copyright: MSW, E2
*
* Beschreibung:
* =============
* Siehe Prüfungsaufgabe
*
* Portbelegung:
* =============
* Port C:   Schalter
* Port A:   LEDs
* für genauere Beschreibung siehe Hardwarestruktur
*
* Verlauf:
* ========
* Datum:      Autor:         Version   Grund der Änderung:
* dd.mm.yyyy  M. Muster      V1.0      Neuerstellung
*
\*********************************************************************************/





//uC-Board-Treiber hinzufügen
#include "ucBoardDriver.h"

#define SIZE        100

//Hauptprogramm
int main(void)
{
    //Variablen
    uint16_t testArray[SIZE] =
    {
        43281, 22997, 41494, 34816, 12464,  3561, 43247, 23766, 19368,  7613,
        35475, 41126, 21888, 28764, 23863, 25549, 46667, 61837, 32188, 63352,
        14076, 12714, 40745, 27126, 12280, 29764, 12883, 63651, 16275,  4568,
        5510, 29348, 20128,  4002, 36525,  9829, 39160, 46921,  4640, 58713,
        27752, 14700, 63101,  9603, 16441,  6517, 20308, 44992, 10797, 57575,
        18275, 53024, 18787, 63561,  8630, 37153, 34816, 15937, 17232, 30198,
        55355, 57460,  5127, 36927, 15777,  4002, 32450, 18189,  8480, 16396,
        27911, 11533, 16461, 40099, 31239, 53667, 39818,  2253, 62228,  2284,
        5797, 65271,  1783, 61756, 23743, 26248,  2212,  4002, 31030, 47946,
        11933, 22958, 61231, 57095, 39850, 39160, 47316, 53204, 20357,  2753
    };
    
    uint16_t i=0;
    uint8_t counter=0;
    //Initialisieren
    initBoard(1);

    for (i=0; i<SIZE; i=i+1)
    {
        if (testArray[i] == 4002 )
        {
            counter=counter+1;
            
        }
    }
        lcdWriteText(0,0,"Zahl kommt %u x vor", counter);
 
    
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        
        //Verarbeitung-------------------------------------------------------------
        
        //Ausgabe------------------------------------------------------------------
        
    }
}

