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

#define     ON_OFF_SCHALTER         (1<<0)
#define     IN_SELBER_LADE_GERAET      (1<<1)
#define     IN_LADE_GERAET             (1<<2)
#define     IN_SPANNUNGSMESSUNG        0b11000000
#define     IN_OFFSET_SPANNUNG         6

#define     OFF                          0
#define     OUT_POWER_LED             0x01
#define     OUT_LADE_ANZEIGE_LED      0x02
#define     MAX_SPANNUG                 3
#define     OUT_SPANNUG_LED_ANZEIGE    0b11100000
#define     OUT_AKKU_LED             (1<<5)

#define SYSTEM_TICK_MS             10
#define ON_TIME_LADEN             250
#define OFF_TIME_LADEN            250
#define PERIOD_LADEN              (ON_TIME_LADEN+OFF_TIME_LADEN)

#define ON_TIME_SELBER             100
#define OFF_TIME_SELBER            400
#define PERIOD_SELBER              (ON_TIME_SELBER+OFF_TIME_SELBER)

#define ON_AKKU                    200
#define OFF_AKKU                   800
#define PERIOD_AKKU              (ON_AKKU+OFF_AKKU)
//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t inSpannungsmessung_schalter = 0;
    uint8_t On = 0;
    uint8_t geraet_Laden = 0;
    uint8_t selber_laden = 0;
    uint16_t powerLed = 0;
    uint16_t ladenAnzeige_Blinken = 0;
    uint16_t selberLaden_blinken = 0;
    uint16_t ladenAnzeige_Led = 0;
    uint16_t akkuanzeige_Led = 0;
    uint64_t timerBlink_laden_ms = 0;
    uint64_t timerBlink_selber_ms = 0;
    uint64_t akkuBlinken = 0;
    uint64_t timerBlink_akku_ms = 0;
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inSpannungsmessung_schalter = switchReadAll() & IN_SPANNUNGSMESSUNG;
        inSpannungsmessung_schalter = inSpannungsmessung_schalter >> IN_OFFSET_SPANNUNG;
        geraet_Laden = switchReadAll() & IN_LADE_GERAET;
        selber_laden = switchReadAll() & IN_SELBER_LADE_GERAET;
        On = switchReadAll() & ON_OFF_SCHALTER;
        
        //Verarbeitung-------------------------------------------------------------
        
        if (geraet_Laden)
        {
            ladenAnzeige_Blinken = 1;
        }
        else
        {
            ladenAnzeige_Blinken = 0;
            ladenAnzeige_Led = OFF;
        }
        if (selber_laden)
        {
            selberLaden_blinken = 1;
        }
        else
        {
            selberLaden_blinken = 0;
            powerLed = OFF;
        }
        if (inSpannungsmessung_schalter == 0)
        {
            akkuBlinken = 1;
        }
        else
        {
            akkuBlinken = 0;
            akkuanzeige_Led = OFF;
        }
        //Ausgabe------------------------------------------------------------------
        if (On)
        {
            powerLed = OUT_POWER_LED;
            
            if (ladenAnzeige_Blinken)
            {
                if (timerBlink_laden_ms >= ON_TIME_LADEN)
                {
                    ladenAnzeige_Led = OFF;
                }
                if (timerBlink_laden_ms >= PERIOD_LADEN)
                {
                    ladenAnzeige_Led = IN_LADE_GERAET;
                    timerBlink_laden_ms = 0;
                }
            }
            else
            {
                timerBlink_laden_ms = PERIOD_LADEN;
            }
            if (inSpannungsmessung_schalter == 3)
            {
                powerLed = OUT_POWER_LED;
            }
            
            //Einzeiler      wo die LED angezeigt wärden (0b11100000) / welcher Schalter (0b11000000)
            akkuanzeige_Led =  (OUT_SPANNUG_LED_ANZEIGE>>(MAX_SPANNUG-inSpannungsmessung_schalter)) &      // mit dem schalter kann man auf LED binär zählen
            OUT_SPANNUG_LED_ANZEIGE;
            if (inSpannungsmessung_schalter == 0)
            {
                
                if (akkuBlinken)
                {
                    if (timerBlink_akku_ms >= ON_AKKU)
                    {
                        
                        akkuanzeige_Led = OFF;
                    }
                    if (timerBlink_akku_ms >= PERIOD_AKKU)
                    {
                        
                        akkuanzeige_Led = OUT_AKKU_LED;
                        timerBlink_akku_ms = 0;
                    }
                }
                else
                {
                    timerBlink_akku_ms = PERIOD_AKKU;
                }
            }
            
        }
        else
        {
            ladenAnzeige_Led = OFF;
            if (inSpannungsmessung_schalter == 3)
            {
                powerLed = OUT_POWER_LED;
            }
            
        }
        
        if (selberLaden_blinken )
        {
            if (timerBlink_selber_ms >= ON_TIME_SELBER)
            {
                powerLed = OFF;
            }
            if (timerBlink_selber_ms >= PERIOD_SELBER)
            {
                powerLed = OUT_POWER_LED;
                timerBlink_selber_ms = 0;
            }
        }
        else
        {
            timerBlink_selber_ms = PERIOD_SELBER;
        }
        
        
        ledWriteAll(powerLed | ladenAnzeige_Led | akkuanzeige_Led);
        
        //Warten-------------------------------------------------------------------
        _delay_ms(SYSTEM_TICK_MS);
        timerBlink_laden_ms = timerBlink_laden_ms + SYSTEM_TICK_MS;
        timerBlink_selber_ms = timerBlink_selber_ms + SYSTEM_TICK_MS;
        timerBlink_akku_ms = timerBlink_akku_ms + SYSTEM_TICK_MS;
    }
}

