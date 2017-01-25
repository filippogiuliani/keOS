/**
 *    Brief comment
 *    
 *    Detailed comment.
 *
 *
 *    File:       <FILENAME>
 *    Author:     <AUTHORNAME>
 *    Mail:       <AUTHORMAIL>
 *    Date:       <COMMITTERDATEISO8601>
 *    Ident:      <COMMITHASH>
 *    Branch:     <BRANCH>
 *
 *    <CHANGELOG:--reverse --grep "^tags.*relevant":-1:%an : %ai : %s>
 *
 *
 *    (C) Copyright 2016 Filippo Giuliani <mail@filippogiuliani.it>
 *
 *    This file is part of keOS.
 *
 *    keOS is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    keOS is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with keOS.  If not, see <http://www.gnu.org/licenses/>.
 */

/******************************************************************************
 ***** Include                                                            *****
 ******************************************************************************/

#include <types.h>
#include <bsp.h>
#include <private.h>

/******************************************************************************
 ***** Constant definitions                                               *****
 ******************************************************************************/

/******************************************************************************
 ***** Type definitions                                                   *****
 ******************************************************************************/

typedef struct uart_t
{
    struct DR                       /**< The UART_DR Register is the data
                                     *   register. For words to be
                                     *   transmitted:
                                     *   if the FIFOs are enabled, data
                                     *   written to this location is pushed
                                     *   onto the transmit FIFO.
                                     *   if the FIFOs are not enabled, data
                                     *   is stored in the transmitter holding
                                     *   register (the bottom word of the
                                     *   transmit FIFO).
                                     *   The write operation initiates
                                     *   transmission from the UART. The data
                                     *   is prefixed with a start bit,
                                     *   appended with the appropriate parity
                                     *   bit (if parity is enabled), and a
                                     *   stop bit. The resultant word is then
                                     *   transmitted.
                                     *   For received words:
                                     *   if the FIFOs are enabled,
                                     *   the data byte and the 4-bit status
                                     *   (break, frame, parity, and overrun)
                                     *   is pushed onto the 12-bit wide
                                     *   receive FIFO if the FIFOs are not
                                     *   enabled, the data byte and status
                                     *   are stored in the receiving holding
                                     *   register (the bottom word of the
                                     *   receive FIFO).                      */
    {
        DEF_BITS  (DATA,8);         /**< Receive (read) data character.
                                     *   Transmit (write) data character.    */
        DEF_BIT   (FE);             /**< Framing error. When set to 1, it
                                     *   indicates that the received
                                     *   character did not have a valid stop
                                     *   bit (a valid stop bit is 1). In FIFO
                                     *   mode, this error is associated with
                                     *   the character at the top of the
                                     *   FIFO.                               */
        DEF_BIT   (PE);             /**< Parity error. When set to 1, it
                                     *   indicates that the parity of the
                                     *   received data character does not
                                     *   match the parity that the EPS and
                                     *   SPS bits in the Line Control
                                     *   Register, UART_LCRH select. In FIFO
                                     *   mode, this error is associated with
                                     *   the character at the top of the
                                     *   FIFO.                               */
        DEF_BIT   (BE);             /**< Break error. This bit is set to 1 if
                                     *   a break condition was detected,
                                     *   indicating that the received data
                                     *   input was held LOW for longer than a
                                     *   full-word transmission time (defined
                                     *   as start, data, parity and stop
                                     *   bits).
                                     *   In FIFO mode, this error is
                                     *   associated with the character at the
                                     *   top of the FIFO. When a break
                                     *   occurs, only one 0 character is
                                     *   loaded into the FIFO. The next
                                     *   character is only enabled after the
                                     *   receive data input goes to a 1
                                     *   (marking state), and the next valid
                                     *   start bit is received.              */
        DEF_BIT   (OE);             /**< Overrun error. This bit is set to 1
                                     *   if data is received and the receive
                                     *   FIFO is already full.
                                     *   This is cleared to 0 once there is
                                     *   an empty space in the FIFO and a new
                                     *   character can be written to it.     */
        DEF_CBITS (RSRVD,20);       /**< Reserved - Write as 0, read as don't
                                     *   care                                */
    } DR;
    DEF_UNDEFREG (RSRECR);
    DEF_NOREG (WAITSPACE0x08);
    DEF_NOREG (WAITSPACE0x0C);
    DEF_NOREG (WAITSPACE0x10);
    DEF_NOREG (WAITSPACE0x14);
    DEF_UNDEFREG (FR);
    DEF_NOREG (WAITSPACE0x1C);
    DEF_UNDEFREG (ILPR);            /**< Reserved - write zero, read as don't
                                     *   care.                               */
    struct IBRD                     /**< The UART_IBRD Register is the
                                     *   integer part of the baud rate
                                     *   divisor value.                      */
    {
        DEF_BITS  (IBRD,16);        /**< The integer baud rate divisor.      */
        DEF_CBITS (RSRVD,16);       /**< Reserved - Write as 0, read as don't
                                     *   care                                */
    } IBRD;
    struct FBRD                     /**< The UART_FBRD Register is the
                                     *   fractional part of the baud rate
                                     *   divisor value. The baud rate divisor
                                     *   is calculated as follows: Baud rate
                                     *   divisor BAUDDIV = (FUARTCLK/(16 Baud
                                     *   rate)) where FUARTCLK is the UART
                                     *   reference clock frequency. The
                                     *   BAUDDIV is comprised of the integer
                                     *   value IBRD and the fractional value
                                     *   FBRD. NOTE: The contents of the IBRD
                                     *   and FBRD registers are not up dated
                                     *   until transmission or reception of
                                     *   the current character is complete.  */
    {
        DEF_BITS  (FBRD,6);         /**< The fractional baud rate divisor.   */
        DEF_CBITS (RSRVD,26);       /**< Reserved - Write as 0, read as don't
                                     *   care                                */
    }  FBRD;
    struct LCRH                     /**< The UARTLCR_ LCRH Register is the
                                     *   line control register.
                                     *   NOTE: The UART_LCRH, UART_IBRD, and
                                     *   UART_FBRD registers must not be
                                     *   changed: when the UART is enabled
                                     *   when completing a transmission or a
                                     *   reception when it has been
                                     *   programmed to become disabled.      */
    {
        DEF_BIT   (BRK );           /**< Send break. If this bit is set to 1,
                                     *   a low-level is continually output on
                                     *   the TXD output, after completing
                                     *   transmission of the current
                                     *   character.                          */
        DEF_BIT   (PEN );           /**< Parity enable:
                                     *   0 = parity is disabled and no parity
                                     *       bit added to the data frame
                                     *   1 = parity checking and generation
                                     *       is enabled. See Table 25 9.     */
        DEF_BIT   (EPS );           /**< Even parity select. Controls the
                                     *   type of parity the UART uses during
                                     *   transmission and reception:
                                     *   0 = odd parity. The UART generates
                                     *       or checks for an odd number of
                                     *       1s in the data and parity bits.
                                     *   1 = even parity. The UART generates
                                     *   or checks for an even number of 1s
                                     *   in the data and parity bits.
                                     *   This bit has no effect when the PEN
                                     *   bit disables parity checking and
                                     *   generation. See Table 25 9.         */
        DEF_BIT   (STP2 );          /**< Two stop bits select. If this bit is
                                     *   set to 1, two stop bits are
                                     *   transmitted at the end of the frame.
                                     *   The receive logic does not check for
                                     *   two stop bits being received.       */
        DEF_BIT   (FEN );           /**< Enable FIFOs:
                                     *   0 = FIFOs are disabled (character
                                     *       mode) that is, the FIFOs become
                                     *       1-byte-deep holding registers
                                     *   1 = transmit and receive FIFO
                                     *       buffers are enabled (FIFO mode).*/
        DEF_BITS  (WLEN,2);         /**< Word length. These bits indicate the
                                     *   number of data bits transmitted or
                                     *   received in a frame as follows:
                                     *      b11 = 8 bits
                                     *      b10 = 7 bits
                                     *      b01 = 6 bits
                                     *      b00 = 5 bits.                    */
        DEF_CBIT  (SPS );           /**< Stick parity select.
                                     *   0 = stick parity is disabled
                                     *   1 = either: if the EPS bit is 0 then
                                     *       the parity bit is transmitted
                                     *       and checked as a 1 if the EPS
                                     *       bit is 1 then the parity bit is
                                     *       transmitted and checked as a 0.
                                     *       See Table 25 9.                 */
        DEF_CBITS (RSRVD,24);       /**< Reserved - Write as 0, read as don't
                                     *   care                                */
    } LCRH;
    struct CR                       /**< The UART_CR Register is the control
                                     *   register.
                                     *   NOTE: To enable transmission, the
                                     *   TXE bit and UARTEN bit must be set
                                     *   to 1.
                                     *   Similarly, to enable reception, the
                                     *   RXE bit and UARTEN bit, must be set
                                     *   to 1.
                                     *   NOTE: Program the control registers
                                     *   as follows:
                                     *   1. Disable the UART.
                                     *   2. Wait for the end of transmission
                                     *      or reception of the current
                                     *      character.
                                     *   3. Flush the transmit FIFO by
                                     *      setting the FEN bit to 0 in the
                                     *      Line Control Register, UART_LCRH.
                                     *   4. Reprogram the Control Register,
                                     *      UART_CR.
                                     *   5. Enable the UART.                 */
    {
        DEF_BIT   (UARTEN);         /**< UART enable:
                                     *   0 = UART is disabled. If the UART is
                                     *   disabled in the middle of
                                     *   transmission or reception, it
                                     *   completes the current character
                                     *   before stopping.
                                     *   1 = the UART is enabled.            */
        DEF_CBIT  (SIREN);          /**< Unsupported, write zero, read as
                                     *   don't care                          */
        DEF_CBIT  (SIRLP);          /**< Unsupported, write zero, read as
                                     *   don't care                          */
        DEF_CBITS (RSRVD0,4);       /**< Reserved - Write as 0, read as don't
                                     *   care                                */
        DEF_BIT   (LBE);            /**< Loopback enable. If this bit is set
                                     *   to 1, the UARTTXD path is fed
                                     *   through to the UARTRXD path. In UART
                                     *   mode, when this bit is set, the
                                     *   modem outputs are also fed through
                                     *   to the modem inputs. This bit is
                                     *   cleared to 0 on reset, to disable
                                     *   loopback.                           */
        DEF_BIT   (TXE);            /**< Transmit enable. If this bit is set
                                     *   to 1, the transmit section of the
                                     *   UART is enabled. Data transmission
                                     *   occurs for UART signals. When the
                                     *   UART is disabled in the middle of
                                     *   transmission, it completes the
                                     *   current character before stopping.  */
        DEF_BIT   (RXE);            /**< Receive enable. If this bit is set
                                     *   to 1, the receive section of the
                                     *   UART is enabled. Data reception
                                     *   occurs for UART signals. When the
                                     *   UART is disabled in the middle of
                                     *   reception, it completes the current
                                     *   character before stopping.          */
        DEF_CBIT  (DTR);            /**< Unsupported, write zero, read as
                                     *   don't care                          */
        DEF_BIT   (RTS);            /**< Request to send. This bit is the
                                     *   complement of the UART request to
                                     *   send, nUARTRTS, modem status output.
                                     *   That is, when the bit is programmed
                                     *   to a 1 then nUARTRTS is LOW.        */
        DEF_CBIT  (OUT1);           /**< Unsupported, write zero, read as
                                     *   don't care                          */
        DEF_CBIT  (OUT2);           /**< Unsupported, write zero, read as
                                     *   don't care                          */
        DEF_BIT   (RTSEN);          /**< RTS hardware flow control enable. If
                                     *   this bit is set to 1, RTS hardware
                                     *   flow control is enabled. Data is
                                     *   only requested when there is space
                                     *   in the receive FIFO for it to be
                                     *   received.                           */
        DEF_BIT   (CTSEN);          /**< CTS hardware flow control enable. If
                                     *   this bit is set to 1, CTS hardware
                                     *   flow control is enabled. Data is
                                     *   only transmitted when the nUARTCTS
                                     *   signal is asserted.                 */
        DEF_CBITS (RSRVD1,16);      /**< Reserved - Write as 0, read as don't
                                     *   care                                */
    } CR;
    DEF_UNDEFREG (IFLS);
    DEF_UNDEFREG (IMSC);
    DEF_UNDEFREG (RIS);
    DEF_UNDEFREG (MIS);
    DEF_UNDEFREG (ICR);
    DEF_UNDEFREG (DMACR);
    DEF_NOREG (WAITSPACE0x4C);
    DEF_NOREG (WAITSPACE0x50);
    DEF_NOREG (WAITSPACE0x54);
    DEF_NOREG (WAITSPACE0x58);
    DEF_NOREG (WAITSPACE0x5C);
    DEF_NOREG (WAITSPACE0x60);
    DEF_NOREG (WAITSPACE0x64);
    DEF_NOREG (WAITSPACE0x68);
    DEF_NOREG (WAITSPACE0x6C);
    DEF_NOREG (WAITSPACE0x70);
    DEF_NOREG (WAITSPACE0x74);
    DEF_NOREG (WAITSPACE0x78);
    DEF_NOREG (WAITSPACE0x7C);
    DEF_UNDEFREG (ITCR);
    DEF_UNDEFREG (ITIP);
    DEF_UNDEFREG (ITOP);
    DEF_UNDEFREG (TDR);

/*    struct RSRECR
    {
        DEF_CBITS(TBD,32);
    } RSRECR;
    struct FR
    {
        DEF_CBITS(TBD,32);
    } FR;
    struct ILPR
    {
        DEF_CBITS(TBD,32);
    } ILPR;




    struct IFLS
    {
        DEF_CBITS(TBD,32);
    } IFLS;
    struct IMSC
    {
        DEF_CBITS(TBD,32);
    } IMSC;
    struct RIS
    {
        DEF_CBITS(TBD,32);
    } RIS;
    struct MIS
    {
        DEF_CBITS(TBD,32);
    } MIS;
    struct ICR
    {
        DEF_CBITS(TBD,32);
    } ICR;
    struct DMACR
    {
        DEF_CBITS(TBD,32);
    } DMACR;
    struct ITCR
    {
        DEF_CBITS(TBD,32);
    } ITCR;
    struct ITIP
    {
        DEF_CBITS(TBD,32);
    } ITIP;
    struct ITOP
    {
        DEF_CBITS(TBD,32);
    } ITOP;
    struct TDR
    {
        DEF_CBITS(TBD,32);
    } TDR;*/
} uart_t;

/******************************************************************************
 ***** Vars definitions                                                   *****
 ******************************************************************************/

static uart_t * dev = (uart_t *)0x2020100;

/******************************************************************************
 ***** Private function declaration                                       *****
 ******************************************************************************/


/******************************************************************************
 ***** Public function definitions                                        *****
 ******************************************************************************/


/******************************************************************************
 ***** Private function definitions                                       *****
 ******************************************************************************/

void print_uart(const char *s)
{
    dev->CR.UARTEN = 0;
    dev->CR.TXE = 0;
    dev->LCRH.WLEN = 3;
    dev->IBRD.IBRD = 1;
    dev->CR.TXE = 1;
    dev->CR.UARTEN = 1;

    while(*s != '\0') { /* Loop until end of string */
        dev->DR.DATA = (unsigned int)(*s); /* Transmit char */
        s++; /* Next char */
    }
}
