
#include <Keypad.h>

const int LCDpin = 13;
const int PIZOpin = 3;

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'0','#','*','D'}
};

byte rowPins[ROWS] = {11, 10, 9, 8};
byte colPins[COLS] = {7, 6, 5, 4}; 

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String keyCode = "4132";
String enteredKey = "";

void setup() {

	pinMode(PIZOpin, OUTPUT);
	pinMode(LCDpin, OUTPUT);
	digitalWrite(LCDpin, HIGH);
	Serial.begin(9600);
	Serial.flush();
	delay(1250);
	
	setBacklight();
	delay(100);
	clearDisplay();
	delay(100);
	blinkCursorOn();
	writeEnter();

}

void loop() {

	char key = kpd.getKey();
	if(key) {

		switch (key) {
			case 'A':
				verifyKey();
				break;

			case 'B':
				break;

			case 'C':
				break;

			case 'D':
				clearDisplay();
				enteredKey = "";
				writeEnter();
				break;
				
			default:
				serWrite("*");
				enteredKey = enteredKey + key;
		}

	}

}

void writeEnter() {
	Serial.write("Enter> ");
}

void clearDisplay() {
	serCmd(0x01);
	moveCursorTo(0, 0);
}

void moveCursorRight() {
	serCmd(0x14);
}

void moveCursorLeft() {
	serCmd(0x10);
}

void moveCursorTo(short pos, short line) {  // line 1: 0-15, line 2: 64-49, and set Most Significant Bit to 1 (add 128)
	serCmd(128+pos+64*line);
}

void blinkCursorOn() {
	serCmd(0x0D);
}

void blinkCursorOff() {
	serCmd(0x0C);
}

void serWrite(char* c) {
	Serial.write(c);
}

void verifyKey() {

	clearDisplay();
	blinkCursorOff();

	if (keyCode == enteredKey) {
		serWrite("AUTHORIZED");
		tone(PIZOpin, 3000, 35);
		delay(100);
		tone(PIZOpin, 3000, 35);
	} else {
		serWrite("UNAUTHORIZED");
		tone(PIZOpin, 500, 35);
		delay(100);
		tone(PIZOpin, 500, 35);
	}

	enteredKey = "";
	delay(1500);
	clearDisplay();
	blinkCursorOn();
	writeEnter();

}

void serCmd(short cmd) {
	Serial.write(254); // special char (254) to tell SerLCD to accept command
	Serial.write(cmd);
}

void setBacklight() {
	Serial.write(124);
	Serial.write(157);  // 128-157; off - fully on
}

/*
toggle splash screen
	Serial.write(0x7C);
	Serial.write(0x09);

set splash screen
	Serial.write("INITIALIZING...");
	Serial.write(0x7C);
	Serial.write(0x0A);
*/

