![GitHub](https://img.shields.io/github/license/vanvught/GD32207C-EVAL-board-DMX512-RDM)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://img.shields.io/badge/C%2B%2B-11%-blue.svg)
![GitHub issues](https://img.shields.io/github/issues-raw/vanvught/GD32207C-EVAL-board-DMX512-RDM)
![GitHub contributors](https://img.shields.io/github/contributors/vanvught/GD32207C-EVAL-board-DMX512-RDM)
![GitHub Sponsors](https://img.shields.io/github/sponsors/vanvught)
![Main](https://github.com/vanvught/GD32207C-EVAL-board-DMX512-RDM/actions/workflows/c-cpp.yml/badge.svg?branch=main)

# GD32207C-EVAL-board-DMX512-RDM

DMX512 implementation for the GD32F207

The master source code: [https://github.com/vanvught/rpidmx512](https://github.com/vanvught/rpidmx512)

# UARTs
USART0 [USART0 Remapping=0]
<table>
<tbody>
<tr>
<td>&nbsp;TX</td>
<td>&nbsp;PA9</td>
</tr>
<tr>
<td>&nbsp;RX</td>
<td>&nbsp;PA10</td>
</tr>
</tbody>
</table>

USART2 [USART2 Remapping=01  (partial remap)]
<table>
<tbody>
<tr>
<td>&nbsp;TX</td>
<td>&nbsp;PC10</td>
</tr>
<tr>
<td>&nbsp;RX</td>
<td>&nbsp;PC11</td>
</tr>
</tbody>
</table>

UART4 [UART4 Remapping=0]
<table>
<tbody>
<tr>
<td>&nbsp;TX</td>
<td>&nbsp;PC12</td>
</tr>
<tr>
<td>&nbsp;RX</td>
<td>&nbsp;PD2</td>
</tr>
</tbody>
</table>

USART5 [USART5 Remapping=0]
<table>
<tbody>
<tr>
<td>&nbsp;TX</td>
<td>&nbsp;PC6</td>
</tr>
<tr>
<td>&nbsp;RX</td>
<td>&nbsp;PC7</td>
</tr>
</tbody>
</table>
# Timers
<table>
<tbody>
<tr>
<td>&nbsp;1</td>
<td>&nbsp;DMX Transmit</td>
</tr>
<tr>
<td>&nbsp;2</td>
<td>&nbsp;DMX Receive Slot time-out Port 0,1,2,3</td>
</tr>
<tr>
<td>&nbsp;3</td>
<td>&nbsp;DMX Receive Slot time-out Port 4,5,6,7</td>
</tr>
<tr>
<td>&nbsp;5</td>
<td>&nbsp;delay</td>
</tr>
<tr>
<td>&nbsp;6</td>
<td>&nbsp;DMX Receive 1 second interrupt</td>
</tr>
<tr>
<td>&nbsp;8</td>
<td>&nbsp;micros Slave -> ITI2</td>
</tr>
<tr>
<td>&nbsp;9</td>
<td>&nbsp;micros Master -> TIMER9_TRGO</td>
</tr>
</tbody>
</table>

[PayPal.Me Donate](https://paypal.me/AvanVught?locale.x=nl_NL)