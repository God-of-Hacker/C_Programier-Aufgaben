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
* Projekt  : Ozonanlage
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
* 04.05.2022  F. Russom      V1.0      Neuerstellung
*
\*********************************************************************************/

//uC-Board-Treiber hinzufügen
#include "ucBoardDriver.h"
#define IM_MASKE_POWER_SCHALTER             (1<<7)
#define IM_MASKE_ALARM_QUIT_SCHALTER        (1<<6)
#define IM_MASKE_DISORDER_QUIT_SCHALTER     (1<<7)
#define IM_MASKE_OZON_SENSOR                (1<<3)

#define OUT_POWER_LED                       (1<<0)
#define OUT_ALARM_LED                       (1<<1)
#define OUT_DISORDER_LED                    (1<<2)
#define OUT_REMOTE_ALARM_LED                (1<<3)
#define OUT_VALVE_LED                       (1<<4)                                          //Konstanten definieren
#define OFF                                  0

#define ON_TIME                              50
#define OFF_TIME                            150
#define PERIODE                             (ON_TIME + OFF_TIME)

#define DAUER_BIS_FEHLFUNKTION              8000
#define ALARM_FEHLER                       24000

typedef enum zustand_t {AUS, ZU_VIEL_OZON, ZU_WENIG_OZON, ALARM} zustand_t

#define PROGRAMTAKT_MS                      10
//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t powerSchalter=0;
    uint8_t alarmQuitSchalter=0;
    uint8_t disorderQuitSchalter=0;                                                         //Variabeln auf 0 setzen
    uint8_t ozonSensor=0;
    
    uint8_t altTaster=0;
    uint8_t neuTaster=0;
    uint8_t posflanke=0;
    
    
    uint16_t powerLed=0;
    uint16_t alarmLed=0;
    uint16_t disorderLed=0;
    uint16_t remoteAlarmLed=0;
    uint16_t valveLed=0;
    
    uint16_t blinken=0;
    uint16_t timerBlinken=0;
    
    uint16_t timerFehlfunktionOn=0;
    uint16_t timerFehlfunktionOff=0;
    uint16_t timerAlarmFehler=0;

    //Initialisieren
    initBoard(1);                                                                           //Hardware Initialisieren
    
    //Unendlichschlaufe                                                                     //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        altTaster =neuTaster;                                                               //Alter Buttonzustand
        neuTaster = buttonReadAllPL();                                                      //Neuer Buttonzustand
        posflanke = (altTaster ^ neuTaster) & neuTaster;                                    //Positive Flanken aus altem und neuem Buttonzustand auslessen
        powerSchalter = switchReadAll() & IM_MASKE_POWER_SCHALTER;                          //Power schalter
        alarmQuitSchalter = posflanke & IM_MASKE_ALARM_QUIT_SCHALTER;                       //alarm quit Taster  aus positive Flanken auslessen
        disorderQuitSchalter = posflanke & IM_MASKE_DISORDER_QUIT_SCHALTER;                 //disorder quit schalterdisorder quit Taster  aus positive Flanken auslessen
        ozonSensor = switchReadAll() & IM_MASKE_OZON_SENSOR;                                //Ozon Sensor
        //Verarbeitung-------------------------------------------------------------
      
            powerLed= OFF;                                                                  //power Led auf 0 setzen
            alarmLed= OFF;                                                                  //alarm Led auf 0 setzen
            disorderLed= OFF;                                                               //disorder Led auf 0 setzen
            remoteAlarmLed= OFF;                                                            //remoteAlarm Led auf 0 setzen
            valveLed= OFF;                                                                  //valve Led auf 0 setzen
            blinken = 0;                                                                    //blinken auf 0 setzen
            timerFehlfunktionOn=0;                                                          //timerFehlerfunktionOn auf 0 setzen
            timerFehlfunktionOff=0;                                                         //timerFehlerfunktionOff auf 0 setzen
            timerAlarmFehler=0;                                                             //timerAlarmFehler auf 0 setzen
        
        //Ausgabe------------------------------------------------------------------
        if (blinken)                                                                        //blinken auf 1 setzen
        {

            if (timerBlinken>= ON_TIME)                                                     //timerBlinken grösser gleich ON_TIME 50ms ?
            {
                alarmLed = OFF;                                                             //alarm Led 0 setzen
            }
            if (timerBlinken>= PERIODE)                                                     //timerBlinken grösser gleich PERIODE ?
            {
                alarmLed = OUT_ALARM_LED;                                                   //alarm Led 1 setzen
                timerBlinken = 0;                                                           //timerBlinken auf 0 setzen
            }
        }
        else
        {
            timerBlinken = PERIODE;                                                         //timerBlinken gleich PERIODE
        }
        ledWriteAll(powerLed | alarmLed | disorderLed | remoteAlarmLed | valveLed);         //alle led's ausgeben
        //Warten------------------------------------------------------------------
        _delay_ms(PROGRAMTAKT_MS);                                                          //PROGRAMTAKT (10ms) abwarten
        timerFehlfunktionOn = timerFehlfunktionOn + PROGRAMTAKT_MS;                          // timerFehlerfunktionOn hochzählen (10ms)
        timerFehlfunktionOff = timerFehlfunktionOff + PROGRAMTAKT_MS;                        //timerFehlerfunktionOff hochzählen (10ms)
        timerAlarmFehler = timerAlarmFehler + PROGRAMTAKT_MS;                                //timerAlarmFehler hochzählen (10ms)
        timerBlinken = timerBlinken + PROGRAMTAKT_MS;                                       //timerBlinken hochzählen (10ms)
        //lcdLog("s: %u", timerAlarmFehler);
    }
}

