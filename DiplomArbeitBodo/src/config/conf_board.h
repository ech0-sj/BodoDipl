/**
 * \file
 *
 * \brief Board configuration.
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

/* enable usart0 (wifi) */
#define CONF_BOARD_UART_CONSOLE
#define CONF_BOARD_USART_RXD
#define CONF_BOARD_USART_TXD
#define CONSOLE_UART_ID          ID_UART


/* enable usart1 (console)*/ 
#define CONF_BOARD_USART1_RXD
#define CONF_BOARD_USART1_TXD
/* in init.c eingefügt: 
#ifdef CONF_BOARD_USART1_RXD
gpio_configure_pin(PIN_USART1_RXD_IDX, PIN_USART1_RXD_FLAGS);
#endif

#ifdef CONF_BOARD_USART1_TXD
gpio_configure_pin(PIN_USART1_TXD_IDX, PIN_USART1_TXD_FLAGS);
#endif
*/ 


/** SPI0 MACRO definition */
#define CONF_BOARD_SPI0

/** SPI0 slave select0 MACRO definition, sollte PA28 sein */
#define CONF_BOARD_SPI0_NPCS1

/** Spi Hw ID . */
// #define SPI_ID          ID_SPI0


#define W5500_RESET_PIN		IOPORT_CREATE_PIN( PIOA, 2)	/* Pin A2 als reset Pin festlegen  */ 
#define W5500_CS_PIN		IOPORT_CREATE_PIN( PIOA, 28)

#endif // CONF_BOARD_H
