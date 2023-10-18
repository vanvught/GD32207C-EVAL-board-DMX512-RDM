/**
 * @file board_gd32f207vc_2.h
 *
 */
/* Copyright (C) 2023 by Arjan van Vught mailto:info@gd32-dmx.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef GD32_BOARD_GD32F207VC_2_H
#define GD32_BOARD_GD32F207VC_2_H

#include "gd32_board.h"

namespace max {
static constexpr auto OUT = 2U;
static constexpr auto IN = 2U;
}  // namespace max

#define DMX_MAX_PORTS  2

#define DMX_USE_USART0
#define DMX_USE_USART1

static constexpr auto USART0_PORT = 0;
static constexpr auto USART1_PORT = 1;

static constexpr auto DIR_PORT_0_GPIO_PORT = GPIOE;
static constexpr auto DIR_PORT_0_GPIO_PIN  = GPIO_PIN_9;

static constexpr auto DIR_PORT_1_GPIO_PORT = GPIOE;
static constexpr auto DIR_PORT_1_GPIO_PIN  = GPIO_PIN_10;

#endif /* GD32_BOARD_GD32F207VC_2_H */
