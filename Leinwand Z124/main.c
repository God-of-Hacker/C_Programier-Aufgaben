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
#include "ucBoardDriver.h"       //Konstanten definieren

#define IN_MASKE_OBEN   (1<<0)   
#define IN_MASKE_UNTEN  (1<<1)   
#define IN_MASKE_STOP   (1<<6)   

#define IN_MASKE_SCHALER_SENSOR_OBEN    (0b00000001)    
#define IN_MASKE_SCHALER_SENSOR_UNTEN   (0b00000010)    
#define IN_MASKE_SCHALTER_KEIN_SENSOR   (0b00000100)    


#define OFF             (0)
#define PROGRAMMTAKT_MS (10)

typedef enum zustand_t {UNTEN, OBEN, STOP, OBENFAHREN, UNTENFAHREN } zustand_t;

//Hauptprogramm
int main(void)
{
    //Variablen                                     Variable 0 setzen
    uint8_t inTaster=0;
    uint8_t inTasterAlt=0;

    uint16_t outRot=0;
    uint16_t outGruen=0;
    uint16_t outBlau=0;
    
    uint8_t schalterSensorOben=0;
    uint8_t schalterSensorUnten=0;
    uint8_t schalterKeinSensor=0;
    uint8_t posFlanken=0;
    uint8_t posFlankeUnten=0;
    uint8_t posFlankeOben=0;
    uint8_t posFlankeStop=0;
    
    zustand_t state = UNTEN;
    
    //Initialisieren                    Hardware Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe                 Unendlich schleife
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inTasterAlt = inTaster;
        inTaster = buttonReadAllPL();
        posFlanken = (inTaster ^ inTasterAlt) & inTaster;
        posFlankeUnten = posFlanken & IN_MASKE_UNTEN;       //Taster Unten einlesen
        posFlankeOben = posFlanken & IN_MASKE_OBEN;         //Taster Oben einlesen
        posFlankeStop = posFlanken & IN_MASKE_STOP;         //Taster Stop einlesen
        schalterSensorUnten = switchReadAll() & IN_MASKE_SCHALER_SENSOR_UNTEN;      //Sensor Unten einlesen
        schalterSensorOben = switchReadAll() & IN_MASKE_SCHALER_SENSOR_OBEN;        //Sensor Oben einlesen
        schalterKeinSensor = switchReadAll() & IN_MASKE_SCHALTER_KEIN_SENSOR;       //Kein Sensor einlesen
        
        //Verarbeitung-------------------------------------------------------------
        switch (state)
        {
            case UNTEN:
            lcdLog("Unten");            
            outRot = 0;
            outGruen= 0;
            outBlau = 0 ;
            //positive Flanke Taster Oben?
            if (posFlankeOben)
            {  
               //Zustand auf Obenfahren setzen
                state = OBENFAHREN;
            }
           
            
            break;
            
            case OBEN:
            lcdLog("Oben");
            outRot = 1023;
            outGruen= 1023;
            outBlau = 1023;
            //positive Flanke Taster Unten?
            if (posFlankeUnten)                 //positive Flanke Stop?
            {
               //Zustand auf Untenfahren setzen
                state = UNTENFAHREN;
            }
            
            break;//positive Flanke Sensor Oben?//positive Flanke Stop?//positive Flanke Sensor Unten?
            
            case STOP:
            lcdLog("Stop");
            outRot = 1023;
            outGruen= 0;
            outBlau = 0;
            //positive Flanke Taster Oben?
            if (posFlankeOben)
            {
               //Zustand auf Obenfahren setzen
                state = OBENFAHREN;
            }
            if (posFlankeUnten)
            {
                state = UNTENFAHREN;
            }
            break;
            
            case OBENFAHREN:
            lcdLog("Oben fahren");
            outRot = 0;
            outGruen= 0;
            outBlau = 1023;
            if (schalterSensorOben)
            {
                state = OBEN;
            }
            if (posFlankeUnten)
            {
                state = STOP;
            }
             if (posFlankeStop)
             {
                 state = STOP;
             }
            break;
            
            case UNTENFAHREN:
            lcdLog("Unten fahren");
            outRot = 0;
            outGruen= 1023;
            outBlau =0 ;
            if (schalterSensorUnten)
            {
                state = UNTEN;
            }
            if (posFlankeOben)
            {
                state = STOP;
            }
            if (posFlankeStop)
            {
                state = STOP;
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

