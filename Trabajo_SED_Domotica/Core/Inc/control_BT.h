/*
 * control_BT.h
 *
 *  Created on: Dec 23, 2021
 *      Author: Raul
 */

#ifndef INC_CONTROL_BT_H_
#define INC_CONTROL_BT_H_
#include"main.h"
#include "stdlib.h"
#include "string.h"
#include "TFT.h"

UART_HandleTypeDef huart6;
char rx_buffer[50], tx_buffer[50];
void reset_buffer() {
	for (int i = 0; i < 50; i++) {
		rx_buffer[0] = '\0';
	}
}

const char *t_menu[6] = { "LUCES", "PERSIANAS", "RIEGO", "CLIMA", "AUDIO",
		"SEGURIDAD" };
const char *t_menu_luces[6] = { "AUTOMATICO", "MANUAL" };
int opcion = 0;
int seleccion_menu_principal() {
	HAL_UART_Receive(&huart6, (uint8_t*) rx_buffer, 50, 500);
	if (rx_buffer[0] == 'r' && rx_buffer[1] == 'i' && rx_buffer[2] == 'g'
			&& rx_buffer[3] == 'h' && rx_buffer[4] == 't') {
		if (opcion == 3) {
			opcion = 1;
		} else if (opcion == 6) {
			opcion = 4;
		} else {
			opcion++;
		}
		resaltar_boton(opcion);
		setFont(1);
		rellenar_botones(t_menu);
		reset_buffer();
	} else if (rx_buffer[0] == 'l' && rx_buffer[1] == 'e' && rx_buffer[2] == 'f'
			&& rx_buffer[3] == 't') {
		if (opcion == 1) {
			opcion = 3;
		} else if (opcion == 4) {
			opcion = 6;
		} else if (opcion == 0) {
			opcion = 1;
		} else {
			opcion--;
		}
		resaltar_boton(opcion);
		setFont(1);
		rellenar_botones(t_menu);
		reset_buffer();
	} else if (rx_buffer[0] == 'u' && rx_buffer[1] == 'p') {
		if (opcion == 1) {
			opcion = 4;
		} else if (opcion == 2) {
			opcion = 5;
		} else if (opcion == 3) {
			opcion = 6;
		} else if (opcion == 0) {
			opcion = 1;
		} else {
			opcion -= 3;
		}
		resaltar_boton(opcion);
		setFont(1);
		rellenar_botones(t_menu);
		reset_buffer();
	} else if (rx_buffer[0] == 'd' && rx_buffer[1] == 'o' && rx_buffer[2] == 'w'
			&& rx_buffer[3] == 'n') {
		if (opcion == 4) {
			opcion = 1;
		} else if (opcion == 5) {
			opcion = 2;
		} else if (opcion == 0) {
			opcion = 1;
		} else if (opcion == 6) {
			opcion = 3;
		} else {
			opcion += 3;
		}
		resaltar_boton(opcion);
		setFont(1);
		rellenar_botones(t_menu);
		reset_buffer();
	} else if (rx_buffer[0] == 's' && rx_buffer[1] == 't' && rx_buffer[2] == 'a'
			&& rx_buffer[3] == 'r' && rx_buffer[4] == 't') {
		reset_buffer();
		int op = opcion;
		opcion = 0;
		return op;
	}
	return 0;
}

int opcion_luces = 0;
int seleccion_menu_luces() {
	HAL_UART_Receive(&huart6, (uint8_t*) rx_buffer, 50, 500);
	if (rx_buffer[0] == 'r' && rx_buffer[1] == 'i' && rx_buffer[2] == 'g'
			&& rx_buffer[3] == 'h' && rx_buffer[4] == 't') {
		if (opcion_luces == 2) {
			opcion_luces = 1;
		} else {
			opcion_luces++;
		}
		resaltar_boton(opcion_luces);
		setFont(1);
		rellenar_botones(t_menu_luces);
		reset_buffer();
	} else if (rx_buffer[0] == 'l' && rx_buffer[1] == 'e' && rx_buffer[2] == 'f'
			&& rx_buffer[3] == 't') {
		if (opcion_luces == 1) {
			opcion_luces = 2;
		} else if (opcion_luces == 0) {
			opcion_luces = 1;
		} else {
			opcion_luces--;
		}
		resaltar_boton(opcion_luces);
		setFont(1);
		rellenar_botones(t_menu_luces);
		reset_buffer();
	} else if (rx_buffer[0] == 's' && rx_buffer[1] == 'e' && rx_buffer[2] == 'l'
			&& rx_buffer[3] == 'e' && rx_buffer[4] == 'c'
			&& rx_buffer[5] == 't') {
		reset_buffer();
		opcion_luces = 0;
		return 0;
	} else if (rx_buffer[0] == 's' && rx_buffer[1] == 't' && rx_buffer[2] == 'a'
			&& rx_buffer[3] == 'r' && rx_buffer[4] == 't') {
		reset_buffer();
		int op = opcion_luces;
		opcion_luces = 0;
		return op + 6;
	}
	return 1;
}
int menu_luces_manual() {
	HAL_UART_Receive(&huart6, (uint8_t*) rx_buffer, 50, 500);
	if (rx_buffer[0] == 's' && rx_buffer[1] == 'q' && rx_buffer[2] == 'u'
			&& rx_buffer[3] == 'a' && rx_buffer[4] == 'r'
			&& rx_buffer[5] == 'e') {
		if (getEstadoLuces() == 0) {
			setLuces(1);
			setColor(0, 0, 255);
			fillCircle(118, 130, 30);
			setColor(0, 0, 0);
			fillCircle(358, 130, 30);
			setColor(0, 255, 0);
			fillCircle(118, 130, 25);
			setColor(255, 0, 0);
			fillCircle(358, 130, 25);
		}
		reset_buffer();
	}else if (rx_buffer[0] == 'c' && rx_buffer[1] == 'r' && rx_buffer[2] == 'o'
			&& rx_buffer[3] == 's' && rx_buffer[4] == 's') {
		if (getEstadoLuces() == 1) {
			setLuces(0);
			setColor(0, 0, 0);
			fillCircle(118, 130, 30);
			setColor(0, 0, 255);
			fillCircle(358, 130, 30);
			setColor(0, 255, 0);
			fillCircle(118, 130, 25);
			setColor(255, 0, 0);
			fillCircle(358, 130, 25);
		}
		reset_buffer();
	}else if (rx_buffer[0] == 's' && rx_buffer[1] == 'e' && rx_buffer[2] == 'l'
			&& rx_buffer[3] == 'e' && rx_buffer[4] == 'c'
			&& rx_buffer[5] == 't') {
		reset_buffer();
		return 0;
	}
return 8;
}
#endif /* INC_CONTROL_BT_H_ */
