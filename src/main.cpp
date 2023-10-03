#include "Arduino.h"
#include "DHT.h"
#include "LiquidCrystal.h"
#include "Button.h"
#include "LED.h"

//DEFINIÇÃO DE PINOS
#define DHT_PIN_DATA 2
#define LCD_PIN_RS	8
#define LCD_PIN_E	7
#define LCD_PIN_DB4	3
#define LCD_PIN_DB5	4
#define LCD_PIN_DB6	5
#define LCD_PIN_DB7	6
#define PUSHBUTTON_2_PIN_2	9
#define PUSHBUTTON_1_PIN_2	10
#define LEDB_PIN_VIN 11

//BIBLIOTECAS
DHT dht(DHT_PIN_DATA, DHT11);
LiquidCrystal lcd(LCD_PIN_RS,LCD_PIN_E,LCD_PIN_DB4,LCD_PIN_DB5,LCD_PIN_DB6,LCD_PIN_DB7);
Button pushButton_1(PUSHBUTTON_1_PIN_2);
Button pushButton_2(PUSHBUTTON_2_PIN_2);
LED ledB(LEDB_PIN_VIN);

//VARIÁVEIS CONFIGURÁVEIS
float UpperThreshold = 30;
float BottomThreshold = 10;
unsigned int longPressInterval = 3000;

//VARIÁRVIES
bool switchLCDFlag = true;
float currentTemp;
void switchLCDInfo();
bool buttonLongPress(Button pushButton);

void setup() 
{
    Serial.begin(9600);
    while (!Serial);
    Serial.println("Ligado");
    
    dht.begin();
    lcd.begin(16, 2);
    lcd.clear();
    pushButton_1.init();
    pushButton_2.init();

}

void loop() 
{
    currentTemp = dht.readTemperature();
    if (switchLCDFlag) {
        lcd.setCursor(0, 0);
        lcd.print("Temperatura:");  
        lcd.setCursor(0, 1);
        lcd.print(currentTemp);
    } else {
        lcd.setCursor(0, 0);
        lcd.print("Lim  Sup   Inf  ");
        lcd.setCursor(5, 1);
        lcd.print(UpperThreshold);
        lcd.setCursor(11, 1);
        lcd.print(BottomThreshold); 
    }

    if (pushButton_1.onPress()) {
        switchLCDInfo();
    }

    if (buttonLongPress(pushButton_2)){
        lcd.clear();
        lcd.print("TESTE");
        delay(1500);
    }

    while (currentTemp > UpperThreshold || currentTemp < BottomThreshold) {
        ledB.dim(255);
        delay(200);
        ledB.off();
        delay(200);
    }
    ledB.off();
}
    
bool buttonLongPress(Button pushButton) {
    unsigned long buttonHoldStartMillis;
    buttonHoldStartMillis = millis();   
    while (pushButton.read()) {
        if ((millis() - buttonHoldStartMillis > longPressInterval)) {
            return true;
        }
    }
    return false;
}

void switchLCDInfo() {
    switchLCDFlag = !switchLCDFlag;
    lcd.clear();
}

// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
// char menu()
// {

//     Serial.println(F("\nWhich component would you like to test?"));
//     Serial.println(F("(1) DHT22/11 Humidity and Temperature Sensor"));
//     Serial.println(F("(2) LCD 16x2"));
//     Serial.println(F("(3) Mini Pushbutton Switch #1"));
//     Serial.println(F("(4) Mini Pushbutton Switch #2"));
//     Serial.println(F("(menu) send anything else or press on board reset button\n"));
//     while (!Serial.available());

//     // Read data from serial monitor if received
//     while (Serial.available()) 
//     {
//         char c = Serial.read();
//         if (isAlphaNumeric(c)) 
//         {   
            
//             if(c == '1') 
//     			Serial.println(F("Now Testing DHT22/11 Humidity and Temperature Sensor"));
//     		else if(c == '2') 
//     			Serial.println(F("Now Testing LCD 16x2"));
//     		else if(c == '3') 
//     			Serial.println(F("Now Testing Mini Pushbutton Switch #1"));
//     		else if(c == '4') 
//     			Serial.println(F("Now Testing Mini Pushbutton Switch #2"));
//             else if (c == '5')
//                 Serial.println(F("Now Testing LED"));
//             else
//             {
//                 Serial.println(F("illegal input!"));
//                 return 0;
//             }
//             time0 = millis();
//             return c;
//         }
//     }
// }

/*******************************************************

*    Circuito.io is an automatic generator of schematics and code for off
*    the shelf hardware combinations.

*    Copyright (C) 2016 Roboplan Technologies Ltd.

*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.

*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.

*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*    In addition, and without limitation, to the disclaimers of warranties 
*    stated above and in the GNU General Public License version 3 (or any 
*    later version), Roboplan Technologies Ltd. ("Roboplan") offers this 
*    program subject to the following warranty disclaimers and by using 
*    this program you acknowledge and agree to the following:
*    THIS PROGRAM IS PROVIDED ON AN "AS IS" AND "AS AVAILABLE" BASIS, AND 
*    WITHOUT WARRANTIES OF ANY KIND EITHER EXPRESS OR IMPLIED.  ROBOPLAN 
*    HEREBY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT 
*    NOT LIMITED TO IMPLIED WARRANTIES OF MERCHANTABILITY, TITLE, FITNESS 
*    FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND THOSE ARISING BY 
*    STATUTE OR FROM A COURSE OF DEALING OR USAGE OF TRADE. 
*    YOUR RELIANCE ON, OR USE OF THIS PROGRAM IS AT YOUR SOLE RISK.
*    ROBOPLAN DOES NOT GUARANTEE THAT THE PROGRAM WILL BE FREE OF, OR NOT 
*    SUSCEPTIBLE TO, BUGS, SECURITY BREACHES, OR VIRUSES. ROBOPLAN DOES 
*    NOT WARRANT THAT YOUR USE OF THE PROGRAM, INCLUDING PURSUANT TO 
*    SCHEMATICS, INSTRUCTIONS OR RECOMMENDATIONS OF ROBOPLAN, WILL BE SAFE 
*    FOR PERSONAL USE OR FOR PRODUCTION OR COMMERCIAL USE, WILL NOT 
*    VIOLATE ANY THIRD PARTY RIGHTS, WILL PROVIDE THE INTENDED OR DESIRED
*    RESULTS, OR OPERATE AS YOU INTENDED OR AS MAY BE INDICATED BY ROBOPLAN. 
*    YOU HEREBY WAIVE, AGREE NOT TO ASSERT AGAINST, AND RELEASE ROBOPLAN, 
*    ITS LICENSORS AND AFFILIATES FROM, ANY CLAIMS IN CONNECTION WITH ANY OF 
*    THE ABOVE. 
********************************************************/