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

#define IN_MASK_SCHALTER_RECHTS     0x01
#define OUT_MASKE_LED_RECHTS        0xFF
#define OFF                         0x0000
#define SYSTEM_TACK_MS              10
// Konstanten definieren
#define IN_MASK_SCHALTER_LINKS      0x02
#define OUT_MASKE_LED_LINKS         0xFF00
#define OFF                         0x0000

#define ON_TIME_RECHTS     200
#define OFF_TIME_RECHTS    50
#define PERIODE_RECHTS     (ON_TIME_RECHTS+OFF_TIME_RECHTS)

#define ON_TIME_LINKS      400
#define OFF_TIME_LINKS     100
#define PERIODE_LINKS      (ON_TIME_LINKS+OFF_TIME_LINKS)
//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t inPower_Rechts = 0;
    uint8_t inPower_Links = 0;
    
    uint16_t outLeds_Rechts = 0;
    uint16_t outLeds_Links = 0;                 //Variable 0 setzen
    
    uint8_t blinken_Rechts = 0;
    uint64_t timerBlinken_Rechts_ms = 0;
    
    uint8_t blinken_Links = 0;
    uint64_t timerBlinken_Links_ms = 0;
    
    //Initialisieren
    initBoard(0);                   //Hardware Initialisieren
    
    //Unendlichschlaufe
    while(1)                        //Unendlich schleife
    {
        //Eingabe------------------------------------------------------------------
        inPower_Rechts = switchReadAll() & IN_MASK_SCHALTER_RECHTS;          //PowerSwitchRechts einlesen
        inPower_Links = switchReadAll() & IN_MASK_SCHALTER_LINKS;            //PowerSwitchLinks einlesen
        //Verarbeitung-------------------------------------------------------------
        if (inPower_Rechts)               //PowerSwitchRechts ?
        {
            blinken_Rechts = 1;           //Blinken Rechts auf 1 setzen
        }
        else
        {
            blinken_Rechts = 0;           //Blinken Rechts auf 0 setzen
            outLeds_Rechts = OFF;         //LED'S Rechts Aus
        }
        if (inPower_Links)                //PowerSwitchLinks ?
        {
            blinken_Links = 1;            //Blinken Links auf 1 setzen

        }
        else
        {
            blinken_Links = 0;                      //Blinken Links auf 0 setzen
            outLeds_Links = OUT_MASKE_LED_LINKS;    //LED'S Links An
        }
               
        //Ausgabe------------------------------------------------------------------
        if (blinken_Rechts)                                   //Blinken Rechts?
        {
            if (timerBlinken_Rechts_ms >= ON_TIME_RECHTS)     //TimerBlinkenRechts grösser gleich OnTimeRechts ?
            {
                outLeds_Rechts = OFF                          //Led's Rechts Aus
            }
            if (timerBlinken_Rechts_ms >= PERIODE_RECHTS)     //TimerBlinkenRechts grösser gleich PeriodeRechts ?
            {
                outLeds_Rechts = OUT_MASKE_LED_RECHTS;        //Led's Rechts An
                timerBlinken_Rechts_ms = 0;                   //TimerBlinkenRechs auf 0 setzen
            }
        }
        else
        {
            timerBlinken_Rechts_ms = PERIODE_RECHTS;          //TimerBlinkenRechts auf PeriodeRechts setzen
        }
        
        if (blinken_Links)                                    //Blinken Links?
        {
            if (timerBlinken_Links_ms >= ON_TIME_LINKS)       //TimerBlinkenLinks grösser gleich OnTimeLinks ?
            {
                outLeds_Links = OFF;                          //Led's Links Aus
            }
            if (timerBlinken_Links_ms >= PERIODE_LINKS)       //TimerBlinkenLinks grösser gleich PeriodeLinks ?
            {
                outLeds_Links = OUT_MASKE_LED_LINKS;          //Led's Links An
                timerBlinken_Links_ms = 0;                    //TimerBlinkenLinks auf 0 setzen
            }
        }
        else
        {
            timerBlinken_Links_ms = PERIODE_LINKS;             //TimerBlinkenLinks auf PeriodeLinks setzen
        }
        
        ledWriteAll(outLeds_Rechts | outLeds_Links);           // alle Led's ausgeben 
        
        //Warten-------------------------------------------------------------------
        _delay_ms(SYSTEM_TACK_MS);                                              //Programmtakt (10ms) abwarten
        timerBlinken_Rechts_ms = timerBlinken_Rechts_ms + SYSTEM_TACK_MS;       //TimerBlinkenRechts hochzählen (10ms)
        timerBlinken_Links_ms = timerBlinken_Links_ms + SYSTEM_TACK_MS;         //TimerBlinkenLinks hochzählen (10ms)
    }
}

