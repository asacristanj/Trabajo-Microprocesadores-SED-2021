#include <UTFT.h>
#include <string.h>
#include <Wire.h>

//Declaración de fuentes usadas:
//SmallFont==8x12 pixels
//BigFont==16x16 pixels
//SevenSegNumFont==32x50 pixels
extern uint8_t SmallFont[], BigFont[], SevenSegNumFont[];
UTFT myGLCD(CTE32HR, 38, 39, 40, 41);

void setup()
{
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.setClock(400000);
  randomSeed(analogRead(0));
  myGLCD.InitLCD();
}

void loop()
{

}

void receiveEvent(int howMany) {
  int option;
  int cadena[20];
  char t[50];
  int k = 0;
  while (Wire.available()) {
    cadena[k] = Wire.read();
    Serial.print(cadena[k]);
    k++;
  }
  option = cadena[0];
  if (option == 1) {
    myGLCD.clrScr();
  } else if (option == 2) {
    myGLCD.setColor(cadena[1], cadena[2], cadena[3]);
  } else if (option == 3) {
    myGLCD.setBackColor(cadena[1], cadena[2], cadena[3]);
  } else if (option == 4) {
    for (int i = 5; i < 9; i++) {
      if (cadena[i] == 1) {
        cadena[i - 4] = cadena[i - 4] + 256;
      }
    }
    myGLCD.drawRect(cadena[1], cadena[2], cadena[3], cadena[4]);
  } else if (option == 5) {
    for (int i = 5; i < 9; i++) {
      if (cadena[i] == 1) {
        cadena[i - 4] = cadena[i - 4] + 256;
      }
    }
    myGLCD.fillRect(cadena[1], cadena[2], cadena[3], cadena[4]);
  } else if (option == 6) {
    for (int i = 0; i < cadena[1]; i++) {
      t[i] = cadena[i + 4] + '0' - '0';
    }
    for (int i = cadena[1] ; i < 50; i++) {
      t[i] = '\0';
    }
    for (int i = 2; i < 4; i++) {
      if (cadena[i] == 1) {
        cadena[cadena[1] + 2 + i] = cadena[cadena[1] + 2 + i] + 256;
      }
    }
    myGLCD.print(t, cadena[cadena[1] + 4], cadena[cadena[1] + 5]);
  } else if (option == 7) {
    if (cadena[1] == 0) {
      myGLCD.setFont(SmallFont);
    } else if (cadena[1] == 1) {
      myGLCD.setFont(BigFont);
    } else if (cadena[1] == 2) {
      myGLCD.setFont(SevenSegNumFont);
    }
  }else if (option == 8) {
    for (int i = 4; i < 7; i++) {
      if (cadena[i] == 1) {
        cadena[i - 3] = cadena[i - 3] + 256;
      }
    }
    myGLCD.drawCircle(cadena[1], cadena[2], cadena[3]);
  }else if (option == 9) {
    for (int i = 4; i < 7; i++) {
      if (cadena[i] == 1) {
        cadena[i - 3] = cadena[i - 3] + 256;
      }
    }
    myGLCD.fillCircle(cadena[1], cadena[2], cadena[3]);
  }
}
