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
* Projekt  : Powerbank 2
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
#define SWITCH_ACTIV_HIGH   0 //Sind die Schalter an PortC active Low oder High?
#include "ucBoardDriver.h"

#define POGRAMMTAKT_MS              10

#define IN_MASKE_POWER              0b00000001
#define IN_MASKE_LADE_ANDERES       0b00001000
#define IN_MASKE_LADE_SELBER        0b00000100

#define IN_MASKE_SPANNUNGSMESSUNG   0b11000000
#define OFFSET_SPANNUNGSMESSUNG     6

#define OUT_MASKE_POWER             0b00000001
#define OUT_MASKE_LADEANZEIGE       0b00000010
#define OUT_MASKE_AKKU_LEER         0b00100000

#define BALKENGROESSE_LEDS          3//der Balken ist 3 LEDs gross
#define OUT_MASKE_BALKEN            0b11100000//der Balken soll von LED5 an gezeichnet werden

#define PERIODE_LADEANZEIGE_MS      500 //2Hz -> t = 1/f = 1/2 = 500ms
#define EIN_ZEIT_LADEANZEIGE_MS     250 //50% -> 500ms * 50% = 250ms

#define PERIODE_LADE_SELBER_MS      500 //2Hz -> t = 1/f = 1/2 = 500ms
#define EIN_ZEIT_LADE_SELBER_MS     100 //20% -> 500ms * 20% = 100ms

#define PERIODE_AKKU_LEER_MS        1000 //1Hz -> t = 1/f = 1/1 = 1000ms
#define EIN_ZEIT_AKKU_LEER_MS       200  //20% -> 1000ms * 20% = 200ms

//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t inPower=0;
    uint8_t inLadeAnderes=0;
    uint8_t inLadeSelber=0;
    uint8_t inSpannungsmessung=0;

    uint8_t outPower=0;             // 0000 0001
    uint8_t outPowerBlinken=0;      // 0000 0001
    uint8_t outLadeanzeige=0;       // 0000 0010
    uint8_t outBalken=0;            // 1110 0000
    uint8_t outAkkuLeer=0;          // 0010 0000
    
    uint8_t blinkenLadeanzeigeAktiviert=0;
    uint8_t blinkenLadeSelberAktiviert=0;
    uint8_t blinkenAkkuLeerAktiviert=0;
    
    uint16_t stoppuhrLadeanzeigeMs=0;
    uint16_t stoppuhrLadeSelber=0;
    uint16_t stoppuhrAkkuLeer=0;
    
    //Initialisieren
    initBoard(0);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inPower = switchReadAll() & IN_MASKE_POWER;
        inLadeAnderes = switchReadAll() & IN_MASKE_LADE_ANDERES;
        inSpannungsmessung = (switchReadAll() & IN_MASKE_SPANNUNGSMESSUNG)>>OFFSET_SPANNUNGSMESSUNG;
        inLadeSelber = switchReadAll() & IN_MASKE_LADE_SELBER;
        //Verarbeitung-------------------------------------------------------------
        if (inPower)
        {
            outPower = OUT_MASKE_POWER;
            //blinken aktivieren, wenn schalterLadeAnderes wahr ist
            if (inLadeAnderes)
            {
                blinkenLadeanzeigeAktiviert = 1;
            } 
            else
            {
                blinkenLadeanzeigeAktiviert = 0;
                outLadeanzeige=0;
            }
            //Balken zeichnen
            outBalken = (OUT_MASKE_BALKEN>>(BALKENGROESSE_LEDS-inSpannungsmessung)) & OUT_MASKE_BALKEN;
            
            //wenn spannungsmessung nicht wahr (=0) blinkern akku leer aktivieren
            if (inSpannungsmessung == 0)
            {
                blinkenAkkuLeerAktiviert = 1;
            } 
            else
            {
                blinkenAkkuLeerAktiviert = 0;
                outAkkuLeer = 0;
            }
        }
        else
        {
            outPower = 0;
            blinkenLadeanzeigeAktiviert=0;
            outLadeanzeige=0;
            blinkenAkkuLeerAktiviert=0;
            outAkkuLeer = 0;
            outBalken=0;
        }
        
        if (inLadeSelber)
        {
            if (inSpannungsmessung==3)//akku voll: PWR leuchtet durchgehen
            {
                blinkenLadeSelberAktiviert=0;
                outPowerBlinken=0;
                outPower=1;

            }
            else//akku nicht voll: PWR blinkt
            {
                outPower=0;
                blinkenLadeSelberAktiviert=1;
            }
        }
        else
        {
            blinkenLadeSelberAktiviert=0;
            outPowerBlinken=0;
        }
        
        //Ausgabe------------------------------------------------------------------
        if (blinkenLadeanzeigeAktiviert)
        {
             if (stoppuhrLadeanzeigeMs >= EIN_ZEIT_LADEANZEIGE_MS)
             {
                 outLadeanzeige = 0;
             }
             if (stoppuhrLadeanzeigeMs >= PERIODE_LADEANZEIGE_MS)
             {
                 outLadeanzeige = OUT_MASKE_LADEANZEIGE;
                 stoppuhrLadeanzeigeMs = 0;
             }
        }
        else
        {
            stoppuhrLadeanzeigeMs=PERIODE_LADEANZEIGE_MS;
        }
        
        if (blinkenLadeSelberAktiviert)
        {
             if (stoppuhrLadeSelber >= EIN_ZEIT_LADEANZEIGE_MS)
             {
                 outPowerBlinken = 0;
             }
             if (stoppuhrLadeSelber >= PERIODE_LADE_SELBER_MS)
             {
                 outPowerBlinken = OUT_MASKE_POWER;
                 stoppuhrLadeSelber = 0;
             }
        }
        else
        {
            stoppuhrLadeSelber=PERIODE_LADEANZEIGE_MS;
        }
        
        if (blinkenAkkuLeerAktiviert)
        {
             if (stoppuhrAkkuLeer >= EIN_ZEIT_AKKU_LEER_MS)
             {
                 outAkkuLeer = 0;
             }
             if (stoppuhrAkkuLeer >= PERIODE_AKKU_LEER_MS)
             {
                 outAkkuLeer = OUT_MASKE_AKKU_LEER;
                 stoppuhrAkkuLeer = 0;
             }
        }
        else
        {
            stoppuhrAkkuLeer=PERIODE_AKKU_LEER_MS;
        }
        
        ledWriteAll(outPower | outPowerBlinken | outLadeanzeige | outBalken | outAkkuLeer);
        
        //Warten
        _delay_ms(POGRAMMTAKT_MS);
        stoppuhrLadeanzeigeMs=stoppuhrLadeanzeigeMs + POGRAMMTAKT_MS;
        stoppuhrLadeSelber=stoppuhrLadeSelber + POGRAMMTAKT_MS;
        stoppuhrAkkuLeer=stoppuhrAkkuLeer + POGRAMMTAKT_MS;
    }
}
