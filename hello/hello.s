@
@ Sistemas Empotrados
@ El "hola mundo" en la Redwire EconoTAG
@

@
@ Variables
@
    LED_RED_MASK:
      .word     (1 << (44-32))
    LED_GREEN_MASK:
      .word     (1 << (45-32))
    S2_S:
      .word     (1 << 22)
    S2_E:
      .word     (1 << 27)
    S3_S:
      .word     (1 << 23)
    S3_E:
      .word     (1 << 26)


@ Retardo para el parpadeo
    .set DELAY,            0x00080000

@
@ Punto de entrada
@

    .code 32
    .text
    .global _start
    .type   _start, %function

_start:
    bl gpio_init

    @ Direcciones de los registros GPIO_DATA_SET1 y GPIO_DATA_RESET1
    ldr     r6, =GPIO_DATA_SET1
    ldr     r7, =GPIO_DATA_RESET1

		@ldr		r5, =(LED_RED_MASK)
loop:
    bl    test_buttons

    @ Encendemos el led
    str     r5, [r6]

    @ Pausa corta
    ldr     r0, =DELAY
    bl      pause

    @ Apagamos el led
    str     r5, [r7]

    @ Pausa corta
    ldr     r0, =DELAY
    bl      pause

    @ Bucle infinito
    b       loop

@
@ Función que produce un retardo
@ r0: iteraciones del retardo
@
        .type   pause, %function
pause:
    subs    r0, r0, #1
    bne     pause
    mov     pc, lr

gpio_init:
    ldr     r0, =LED_RED_MASK
    ldr		  r1, [r0]
    ldr     r0, =LED_GREEN_MASK
    ldr		  r2, [r0]
    orr 	  r1, r1, r2

    ldr     r0, =GPIO_PAD_DIR1
    str     r1, [r0]

    mov	pc, lr

test_buttons:
		ldr     r0, =GPIO_DATA0
    ldr     r1, [r0]

		ldr     r0, =S2_E
    ldr 	  r2, [r0]
    tst		  r1, r2
    ldrne   r4, =LED_RED_MASK
    ldrne   r5, [r4]

    ldr		  r0, =S3_E
    ldr     r3, [r0]
    tst		  r1, r3
    ldrne   r4, =LED_GREEN_MASK
    ldrne   r5, [r4]

    mov	pc, lr
