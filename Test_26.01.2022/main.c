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
* Projekt  : Abgabe Prüfung Gaming Maus
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
* 26.01.2022  F. Russom       V1.0      Neuerstellung
*
\*********************************************************************************/

//uC-Board-Treiber hinzufügen
#include "ucBoardDriver.h"
#define PROGRAMMTAKT_MS                        10
#define IN_MASKE_COMPUTER_STANDBY              0b00000001
#define IN_MASKE_BEWEGUNG_ERKANNT              0b00000010
#define IN_MASKE_SCHNELFEUER_AKTIVIERT         0b00000100
#define IN_MASKE_PREAZISIONSMODUS_AKTIVIERT    0b00001000

#define IN_MASKE_GESCHWINDIGKEIT               0b11100000
#define OFFSET_GESCHWINDIGKEIT                 5 

#define OFF                                    0
#define OUT_MASKE_BELEUCHTUNG_LOGO_BLAU        0b00000001 //0
#define OUT_MASKE_BELEUCHTUNG_LOGO_ROT         0b00000010 //1
#define OUT_MASKE_BELEUCHTUNG_TASTATUR         0b00000100
#define OUT_MASKE_BELEUCHTUNG_SCROLLRAD        0b00001000

#define BALKENGROESSE_LEDS                     4  //der Balken ist 4 LEDs gross
#define OUT_MASKE_BALKEN_GESCHWINDIGKEIT       0b11110000 //der Balken soll von LED4 an gezeichnet werden


#define PERIODE_EIN_SCHNELFEUER_MS            10
#define PERIODE_AUS_SCHNELFEUER_MS            90
#define PERIOD                  (PERIODE_EIN_SCHNELFEUER_MS+PERIODE_AUS_SCHNELFEUER_MS)

#define PERIODE_PREAZISIONSMODUS_MS      200 
#define EIN_ZEIT_PREAZISIONSMODUS_MS     160
//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t StandbySchalter = 0;
    uint8_t SchnellfeuerSchalter = 0;
    uint8_t Geschwindigkeitschalter = 0;
    uint8_t bewegungSchalter = 0;
    uint8_t preazisionsmodunsSchalter = 0;

    uint16_t outBeleuchtungLogoblauLED = 0;
    uint16_t outBeleuchtungLogorotLED = 0;
    uint16_t outBeleuchtungTastaturLED = 0;
    uint16_t outAnzeigeGeschwindigkeitLED = 0;
    uint16_t outBeleuchtungScrollradLED = 0;
    
    uint16_t blinkenSchnellfeuerAktiviert = 0;
    uint16_t blinkenPreazisionsmodunsAktiviert = 0;
    
    uint16_t stoppuhrSchnellfeuer = 0;
    uint16_t stoppuhrPreazisionsmoduns = 0;
    
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        StandbySchalter = switchReadAll() & IN_MASKE_COMPUTER_STANDBY;
        SchnellfeuerSchalter = switchReadAll() & IN_MASKE_SCHNELFEUER_AKTIVIERT;  
        Geschwindigkeitschalter = (switchReadAll() & IN_MASKE_GESCHWINDIGKEIT)>> OFFSET_GESCHWINDIGKEIT;
        bewegungSchalter = switchReadAll() & IN_MASKE_BEWEGUNG_ERKANNT;
        preazisionsmodunsSchalter = switchReadAll() & IN_MASKE_PREAZISIONSMODUS_AKTIVIERT;
        //Verarbeitung-------------------------------------------------------------
        if (StandbySchalter)
        {
            outBeleuchtungTastaturLED = OFF;
            outBeleuchtungScrollradLED = OFF;
            outBeleuchtungLogoblauLED = OFF;
          if (bewegungSchalter)
          {
              outBeleuchtungLogorotLED = OUT_MASKE_BELEUCHTUNG_LOGO_ROT;
          } 
          else
          { 
              outBeleuchtungLogorotLED = OFF;
          }
        } 
        else
        {
            outBeleuchtungTastaturLED = OUT_MASKE_BELEUCHTUNG_TASTATUR;
            outBeleuchtungLogoblauLED = OUT_MASKE_BELEUCHTUNG_LOGO_BLAU;
            
            if (SchnellfeuerSchalter)
            {
                blinkenSchnellfeuerAktiviert = 1;
                outBeleuchtungScrollradLED = OFF;
            } 
            else
            {
                blinkenSchnellfeuerAktiviert = 0;
                outBeleuchtungScrollradLED = OUT_MASKE_BELEUCHTUNG_SCROLLRAD;
            }
            
            outAnzeigeGeschwindigkeitLED = (OUT_MASKE_BALKEN_GESCHWINDIGKEIT>>(BALKENGROESSE_LEDS-Geschwindigkeitschalter))&OUT_MASKE_BALKEN_GESCHWINDIGKEIT;      
            
            if (preazisionsmodunsSchalter)
            {
                blinkenPreazisionsmodunsAktiviert = 1;
            } 
            else
            {
                blinkenPreazisionsmodunsAktiviert = 0;
                
            }
           outBeleuchtungLogorotLED = OFF; 
        }
        
        
        //Ausgabe------------------------------------------------------------------
        if (blinkenSchnellfeuerAktiviert)
        {
            if (stoppuhrSchnellfeuer >= PERIODE_EIN_SCHNELFEUER_MS )
            {
                outBeleuchtungScrollradLED = OFF;
            }
            if (stoppuhrSchnellfeuer >= PERIOD)
            {
                outBeleuchtungScrollradLED = OUT_MASKE_BELEUCHTUNG_SCROLLRAD;
                stoppuhrSchnellfeuer = 0;
            }
        } 
        else
        {
            stoppuhrSchnellfeuer = PERIOD;
        }
        
        if (blinkenPreazisionsmodunsAktiviert)
        {
            if (stoppuhrPreazisionsmoduns >= EIN_ZEIT_PREAZISIONSMODUS_MS )
            {
                outAnzeigeGeschwindigkeitLED = 0;
            }
            if (stoppuhrPreazisionsmoduns >= PERIODE_PREAZISIONSMODUS_MS)
            {
                outAnzeigeGeschwindigkeitLED = OUT_MASKE_BALKEN_GESCHWINDIGKEIT;
                stoppuhrPreazisionsmoduns = 0;
            }
        }
        else
        {
            stoppuhrSchnellfeuer = PERIODE_PREAZISIONSMODUS_MS;
        }
        
        
        ledWriteAll(outBeleuchtungLogoblauLED | outBeleuchtungLogorotLED | outBeleuchtungTastaturLED | outAnzeigeGeschwindigkeitLED | outBeleuchtungScrollradLED );
        
        //Warten------------------------------------------------------------------
        _delay_ms(PROGRAMMTAKT_MS);
        stoppuhrSchnellfeuer = stoppuhrSchnellfeuer + PROGRAMMTAKT_MS;
        stoppuhrPreazisionsmoduns = stoppuhrPreazisionsmoduns + PROGRAMMTAKT_MS;
    }
}

