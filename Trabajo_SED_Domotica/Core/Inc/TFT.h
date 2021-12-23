/*
 * TFT.h
 *
 *  Created on: Dec 23, 2021
 *      Author: Raul
 */
#ifndef INC_TFT_H_
#define INC_TFT_H_

#include"main.h"
#include "stdlib.h"
#include "string.h"

I2C_HandleTypeDef hi2c1;

//Limpiar pantalla LCD
void clrScr() {
	uint8_t op = 1;
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, &op, 1, 3000);
}

//Seleccionar Color pantalla LCD
void setColor(int red, int green, int blue) {
	uint8_t color[4] = { 2, red, green, blue };
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, &color, 4, 3000);
}
//Seleccionar color fondo pantalla LCD
void setBackColor(int red, int green, int blue) {
	uint8_t color[4] = { 3, red, green, blue };
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, &color, 4, 3000);
}
//Dibuja un rectángulo pantalla LCD
void drawRect(int x1, int y1, int x2, int y2) {
	int desb[4] = { 0, 0, 0, 0 };
	if (x1 > 255) {
		desb[0] = 1;
	}
	if (y1 > 255) {
		desb[1] = 1;
	}
	if (x2 > 255) {
		desb[2] = 1;
	}
	if (y2 > 255) {
		desb[3] = 1;
	}
	uint8_t coord[9] = { 4, x1, y1, x2, y2, desb[0], desb[1], desb[2], desb[3] };
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, &coord, 9, 3000);
}
//Dibuja y rellena un rectángulo pantalla  LCD
void fillRect(int x1, int y1, int x2, int y2) {
	int desb[4] = { 0, 0, 0, 0 };
	if (x1 > 255) {
		desb[0] = 1;
	}
	if (y1 > 255) {
		desb[1] = 1;
	}
	if (x2 > 255) {
		desb[2] = 1;
	}
	if (y2 > 255) {
		desb[3] = 1;
	}
	uint8_t coord[9] = { 5, x1, y1, x2, y2, desb[0], desb[1], desb[2], desb[3] };
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, &coord, 9, 3000);
}
//Imprime una palabra de hasta 28 caracteres
void print(const char *t, int x, int y) {
	int desb[2] = { 0, 0 };
	if (x > 255) {
		desb[0] = 1;
	}
	if (y > 255) {
		desb[1] = 1;
	}
	uint8_t cadena[50];
	cadena[0] = 6;
	int longitud = strlen(t);
	cadena[1] = longitud;
	cadena[2] = desb[0];
	cadena[3] = desb[1];
	for (int i = 4; i < longitud + 4; i++) {
		cadena[i] = t[i - 4];
	}
	cadena[longitud + 4] = x;
	cadena[longitud + 5] = y;
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, &cadena, longitud + 6, 3000);
}
//Imprime una serie de palabras de hasta 28 caracteres cada una en pantalla  LCD
void printfrase(const char *t[], int x, int y, int numpalabras) {
	int long_acumulada = 0;
	for (int i = 0; i < numpalabras; i++) {
		print(t[i], x + long_acumulada, y);
		long_acumulada = long_acumulada + strlen(t[i]) * 8;
	}
}
//Elije fuente: 0 para pequeño, 1 para grande y 3 para numeros en pantalla  LCD
void setFont(int f) {
	uint8_t font[2] = { 7, f };
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, &font, 2, 3000);
}

//estructura para elegir colores mas facilmente
typedef struct rgb {
	int r, g, b;
} rgb;

int dist_lateral_pared = 30;
int dist_altura_boton = 70;
int ancho_boton = 100;
int alto_boton = 70;
int marco_boton = 2;
int nuevo_marco = 5;
rgb cboton = { 0, 0, 255 };
rgb cmarco = { 255, 255, 255 };
rgb cmarcon = { 255, 0, 0 };
rgb ctexto = { 255, 255, 255 };
int filas = 2;
int columnas = 3;
int boton_resaltado = 0;
int espacio_entre_botones;
int alto_entre_botones;

void botones(int distl, int distalt, int ancho, int alto, int marco, int nmarco,
		rgb color_boton, rgb color_marco, rgb color_marco_nuevo,
		rgb color_texto, int f, int c) {
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
		espacio_entre_botones = (480 - ancho_boton * columnas
				- dist_lateral_pared * 2) / (columnas - 1);
	} else {
		espacio_entre_botones = (480 - ancho_boton * columnas
				- dist_lateral_pared * 2) / columnas;
	}
	if (filas != 1) {
		alto_entre_botones = (320 - alto_boton * filas - dist_altura_boton * 2)
				/ (filas - 1);
	} else {
		alto_entre_botones = (320 - alto_boton * filas - dist_altura_boton * 2)
				/ filas;
	}
}

void crear_botonera() {
	for (int j = 0; j < filas; j++) {
		for (int i = 0; i < columnas; i++) {
			int x = dist_lateral_pared
					+ ((espacio_entre_botones + ancho_boton) * i);
			int y = dist_altura_boton + ((alto_entre_botones + alto_boton) * j);
			setColor(cmarco.r, cmarco.g, cmarco.b);
			fillRect(x - marco_boton, y - marco_boton,
					x + ancho_boton + marco_boton,
					y + alto_boton + marco_boton);
			setColor(cboton.r, cboton.g, cboton.b);
			fillRect(x, y, x + ancho_boton, y + alto_boton);
		}
	}
}

void resaltar_boton(int b) {
	int k = 1;
	for (int j = 0; j < filas; j++) {
		for (int i = 0; i < columnas; i++) {
			if (boton_resaltado == k) {
				int x = dist_lateral_pared
						+ (espacio_entre_botones + ancho_boton) * i;
				int y = dist_altura_boton
						+ (alto_entre_botones + alto_boton) * j;
				setColor(0, 0, 0);
				fillRect(x - nuevo_marco, y - nuevo_marco,
						x + ancho_boton + nuevo_marco,
						y + alto_boton + nuevo_marco);
				setColor(cmarco.r, cmarco.g, cmarco.b);
				fillRect(x - marco_boton, y - marco_boton,
						x + ancho_boton + marco_boton,
						y + alto_boton + marco_boton);
				setColor(cboton.r, cboton.g, cboton.b);
				fillRect(x, y, x + ancho_boton, y + alto_boton);
			}
			k++;
		}
	}
	k = 1;
	boton_resaltado = b;
	for (int j = 0; j < filas; j++) {
		for (int i = 0; i < columnas; i++) {
			if (boton_resaltado == k) {
				int x = dist_lateral_pared
						+ (espacio_entre_botones + ancho_boton) * i;
				int y = dist_altura_boton
						+ (alto_entre_botones + alto_boton) * j;
				setColor(cmarcon.r, cmarcon.g, cmarcon.b);
				fillRect(x - nuevo_marco, y - nuevo_marco,
						x + ancho_boton + nuevo_marco,
						y + alto_boton + nuevo_marco);
				setColor(cboton.r, cboton.g, cboton.b);
				fillRect(x, y, x + ancho_boton, y + alto_boton);
			}
			k++;
		}
	}
}

void rellenar_botones(const char *t[]) {
	int k = 1;
	int alto_letra = 16;
	int ancho_letra = 16;
	setColor(ctexto.r, ctexto.g, ctexto.b);
	setBackColor(cboton.r, cboton.g, cboton.b);
	for (int j = 0; j < filas; j++) {
		for (int i = 0; i < columnas; i++) {
			int x = dist_lateral_pared
					+ (espacio_entre_botones + ancho_boton) * i;
			int y = dist_altura_boton + (alto_entre_botones + alto_boton) * j;

			if (ancho_boton >= (ancho_letra * strlen(t[k - 1]))) {
				print(t[k - 1],
						x + (ancho_boton - strlen(t[k - 1]) * ancho_letra) / 2,
						y + (alto_boton - alto_letra) / 2);
			} else {
				char t1[20], t2[20];
				for (int i = 0; i < 20; i++) {
					t1[i] = 0;
					t2[i] = 0;
				}
				for (int g = 0; g < (ancho_boton / ancho_letra); g++) {
					t1[g] = t[k - 1][g];
				}
				for (int g = (ancho_boton / ancho_letra); g <= strlen(t[k - 1]);
						g++) {
					t2[g - (ancho_boton / ancho_letra)] = t[k - 1][g];
				}
				print(t1, x + (ancho_boton - strlen(t1) * ancho_letra) / 2,
						y + (alto_boton - alto_letra * 2) / 2);
				print(t2, x + (ancho_boton - strlen(t2) * ancho_letra) / 2,
						y + (alto_boton - alto_letra * 2) / 2 + alto_letra);
			}

			k++;
		}
	}
}

//-------------------------------------------------------------------------------//

void pantalla_principal(){
	const char *t_menu[6] = { "LUCES", "PERSIANAS", "RIEGO", "CLIMA", "AUDIO",
			"SEGURIDAD" };
	rgb boton = { 0, 0, 255 };
		rgb marco = { 255, 255, 255 };
		rgb nmarco = { 255, 0, 0 };
		rgb texto = { 255, 255, 255 };
		clrScr();
		setColor(255, 0, 0);
		fillRect(1, 1, 478, 40);
		setColor(64, 64, 64);
		fillRect(1, 318, 478, 280);
		setColor(255, 255, 255);
		setBackColor(255, 0, 0);
		setFont(1);
		print("* CASA DOMOTICA *", 50, 12);
		setBackColor(64, 64, 64);
		setColor(255, 255, 0);
		setFont(0);
		const char *t[3] = { "Sistemas", " Electronicos", " Digitales" };
		printfrase(t, 20, 281, 3);
		const char *t2[3] = { "Raul Herranz,", " Alejandro Sacristan",
				", Carlos Murillo" };
		printfrase(t2, 20, 300, 3);
		setColor(0, 0, 255);
		drawRect(0, 0, 479, 319);
		botones(30, 70, 100, 70, 2, 5, boton, marco, nmarco, texto, 2, 3);
		crear_botonera();
		setFont(1);
		rellenar_botones(t_menu);
}

void pantalla_luces(){
	const char *t_menu_luces[6] = { "AUTOMATICO", "MANUAL"};
	rgb boton = { 0, 0, 255 };
		rgb marco = { 255, 255, 255 };
		rgb nmarco = { 255, 0, 0 };
		rgb texto = { 255, 255, 255 };
		clrScr();
		setColor(255, 0, 0);
		fillRect(1, 1, 478, 40);
		setColor(64, 64, 64);
		fillRect(1, 318, 478, 280);
		setColor(255, 255, 255);
		setBackColor(255, 0, 0);
		setFont(1);
		print("* MENU LUCES *", 100, 12);
		setBackColor(64, 64, 64);
		setColor(255, 255, 0);
		setFont(0);
		const char *t[3] = { "Sistemas", " Electronicos", " Digitales" };
		printfrase(t, 20, 281, 3);
		const char *t2[3] = { "Raul Herranz,", " Alejandro Sacristan,",
				" Carlos Murillo" };
		printfrase(t2, 20, 300, 3);
		setColor(0, 0, 255);
		drawRect(0, 0, 479, 319);
		botones(40, 85, 180, 150, 2, 5, boton, marco, nmarco, texto, 1, 2);
		crear_botonera();
		setFont(1);
		rellenar_botones(t_menu_luces);
}

#endif /* INC_TFT_H_ */
