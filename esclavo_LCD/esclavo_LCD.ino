#include <UTFT.h>
#include <string.h>
#include <Wire.h>

//Declaración de fuentes usadas:
//SmallFont==8x12 pixels
//BigFont==16x16 pixels
//SevenSegNumFont==32x50 pixels
extern uint8_t SmallFont[], BigFont[], SevenSegNumFont[];
UTFT myGLCD(CTE32HR, 38, 39, 40, 41);

typedef struct rgb {
  int r, g, b;
} rgb;

class botones {
  public:
    int dist_lateral_pared = 30;
    int dist_altura_boton = 70;
    int ancho_boton = 100;
    int alto_boton = 70;
    int marco_boton = 2;
    int nuevo_marco = 5;
    rgb cboton = {0, 0, 255};
    rgb cmarco = {255, 255, 255};
    rgb cmarcon = {255, 0, 0};
    rgb ctexto = {255, 255, 255};
    int filas = 2;
    int columnas = 3;
    int boton_resaltado = 0;
    int espacio_entre_botones = (480 - ancho_boton * columnas - dist_lateral_pared * 2) / (columnas - 1);
    int alto_entre_botones = (320 - alto_boton * filas - dist_altura_boton * 2) / (filas - 1);
    botones(int distl, int distalt, int ancho, int alto, int marco, int nmarco, rgb color_boton, rgb color_marco, rgb color_marco_nuevo, rgb color_texto, int  f, int c ) {
      dist_lateral_pared = distl;
      dist_altura_boton = distalt;
      ancho_boton = ancho;
      alto_boton = alto;
      marco_boton = marco;
      nuevo_marco = nmarco;
      cboton = color_boton;
      cmarco = color_marco;
      cmarcon = color_marco_nuevo;
      ctexto = color_texto;
      filas = f;
      columnas = c;
      if (columnas != 1) {
        espacio_entre_botones = (480 - ancho_boton * columnas - dist_lateral_pared * 2) / (columnas - 1);
      } else {
        espacio_entre_botones = (480 - ancho_boton * columnas - dist_lateral_pared * 2) / columnas;
      }
      if (filas != 1) {
        alto_entre_botones = (320 - alto_boton * filas - dist_altura_boton * 2) / (filas - 1);
      } else {
        alto_entre_botones = (320 - alto_boton * filas - dist_altura_boton * 2) / filas;
      }
    }
    void crear_botonera() {
      for (int j = 0; j < filas; j++) {
        for (int i = 0; i < columnas; i++) {
          int x = dist_lateral_pared + (espacio_entre_botones + ancho_boton) * i;
          int y = dist_altura_boton + (alto_entre_botones + alto_boton) * j;
          myGLCD.setColor(cmarco.r, cmarco.g, cmarco.b);
          myGLCD.fillRect(x - marco_boton, y - marco_boton, x + ancho_boton + marco_boton, y + alto_boton + marco_boton);
          myGLCD.setColor(cboton.r, cboton.g, cboton.b);
          myGLCD.fillRect(x, y, x + ancho_boton, y + alto_boton);
        }
      }
    }

    void resaltar_boton(int b) {
      int k = 1;
      for (int j = 0; j < filas; j++) {
        for (int i = 0; i < columnas; i++) {
          if (boton_resaltado == k) {
            int x = dist_lateral_pared + (espacio_entre_botones + ancho_boton) * i;
            int y = dist_altura_boton + (alto_entre_botones + alto_boton) * j;
            myGLCD.setColor(0, 0, 0);
            myGLCD.fillRect(x - nuevo_marco, y - nuevo_marco, x + ancho_boton + nuevo_marco, y + alto_boton + nuevo_marco);
            myGLCD.setColor(cmarco.r, cmarco.g, cmarco.b);
            myGLCD.fillRect(x - marco_boton, y - marco_boton, x + ancho_boton + marco_boton, y + alto_boton + marco_boton);
            myGLCD.setColor(cboton.r, cboton.g, cboton.b);
            myGLCD.fillRect(x, y, x + ancho_boton, y + alto_boton);
          }
          k++;
        }
      }
      k = 1;
      boton_resaltado = b;
      for (int j = 0; j < filas; j++) {
        for (int i = 0; i < columnas; i++) {
          if (boton_resaltado == k) {
            int x = dist_lateral_pared + (espacio_entre_botones + ancho_boton) * i;
            int y = dist_altura_boton + (alto_entre_botones + alto_boton) * j;
            myGLCD.setColor(cmarcon.r, cmarcon.g, cmarcon.b);
            myGLCD.fillRect(x - nuevo_marco, y - nuevo_marco, x + ancho_boton + nuevo_marco, y + alto_boton + nuevo_marco);
            myGLCD.setColor(cboton.r, cboton.g, cboton.b);
            myGLCD.fillRect(x, y, x + ancho_boton, y + alto_boton);
          }
          k++;
        }
      }
    }

    void rellenar_botones(const char* t[]) {
      int k = 1;
      int alto_letra = 16;
      int ancho_letra = 16;
      myGLCD.setColor(ctexto.r, ctexto.g, ctexto.b);
      myGLCD.setBackColor(cboton.r, cboton.g, cboton.b);
      for (int j = 0; j < filas; j++) {
        for (int i = 0; i < columnas; i++) {
          int x = dist_lateral_pared + (espacio_entre_botones + ancho_boton) * i;
          int y = dist_altura_boton + (alto_entre_botones + alto_boton) * j;

          if (ancho_boton >= (ancho_letra * strlen(t[k - 1]))) {
            myGLCD.print(t[k - 1], x + (ancho_boton - strlen(t[k - 1])*ancho_letra) / 2, y + (alto_boton - alto_letra) / 2);
          }
          else {
            char t1[20], t2[20];
            for (int i = 0; i < 20; i++)
            {
              t1[i] = 0;
              t2[i] = 0;
            }
            for (int g = 0; g < (ancho_boton / ancho_letra); g++) {
              t1[g] = t[k - 1][g];
            }
            for (int g = (ancho_boton / ancho_letra); g <= strlen(t[k - 1]); g++) {
              t2[g - (ancho_boton / ancho_letra)] = t[k - 1][g];
            }
            myGLCD.print(t1, x + (ancho_boton - strlen(t1)*ancho_letra) / 2, y + (alto_boton - alto_letra * 2) / 2);
            myGLCD.print(t2, x + (ancho_boton - strlen(t2)*ancho_letra) / 2, y + (alto_boton - alto_letra * 2) / 2 + alto_letra);
          }

          k++;
        }
      }
    }
};

void crear_fondo_principal() {
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(1, 1, 478, 40);
  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(1, 318, 478, 280);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.setFont(BigFont);
  myGLCD.print("* CASA DOMOTICA *", CENTER, 12);
  myGLCD.setFont(SmallFont);
  myGLCD.setBackColor(64, 64, 64);
  myGLCD.setColor(255, 255, 0);
  myGLCD.print("Sistemas Electronicos Digitales", CENTER, 281);
  myGLCD.print("Raul Herranz, Alejandro Sacristan, Carlos Murillo", CENTER, 300);
  myGLCD.setColor(0, 0, 255);
  myGLCD.drawRect(0, 0, 479, 319);
}

botones b_menu(30, 70, 100, 70, 2, 5, {0, 0, 255}, {255, 255, 255}, {255, 0, 0}, {255, 255, 255}, 2, 3 );
botones b_luces(30, 70, 150, 140, 2, 5, {0, 0, 255}, {255, 255, 255}, {255, 0, 0}, {255, 255, 255}, 1, 2 );
const char* t_menu[6] = {"LUCES", "PERSIANAS", "RIEGO", "CLIMA", "AUDIO", "SEGURIDAD"};
const char* t_luces[2] = {"AUTOMATICO", "MANUAL"};

void setup()
{
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  randomSeed(analogRead(0));
  myGLCD.InitLCD();
  Serial.begin(9600);
}

void loop()
{
  /*
    char option = ' ';
    if (Serial.available() != 0) {
    option = Serial.read();
    if ((option >= 'a') && (option <= 'f')) {
      b_menu.resaltar_boton(option - 'a' + 1);
      myGLCD.setFont(BigFont);
      b_menu.rellenar_botones(t_menu);
    }
    if (option == 'g') {
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillRect(1, 50, 478, 270);
      b_luces.crear_botonera();
      b_luces.resaltar_boton(0);
      myGLCD.setFont(BigFont);
      b_luces.rellenar_botones(t_luces);
    }
    if ((option >= 'h') && (option <= 'l')) {
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillRect(1, 50, 478, 270);
    }
    if (option == 'm') {
      myGLCD.setColor(0, 0, 0);
      myGLCD.fillRect(1, 50, 478, 270);
      b_menu.crear_botonera();
      b_menu.resaltar_boton(0);
      myGLCD.setFont(BigFont);
      b_menu.rellenar_botones(t_menu);
    }
    }
  */
}

void receiveEvent(int howMany) {
  int option;
  int cadena[100];
  char t[100];
  int k = 0;
  while (Wire.available()) {
    cadena[k] = Wire.read();
    Serial.print(cadena[k]);
    Serial.print(' ');
    k++;
  }
  option = cadena[0];
  if (option == 1) {
    myGLCD.clrScr();
    Serial.println(' ');
  } else if (option == 2) {
    myGLCD.setColor(cadena[1], cadena[2], cadena[3]);
    Serial.println(' ');
  } else if (option == 3) {
    myGLCD.setBackColor(cadena[1], cadena[2], cadena[3]);
    Serial.println(' ');
  } else if (option == 4) {
    for (int i = 5; i < 9; i++) {
      if (cadena[i] == 1) {
        cadena[i - 4] = cadena[i - 4] + 256;
      }
    }
    myGLCD.drawRect(cadena[1], cadena[2], cadena[3], cadena[4]);
    Serial.println(' ');
  } else if (option == 5) {
    for (int i = 5; i < 9; i++) {
      if (cadena[i] == 1) {
        cadena[i - 4] = cadena[i - 4] + 256;
      }
    }
    myGLCD.fillRect(cadena[1], cadena[2], cadena[3], cadena[4]);
    Serial.println(' ');
  } else if (option == 6) {
    for (int i = 0; i < cadena[1]; i++) {
      t[i] = cadena[i + 4] + '0' - '0';
    }
    for (int i = cadena[1] ; i < 100; i++) {
      t[i] = '\0';
    }
    for (int i = 2; i < 4; i++) {
      if (cadena[i] == 1) {
        cadena[cadena[1] + 2 + i] = cadena[cadena[1] + 2 + i] + 256;
      }
    }
    myGLCD.print(t, cadena[cadena[1] + 4], cadena[cadena[1] + 5]);
    Serial.print(t);
    Serial.println(' ');
  } else if (option == 7) {
    if (cadena[1] == 0) {
      myGLCD.setFont(SmallFont);
    } else if (cadena[1] == 1) {
      myGLCD.setFont(BigFont);
    } else if (cadena[1] == 2) {
      myGLCD.setFont(SevenSegNumFont);
    }
  }


}
