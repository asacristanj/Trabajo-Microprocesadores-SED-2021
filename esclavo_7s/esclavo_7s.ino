#include "string.h"
#include <Wire.h>
//Display 7 Segmetos                                                            A    b   C    d   E    F    g    H   I  J   K    L   M   n   O    P    q   r   S     t   u    V   W   X    y    Z  -   ' '  .
int display7s[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67, 119, 124, 57, 94, 121, 113, 111, 118, 6, 30, 122, 56, 55, 84, 63, 115, 103, 80, 109, 120, 28, 62, 29, 112, 110, 73, 64, 0,  128};
//Display 7 Segmetos                                                            0    1   2    3   4    5    6    7   8  9   10   11  12  13   14  15   16  17  18   19   20  21  22  23   24   25  26  27  28
//Display 7 Segmentos Anodo Común
//Delcaración de Los Pines del Display
byte a = 5;
byte b = 6;
byte c = 7;
byte d = 8;
byte e = 9;
byte f = 10;
byte g = 11;
byte p = 12;
// Declaración de los Pines de los Transistores
byte dig1 = 2;
byte dig2 = 3;
byte dig3 = 4;
int  punto = 0;
float x;
char t[50];
int seleccion = 0;
int longitud_cadena = 0;
//Función que coloca en el puerto de salida los bits comenzando
// desde el pin ini hasta el pin fin
int salir = 0;
void puerto(int bits, int pin) {
  if (salir == 0) {
    for (int i = a; i <= p; i++)
    {
      int escribir = bitRead(bits, i - a);
      digitalWrite(i, escribir);
    }
    digitalWrite(pin, LOW);
    if (seleccion == 0) {
      if (punto == 1 && pin == dig1) //punto en dig1
        digitalWrite(p, HIGH);
      else if (punto == 2 && pin == dig2)
        digitalWrite(p, HIGH);
      else if (punto == 3 && pin == dig3)
        digitalWrite(p, HIGH);
    } else {
      digitalWrite(p, LOW);
    }


    delay(1);
    digitalWrite(pin, HIGH);

  }

}
void escribir7s(int bits1, int bits2, int bits3) {
  puerto(bits1, dig1);
  puerto(bits2,  dig2);
  puerto(bits3,  dig3);
}
//Función encargada de la multiplexación
void mostrarnum(float num) //Rutina mostrar
{
  int dig[3];
  punto = 0;
  if (num < 0 || num > 1000) {
    mostrarpalabra("error");
  } else {
    if (num < 10) {
      num = num * 100;
      punto = 1;
    } else if (num < 100) {
      num = num * 10;
      punto = 2;
    }
    //Dígito Centena
    dig[0] = (num / 100);
    //Dígito Decena
    dig[1] = num / 10 - dig[0] * 10;
    //Dígito Unidad
    dig[2] = (num - dig[0] * 100 - dig[1] * 10);
    escribir7s(display7s[dig[0]], display7s[dig[1]], display7s[dig[2]]);
  }
}
void mostrarpalabra(const char texto[]) //Rutina mostrar
{
  int longi = longitud_cadena;
  int disp[longi + 3];
  int velocidad = 80;
  for (int i = 0; i < longi; i++) {
    for (char j = 'a'; j <= 'z'; j++) {
      if (j == texto[i])
        disp[i] = j - 'a' + 10;
      else if (texto[i] == '-')
        disp[i] = 36;
      else if (texto[i] == ' ')
        disp[i] = 37;
      else if (texto[i] == '.')
      {
        disp[i] = 38;
      }
    }
    for (char j = '0'; j <= '9'; j++)
      if (j == texto[i])
        disp[i] = j - '0';
  }
  for (int i = longi; i <= longi + 3; i++)
    disp[i] = 37;

  //Rutina de Multiplexación

  if (longi <= 3) {
    escribir7s(display7s[disp[0]], display7s[disp[1]], display7s[disp[2]]);
  } else {

    int temp = velocidad;
    while (temp > 0) {
      escribir7s(display7s[37], display7s[37], display7s[disp[0]]);
      temp--;
    }
    temp = velocidad;
    while (temp > 0) {
      escribir7s(display7s[37], display7s[disp[0]], display7s[disp[1]]);
      temp--;
    }
    for (int i = 0; i <= longi; i++) {
      temp = velocidad;
      while (temp > 0) {
        escribir7s(display7s[disp[i]], display7s[disp[i + 1]], display7s[disp[i + 2]]);
        temp--;
      }
      temp = velocidad;
    }
  }
}

//Configura todos los pines como Salidas
void setup() {
  Wire.begin(7);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  //Configura los 8 Pines digitales como SALIDAS
  for (int i = dig1; i <= p; i++) {
    pinMode(i, OUTPUT);
  }
  digitalWrite(12, LOW);
}

//Programa Principal

void loop() {

  if (seleccion == 0) {
    mostrarnum(x);
  } else {
    mostrarpalabra(t);
  }
  salir = 0;
}
void receiveEvent(int howMany) {
  int k=0;
  longitud_cadena = 0;
  float cadena_recibida[50];
  while (Wire.available()) { // loop through all but the last
    cadena_recibida[k] = Wire.read(); // receive byte as a character
    k++;
  }
  longitud_cadena =k-1;
  if (cadena_recibida[0] == 0) {
    seleccion = 0;
    if (cadena_recibida[1] == 0) {
      x = cadena_recibida[2] * 100 + cadena_recibida[3] * 10 + cadena_recibida[4];
    } else if (cadena_recibida[1] == 1) {
      x = cadena_recibida[2] * 10 + cadena_recibida[3] + cadena_recibida[4] / 10.0;
    } else if (cadena_recibida[1] == 2) {
      x = cadena_recibida[2] + cadena_recibida[3] / 10.0 + cadena_recibida[4] / 100.0;
    }
    Serial.println(x);
  } else if (cadena_recibida[0] == 1) {
    seleccion = 1;
    for (int i = 1; i < k; i++) {
      t[i - 1] = cadena_recibida[i];
    }
  }
  salir++;
}
