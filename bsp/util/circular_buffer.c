/*
 * Sistemas operativos empotrados
 * Búfer circular
 */

#include "circular_buffer.h"

/*****************************************************************************/

/**
 * Inicializa un búfer circular dado un puntero a una zona de memoria y su tamaño
 * @param cb	Puntero a la estructura de gestión del búfer circular
 * @param addr	Puntero a la zona de memoria que se gestionará como un búfer circular
 * @param size	Tamaño en bytes del búfer
 */
void circular_buffer_init (volatile circular_buffer_t *cb, uint8_t *addr, uint32_t size)
{
	cb->data = addr;
	cb->size = size;
	cb->start = 0;
	cb->end = 0;
	cb->count = 0;
}

/*****************************************************************************/

/**
 * Retorna 1 si el búfer está lleno
 * @param cb	Búfer circular
 */
inline uint32_t circular_buffer_is_full (volatile circular_buffer_t *cb)
{
    return cb->count == cb->size;
}

/*****************************************************************************/

/**
 * Retorna 1 si el búfer está vacío
 * @param cb	Búfer circular
 */
inline uint32_t circular_buffer_is_empty (volatile circular_buffer_t *cb)
{
    return cb->count == 0;
}

/*****************************************************************************/

/**
 * Escribe un byte en un búfer circular
 * @param cb	Búfer circular
 * @param byte	Byte a escribir
 * @return		El byte como un casting de uint8_t a int32_t en caso de éxito
 * 				o -1 en caso de error
 */
int32_t circular_buffer_write (volatile circular_buffer_t *cb, uint8_t byte)
{
    /* Escribimos en el búfer sólo si hay espacio */
    if (circular_buffer_is_full (cb))
    	return -1;
    else
    {
        cb->data[cb->end] = byte;
        cb->count++;

        cb->end++;
    	if (cb->end == cb->size)
    		cb->end = 0;
        return byte;
    }
}

/*****************************************************************************/

/**
 * Lee un byte en un búfer circular
 * @param cb	Búfer circular
 * @return		El byte como un casting de uint8_t a int32_t en caso de éxito
 * 				o -1 en caso de error
 */
int32_t circular_buffer_read (volatile circular_buffer_t *cb)
{
	int32_t byte;
    if (circular_buffer_is_empty (cb))
    	return -1;
    else
    {
        byte = cb->data[cb->start];
        cb->count--;

        cb->start++;
    	if (cb->start == cb->size)
    		cb->start = 0;
        return byte;
    }
}

/*****************************************************************************/
