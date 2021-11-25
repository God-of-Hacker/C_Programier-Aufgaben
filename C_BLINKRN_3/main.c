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

#define IN_MASK_Pause       0x01
#define ON_PAUSE            0xFF
#define OFF           0x0000
#define SYSTEM_TICK_MS      10

#define IN_MASK_STOP       0x02
#define ON_STOP            0xFF00
#define OFF              0x0000

#define ON_TIME_Pause            200
#define OFF_TIME_Pause            50
#define PERIOD              (ON_TIME_Pause+OFF_TIME_Pause)

#define ON_TIME_STOP           400
#define OFF_TIME_STOP          100
#define PERIOD_STOP             (ON_TIME_STOP+OFF_TIME_STOP)
//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t inPower_Pause = 0;
    uint8_t inPower_Stop = 0;
    uint16_t outLeds_Pause = 0;
    uint16_t outLeds_Stop = 0;
    uint8_t blink_Pause = 0;
    uint64_t timerBlink_Pause_ms = 0;
    uint8_t blink_Stop = 0;
    uint64_t timerBlink_Stop_ms = 0;
    
    //Initialisieren
    initBoard(0);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inPower_Pause = switchReadAll() & IN_MASK_Pause;
        inPower_Stop = switchReadAll() & IN_MASK_STOP;
        //Verarbeitung-------------------------------------------------------------
        if (inPower_Pause)
        {
            blink_Pause = 1;
        }
        else
        {
            blink_Pause = 0;
            outLeds_Pause = OFF;
        }
        if (inPower_Stop)
        {
            blink_Stop = 1;
        }
        else
        {
            blink_Stop = 0;
            outLeds_Stop = OFF;
        }
        
        
        //Ausgabe------------------------------------------------------------------
        if (blink_Pause)
        {
            if (timerBlink_Pause_ms >= ON_TIME_Pause)
            {
                outLeds_Pause = OFF;
            }
            if (timerBlink_Pause_ms >= PERIOD)
            {
                outLeds_Pause = ON_PAUSE;
                timerBlink_Pause_ms = 0;
            }
        }
        else
        {
            timerBlink_Pause_ms = PERIOD;
        }
        
        if (blink_Stop)
        {
            if (timerBlink_Stop_ms >= ON_TIME_STOP)
            {
                outLeds_Stop = OFF;
            }
            if (timerBlink_Stop_ms >= PERIOD_STOP)
            {
                outLeds_Stop = ON_STOP;
                timerBlink_Stop_ms = 0;
            }
        }
        else
        {
            timerBlink_Stop_ms = PERIOD_STOP;
        }
        
        
        ledWriteAll(outLeds_Pause | outLeds_Stop);
       
        
        //Warten-------------------------------------------------------------------
        _delay_ms(SYSTEM_TICK_MS);
        timerBlink_Pause_ms = timerBlink_Pause_ms + SYSTEM_TICK_MS;
        timerBlink_Stop_ms = timerBlink_Stop_ms + SYSTEM_TICK_MS;
    }
}

