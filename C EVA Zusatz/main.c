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

#define     IN_MASKE_SENSOR_ZEUGHAUSSTR         (1<<0)
#define     IN_MASKE_SENSOR_WILDBACHSTR_NORD    (1<<1)
#define     IN_MASKE_SENSOR_WILDBACHSTR_SUED    (1<<2)
#define     IN_MASKE_SENSOR_OBERMUEHLSTR_NORD   (1<<3)
#define     IN_MASKE_SENSOR_OBERMUEHLSTR_SUED   (1<<4)
#define     IN_MASKE_SENSOR_MATTENBACHSTR_WEST  (1<<5)
#define     IN_MASKE_SENSOR_MATTENBACHSTR_SUED  (1<<6)

#define     OUT_MASKE_BELEUCHTUNG_ZEUGHAUSSTR     (0x01)
#define     OUT_MASKE_BELEUCHTUNG_WILDBACHSTR     (0x02)
#define     OUT_MASKE_BELEUCHTUNG_QUATIER_C       (0x04)
#define     OUT_MASKE_BELEUCHTUNG_QUATIER_D       (0x08)
#define     OUT_MASKE_BELEUCHTUNG_QUATIER_F       (0x10)
#define     OUT_MASKE_BELEUCHTUNG_OBERMUEHLSTR    (0x20)
#define     OUT_MASKE_BELEUCHTUNG_MATTENBACHSTR   (0x40)
#define     OFF                                       0

//Hauptprogramm
int main(void)
{
    //Variablen
    uint8_t schalter = 0;
    uint8_t S5 = 0; 
    uint8_t S6 = 0; 
    uint8_t S7 = 0; 
    uint8_t S8 = 0; 
    uint8_t S9 = 0;
    uint8_t S10 = 0;
    uint8_t S11 = 0;
    uint16_t H0 = 0;
    uint16_t H1 = 0;
    uint16_t H2 = 0;
    uint16_t H3 = 0;
    uint16_t H4 = 0;
    uint16_t H5 = 0;
    uint16_t H6 = 0;
    
    //Initialisieren
    initBoard(1);
    
    //Unendlichschlaufe
    while(1)
    {
        //Eingabe------------------------------------------------------------------
        schalter = switchReadAll();
        S5 = schalter & IN_MASKE_SENSOR_ZEUGHAUSSTR;
        S6 = schalter & IN_MASKE_SENSOR_WILDBACHSTR_NORD;
        S7 = schalter & IN_MASKE_SENSOR_WILDBACHSTR_SUED;
        S8 = schalter & IN_MASKE_SENSOR_OBERMUEHLSTR_NORD;
        S9 = schalter & IN_MASKE_SENSOR_OBERMUEHLSTR_SUED;
        S10 = schalter & IN_MASKE_SENSOR_MATTENBACHSTR_WEST;
        S11 = schalter & IN_MASKE_SENSOR_MATTENBACHSTR_SUED;
        
        
        
        //Verarbeitung-------------------------------------------------------------
        
        if (S5)
        {
            H0 = OUT_MASKE_BELEUCHTUNG_ZEUGHAUSSTR;
        } 
        else
        {
            H0 =  OFF;
        }
        if (S6)
        {
            H1 =  OUT_MASKE_BELEUCHTUNG_WILDBACHSTR;
        } 
        else
        {
             H1 = OFF;
        }
       if (S7)
       {
           H1 =  OUT_MASKE_BELEUCHTUNG_WILDBACHSTR;
           H2 =  OUT_MASKE_BELEUCHTUNG_QUATIER_C;
           H3 =  OUT_MASKE_BELEUCHTUNG_QUATIER_D;
           H4 =  OUT_MASKE_BELEUCHTUNG_QUATIER_F;
           
       } 
       else
       {
           
           H2 = OFF;
           H3 = OFF;
           H4 = OFF;
       }
       if (S8 || S9)
       {
           H5 =  OUT_MASKE_BELEUCHTUNG_OBERMUEHLSTR; 
       } 
       else
       {
           H5 =  OFF;
       }
       if (S10 || S11)
       {
           H6 = OUT_MASKE_BELEUCHTUNG_MATTENBACHSTR;
       } 
       else
       {
           H6 = OFF;
       }
      
        
        
        //Ausgabe------------------------------------------------------------------
        
        ledWriteAll(H0 | H1 | H2 | H3 | H4 | H5 | H6);
        
    }
}

