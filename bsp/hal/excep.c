/*

 * FUNCIONES COPIADAS DE LA LECCION 10 y 11 de TEORIA 

 * Sistemas operativos empotrados
 * Gestión de excepciones del MC1322x
 */

#include "system.h"

/*****************************************************************************/

/**
 * Tabla de manejadores
 */
extern volatile excep_handler_t _excep_handlers[excep_max];

/*****************************************************************************/

/**
 * Inicializa los manejadores de excepción
 */
void excep_init ()
{
	excep_set_handler (excep_irq, excep_nonnested_irq_handler);
}

/*****************************************************************************/

/**
 * Deshabilita todas las interrupciones
 * Esta función sólo funciona en modos privilegiados. Desde modo USER no se
 * permite alterar los bits I y F de los registros de control
 * @return	El valor de los bits I y F antes de deshabilitar las interrupciones:
 * 			0: I=0, F=0	(IRQ habilitadas,    FIQ habilitadas)
 * 			1: I=0, F=1	(IRQ habilitadas,    FIQ deshabilitadas)
 * 			2: I=1, F=0	(IRQ deshabilitadas, FIQ habilitadas)
 * 			3: I=1, F=1	(IRQ deshabilitadas, FIQ deshabilitadas)
 */
inline uint32_t excep_disable_ints ()
{
	uint32_t if_bits;
	asm volatile("mrs %[bits], cpsr\n\t" 
		     "orr r12, %[bits], #0xC0\n\t" /*  la mascara hay que adaptarla segun los bits*/
		     "msr cpsr_c, r12"

			
			:[bits] "=r" (if_bits)
			:
			:"r12", "cc");
			
			return (if_bits >> 6) & 3; /*  para ajustar las otras funciones hay que tocar en este bit
						  6, dependiendo del que usemos, y el 3 si usamos 2 bits o un 1 si usamos 1*/ 

	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 5 */
	return 0;
}

/*****************************************************************************/

/**
 * Deshabilita las interrupciones normales
 * Esta función sólo funciona en modos privilegiados. Desde modo USER no se
 * permite alterar el bit I de los registros de control
 * @return	El valor del bit I antes de deshabilitar las interrupciones:
 * 			0: I=0	(IRQ habilitadas)
 * 			1: I=1	(IRQ deshabilitadas)
 */

inline uint32_t excep_disable_irq ()
{
	  uint32_t if_bits;
	asm volatile("mrs %[bits], cpsr\n\t" 
		     "orr r12, %[bits], #0x80\n\t" /*  la mascara hay que adaptarla segun los bits*/
		     "msr cpsr_c, r12"

			
			:[bits] "=r" (if_bits)
			:
			:"r12", "cc");
			
			return (if_bits >> 7) & 1; /*  para ajustar las otras funciones hay que tocar en este bit
						  6, dependiendo del que usemos, y el 3 si usamos 2 bits o un 1 si usamos 1*/

/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 5 */
	return 0;
}

/*****************************************************************************/

/**
 * Deshabilita las interrupciones rápidas
 * Esta función sólo funciona en modos privilegiados. Desde modo USER no se
 * permite alterar el bit F de los registros de control
 * @return	El valor del bit F antes de deshabilitar las interrupciones:
 * 			0: F=0	(FIQ habilitadas)
 * 			1: F=1	(FIQ deshabilitadas)
 */
inline uint32_t excep_disable_fiq ()
{

	uint32_t if_bits;
	asm volatile("mrs %[bits], cpsr\n\t" 
		     "orr r12, %[bits], #0x40\n\t" /*  la mascara hay que adaptarla segun los bits*/
		     "msr cpsr_c, r12"

			
			:[bits] "=r" (if_bits)
			:
			:"r12", "cc");
			
			return (if_bits >> 6) & 1; /*  para ajustar las otras funciones hay que tocar en este bit
						  6, dependiendo del que usemos, y el 3 si usamos 2 bits o un 1 si usamos 1*/


	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 5 */
	return 0;
}

/*****************************************************************************/

/**
 * Restaura los antiguos valores de las máscaras de interrupción
 * Esta función sólo funciona en modos privilegiados. Desde modo USER no se
 * permite alterar los bits I y F de los registros de control
 * @param if_bits	Valores anteriores de las máscaras
 * 						0: I=0, F=0	(IRQ habilitadas,    FIQ habilitadas)
 *			 			1: I=0, F=1	(IRQ habilitadas,    FIQ deshabilitadas)
 * 						2: I=1, F=0	(IRQ deshabilitadas, FIQ habilitadas)
 *			 			3: I=1, F=1	(IRQ deshabilitadas, FIQ deshabilitadas)
 */
inline void excep_restore_ints (uint32_t if_bits)
{
	asm volatile ("mrs r12, cpsr\n\t"
			"bic r12, r12, #0xC0\n\t"
			"orr r12, r12, %[bits], LSL #6\n\t"
			"msr cpsr_c, r12"
			:
			: [bits] "r" (if_bits & 3)
			: "r12", "cc");




	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 5 */
}

/*****************************************************************************/

/**
 * Restaura el antiguo valor de la máscara de interrupciones normales
 * Esta función sólo funciona en modos privilegiados. Desde modo USER no se
 * permite alterar el bit I de los registros de control
 * @param i_bit	Valor anterior de la máscara
 * 						0: I=0	(IRQ habilitadas)
 * 						1: I=1	(IRQ deshabilitadas)
 */
inline void excep_restore_irq (uint32_t i_bit)
{

	asm volatile ("mrs r12, cpsr\n\t"
			"bic r12, r12, #0x80\n\t"
			"orr r12, r12, %[bits], LSL #7\n\t"
			"msr cpsr_c, r12"
			:
			: [bits] "r" (i_bit & 1)
			: "r12", "cc");

	

	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 5 */
}

/*****************************************************************************/

/**
 * Restaura el antiguo valor de la máscara de interrupciones rápidas
 * Esta función sólo funciona en modos privilegiados. Desde modo USER no se
 * permite alterar el bit F de los registros de control
 * @param f_bit	Valor anterior de la máscara
 * 						0: F=0	(FIQ habilitadas)
 * 						1: F=1	(FIQ deshabilitadas)
 */
inline void excep_restore_fiq (uint32_t f_bit)
{
	asm volatile ("mrs r12, cpsr\n\t"
			"bic r12, r12, #0x40\n\t"
			"orr r12, r12, %[bits], LSL #6\n\t"
			"msr cpsr_c, r12"
			:
			: [bits] "r" (f_bit & 1)
			: "r12", "cc");

	


	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 5 */
}

/*****************************************************************************/

/**
 * Asigna un manejador de interrupción/excepción
 * @param excep		Tipo de excepción
 * @param handler	Manejador
 */
inline void excep_set_handler (excep_t excep, excep_handler_t handler)
{	
	_excep_handlers[excep] = handler;

/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 5 */
}

/*****************************************************************************/

/**
 * Retorna un manejador de interrupción/excepción
 * @param excep		Tipo de excepción
 */
inline excep_handler_t excep_get_handler (excep_t excep)
{

	return _excep_handlers[excep];

	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 5 */
        
}

/*****************************************************************************/

/**
 * Manejador en C para interrupciones normales no anidadas
 * El atributo interrupt no guarda en la pila el registro spsr, por lo que
 * sólo genera manejadores para interrupciones no anidadas
 * Para poder gestionar interrupciones anidadas y sacar partiro al controlador
 * de interrupciones es necesario escribir el manejador en ensamblador
 */
 __attribute__ ((interrupt ("IRQ")))
void excep_nonnested_irq_handler ()
{
	itc_service_normal_interrupt();
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 6 */
}

/*****************************************************************************/
