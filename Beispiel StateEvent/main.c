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

#define IN_MASKE_POWER  (1<<0)
#define IN_MASKE_FARBWECHSEL  (1<<1)

#define OFF             (0)
#define PROGRAMMTAKT_MS (10)

typedef enum zustand_t {AUS, ROT, GRUEN, BLAU } zustand_t;

//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t inTaster=0;
    uint8_t inTasterAlt=0;

    uint16_t outRot=0;
    uint16_t outGruen=0;
    uint16_t outBlau=0;
    
    uint8_t posFlanken=0;
    uint8_t posFlankePower = 0;
    uint8_t posFlankePowerFarbwechsel = 0;
    
    zustand_t state = AUS;
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inTasterAlt = inTaster;
        inTaster = buttonReadAllPL();
        posFlanken = (inTaster ^ inTasterAlt) & inTaster;
        posFlankePower = posFlanken & IN_MASKE_POWER;
        posFlankePowerFarbwechsel = posFlanken & IN_MASKE_FARBWECHSEL;
        //Verarbeitung-------------------------------------------------------------
        switch (state)
        {
            case AUS:
            outRot = 0;
            outGruen= 0;
            outBlau = 0;

            if (posFlankePower)
            {
                state = ROT;
            }
            
            break;
            
            case ROT:
            outRot = 1023;
            outGruen= 0;
            outBlau = 0;
            if (posFlankePower)
            {
                state = AUS;
            }
            if (posFlankePowerFarbwechsel)
            {
                state = GRUEN;
            }
            break;
            
            case GRUEN:
            outRot = 0;
            outGruen= 1023;
            outBlau = 0;
            if (posFlankePower)
            {
                state = AUS;
            }
            if (posFlankePowerFarbwechsel)
            {
                state = BLAU;
            }
            break;
            
           case BLAU:
           outRot = 0;
           outGruen= 0;
           outBlau = 1023;
           if (posFlankePower)
           {
               state = AUS;
           }
           if (posFlankePowerFarbwechsel)
           {
               state = ROT;
           }
           break;
            
            default:
            //Wird nicht erreicht
            break;
        }
        
        //Ausgabe------------------------------------------------------------------
        rgbWrite(outRot,outGruen,outBlau);
        _delay_ms(PROGRAMMTAKT_MS);
        
    }
}

