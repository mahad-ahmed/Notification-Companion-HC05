#include <Arduino.h>
#include <LiquidCrystal.h>

#define LCD_WIDTH 16
// #define LCD_HEIGHT 2
#define SCROLL_DELAY 700

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

String str[2] = {"", ""};
int diff[2];
int offset[2];
unsigned long t = 0;

void setup() {
    lcd.begin(LCD_WIDTH, 2);
    str[0] = "Hello there! How is it?";
    str[1] = "Okay then, bye bye now";
    diff[0] = str[0].length() - LCD_WIDTH;
    diff[1] = str[1].length() - LCD_WIDTH;

    Serial1.begin(9600);
    // Serial.begin(9600);
}

void clearLine(int line) {
    lcd.setCursor(0, line);
    for(int i = 0; i < LCD_WIDTH; i++) {
        lcd.write(" ");
    }
    lcd.setCursor(0, line);
}

void renderLine(int line) {
    clearLine(line);
    if(diff[line] <= 0) {
        // clearLine(line);
        for(int i = 0; i < str[line].length(); i++) {
            lcd.write(str[line].charAt(i));
        }
        return;
    }
    for(int i = 0; i < 16; i++) {
        lcd.write(str[line].charAt(i + offset[line]));
    }
    offset[line] = (offset[line] + 1) % (diff[line]+1);
}

String buffer[2] = {"", ""};
int pos = 0;
void loop() {
    if((millis() - t) > SCROLL_DELAY) {
        renderLine(0);
        renderLine(1);
        t = millis();
    }

    if(Serial1.available()) {
        int n = Serial1.read();
        if(n == '\0') {
            if(pos < 1) {
                pos = 1;
            }
            else {
                pos = 0;
                // Serial.println("Title: "+buffer[0]);
                // Serial.println("Msg: "+buffer[1]);
                str[0] = buffer[0];
                str[1] = buffer[1];
                diff[0] = str[0].length() - LCD_WIDTH;
                diff[1] = str[1].length() - LCD_WIDTH;
                buffer[0] = "";
                buffer[1] = "";
            }
        }
        else {
            buffer[pos] = buffer[pos] + (char) n;
        }
    }
}
