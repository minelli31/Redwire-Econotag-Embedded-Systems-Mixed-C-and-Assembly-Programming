/*
 * Sistemas operativos empotrados
 * Búfer circular
 */

#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <stdint.h>

/*****************************************************************************/

/**
 * Estructura para gestionar un búfer circular
 */
typedef struct
{
	uint8_t *data;
	uint32_t size;
	uint32_t start;
	uint32_t end;
	uint32_t count;
} circular_buffer_t;

/*****************************************************************************/

/**
 * Inicializa un búfer circular dado un puntero a una zona de memoria y su tamaño
 * @param cb	Puntero a la estructura de gestión del búfer circular
 * @param addr	Puntero a la zona de memoria que se gestionará como un búfer circular
 * @param size	Tamaño en bytes del búfer
 */
void circular_buffer_init (volatile circular_buffer_t *cb, uint8_t *addr, uint32_t size);

/*****************************************************************************/

/**
 * Retorna 1 si el búfer está lleno
 * @param cb	Búfer circular
 */
inline uint32_t circular_buffer_is_full (volatile circular_buffer_t *cb);

/*****************************************************************************/

/**
 * Retorna 1 si el búfer está vacío
 * @param cb	Búfer circular
 */
inline uint32_t circular_buffer_is_empty (volatile circular_buffer_t *cb);

/*****************************************************************************/

/**
 * Escribe un byte en un búfer circular
 * @param cb	Búfer circular
 * @param byte	Byte a escribir
 * @return		El byte como un casting de uint8_t a int32_t en caso de éxito
 * 				o -1 en caso de error
 */
int32_t circular_buffer_write (volatile circular_buffer_t *cb, uint8_t byte);

/*****************************************************************************/

/**
 * Lee un byte en un búfer circular
 * @param cb	Búfer circular
 * @return		El byte como un casting de uint8_t a int32_t en caso de éxito
 * 				o -1 en caso de error
 */
int32_t circular_buffer_read (volatile circular_buffer_t *cb);

/*****************************************************************************/

#endif /* __CIRCULAR_BUFFER_H__ */
