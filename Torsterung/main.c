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
* Projekt  : Toesteuerung
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
* 30.03.22    F.Russom        V1.0      Neuerstellung
*
\*********************************************************************************/

//uC-Board-Treiber hinzufügen
#include "ucBoardDriver.h"
#define IN_MASKE_BUTTON_GATE_OPEN       (1<<0)
#define IN_MASKE_BUTTON_GATE_CLOSE      (1<<1)
#define IN_MASKE_BUTTON_GATE_STOP       (1<<6)

#define IN_MASKE_SCHALTER_OFFEN                 (1<<0)
#define IN_MASKE_SCHALTER_GESCHLOSSEN           (1<<1)
#define IN_MASKE_SCHALTER_SAFETY_LIGHT_BARRIER  (1<<2)

#define OUT_MASKE_LED_GATE_OPPEN         (1<<0)
#define OUT_MASKE_LED_GATE_CLOSE         (1<<1)                             //Konstanten definieren
#define OUT_MASKE_LED_LIGHT_BARRIERE     (1<<2)
#define OUT_MASKE_LED_SENSOR_ERROR       (1<<3)
#define OUT_MASKE_LED_FLASHING_LIGHT     (1<<4)
#define OUT_MASKE_LED_MOTOR_CLOSE        (1<<7)
#define OUT_MASKE_LED_MOTOR_OPPEN        (1<<6)
#define OFF                              0

#define ON_TIME             400
#define OFF_TIME            100
#define PERIOD              (ON_TIME+OFF_TIME)

#define PROGRAMMTAKT_MS (100)
typedef enum Zustand_t {STARTUP, STOP, OPEN, CLOSE, ERROR, SAFETY_STOP} zustand_t;

//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t inTaster=0;
    uint8_t inTasterAlt=0;
    uint8_t posFlanken=0;
    uint8_t posFlankeButtonGateOpen=0;
    uint8_t posFlankeButtonGateClose=0;
    uint8_t posFlankeButtonGateStop=0;
    
    uint8_t SchalterSensorGateOffen=0;
    uint8_t SchalterSensorGategeschlossen=0;
    uint8_t SchalterSensorSafetyLightBarrier=0;                                     //Variable 0 setzen
    
    uint8_t ledGateOpen=0;
    uint8_t ledGateClose =0;
    uint8_t ledSafetyLightBariere =0;
    uint8_t ledSensorError =0;
    uint8_t ledFlashingLight =0;
    uint8_t ledMotorOpen =0;
    uint8_t ledMotorClose =0;
    
    uint8_t blink = 0;
    uint64_t timerBlink_ms = 0;
    
    zustand_t state = STARTUP;                                                  //Anfangszustad auf STARTUP setzen
    
    //Initialisieren                                                            //Hardware Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inTasterAlt = inTaster;                                                                                 //Alter Buttonzustand
        inTaster = buttonReadAllPL();                                                                           //Neuer Buttonzustand
        posFlanken = (inTaster ^ inTasterAlt) & inTaster;                                                       //Positive Flanken aus altem und neuem Buttonzustand auslessen
        posFlankeButtonGateOpen = posFlanken & IN_MASKE_BUTTON_GATE_OPEN;                                       //posFlankeButtonGateOpen aus positive Flanken auslessen
        posFlankeButtonGateClose = posFlanken & IN_MASKE_BUTTON_GATE_CLOSE;                                     //posFlankeButtonGateClose aus positive Flanken auslessen
        posFlankeButtonGateStop = posFlanken & IN_MASKE_BUTTON_GATE_STOP;                                       //posFlankeButtonGateStop aus positive Flanken auslessen
        SchalterSensorGateOffen = switchReadAll() & IN_MASKE_SCHALTER_OFFEN;                                    //SchalterSensorGate geöffnet auslessen
        SchalterSensorGategeschlossen = switchReadAll() & IN_MASKE_SCHALTER_GESCHLOSSEN;                        //SchalterSensorGate geschlossen auslessen
        SchalterSensorSafetyLightBarrier = switchReadAll() & IN_MASKE_SCHALTER_SAFETY_LIGHT_BARRIER;            //SchalterSensorSafety light barrier auslessen
        
        //Verarbeitung-------------------------------------------------------------
        if (CLOSE | OPEN)                                   //OPEN oder CLOSE ?
        {
            blink = 1;                                      //blinken aktiv
        }
        else
        {
            blink = OFF;                                    //blingen deaktivieren
            
        }
        
        switch (state)                                      //Zustand
        {
            case STARTUP:                                   //STARTUP
            blink =OFF;                                     //blingen deaktivieren
            ledFlashingLight = OFF;                         //LED flashig light auf 0 setzen
            lcdLog("Startup");
            if (SchalterSensorGategeschlossen )             //SchalterSensorGate geschlossen ?
            {
                state = STOP ;                              //Zustand auf STOP setzen
            }
            break;
            case STOP:                                       //STOP
            lcdLog("Stop");
            ledFlashingLight = OFF;                          //LED flashig light auf 0 setzen
            ledMotorOpen = OFF;                              //motor gate open deaktivieren
            ledMotorClose = OFF;                             //motor gate close deaktivieren
            ledSafetyLightBariere = OFF;                     //LED safty light barrier auf 0 setzen
            blink = OFF;                                     //blingen deaktivieren
            ledSensorError = OFF ;                           //LED sensor error auf 0 stzen
            if (posFlankeButtonGateOpen)                     //posFlankeButtonGateOpen ?
            {
                state = OPEN;                                //Zustand auf OPEN setzen
            }
            if (posFlankeButtonGateClose)                    //posFlankeButtonGateClose ?
            {
                state = CLOSE;                               //Zustand auf CLOSE setzen
            }
            if (SchalterSensorGateOffen && SchalterSensorGategeschlossen)  //SchalterSensorGate geöffnet und SchalterSensorGate geschlossen?
            {
                state = ERROR ;                              //Zustand auf ERROR setzen
            }
            break;
            case OPEN:
            lcdLog("Open");
            ledGateClose = OFF;                              //LED gate closed auf 0 setzen
            ledSafetyLightBariere = OFF;                     //LED safty light barrier auf 0 setzen
            ledMotorOpen = OUT_MASKE_LED_MOTOR_OPPEN;        //motor gate open aktivieren
            if (SchalterSensorGategeschlossen && SchalterSensorGateOffen)  //SchalterSensorGate geschlossen und SchalterSensorGate geöffnet ?
            {
                state = ERROR ;                              //Zustand auf ERROR setzen
            }
            if (posFlankeButtonGateStop)                     //posFlankeButtonGateStop ?
            {
                state = STOP ;                               //Zustand auf STOP setzen
            }
            if (SchalterSensorGateOffen)                     //SchalterSensorGate geöffnet
            {
                ledGateOpen = OUT_MASKE_LED_GATE_OPPEN;      //LED gate opened auslesen
                ledMotorOpen = OFF;                          //motor gate open deaktivieren
                state = STOP ;                               //Zustand auf STOP setzen
            }
            break;
            case CLOSE:
            lcdLog("Close");
            ledMotorClose = OUT_MASKE_LED_MOTOR_CLOSE;       //motor gate close aktivieren
            ledGateOpen = OFF;                               //LED gate opened auf  setzen
            ledSafetyLightBariere = OFF;                     //LED safty light barrier auf 0 setzen
            
            if (SchalterSensorGateOffen && SchalterSensorGategeschlossen)  //SchalterSensorGate geöffnet und SchalterSensorGate  geschlossen?
            {
                state = ERROR ;                                            //Zustand auf ERROR setzen
            }
            if (posFlankeButtonGateStop )                                  //posFlankeButtonGateStop ?
            {
                state = STOP ;                                             //Zustand auf STOP setzen
            }
            if (SchalterSensorSafetyLightBarrier)                          //SchalterSensorSafety light barrier  ?
            {
                state = SAFETY_STOP;                                       //Zustand auf SAFETY_STOP setzen
            }
            if (SchalterSensorGategeschlossen)                             //SchalterSensorGate  geschlossen?
            {
                state = STOP ;                                             //Zustand auf STOP setzen
                ledGateClose = OUT_MASKE_LED_GATE_CLOSE;                   //LED gate closed auslesen
            }
            break;
            case ERROR:
            lcdLog("Error");
            if (SchalterSensorGateOffen && SchalterSensorGategeschlossen)  //SchalterSensorGate geöffnet und SchalterSensorGate  geschlossen?
            {
                state = ERROR ;                                            //Zustand auf ERROR setzen
            }
            else
            {
                state = STOP;                                              //Zustand auf STOP setzen
            }
            
            ledMotorOpen = OFF;                                            //motor gate open deaktivieren
            ledMotorClose = OFF;                                           //motor gate close deaktivieren
            ledSafetyLightBariere = OFF;                                   //LED safty light barrier auf 0 setzen
            blink = OFF;                                                   //blingen deaktivieren
            ledSensorError = OUT_MASKE_LED_SENSOR_ERROR ;                  //LED sensor error auslesen
            ledGateOpen = OFF;                                             //LED gate opened auf  setzen
            ledGateClose =OFF;                                             //LED gate closed auf 0 setzen
            break;
            case SAFETY_STOP:
            lcdLog("Safety Stop");
            ledSafetyLightBariere = OUT_MASKE_LED_LIGHT_BARRIERE;         //LED safty light barrier auslesen
            blink = OFF;                                                  //blingen deaktivieren
            ledGateOpen = OFF;                                            //LED gate opened auf  setzen
            ledGateClose = OFF;                                           //LED gate closed auf 0 setzen
            ledSensorError = OFF;                                         //LED sensor error auf  sezten
            ledFlashingLight = OFF;
            ledMotorOpen = OFF;                                            //motor gate open deaktivieren
            ledMotorClose = OFF;                                           //motor gate close deaktivieren
            if (posFlankeButtonGateOpen)
            {
                state = OPEN ;
            }
            break;

            default:
            //Wird nicht erreicht
            break;
        }
        //Ausgabe------------------------------------------------------------------

        if (blink)
        {
            if (timerBlink_ms >= ON_TIME)
            {
                ledFlashingLight = OFF;
            }
            if (timerBlink_ms >= PERIOD)
            {
                ledFlashingLight = OUT_MASKE_LED_FLASHING_LIGHT;
                timerBlink_ms = OFF;
            }
        }
        else
        {
            timerBlink_ms = PERIOD;
        }
        ledWriteAll(ledGateOpen | ledGateClose | ledSafetyLightBariere | ledSensorError | ledFlashingLight | ledMotorOpen | ledMotorClose);
        timerBlink_ms = timerBlink_ms + PROGRAMMTAKT_MS;
        _delay_ms(PROGRAMMTAKT_MS);
    }
}

