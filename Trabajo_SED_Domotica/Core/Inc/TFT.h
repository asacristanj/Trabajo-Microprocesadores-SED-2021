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
#include "control_BT.h"
#include "control_luces.h"
#include "control_persianas.h"
#include "control_riego.h"
#include "control_seguridad.h"
#include "control_clima.h"

I2C_HandleTypeDef hi2c1;

//Limpiar pantalla LCD
void clrScr() {
	uint8_t op = 1;
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, &op, 1, 3000);
}

//Seleccionar Color pantalla LCD
void setColor(int red, int green, int blue) {
	uint8_t color[4] = { 2, red, green, blue };
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, (uint8_t*) color, 4, 3000);
}
//Seleccionar color fondo pantalla LCD
void setBackColor(int red, int green, int blue) {
	uint8_t color[4] = { 3, red, green, blue };
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, (uint8_t*) color, 4, 3000);
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
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, (uint8_t*) coord, 9, 3000);
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
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, (uint8_t*) coord, 9, 3000);
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
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, (uint8_t*) cadena, longitud + 6, 3000);
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
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, (uint8_t*) font, 2, 3000);
}
//Imprime circulos
void drawCircle(int x, int y, int r) {
	int desb[3] = { 0, 0, 0 };
	if (x > 255) {
		desb[0] = 1;
	}
	if (y > 255) {
		desb[1] = 1;
	}
	if (r > 255) {
		desb[2] = 1;
	}
	uint8_t coord[7] = { 8, x, y, r, desb[0], desb[1], desb[2] };
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, (uint8_t*) coord, 7, 3000);
}
//Imprime circulos rellenos
void fillCircle(int x, int y, int r) {
	int desb[3] = { 0, 0, 0 };
	if (x > 255) {
		desb[0] = 1;
	}
	if (y > 255) {
		desb[1] = 1;
	}
	if (r > 255) {
		desb[2] = 1;
	}
	uint8_t coord[7] = { 9, x, y, r, desb[0], desb[1], desb[2] };
	HAL_I2C_Master_Transmit(&hi2c1, 0x8 << 1, (uint8_t*) coord, 7, 3000);
}


void pantalla_principal() {
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
	setFont(1);
	setColor(255, 255, 255);
	setBackColor(0, 0, 0);
	print("LUCES:", 50, 94);
	print("RIEGO:", 50, 138);
	print("ALARMA:", 50, 182);
	print("PERSIANA:", 50, 226);
	print("CLIMA:", 240, 94);
	print("TEMP:", 240, 138);
	print("HUMEDAD:", 240, 182);
}

void actualizar_pantalla(int act){
	setColor(0, 0, 0);
	if(act==1){
		fillRect(146, 94, 210,110);
		setColor(255, 255, 255);
		setBackColor(0, 0, 0);
			if(getEstadoLuces()==0){
				print("OFF", 146, 94);
			}else if(getEstadoLuces()==1){
				print("ON", 146, 94);
			}else{
				print("AUTO", 146, 94);
			}
	}else if(act==2){
		fillRect(146, 138, 210,154);
		setColor(255, 255, 255);
			if(getEstadoAnteriorRiego()==0){
				print("ON", 146, 138);
			}
			else{
				print("OFF", 146, 138);
			}
	}else if(act==3){
		fillRect(162, 182, 226,198);
		setColor(255, 255, 255);
			if(getEstadoSeguridad()==0){
				print("OFF", 162, 182);
			}
			else{
				print("ON", 162, 182);
			}
	}else if(act==4){
		fillRect(194, 226, 258,242);
		setColor(255, 255, 255);
			if(getEstadoPersianas()==0){
				print("STOP", 194, 226);
			}
			else if(getEstadoPersianas()==1){
				print("DOWN", 194, 226);
			}else if(getEstadoPersianas()==2){
				print("UP", 194, 226);
			}
	}else if(act==5){
		fillRect(336, 94, 400,110);
		setColor(255, 255, 255);
		setBackColor(0, 0, 0);
		if(getControlClima()==1){
			print("AUTO", 336, 94);
		}else if(getEstadoClima()==0){
				print("OFF", 336, 94);
			}else if(getEstadoClima()==1){
				print("CALD", 336, 94);
			}else if(getEstadoClima()==2){
				print("AIRE", 336, 94);
			}
	}else if(act==6){
		fillRect(336, 138, 400,154);
		setColor(255, 255, 255);
		setBackColor(0, 0, 0);
		char bufff[2];
		sprintf(bufff, "%i", (int) Temperature);
		print(bufff, 336, 138);

		setColor(0, 0, 0);
		fillRect(384, 182, 470,198);
		setColor(255, 255, 255);
		setBackColor(0, 0, 0);
		sprintf(bufff, "%i", (int) Humidity);
		print(bufff, 384, 182);
	}




}



#endif /* INC_TFT_H_ */
