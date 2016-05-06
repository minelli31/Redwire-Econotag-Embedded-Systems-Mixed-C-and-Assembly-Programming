/*
 * Sistemas operativos empotrados
 * Driver para el controlador de interrupciones del MC1322x
 */

#ifndef __ITC_H__
#define __ITC_H__

#include <stdint.h>

/*****************************************************************************/

/**
 * Fuentes de interrupción externas
 */
typedef enum
{
	itc_src_asm = 0,
	itc_src_uart1,
	itc_src_uart2,
	itc_src_crm,
	itc_src_i2c,
	itc_src_tmr,
	itc_src_spif,
	itc_src_maca,
	itc_src_ssi,
	itc_src_adc,
	itc_src_spi,
	itc_src_max
} itc_src_t;

/*****************************************************************************/

/**
 * Prioridades para las fuentes de interrupción
 */
typedef enum
{
	itc_priority_normal = 0,
	itc_priority_fast,
	itc_priority_max
} itc_priority_t;

/*****************************************************************************/

/**
 * Prototipo para los manejadores de interrupción
 */
typedef void (* itc_handler_t) (void);

/*****************************************************************************/

/**
 * Inicializa el controlador de interrupciones.
 * Deshabilita los bits I y F de la CPU, inicializa la tabla de manejadores a NULL,
 * y habilita el arbitraje de interrupciones Normales y rápidas en el controlador
 * de interupciones.
 */
inline void itc_init ();

/*****************************************************************************/

/**
 * Deshabilita el envío de peticiones de interrupción a la CPU
 * Permite implementar regiones críticas en modo USER
 */
inline void itc_disable_ints ();

/*****************************************************************************/

/**
 * Vuelve a habilitar el envío de peticiones de interrupción a la CPU
 * Permite implementar regiones críticas en modo USER
 */
inline void itc_restore_ints ();

/*****************************************************************************/

/**
 * Asigna un manejador de interrupción
 * @param src		Identificador de la fuente
 * @param handler	Manejador
 */
inline void itc_set_handler (itc_src_t src, itc_handler_t handler);

/*****************************************************************************/

/**
 * Asigna una prioridad (normal o fast) a una fuente de interrupción
 * @param src		Identificador de la fuente
 * @param priority	Tipo de prioridad
 */
inline void itc_set_priority (itc_src_t src, itc_priority_t priority);

/*****************************************************************************/
/**
 * Habilita las interrupciones de una determinda fuente
 * @param src		Identificador de la fuente
 */
inline void itc_enable_interrupt (itc_src_t src);

/*****************************************************************************/

/**
 * Deshabilita las interrupciones de una determinda fuente
 * @param src		Identificador de la fuente
 */
inline void itc_disable_interrupt (itc_src_t src);

/*****************************************************************************/

/**
 * Fuerza una interrupción con propósitos de depuración
 * @param src		Identificador de la fuente
 */
inline void itc_force_interrupt (itc_src_t src);

/*****************************************************************************/

/**
 * Desfuerza una interrupción con propósitos de depuración
 * @param src		Identificador de la fuente
 */
inline void itc_unforce_interrupt (itc_src_t src);

/*****************************************************************************/

/**
 * Da servicio a la interrupción normal pendiente de más prioridad
 */
void itc_service_normal_interrupt ();

/*****************************************************************************/

/**
 * Da servicio a la interrupción rápida pendiente de más prioridad
 */
void itc_service_fast_interrupt ();

/*****************************************************************************/

#endif /* __ITC_H__ */
