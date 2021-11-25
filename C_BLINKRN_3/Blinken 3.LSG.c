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

#define IN_MASK_POWER       0x01
#define ON                  0xFFFF
#define OFF                 0x0000
#define SYSTEM_TICK_MS      10

#define ON_TIME             400
#define OFF_TIME            500
#define PERIOD              (ON_TIME+OFF_TIME)

//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t inPower = 0;
    uint16_t outLeds = 0;
    uint8_t blink = 0;
    uint64_t timerBlink_ms = 0;
    
    //Initialisieren
    initBoard(0);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inPower = switchReadAll() & IN_MASK_POWER;
        
        //Verarbeitung-------------------------------------------------------------
        if (inPower)
        {
            blink = 1;
        } 
        else
        {
            blink = 0;
            outLeds = OFF;
        }
        
        
        //Ausgabe------------------------------------------------------------------
        if (blink)
        {
            if (timerBlink_ms >= ON_TIME)
            {
                outLeds = OFF;
            }
            if (timerBlink_ms >= PERIOD)
            {
                outLeds = ON;
                timerBlink_ms = 0;
            }
        }
        else
        {
            timerBlink_ms = PERIOD;
        }        
        
        
        ledWriteAll(outLeds);
        
        //Warten-------------------------------------------------------------------
        _delay_ms(SYSTEM_TICK_MS);
        timerBlink_ms = timerBlink_ms + SYSTEM_TICK_MS;
    }
}

