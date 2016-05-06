/*
 * Sistemas operativos empotrados
 * Driver para el GPIO del MC1322x
 */

#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>

/*****************************************************************************/

/**
 * Definición de los valores de retorno para las funciones del driver
 */
typedef enum
{
	gpio_no_error,
	gpio_invalid_parameter
} gpio_err_t;

/*****************************************************************************/

/**
 * Definición de los puertos del GPIO
 */
typedef enum
{
	gpio_port_0,
	gpio_port_1,
	gpio_port_max
} gpio_port_t;

/*****************************************************************************/

/**
 * Definición de las diferentes funciones para cada pin del GPIO
 */
typedef enum
{
	gpio_func_normal,
	gpio_func_alternate_1,
	gpio_func_alternate_2,
	gpio_func_alternate_3,
	gpio_func_max
} gpio_func_t;

/*****************************************************************************/

/**
 * Definición de los pines del GPIO
 */
typedef enum
{
	gpio_pin_0,  gpio_pin_1,  gpio_pin_2,  gpio_pin_3,  gpio_pin_4,  gpio_pin_5,  gpio_pin_6,  gpio_pin_7,
	gpio_pin_8,  gpio_pin_9,  gpio_pin_10, gpio_pin_11, gpio_pin_12, gpio_pin_13, gpio_pin_14, gpio_pin_15,
	gpio_pin_16, gpio_pin_17, gpio_pin_18, gpio_pin_19, gpio_pin_20, gpio_pin_21, gpio_pin_22, gpio_pin_23,
	gpio_pin_24, gpio_pin_25, gpio_pin_26, gpio_pin_27, gpio_pin_28, gpio_pin_29, gpio_pin_30, gpio_pin_31,
	gpio_pin_32, gpio_pin_33, gpio_pin_34, gpio_pin_35, gpio_pin_36, gpio_pin_37, gpio_pin_38, gpio_pin_39,
	gpio_pin_40, gpio_pin_41, gpio_pin_42, gpio_pin_43, gpio_pin_44, gpio_pin_45, gpio_pin_46, gpio_pin_47,
	gpio_pin_48, gpio_pin_49, gpio_pin_50, gpio_pin_51, gpio_pin_52, gpio_pin_53, gpio_pin_54, gpio_pin_55,
	gpio_pin_56, gpio_pin_57, gpio_pin_58, gpio_pin_59, gpio_pin_60, gpio_pin_61, gpio_pin_62, gpio_pin_63,
	gpio_pin_max
} gpio_pin_t;

/*****************************************************************************/

/**
 * Fija la dirección los pines seleccionados en la máscara como de entrada
 *
 * @param 	port	Puerto
 * @param 	mask	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_dir_input (gpio_port_t port, uint32_t mask);

/*****************************************************************************/

/**
 * Fija la dirección los pines seleccionados en la máscara como de salida
 *
 * @param	port	Puerto
 * @param	mask	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_dir_output (gpio_port_t port, uint32_t mask);

/*****************************************************************************/

/**
 * Fija la dirección del pin indicado como de entrada
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_dir_input (gpio_pin_t pin);

/*****************************************************************************/

/**
 * Fija la dirección del pin indicado como de salida
 *
 * @param	pin		Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_dir_output (gpio_pin_t pin);

/*****************************************************************************/

/**
 * Escribe unos en los pines seleccionados en la máscara
 *
 * @param	port	Puerto
 * @param	mask	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port (gpio_port_t port, uint32_t mask);

/*****************************************************************************/

/**
 * Escribe ceros en los pines seleccionados en la máscara
 *
 * @param	port	Puerto
 * @param	mask	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_clear_port (gpio_port_t port, uint32_t mask);

/*****************************************************************************/

/**
 * Escribe un uno en el pin indicado
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin (gpio_pin_t pin);

/*****************************************************************************/

/**
 * Escribe un cero en el pin indicado
 *
 * @param	pin 	Número de pin
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_clear_pin (gpio_pin_t pin);

/*****************************************************************************/

/**
 * Lee el valor de los pines de un puerto
 *
 * @param	port	  Puerto
 * @param	port_data Valor de los pines del puerto
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			  gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_get_port (gpio_port_t port, uint32_t *port_data);

/*****************************************************************************/

/**
 * Lee el valor del pin indicado
 *
 * @param	pin	  Número de pin
 * @param       pin_data  Cero si el pin está a cero, distinto de cero en otro caso
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			  gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_get_pin (gpio_pin_t pin, uint32_t *pin_data);

/*****************************************************************************/

/**
 * Fija los pines seleccionados a una función
 *
 * @param	port 	Puerto
 * @param	func	Función
 * @param	mask	Máscara para seleccionar los pines
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_port_func (gpio_port_t port, gpio_func_t func, uint32_t mask);

/*****************************************************************************/

/**
 * Fija el pin seleccionado a una función
 *
 * @param	pin 	Pin
 * @param	func	Función
 * @return	gpio_no_error si los parámetros de entrada son corectos o
 *			gpio_invalid_parameter en otro caso
 */
inline gpio_err_t gpio_set_pin_func (gpio_pin_t pin, gpio_func_t func);

/*****************************************************************************/

#endif /* __GPIO_H__ */
