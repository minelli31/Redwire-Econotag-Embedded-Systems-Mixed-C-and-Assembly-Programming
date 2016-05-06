/*****************************************************************************/
/*                                                                           */
/* Sistemas Empotrados                                                       */
/*                                                                           */
/* Linker script para la Redwire EconoTAG                                    */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Punto de entrada                                                          */
/*****************************************************************************/

ENTRY(_start)

/*
 * Mapa de memoria de la placa
 */
MEMORY
{
        ram0 :  org = 0x00400000, len = 0x00002000        /* ( 8 KB) */
        ram1 :  org = 0x00402000, len = 0x00006000        /* (24 KB) */
        ram2 :  org = 0x00408000, len = 0x00008000        /* (32 KB) */
        ram3 :  org = 0x00410000, len = 0x00008000        /* (32 KB) */
        gpio :  org = 0x80000000, len = 0x00000068        /* (104 B) */
}

SECTIONS
{
        /* Código del programa */
        .text : ALIGN(4)
        {
                *(.text);
        } > ram0

        /* Variables inicializadas */
        .data : ALIGN(4)
        {
                *(.data);
        } > ram1

        /* GPIO */
        .gpio :
        {
          GPIO_BASE =               .;
          GPIO_DATA0 =              GPIO_BASE + 0x00000008;
          GPIO_PAD_DIR0 =           GPIO_BASE + 0x00000000;
          GPIO_PAD_DIR1 =           GPIO_BASE + 0x00000004;
          GPIO_DATA_SET0 =          GPIO_BASE + 0x00000048;
          GPIO_DATA_SET1 =          GPIO_BASE + 0x0000004c;
          GPIO_DATA_RESET1 =        GPIO_BASE + 0x00000054;
        } > gpio
}
