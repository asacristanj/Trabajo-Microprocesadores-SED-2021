/*
 * control_7s.h
 *
 *  Created on: Dec 23, 2021
 *      Author: Raul
 */

#ifndef INC_CONTROL_7S_H_
#define INC_CONTROL_7S_H_

#include"main.h"
#include "stdlib.h"
#include "string.h"

I2C_HandleTypeDef hi2c1;

//--------------------------------7 SEGMENTOS------------------------//
//Mostrar texto en pantalla 7 segmentos
void mostrartexto7s(const char palabra[]) {
	uint8_t longitud = strlen(palabra);
	uint8_t texto[longitud + 1];
	texto[0] = 1;
	for (int i = 1; i < longitud + 1; i++) {
		texto[i] = palabra[i - 1];
	}
	HAL_I2C_Master_Transmit(&hi2c1, 0x7 << 1, texto, longitud + 1, 30);
}
//Mostrar numero en pantalla 7 segmentos
void mostrarnum7s(float num) {
	uint8_t cadena_num[5];
	cadena_num[0] = 0;
	if (num < 10) {
		cadena_num[1] = 2;
		num = num * 100;
	} else if (num < 100) {
		cadena_num[1] = 1;
		num = num * 10;
	} else {
		cadena_num[1] = 0;
	}
	cadena_num[2] = (num / 100);
	cadena_num[3] = num / 10 - cadena_num[2] * 10;
	cadena_num[4] = (num - cadena_num[2] * 100 - cadena_num[3] * 10);
	HAL_I2C_Master_Transmit(&hi2c1, 0x7 << 1, cadena_num, 5, 30);
}


#endif /* INC_CONTROL_7S_H_ */
