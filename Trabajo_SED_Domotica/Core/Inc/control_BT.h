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
#include "control_luces.h"
#include "control_persianas.h"
#include "control_riego.h"
#include "control_seguridad.h"
#include "control_clima.h"
#include "stdio.h"

UART_HandleTypeDef huart6;
char rx_buffer, tx_buffer[50];
/*
 void reset_buffer() {
 for (int i = 0; i < 50; i++) {
 rx_buffer[0] = '\0';
 }
 }
 */
void bluetooth(char recibido[]) {
	if (strcmp(recibido, "a") == 0) {
		if (getEstadoLuces() == 1)
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Las luces ya estaban activadas\n"),
					500);
		else {
			setLuces(1);
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Luces activadas\n"), 500);
		}
	} else if (strcmp(recibido, "b") == 0) {
		if (getEstadoLuces() == 0)
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Luces ya estaban desactivadas\n"), 500);
		else {
			setLuces(0);
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Luces desactivadas\n"), 500);
		}
	} else if (strcmp(recibido, "c") == 0) {
		if (getEstadoLuces() == 2)
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Luces ya estaban en modo automatico\n"),
					500);
		else {
			setLuces(2);
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Luces en modo automatico\n"), 500);
		}
	} else if (strcmp(recibido, "d") == 0) {
		if (getEstadoAnteriorRiego() == 0)
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Ya estabas regando\n"), 500);
		else {
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Abriendo electrovalvula...\n"), 500);
			setEstadoRiego(0);
		}
	} else if (strcmp(recibido, "e") == 0) {
		if (getEstadoAnteriorRiego() == 1)
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "La electrovalvula ya estaba cerrada\n"),
					500);
		else {
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Cerrando electrovalvula...\n"), 500);
			setEstadoRiego(1);
		}
	} else if (strcmp(recibido, "f") == 0) {
		if (getEstadoPersianas() == 1)
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Ya se esta subiendo la persiana\n"),
					500);
		else {
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Subiendo persiana...\n"), 500);
			setEstadoPersianas(1);
		}
	} else if (strcmp(recibido, "g") == 0) {
		if (getEstadoPersianas() == 2)
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Ya se esta bajando la persiana\n"),
					500);
		else {
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Bajando persiana...\n"), 500);
			setEstadoPersianas(2);
		}
	} else if (strcmp(recibido, "h") == 0) {
		if (getEstadoPersianas() == 0)
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "La persiana ya estaba parada\n"), 500);
		else {
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Parando persiana...\n"), 500);
			setEstadoPersianas(0);
		}
	} else if (strcmp(recibido, "i") == 0) {
		if(getEstadoClima()==1){
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
							sprintf(tx_buffer, "La calefaccion ya estaba encendida\n"), 500);
		}else{
			setEstadoClima(1);
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Encendiendo calefaccion...\n"), 500);
		}
	} else if (strcmp(recibido, "j") == 0) {
		if(getEstadoClima()==2){
					HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
									sprintf(tx_buffer, "El ventilador ya estaba encendido\n"), 500);
				}else{
					setEstadoClima(2);
					HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
							sprintf(tx_buffer, "Encendiendo ventilador...\n"), 500);
				}
	} else if (strcmp(recibido, "k") == 0) {
		if (getEstadoClima() == 1)
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Apagando calefaccion\n"), 500);
		else if (getEstadoClima() == 2)
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Apagando ventilador\n"), 500);
		else
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer,
							"Calefaccion y ventilador ya estaban apagadas\n"),
					500);
		setEstadoClima(0);
	} else if (strcmp(recibido, "l") == 0) {
		if (getControlClima() == 1)
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Termostato ya estaba activado\n"), 500);
		else {
			setControlClima(1);
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Termostato activado\n"), 500);
		}
	} else if (strcmp(recibido, "m") == 0) {
		if (getControlClima() == 0)
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Termostato ya estaba desactivado\n"),
					500);
		else {
			setControlClima(0);
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Termostato desactivado\n"), 500);
		}
	}else if(strcmp(recibido,"n")==0){
	lectura_dht11();
	 HAL_UART_Transmit(&huart6, (uint8_t *)tx_buffer, sprintf(tx_buffer, "La temperatura es de %i ºC\n", (int) Temperature), 500);
	 }else if(strcmp(recibido,"o")==0){
		 lectura_dht11();
		 	 HAL_UART_Transmit(&huart6, (uint8_t *)tx_buffer, sprintf(tx_buffer, "La humedad relativa es del %i %%\n",(int) Humidity), 500);
		 }else if (strcmp(recibido, "p") == 0) {
		if (getEstadoSeguridad() == 2)
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer,
							"La alarma ya estaba encendida, reiniciandola...\n"),
					500);
		else
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Encendiendo alarma...\n"), 500);
		setEstadoSeguridad(1);
	} else if (strcmp(recibido, "q") == 0) {
		if (getEstadoSeguridad() == 0)
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "La alarma ya estaba apagada\n"), 500);
		else {
			HAL_UART_Transmit(&huart6, (uint8_t*) tx_buffer,
					sprintf(tx_buffer, "Apagando alarma...\n"), 500);
			setEstadoSeguridad(0);
		}

	}
}

/*
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
 */
#endif /* INC_CONTROL_BT_H_ */
