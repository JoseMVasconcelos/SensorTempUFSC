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

//CONSTANTES
unsigned int DHT11_MAX_TEMP = 50;
unsigned int DHT11_MIN_TEMP = 0;

//VARIÁVEIS CONFIGURÁVEIS
float UpperThreshold = 30;
float BottomThreshold = 10;
unsigned int longPressInterval = 2000;

//VARIÁRVIES
bool switchLCDFlag = true;
float currentTemp;
void switchLCDInfo();
bool buttonLongPress(Button pushButton);
bool configModeFlag = false;
bool configModeThresholdFlag = true;
void configMode();

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
    if (!configModeFlag) {
        currentTemp = dht.readTemperature();
        if (switchLCDFlag) {
            lcd.setCursor(0, 0);
            lcd.print("Temperatura:");  
            lcd.setCursor(0, 1);
            lcd.print(currentTemp);
        } else {
            lcd.setCursor(0, 0);
            lcd.print("Lim  Inf   Sup  ");
            lcd.setCursor(5, 1);
            lcd.print(BottomThreshold); 
            lcd.setCursor(11, 1);
            lcd.print(UpperThreshold);
        }
        if (pushButton_1.onPress()) {
            switchLCDInfo();
        }
        if (pushButton_2.onPress()){
            configMode();
    }
    } else {
        if (configModeThresholdFlag) {
            lcd.setCursor(0, 0);
            lcd.print("Lim  SET   Sup  ");
            lcd.setCursor(0, 1);
            lcd.print("-  +");
            lcd.setCursor(5, 1);
            lcd.print(BottomThreshold); 
            lcd.setCursor(11, 1);
            lcd.print(UpperThreshold);
            if (pushButton_1.onPress()) {
                if (buttonLongPress(pushButton_1)) {
                    configMode();
                } else {
                    if ((BottomThreshold-1) >= DHT11_MIN_TEMP) {
                        BottomThreshold -= 1;
                    } else {
                        lcd.clear();
                        lcd.println("Limite inferior");
                        lcd.setCursor(0, 1);
                        lcd.print("alcancado!");
                        delay(1500);
                        lcd.clear();
                    }
                }
            }
            if (pushButton_2.onPress()) {
                if (buttonLongPress(pushButton_2)) {
                    configModeThresholdFlag = !configModeThresholdFlag;
                } else {  
                    if ((BottomThreshold + 1) < UpperThreshold) {
                        BottomThreshold += 1;
                    } else {
                        lcd.clear();
                        lcd.println("Limite superior");
                        lcd.setCursor(0, 1);
                        lcd.print("alcancado!");
                        delay(1500);
                        lcd.clear();
                    }
                }
            }
        } else {
            lcd.setCursor(0, 0);
            lcd.print("Lim  Inf   SET  ");
            lcd.setCursor(0, 1);
            lcd.print("-  +");
            lcd.setCursor(5, 1);
            lcd.print(BottomThreshold); 
            lcd.setCursor(11, 1);
            lcd.print(UpperThreshold);
            if (pushButton_1.onPress()) {
                if (buttonLongPress(pushButton_1)) {
                    configMode();
                } else {
                    if ((UpperThreshold - 1) > BottomThreshold) {
                        UpperThreshold -= 1;
                    } else {
                        lcd.clear();
                        lcd.println("Limite inferior");
                        lcd.setCursor(0, 1);
                        lcd.print("alcancado!");
                        delay(1500);
                        lcd.clear();
                    }
                }
            }
            if (pushButton_2.onPress()) {
                if (buttonLongPress(pushButton_2)) {
                    configModeThresholdFlag = !configModeThresholdFlag;
                } else {
                    if ((UpperThreshold + 1) <= DHT11_MAX_TEMP) {
                        UpperThreshold += 1;
                    } else {
                        lcd.clear();
                        lcd.print("Limite maximo");
                        lcd.setCursor(0, 1);
                        lcd.print("alcancado!");
                        delay(1500);
                        lcd.clear();
                    }
                }
            }
        }
        
    }


    if (currentTemp > UpperThreshold || currentTemp < BottomThreshold) {
        ledB.dim(255);
    } else {
        ledB.off();
    }
}
void switchLCDInfo() {
    switchLCDFlag = !switchLCDFlag;
    lcd.clear();
}

void configMode() {
    configModeFlag = !configModeFlag;
    lcd.clear();
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
