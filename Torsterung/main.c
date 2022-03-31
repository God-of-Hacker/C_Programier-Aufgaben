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
#define IN_MASKE_BUTTON_GATE_OPEN       (1<<0)
#define IN_MASKE_BUTTON_GATE_CLOSE      (1<<1)
#define IN_MASKE_BUTTON_GATE_STOP       (1<<6)

#define IN_MASKE_SCHALTER_OFFEN                 (1<<0)
#define IN_MASKE_SCHALTER_GESCHLOSSEN           (1<<1)
#define IN_MASKE_SCHALTER_SAFETY_LIGHT_BARRIER  (1<<2)

#define IN_MASKE_LED_GATE_OPPEN         (1<<0)
#define IN_MASKE_LED_GATE_CLOSE         (1<<1)
#define IN_MASKE_LED_LIGHT_BARRIERE     (1<<2)
#define IN_MASKE_LED_SENSOR_ERROR       (1<<3)
#define IN_MASKE_LED_FLASHING_LIGHT     (1<<4)
#define IN_MASKE_LED_MOTOR_OPPEN        (1<<6)
#define IN_MASKE_LED_MOTOR_CLOSE        (1<<7)
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
    uint8_t SchalterSensorSafetyLightBarrier=0;

    
    uint8_t ledGateOpen=0;
    uint8_t ledGateClose =0;
    uint8_t ledLightBariere =0;
    uint8_t ledSensorError =0;
    uint8_t ledFlashingLight =0;
    uint8_t ledMotorOpen =0;
    uint8_t ledMotorClose =0;


    uint16_t outRot=0;
    uint16_t outGruen=0;
    uint16_t outBlau=0;
    
    
    uint8_t blink = 0;
    uint64_t timerBlink_ms = 0;
    zustand_t state = STARTUP;
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inTasterAlt = inTaster;
        inTaster = buttonReadAllPL();
        posFlanken = (inTaster ^ inTasterAlt) & inTaster;
        posFlankeButtonGateOpen = posFlanken & IN_MASKE_BUTTON_GATE_OPEN;
        posFlankeButtonGateClose = posFlanken & IN_MASKE_BUTTON_GATE_CLOSE;
        posFlankeButtonGateStop = posFlanken & IN_MASKE_BUTTON_GATE_STOP;
        SchalterSensorGateOffen = switchReadAll() & IN_MASKE_SCHALTER_OFFEN;
        SchalterSensorGategeschlossen = switchReadAll() & IN_MASKE_SCHALTER_GESCHLOSSEN;
        SchalterSensorSafetyLightBarrier = switchReadAll() & IN_MASKE_SCHALTER_SAFETY_LIGHT_BARRIER;
        
        //Verarbeitung-------------------------------------------------------------
        if (CLOSE | OPEN)
        {
            blink = 1;
        }
        else
        {
            blink = OFF;
            
        }
        
        switch (state)
        {
            case STARTUP:
            blink =OFF;
            lcdLog("Startup");
            if (SchalterSensorGategeschlossen )
            {
                state = STOP ;
            }
            break;
            case STOP:
            lcdLog("Stop");
            ledFlashingLight = OFF;
            ledMotorOpen = OFF;
            ledMotorClose = OFF;
            ledLightBariere = OFF;
            
            blink = OFF;
            ledSensorError = OFF ;
            if (posFlankeButtonGateOpen)
            {
                state = OPEN;
            }
            if (posFlankeButtonGateClose )
            {
                state = CLOSE;
            }
            if (SchalterSensorGateOffen && SchalterSensorGategeschlossen)
            {
                state = ERROR ;
            }
            break;
            case OPEN:
            lcdLog("Open");
            ledGateClose = OFF;
            ledLightBariere = OFF;
            ledMotorOpen = IN_MASKE_LED_MOTOR_OPPEN;
            if (SchalterSensorGategeschlossen && SchalterSensorGateOffen )
            {
                state = ERROR ;
            }
            if (posFlankeButtonGateStop)
            {
                state = STOP ;
            }
            if (SchalterSensorGateOffen)
            {
                state = STOP ;
                ledGateOpen = IN_MASKE_LED_GATE_OPPEN;
                ledMotorOpen = OFF;
            }
            break;
            case CLOSE:
            ledMotorClose = IN_MASKE_LED_MOTOR_CLOSE;
            ledGateOpen = OFF;
            lcdLog("Close");
            ledLightBariere = OFF;
            
            if (SchalterSensorGateOffen && SchalterSensorGategeschlossen)
            {
                state = ERROR ;
            }
            if (posFlankeButtonGateStop )
            {
                state = STOP ;
            }
            if (SchalterSensorSafetyLightBarrier)
            {
                state = SAFETY_STOP;
            }
            if (SchalterSensorGategeschlossen)
            {
                state = STOP ;
                ledGateClose = IN_MASKE_LED_GATE_CLOSE;
            }
            break;
            case ERROR:
            lcdLog("Error");
            if (SchalterSensorGateOffen && SchalterSensorGategeschlossen)
            {
                state = ERROR ;
            }
            else
            {
                ledSensorError = OFF;
                state = STOP;
            }
            
            ledMotorOpen = OFF;
            ledMotorClose = OFF;
            ledLightBariere = OFF;
            blink = OFF;
            ledSensorError = IN_MASKE_LED_SENSOR_ERROR ;
            ledGateOpen = OFF;
            ledGateClose =OFF;
            break;
            case SAFETY_STOP:
            lcdLog("Safety Stop");
            ledLightBariere = IN_MASKE_LED_LIGHT_BARRIERE;
            blink = OFF;
            ledGateOpen = OFF;
            ledGateClose = OFF;
            ledSensorError = OFF;
            ledFlashingLight = OFF;
            ledMotorOpen = OFF;
            ledMotorClose = OFF;
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
                ledFlashingLight = IN_MASKE_LED_FLASHING_LIGHT;
                timerBlink_ms = OFF;
            }
        }
        else
        {
            timerBlink_ms = PERIOD;
        }
        ledWriteAll(ledGateOpen | ledGateClose | ledLightBariere | ledSensorError | ledFlashingLight | ledMotorOpen | ledMotorClose);
        rgbWrite(outRot,outGruen,outBlau);
        timerBlink_ms = timerBlink_ms + PROGRAMMTAKT_MS;
        _delay_ms(PROGRAMMTAKT_MS);
    }
}

