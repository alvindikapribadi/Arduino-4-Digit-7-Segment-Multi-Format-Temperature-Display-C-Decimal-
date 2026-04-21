/*
============================================================
📌 Project Title
Arduino 4 Digit 7 Segment Multi Format Temperature Display (4 Modes)
[COMMON ANODE VERSION]

📝 Description
This project displays temperature on a 4-digit 7-segment display
in 4 different formats:

1. XX°C   (integer + degree + C)
2. XX.X°  (decimal + degree)
3. XX.XC  (decimal + C)
4. XX.X   (numeric only)

Features:
- Multiplexing 4-digit display
- Decimal point control
- Degree (°) and Celsius (C) symbols
- Clean and modular code

⚙️ Components List
- Arduino UNO / Nano
- 4 Digit 7 Segment Display (Common Anode)
- 8x Resistor 220Ω (for segments a–g + dp)
- Jumper wires

🔌 Wiring

Segment Pins (WITH resistor 220Ω):
D2  → Resistor → Segment a
D3  → Resistor → Segment b
D4  → Resistor → Segment c
D5  → Resistor → Segment d
D6  → Resistor → Segment e
D7  → Resistor → Segment f
D8  → Resistor → Segment g
D13 → Resistor → Segment dp

Digit Pins (NO resistor):
D9  → Digit 1
D10 → Digit 2
D11 → Digit 3
D12 → Digit 4

Notes:
- COMMON ANODE logic:
  Segment LOW  = ON
  Digit   HIGH = ON
============================================================
*/

// ======================
// PIN SETUP
// ======================

int seg[]   = {2,3,4,5,6,7,8,13};
int digit[] = {9,10,11,12};

// ======================
// CHARACTER MAP
// ======================

byte num0[] = {0,0,0,0,0,0,1,1};
byte num1[] = {1,0,0,1,1,1,1,1};
byte num2[] = {0,0,1,0,0,1,0,1};
byte num3[] = {0,0,0,0,1,1,0,1};
byte num4[] = {1,0,0,1,1,0,0,1};
byte num5[] = {0,1,0,0,1,0,0,1};
byte num6[] = {0,1,0,0,0,0,0,1};
byte num7[] = {0,0,0,1,1,1,1,1};
byte num8[] = {0,0,0,0,0,0,0,1};
byte num9[] = {0,0,0,0,1,0,0,1};

byte degree[] = {0,0,1,1,1,0,0,1};
byte C_char[] = {0,1,1,0,0,0,1,1};
byte OFF[]    = {1,1,1,1,1,1,1,1};

// ======================
// SETUP
// ======================

void setup() {
  for(int i=0;i<8;i++) pinMode(seg[i], OUTPUT);
  for(int i=0;i<4;i++) pinMode(digit[i], OUTPUT);

  for(int i=0;i<4;i++) digitalWrite(digit[i], LOW);
}

// ======================
// DISPLAY CORE
// ======================

void displayDigit(byte data[], int d) {

  for(int i=0;i<4;i++) digitalWrite(digit[i], LOW);

  for(int i=0;i<8;i++) digitalWrite(seg[i], HIGH);

  delayMicroseconds(200);

  for(int i=0;i<8;i++) digitalWrite(seg[i], data[i]);

  digitalWrite(digit[d], HIGH);

  delay(2);
}

void show4(byte d1[], byte d2[], byte d3[], byte d4[], int duration){
  unsigned long start = millis();
  while(millis() - start < duration){
    displayDigit(d1,0);
    displayDigit(d2,1);
    displayDigit(d3,2);
    displayDigit(d4,3);
  }
}

// ======================
// HELPER
// ======================

byte* getNum(int n){
  switch(n){
    case 0: return num0;
    case 1: return num1;
    case 2: return num2;
    case 3: return num3;
    case 4: return num4;
    case 5: return num5;
    case 6: return num6;
    case 7: return num7;
    case 8: return num8;
    case 9: return num9;
  }
  return OFF;
}

void copyDigit(byte src[], byte dest[], bool dp){
  for(int i=0;i<8;i++) dest[i] = src[i];
  if(dp) dest[7] = 0; // DP ON
}

// ======================
// DISPLAY MODES
// ======================

// 1. XX°C
void showFormat1(int temp, int duration){
  show4(
    getNum((temp/10)%10),
    getNum(temp%10),
    degree,
    C_char,
    duration
  );
}

// 2. XX.X°
void showFormat2(float temp, int duration){
  int t = temp * 10;

  byte b1[8], b2[8], b3[8];

  copyDigit(getNum((t/100)%10), b1, false);
  copyDigit(getNum((t/10)%10),  b2, true);
  copyDigit(getNum(t%10),       b3, false);

  show4(b1, b2, b3, degree, duration);
}

// 3. XX.XC
void showFormat3(float temp, int duration){
  int t = temp * 10;

  byte b1[8], b2[8], b3[8];

  copyDigit(getNum((t/100)%10), b1, false);
  copyDigit(getNum((t/10)%10),  b2, true);
  copyDigit(getNum(t%10),       b3, false);

  show4(b1, b2, b3, C_char, duration);
}

// 4. XX.X
void showFormat4(float temp, int duration){
  int t = temp * 10;

  byte b1[8], b2[8], b3[8];

  copyDigit(getNum((t/100)%10), b1, false);
  copyDigit(getNum((t/10)%10),  b2, true);
  copyDigit(getNum(t%10),       b3, false);

  show4(b1, b2, b3, OFF, duration);
}

// ======================
// LOOP
// ======================

void loop() {

  showFormat1(27, 3000);    // 27°C
  showFormat2(27.5, 3000);  // 27.5°
  showFormat3(27.5, 3000);  // 27.5C
  showFormat4(27.5, 3000);  // 27.5
}