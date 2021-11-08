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

//Hauptprogramm
int main(void)
{
    //Variablen
    //adc Wert der Speisung
    uint16_t inAdcSpeisungEingang = 0;
    //adc Wert am FET
    uint16_t inAdcFET = 0;
    //spannung Speisung
    uint16_t inSpeisungEingang_mv = 0;
    //Spannung FET
    uint16_t inFET_mv = 0;
    //Contoll Loop (digital)
    uint8_t inControlLoop = 0;
    //Schalter (digital)
    uint8_t inSchalterLampe=0;
    
    //Initialisieren
    initBoard(1);
    //Port L Bit 2 auf Ausgang
    pinInitX4PortL(2,OUTPUT);
    //Port L Bit 3 auf Ausgang
    pinInitX4PortL(3, OUTPUT);
    //An Port L Bit 3 0 V ausgeben für Contolloop
    pinWriteX4PortL(3, 0);
    //Port L Bit 4 auf Eingang mit Pullup
    pinInitX4PortL(4,INPUT_PULLUP);
   
    //Unendlichschlaufe
    while(1)
    {
        //Speisung einlesen
        inAdcSpeisungEingang = adcRead(ADC_00_X4_PORTF_BIT0);
        //lcdLogWaitBtn(1,"VCC:%u",inAdcSpeisungEingang);
        //ADC Wert in Spannung (0...12000mv umrechnen)
        inSpeisungEingang_mv = (uint32_t)5000 * inAdcSpeisungEingang * 32 / 1023 / 10;
        //Port L Bit 4 einlesen (zuerst mit init.... initialisieren)
        inControlLoop = !pinReadX4PortL(4);
        //Schalter 1 einlesen
        inSchalterLampe = switchReadAll() & 0x01;
        
        //Eingesteckt?
        if (inControlLoop)
        {
            //Speisung vorhanden
            if (inSpeisungEingang_mv > 11500)
            {
                //Schalter ein?->Lampe soll leuchten
                if (inSchalterLampe)
                {
                    //FET leitend schalten
                    pinWriteX4PortL(2,1);
                    //Spannung FET einlesen
                    inAdcFET = adcRead(ADC_01_X4_PORTF_BIT1);
                    //lcdLogWaitBtn(1,"FET:%u",inAdcFET);
                    inFET_mv = (uint32_t)5000 * inAdcFET * 32 / 1023 / 10;
                    //fliesst Strom?
                    if (inFET_mv > 0)
                    {
                        lcdLog("Lampe ein und OK");
                    }
                    else
                    {
                        lcdLog("Lampe ein und defekt");
                    }
                }
                else
                {
                    //FET sperrend schalten
                    pinWriteX4PortL(2,0);
                    //Spannung FET einlesen
                    inAdcFET = adcRead(ADC_01_X4_PORTF_BIT1);
                    //lcdLogWaitBtn(1,"FET:%u",inAdcFET);
                    inFET_mv = (uint32_t)5000 * inAdcFET * 32 / 1023 / 10;
                    //Leitet die Lampe?
                    if (inFET_mv > 11500)
                    {
                        lcdLog("Lampe aus und OK");
                    }
                    else
                    {
                        lcdLog("Lampe aus und defekt");
                    }
                }
                
                
            }
            else
            {
                lcdLog("Keine Speisung");
            }
        }
        else
        {
            lcdLog("keine Verbindung");
        }
        
        
        
        
    }
}

