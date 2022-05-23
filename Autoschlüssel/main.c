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
* dd.mm.yyyy  F. Russom      V1.0      Neuerstellung
*
\*********************************************************************************/

//uC-Board-Treiber hinzufügen
#include "ucBoardDriver.h"
#define IN_MASKE_BUTTON_AUTO_START               (1<<0)
#define IN_MASKE_BUTTON_IGNORIEREN               (1<<1)
#define IN_MASKE_BUTTON_BATTERIE                 (1<<6)

#define IN_MASKE_SCHALTER_ENTSPERREN             (1<<0)
#define IN_MASKE_SCHALTER_FAHRER_TUERE           (1<<1)
#define IN_MASKE_SCHALTER_BEIFAHRER_TUERE        (1<<2)
#define IN_MASKE_SCHALTER_KOFFERRAUM             (1<<3)
#define IN_MASKE_SENSOR_ZUENDUNG                 (1<<4)



#define OUT_MASKE_LED_SPERREN           (1<<0)
#define OUT_MASKE_LED_TUERE_AUF         (1<<1)                             //Konstanten definieren
#define OUT_MASKE_LED_ZUENDUNG          (1<<4)
#define OUT_MASKE_LED_BATTERIE_LEER     (1<<5)
#define OUT_MASKE_LED_AUTO_START        (1<<7)
#define OFF                              0

#define ON_TIME             200
#define OFF_TIME            200
#define PERIOD              (ON_TIME+OFF_TIME)

#define PROGRAMMTAKT_MS (10)
typedef enum Zustand_t {GESPERRT, ENTSPERRT, AUTO_TUERE , MELDUNG, BATTERIE} zustand_t;
//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t inTaster=0;
    uint8_t inTasterAlt=0;
    uint8_t posFlanken=0;
    uint8_t posFlankeButtonAutoStart=0;
    uint8_t posFlankeButtonIgnorieren=0;
    
    uint8_t autogestartet = 0;
    
    uint8_t schalterEntsperren=0;
    uint8_t schalterFahrerTuere=0;
    uint8_t schalterBeifahrerTuere=0;                                     //Variable 0 setzen
    uint8_t schalterKofferraum=0;
    uint8_t sensorZuendung=0;
    uint8_t posFlankeButtonBatterie=0;

    
    uint8_t ledSperren=0;
    uint8_t ledTuereAuf =0;
    uint8_t ledZuendung =0;
    uint8_t ledBatterieLeer =0;
    uint8_t ledAutoStart =0;
    
    uint8_t blink = 0;
    uint64_t timerBlink_ms = 0;
    
    zustand_t state = GESPERRT;
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        inTasterAlt = inTaster;                                                                                 //Alter Buttonzustand
        inTaster = buttonReadAllPL();                                                                           //Neuer Buttonzustand
        posFlanken = (inTaster ^ inTasterAlt) & inTaster;                                                       //Positive Flanken aus altem und neuem Buttonzustand auslessen
        posFlankeButtonAutoStart = posFlanken & IN_MASKE_BUTTON_AUTO_START;                                     //Tater auto start aus positive Flanken auslessen
        posFlankeButtonIgnorieren = posFlanken & IN_MASKE_BUTTON_IGNORIEREN;                                    //posFlankeButtonIgnorieren aus positive Flanken auslessen
        posFlankeButtonBatterie = posFlanken & IN_MASKE_BUTTON_BATTERIE;                                        //Taster Batterie aus positive Flanken auslesen ?
        schalterEntsperren = switchReadAll() & IN_MASKE_SCHALTER_ENTSPERREN;                                    //Schalter Entsperren auslesen
        schalterFahrerTuere = switchReadAll() & IN_MASKE_SCHALTER_FAHRER_TUERE;                                 //Schalter Fahrer Tür auslesen
        schalterBeifahrerTuere = switchReadAll() & IN_MASKE_SCHALTER_BEIFAHRER_TUERE;                           //Schalter Beifahrer Tür auslesen
        schalterKofferraum = switchReadAll() & IN_MASKE_SCHALTER_KOFFERRAUM;                                    //Schalter Kofferraum auslesen
        sensorZuendung = switchReadAll() & IN_MASKE_SENSOR_ZUENDUNG;                                            //Sensor Zündung auslesen
        
        //Verarbeitung-------------------------------------------------------------
        if ( posFlankeButtonBatterie)  //Taster Batterie aus positive Flanken  ?
        {
            state = BATTERIE;                               // auf BATTERIE  setzen;
        }
        if (posFlankeButtonAutoStart && !autogestartet)//Taster Auto Start auf positive Flanken
        {
            autogestartet = 1;          //autogestartet auf 1 setzen;
            ledAutoStart = OUT_MASKE_LED_AUTO_START;                //-Auto Start auf 1 setzen;
        }
        else if (posFlankeButtonAutoStart && autogestartet)//Taster Auto Start auf positive Flanken und autogestartet auf 1
        {
            ledAutoStart = OFF;                     //Auto Start auf 0 setzen;
            autogestartet = 0;               //autogestartet auf 1 setzen;
        }
        switch (state)
        {
            case GESPERRT:
            lcdLog("GESPERRT");
            ledTuereAuf = OFF; //LED-Tür_auf auf 0 setzen
            timerBlink_ms = 0; //timerBlinken auf 0 setzen
            ledSperren = OUT_MASKE_LED_SPERREN;             //- Sperren auf 1 setzen;
            ledBatterieLeer = OFF;              //- Batterie _ leer auf 0 setzen;
            blink = 0;                              //blink auf 0 setzen;
            ledZuendung = 0;                               //Led Zündung auf 0 setzen
            rgbWrite(0,0,0);                                // ausschalten;
            if (schalterEntsperren)   //Schalter Entsperren ?
            {
                state = ENTSPERRT;                      // auf ENTSPERRT  setzen;
            }
            break;
            case ENTSPERRT:
            lcdLog("ENTSPERRT");
            blink = 0;
            rgbWrite(0,0,0);
            ledSperren = OFF; //LED- Sperren auf 0 setzen
            ledBatterieLeer = OFF;              //- Batterie _ leer auf 0 setzen;
            if (schalterFahrerTuere || schalterBeifahrerTuere || schalterKofferraum)//Schalter Fahrer Tür oder Schalter Beifahrer Tür oder Schalter Kofferraum ?
            {
                ledTuereAuf = OUT_MASKE_LED_TUERE_AUF;              //-Tür_auf auf 1 setzen;
                if (posFlankeButtonAutoStart )    //Taster Auto Start aus positive Flanken
                {
                    ledAutoStart = OUT_MASKE_LED_AUTO_START;          //led-Auto Start auf 1 setzen;
                    state = MELDUNG;        // auf MELDUNG setzen;
                }
            }
            else
            {
                ledTuereAuf = OFF; //LED-Tür_auf auf 0 setzen
            }

            if (!schalterEntsperren)   //Schalter Entsperren ?
            {
                state = GESPERRT;                      // auf ENTSPERRT  setzen;
            }
            break;

            case MELDUNG:
            lcdLog("MELDUNG");
            blink = 1;                  //blink auf 1 setzen;
            ledAutoStart = 0;               //-Auto Start auf 0 setzen;
            if (posFlankeButtonIgnorieren)     //Taster Ignorieren aus positive Flanken
            {
                
                ledAutoStart = OUT_MASKE_LED_AUTO_START;          //-Auto Start auf 1 setzen;
                state = GESPERRT;            //state auf GESPERRT setzen
            }
            if (!schalterFahrerTuere && !schalterBeifahrerTuere && !schalterKofferraum)          //Schalter Fahrer Tür und Schalter Beifahrer Tür und Schalter Kofferraum auf 0?
            {
                ledTuereAuf = OFF;                  //-Tür_auf auf 0 setzen;
                ledAutoStart = OUT_MASKE_LED_AUTO_START;          //-Auto Start auf 1 setzen;
                state = ENTSPERRT;                          // auf ENTSPERRT  setzen;
            }
            break;
            case BATTERIE:
            lcdLog("BATTERIE");
            ledBatterieLeer = OUT_MASKE_LED_BATTERIE_LEER;        //- Batterie _ leer auf 1 setzen;
            ledSperren = OFF; //LED- Sperren auf 0 setzen
            blink = 0;                              //blink auf 0 setzen;
            rgbWrite(0,0,0);                                // ausschalten;
            if (sensorZuendung)       //Senor Zündung?
            {
                ledZuendung = OUT_MASKE_LED_ZUENDUNG;                  // Zündung auf 1 setzen;
                if (posFlankeButtonAutoStart)                          //Sensor Zündung und Taster Auto Start aus positive Flanken?
                {
                    state = GESPERRT;                     // auf GESPERRT  setzen;
                }
            }
            else
            {
                ledZuendung = OFF;              // Zündung auf 0 setzen;
            }
            
            break;
            default:
            ;
        }
        
        
        //Ausgabe------------------------------------------------------------------
        //Blinken aktiviert 2.5Hz und einem Impuls-Pausenverhältnis von 1:1
        if (blink)
        {
            if (timerBlink_ms >= ON_TIME)
            {
                rgbWrite(0,0,0);
            }
            if (timerBlink_ms >= PERIOD)
            {
                rgbWrite(100,0,0);
                timerBlink_ms = OFF;
            }
        }
        else
        {
            timerBlink_ms = PERIOD;
        }
        ledWriteAll(ledSperren | ledTuereAuf | ledZuendung | ledBatterieLeer | ledAutoStart);//Leds ausgeben
        timerBlink_ms = timerBlink_ms + PROGRAMMTAKT_MS; // timerBlink_ms hochzählen
        _delay_ms(PROGRAMMTAKT_MS); //Programmtakt (10ms) abwarten
    }
}

