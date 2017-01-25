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

/** Registers definition of the common part of the auxiliaries module */
typedef struct auxcommon_t
{
    struct IRQ                       /**< The AUXIRQ register is used to check 
                                      *   any pending interrupts which may be 
                                      *   asserted by the three Auxiliary sub
                                      *   blocks.                             */
    {
        DEF_CBIT  (MiniUART);        /**< If set the mini UART has an 
                                      *   interrupt pending.                  */
        DEF_CBIT  (SPI1);            /**< If set the SPI1 module has an
                                      *  interrupt pending.                   */
        DEF_CBIT  (SPI2);            /**< If set the SPI2 module has an
                                      *  interrupt pending.                   */
        DEF_CBITS (Reserved,29);     /**< Reserved                            */
    } IRQ;                           
    struct ENABLES                   /**< The AUXENB register is used to
                                      *  enable the three modules; UART, SPI1,
                                      *  SPI2.                                */
    {                                
        DEF_BIT   (MiniUART);        /**< If set the mini UART is enabled.
                                      *  The UART will immediately start
                                      *  receiving data, especially if the
                                      *  UART1_RX line is low.
                                      *  If clear the mini UART is disabled.
                                      *  That also disable any mini UART
                                      *  register access                      */
        DEF_BIT   (SPI1);            /**< If set the SPI 1 module is enabled.
                                      *  If clear the SPI 1 module is
                                      *  disabled. That also disable any SPI
                                      *  1 module register acess              */
        DEF_BIT   (SPI2);            /**< If set the SPI 2 moduel is enabled.
                                      *  If clear the SPI2 moduel is
                                      *  disabled. That also disable any SPI
                                      *  2 module register access             */
        DEF_CBITS (Reserved,29);     /**< Reserved                            */
    } ENABLES;
} auxcommon_t;
/** Registers definition of the miniUART module of the auxiliaries module */
typedef struct miniUART_t
{
    struct MU_IO_REG                 /**< The AUX_MU_IO_REG register is 
                                      *   primary used to write data and read
                                      *   data from the UART FIFOs.
                                      *   If the DLAB bit in the line control
                                      *   register is set this register gives
                                      *   access to the LS 8 bits of the baud
                                      *   rate. (Note: there is easier access 
                                      *   to the baud rate register)          */
    {
        union
        {
            DEF_BITS  (LSBAUD,8);    /**< Access to the LS 8 bits of the 
                                      *   16-bit baudrate register. (Only If 
                                      *   bit 7 of the line control register
                                      *  (DLAB bit) is set)                   */
            DEF_BITS  (TXDATA,8);    /**< Data written is put in the transmit
                                      *   FIFO (Provided it is not full)
                                      *   (Only If bit 7 of the line control 
                                      *   register (DLAB bit) is clear)       */
            DEF_CBITS (RXDATA,8);    /**< Data read is taken from the receive
                                      *   FIFO (Provided it is not empty)
                                      *   (Only If bit 7 of the line control
                                      *   register (DLAB bit) is clear)       */
        };
        DEF_CBITS (Reserved,24);     /**< Reserved                            */
    } MU_IO_REG;

    struct MU_IER_REG                /**< The AUX_MU_IER_REG register is
                                      *   primary used to enable interrupts.
                                      *   If the DLAB bit in the line control
                                      *   register is set this register gives
                                      *   access to the MS 8 bits of the baud
                                      *   rate. (Note: there is easier access
                                      *   to the baud rate register)          */
    {
        union
        {
            DEF_BITS  (MSBAUD,8);    /**< Access to the MS 8 bits of the 
                                      *  16-bit baudrate register. (Only If
                                      *   bit 7 of the line control register
                                      *  (DLAB bit) is set)                   */
            struct
            {
                DEF_CBITS (ETI,1);   /**< If this bit is set the interrupt
                                      *   line is asserted whenever the 
                                      *   transmit FIFO is empty.
                                      *   If this bit is clear no transmit
                                      *   interrupts are generated.           */
                DEF_CBITS (ERI,1);   /**< If this bit is set the interrupt
                                      *   line is asserted whenever the 
                                      *   reveice FIFO holds at least 1 byte.
                                      *   If this bit is clear no reveice
                                      *   interrupts are generated.           */
                DEF_CBITS (U,6);     /**< Reserved, write zero, read as don't
                                      *   care. Some of these bits have
                                      *   functions in a 16550 compatible UART
                                      *   but are ignored here                */
            };
        };
        DEF_CBITS (Reserved,24);     /**< Reserved                            */
    } MU_IER_REG;

    struct MU_IIR_REG                /**< The AUX_MU_IIR_REG register shows
                                      *   the interrupt status.
                                      *   It also has two FIFO enable status
                                      *   bits and (when writing) FIFO clear
                                      *   bits.                               */
    {
        DEF_CBIT  (IP);              /**< This bit is clear whenever an 
                                      *   interrupt is pending                */
        DEF_BITS  (INTID_FIFOCLR,2); /**< On read this register shows the
                                      *   interrupt ID bit
                                      *     00 : No interrupts
                                      *     01 : Transmit holding register 
                                      *          empty
                                      *     10 : Receiver holds valid byte
                                      *     11 : <Not possible>
                                      *   On write:
                                      *   Writing with bit 1 set will clear 
                                      *   the receive FIFO
                                      *   Writing with bit 2 set will clear
                                      *   the transmit FIFO                   */
        DEF_CBIT  (NONE0);           /**< Always read as zero as the mini UART
                                      *   has no timeout function             */
        DEF_CBITS (NONE1, 2);        /**< Always read as zero                 */
        DEF_CBITS (FIFOENS, 2);      /**< Both bits always read as 1 as the
                                      *   FIFOs are always enabled            */
        DEF_CBITS (Reserved,24);     /**< Reserved                            */
    } MU_IIR_REG;

    struct MU_LCR_REG                /**< The AUX_MU_LCR_REG register controls
                                      *   the line data format and gives
                                      *   access to the baudrate register     */
    {
        DEF_BIT   (DATA_SIZE);       /**< If clear the UART works in 7-bit 
                                      *   mode
                                      *   If set the UART works in 8-bit mode */
        DEF_CBITS (Rsrvd0,5);        /**< Reserved, write zero, read as don't
                                      *   care
                                      *   Some of these bits have functions in
                                      *   a 16550 compatible UART but are 
                                      *   ignored here                        */
        DEF_BIT   (BREAK);           /**< If set high the UART1_TX line is
                                      *   pulled low continuously. If held for
                                      *   at least 12 bits times that will
                                      *   indicate a break condition.         */
        DEF_BIT   (DLAB);            /**< If set the first to Mini UART 
                                      *   register give access the Baudrate
                                      *   register. During operation this bit
                                      *   must be cleared.                    */
        DEF_CBITS (Rsrvd1,24);       /**< Reserved                            */
    } MU_LCR_REG;

    struct MU_MCR_REG                /**< The AUX_MU_MCR_REG register controls
                                      *   the 'modem' signals.                */
    {
        DEF_CBIT  (Rsrvd0);          /**< Reserved, write zero, read as don't
                                      *   care
                                      *   This bit has a function in a 16550
                                      *   compatible UART but is ignored here */
        DEF_BIT   (RTS);             /**< If clear the UART1_RTS line is high
                                      *   If set the UART1_RTS line is low
                                      *   This bit is ignored if the RTS is
                                      *   used for auto-flow control. See the 
                                      *   Mini Uart Extra Control register
                                      *   description)                        */
        DEF_CBITS (Rsrvd1,6);        /**< Reserved, write zero, read as don't
                                      *   care
                                      *   Some of these bits have functions in
                                      *   a 16550 compatible UART but are
                                      *   ignored here                        */
        DEF_CBITS (Rsrvd,24);        /**< Reserved                            */
    } MU_MCR_REG;

    struct MU_LSR_REG                /**< The AUX_MU_LSR_REG register shows
                                      *   the data status.                    */
    {
        DEF_CBIT  (DATA_READY);      /**< This bit is set if the receive FIFO
                                      *   holds at least 1 symbol.            */
        DEF_BIT   (RCVR_OVRUN);      /**< This bit is set if there was a
                                      *   receiver overrun. That is: one or
                                      *   more characters arrived whilst the
                                      *   receive FIFO was full. The newly
                                      *   arrived charters have been
                                      *   discarded. This bit is cleared each
                                      *   time this register is read. To do a
                                      *   non-destructive read of this overrun
                                      *   bit use the Mini Uart Extra Status
                                      *   register .                          */
        DEF_CBITS (Rsrvd0, 3);       /**< Reserved, write zero, read as don't
                                      *   care
                                      *   Some of these bits have functions in
                                      *   a 16550 compatible UART but are
                                      *   ignored here                        */
        DEF_CBIT  (TX_EMPTY);        /**< This bit is set if the transmit FIFO
                                      *   can accept at least one byte.       */
        DEF_CBIT  (TX_IDLE);         /**< This bit is set if the transmit FIFO
                                      *   is empty and the transmitter is
                                      *   idle. (Finished shifting out the
                                      *   last bit).                          */
        DEF_CBIT  (Rsrvd1);          /**< Reserved, write zero, read as don't
                                      *   care
                                      *   This bit has a function in a 16550
                                      *   compatible UART but is ignored here */
        DEF_CBITS (Rsrvd,24);        /**< Reserved                            */
    } MU_LSR_REG;

    struct MU_MSR_REG                /**< The AUX_MU_MSR_REG register shows
                                      *   the 'modem' status.                 */
    {
        DEF_CBITS (Rsrvd0,4);        /**< Reserved, write zero, read as don't
                                      *   care
                                      *   Some of these bits have functions in
                                      *   a 16550 compatible UART but are
                                      *   ignored here                        */
        DEF_BIT   (RTS);             /**< This bit is the inverse of the
                                      *   UART1_CTS input Thus:
                                      *     If set the UART1_CTS pin is low
                                      *     If clear the UART1_CTS pin is high*/
        DEF_CBITS (Rsrvd1,2);        /**< Reserved, write zero, read as don't
                                      *   care
                                      *   Some of these bits have functions in
                                      *   a 16550 compatible UART but are
                                      *   ignored here                        */
        DEF_CBITS (Rsrvd,24);        /**< Reserved                            */
    } MU_MSR_REG;

    struct MU_SCRATCH                /**< The AUX_MU_SCRATCH is a single byte
                                      *   storage.                            */
    {
        DEF_BITS  (SCRATCH,8);       /**< One whole byte extra on top of the
                                      *   134217728 provided by the SDC       */
        DEF_CBITS (Rsrvd,24);        /**< Reserved                            */
    } MU_SCRATCH;

    struct MU_CNTL_REG               /**< The AUX_MU_CNTL_REG provides access
                                      *   to some extra useful and nice
                                      *   features not found on a normal 16550
                                      *   UART.                               */
    {
        DEF_BIT   (RX_EN);           /**< If this bit is set the mini UART
                                      *   receiver is enabled.
                                      *   If this bit is clear the mini UART
                                      *   receiver is disabled                */
        DEF_BIT   (TX_EN);           /**< If this bit is set the mini UART
                                      *   transmitter is enabled.
                                      *   If this bit is clear the mini UART
                                      *   transmitter is disabled             */
        DEF_BIT   (RX_RTS_EN);       /**< If this bit is set the RTS line will
                                      *   de-assert if the receive FIFO reaches
                                      *   it 'auto flow' level. In fact the RTS
                                      *   line will behave as an RTR (Ready To
                                      *   Receive) line.
                                      *   If this bit is clear the RTS line is
                                      *   controlled by the AUX_MU_MCR_REG
                                      *   register bit 1.                     */
        DEF_BIT   (TX_CTS_EN);       /**< If this bit is set the transmitter
                                      *   will stop if the CTS line is
                                      *   de-asserted.
                                      *   If this bit is clear the transmitter
                                      *   will ignore the status of the CTS
                                      *   line                                */
        DEF_BITS  (RTS_LVL,2);       /**< These two bits specify at what
                                      *   receiver FIFO level the RTS line is
                                      *   de-asserted in auto-flow mode.
                                      *     00 : De-assert RTS when the receive
                                      *          FIFO has 3 empty spaces left.
                                      *     01 : De-assert RTS when the receive
                                      *          FIFO has 2 empty spaces left.
                                      *     10 : De-assert RTS when the receive
                                      *          FIFO has 1 empty space left.
                                      *     11 : De-assert RTS when the receive
                                      *          FIFO has 4 empty spaces left.*/
        DEF_BIT   (RTS_POL);         /**< This bit allows one to invert the RTS
                                      *   auto flow operation polarity.
                                      *   If set the RTS auto flow assert level
                                      *   is low*
                                      *   If clear the RTS auto flow assert
                                      *   level is high*                      */
        DEF_BIT   (CTS_POL);         /**< This bit allows one to invert the CTS
                                      *   auto flow operation polarity.
                                      *   If set the CTS auto flow assert level
                                      *   is low*
                                      *   If clear the CTS auto flow assert
                                      *   level is high*                      */
        DEF_CBITS (Rsrvd,24);        /**< Reserved                            */
    } MU_CNTL_REG;

    struct MU_STAT_REG               /**< The AUX_MU_STAT_REG provides a lot
                                      *   of useful information about the
                                      *   internal status of the mini UART
                                      *   not found on a normal 16550 UART.   */
    {                                
        DEF_CBIT  (SYM_AVAIL);       /**< If this bit is set the mini UART
                                      *   receive FIFO contains at least 1
                                      *   symbol
                                      *   If this bit is clear the mini UART
                                      *   receiver FIFO  is empty             */
        DEF_CBIT  (SPACE_AVAIL);     /**< If this bit is set the mini UART
                                      *   transmitter FIFO can accept at
                                      *   least one more symbol.
                                      *   If this bit is clear the mini UART
                                      *   transmitter FIFO is full            */
        DEF_CBIT  (RX_IDLE);         /**< If this bit is set the receiver is
                                      *   idle.
                                      *   If this bit is clear the receiver is
                                      *   busy.
                                      *   This bit can change unless the
                                      *   receiver is disabled                */
        DEF_CBIT  (TX_IDLE);         /**< If this bit is set the transmitter
                                      *   is idle.
                                      *   If this bit is clear the transmitter
                                      *   is idle.                            */
        DEF_CBIT  (RX_OVRUN);        /**< This bit is set if there was a
                                      *   receiver overrun. That is: one or
                                      *   more characters arrived whilst the
                                      *   receive FIFO was full. The newly 
                                      *   arrived characters have been 
                                      *   discarded. This bit is cleared each
                                      *   time the AUX_MU_LSR_REG register is
                                      *   read.                               */
        DEF_CBIT  (TX_FULL);         /**< This is the inverse of bit 1        */
        DEF_CBIT  (RTS_STAT);        /**< This bit shows the status of the
                                      *   UART1_RTS line.                     */
        DEF_CBIT  (CTS_LINE);        /**< This bit shows the status of the
                                      *   UART1_CTS line.                     */
        DEF_CBIT  (TX_EMPTY);        /**< If this bit is set the transmitter
                                      *   FIFO is empty. Thus it can accept 8
                                      *   symbols.                            */
        DEF_CBIT  (TX_DONE);         /**< This bit is set if the transmitter
                                      *   is idle and the transmit FIFO is
                                      *   empty.
                                      *   It is a logic AND of bits 2 and 8   */
        DEF_CBITS (Rsrvd0,6);        /**< Reserved                            */
        DEF_CBITS (RX_LVL,4);        /**< These bits shows how many symbols
                                      *   are stored in the receive FIFO
                                      *   The value is in the range 0-8       */
        DEF_CBITS (Rsrvd1,4);        /**< Reserved                            */
        DEF_CBITS (TX_LVL,4);        /**< These bits shows how many symbols
                                      *   are stored in the transmit FIFO
                                      *   The value is in the range 0-8       */
        DEF_CBITS (Rsrvd2,4);        /**< Reserved                            */
    } MU_STAT_REG;                   
                                     
    struct MU_BAUD_REG               /**< The AUX_MU_BAUD register allows
                                      *   direct access to the 16-bit wide
                                      *   baudrate counter.                   */
    {                                
        DEF_BITS  (BAUDRATE,16);     /**< mini UART baudrate counter          */
        DEF_CBITS (Rsrvd,16);        /**< Reserved                            */
    } MU_BAUD_REG;
}  miniUART_t;
/** Registers definition of the SPI module of the auxiliaries module */
typedef struct SPI_t
{
    struct SPIX_CNTL0_REG            /**< The AUXSPIx_CNTL0 register control
                                      *   many features of the SPI interfaces.*/
    {                                
        DEF_BITS  (SHLEN,6);         /**< Specifies the number of bits to
                                      *   shift
                                      *   This field is ignored when using
                                      *   'variable shift' mode               */
        DEF_BIT   (SHOUTDIR);        /**< If 1 the data is shifted out
                                      *   starting with the MS bit. (bit 15 or
                                      *   bit 11)
                                      *   If 0 the data is shifted out
                                      *   starting with the LS bit. (bit 0)   */
        DEF_BIT   (CLKPOL);          /**< If 1 the 'idle' clock line state is
                                      *   high.
                                      *   If 0 the 'idle' clock line state is
                                      *   low.                                */
        DEF_BIT   (OUTCLKEDGE);      /**< If 1 data is clocked out on the
                                      *   rising edge of the SPI clock
                                      *   If 0 data is clocked out on the
                                      *   falling edge of the SPI clock       */
        DEF_BIT   (CLRFIFO);         /**< If 1 the receive and transmit FIFOs
                                      *   are held in reset (and thus
                                      *   flushed.)
                                      *   This bit should be 0 during normal
                                      *   operation.                          */
        DEF_BIT   (INCLKEDGE);       /**< If 1 data is clocked in on the
                                      *   rising edge of the SPI clock
                                      *   If 0 data is clocked in on the
                                      *   falling edge of the SPI clock       */
        DEF_BIT   (ENABLE);          /**< Enables the SPI interface. Whilst
                                      *   disabled the FIFOs can still be
                                      *   written to or read from
                                      *   This bit should be 1 during normal
                                      *   operation.                          */
        DEF_BITS  (HOLDTIME,2);      /**< Controls the extra DOUT hold time
                                      *   in system clock cycles.
                                      *      00 : No extra hold time
                                      *      01 : 1 system clock extra hold
                                      *           time
                                      *      10 : 4 system clocks extra hold
                                      *           time
                                      *      11 : 7 system clocks extra hold
                                      *           time                        */
        DEF_BIT   (VARWIDTH);        /**< If 1 the SPI takes the shift length
                                      *   and the data from the TX fifo
                                      *   If 0 the SPI takes the shift length
                                      *   from bits 0-5 of this register      */
        DEF_BIT   (VARCS);           /**< If 1 the SPI takes the CS pattern
                                      *   and the data from the TX fifo
                                      *   If 0 the SPI takes the CS pattern
                                      *   from bits 17-19 of this register
                                      *   Set this bit only if also bit 14
                                      *   (variable width) is set             */
        DEF_BIT   (POSTINPUT);       /**< If set the SPI input works in post
                                      *   input mode.                         */
        DEF_BITS  (CS,3);            /**< The pattern output on the CS pins
                                      *   when active.                        */
        DEF_BITS  (SPEED,12);        /**< Sets the SPI clock speed. spi clk
                                      *   freq= system_clock_freq/2*(speed+1) */
    } SPIX_CNTL0_REG;

    struct SPIX_CNTL1_REG            /**< The AUXSPIx_CNTL1 registers control
                                      *   more features of the SPI interfaces.*/
    {                                
        DEF_BIT   (KEEPIN);          /**< If 1 the receiver shift register is
                                      *   NOT cleared. Thus new data is
                                      *   concatenated to old data.
                                      *   If 0 the receiver shift register is
                                      *   cleared before each transaction.    */
        DEF_BIT   (SHINDIR);         /**< If 1 the data is shifted in starting
                                      *   with the MS bit. (bit 15)
                                      *   If 0 the data is shifted in starting
                                      *   with the LS bit. (bit 0)            */
        DEF_CBITS (Rsrvd0,4);        /**< Reserved                            */
        DEF_BIT   (DONEIRQ);         /**< If 1 the interrupt line is high when
                                      *   the interface is idle               */
        DEF_BIT   (TXEMIRQ);         /**< If 1 the interrupt line is high when
                                      *   the transmit FIFO is empty          */
        DEF_BITS  (CSTIME,3);        /**< Additional SPI clock cycles where
                                      *   the CS is high.                     */
        DEF_CBITS (Rsrvd1,21);       /**< Reserved                            */
    } SPIX_CNTL1_REG;

    struct SPIX_STAT_REG             /**< The AUXSPIx_STAT registers show the
                                      *   status of the SPI interfaces.       */
    {
        union
        {
            DEF_BITS  (BITCNT,6);    /**< The number of bits still to be
                                      *   processed. Starts with
                                      *   'shift-length' and counts down.     */
            struct
            {
                DEF_CBITS (Rsrvd0,2);/**< Reserved                            */
                DEF_BIT   (RXEMPTY); /**< If 1 the receiver FIFO is empty
                                      *   If 0 the receiver FIFO holds at
                                      *    least 1 data unit.                 */
                DEF_BIT   (TXEMPTY); /**< If 1 the transmit FIFO is empty
                                      *   If 0 the transmit FIFO holds at
                                      *   least 1 data unit.                  */
                DEF_BIT   (TXFULL);  /**< If 1 the transmit FIFO is full
                                      *   If 0 the transmit FIFO can accept at
                                      *   least 1 data unit.                  */
                DEF_CBIT (Rsrvd1);   /**< Reserved                            */
            };
        };
        DEF_BIT   (BUSY);            /**< Indicates the module is busy
                                      *   transferring data.                  */
        DEF_CBITS (Rsrvd2,5);        /**< Reserved                            */
        DEF_BITS  (RXLVL,12);        /**< The number of data units in the
                                      *   receive data FIFO.                  */
        DEF_BITS  (TXLVL,8);         /**< The number of data units in the
                                      *   transmit data FIFO                  */
    } SPIX_STAT_REG;
                                     
    struct SPIX_PEEK_REG             /**< The AUXSPIx_PEEK registers show
                                      *   received data of the SPI interfaces.*/
    {                                
        DEF_CBITS (DATA,16);         /**< Reads from this address will show 
                                      *   the top entry from the receive FIFO,
                                      *   but the data is not taken from the
                                      *   FIFO. This provides a means of 
                                      *   inspecting the data but not removing
                                      *   it from the FIFO.                   */
        DEF_CBITS (Rsrvd,16);        /**< Reserved                            */
    } SPIX_PEEK_REG;
                                     
    struct SPIX_IO_REG               /**< The AUXSPIx_IO registers are the
                                      *   extended CS port of the SPI
                                      *   interfaces
                                      *   These four addresses all write to 
                                      *   the same FIFO.                      */
    {                                
        DEF_CBITS (DATA,16);         /**< Writes to this address range end up
                                      *   in the transmit FIFO. Data is lost 
                                      *   when writing whilst the transmit 
                                      *   FIFO is full.
                                      *   Reads from this address will take
                                      *   the top entry from the receive FIFO.
                                      *   Reading whilst the receive FIFO is
                                      *   will return the last data received. */
        DEF_CBITS (Rsrvd,16);        /**< Reserved                            */
    } SPIX_IO_REG;
} SPI_t;
/** Registers address definitions */
typedef struct aux_t
{
    auxcommon_t* common;
    miniUART_t*  miniUART;
    SPI_t*       SPI0;
    SPI_t*       SPI1;
} aux_t;

/******************************************************************************
 ***** Vars definitions                                                   *****
 ******************************************************************************/

static aux_t AUX =
{
        (auxcommon_t *)0x20215000,  /**< Addr of the common part of the
                                     *   Auxiliaries module */
        (miniUART_t *)0x20215040,   /**< Addr of the miniUART */
        (SPI_t *)0x20215080,        /**< Addr of the SPI0 */
        (SPI_t *)0x202150C0         /**< Addr of the SPI1 */
};

/******************************************************************************
 ***** Private function declaration                                       *****
 ******************************************************************************/


/******************************************************************************
 ***** Public function definitions                                        *****
 ******************************************************************************/


/******************************************************************************
 ***** Private function definitions                                       *****
 ******************************************************************************/

void print_miniuart(const char *s)
{
    miniUART_t* dev = AUX.miniUART;
    AUX.common->ENABLES.MiniUART = 1;

    while(*s != '\0') { /* Loop until end of string */
        dev->MU_IO_REG.TXDATA = (unsigned int)(*s); /* Transmit char */
        s++; /* Next char */
    }
}
