
#include <Keypad.h>

const int LCDpin = 13;

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
// int rx = 0;

void setup() {

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

}

void loop() {

	char key = kpd.getKey();
	if(key) {
		switch (key) {
			case '1':
				serWrite('1');
				break;
			case '2':
				serWrite('2');
				break;
			case '3':
				serWrite('3');
				break;
			case '4':
				serWrite('4');
				break;
			case '5':
				serWrite('5');
				break;
			case '6':
				serWrite('6');
				break;
			case '7':
				serWrite('7');
				break;
			case '8':
				serWrite('8');
				break;
			case '9':
				serWrite('9');
				break;
			case '0':
				serWrite('0');
				break;
			case 'A':
				verifyKey();
				break;
			case 'B':
				blinkCursorOn();
				break;
			case 'C':
				setBacklight();
				break;
			case 'D':
				clearDisplay();
				enteredKey = "";
				break;
			case '#':
				serWrite('#');
				break;
			case '*':
				serWrite('*');
				break;
		}
	}

	// if (Serial.available()) {
	// 	rx = Serial.read();
	// 	Serial.write(rx);
	// }

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

void serWrite(char c) {
	Serial.write(c);
	enteredKey = enteredKey + c;
}

void verifyKey() {

	clearDisplay();

	if (keyCode == enteredKey) {
		Serial.write("AUTHORIZED");
	} else {
		Serial.write("UNAUTHORIZED");
	}

	enteredKey = "";
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

