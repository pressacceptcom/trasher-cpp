/**
 * The Motorola MC68B09E Emulator Component
 *
 * Emulates the 8-bit microprocessing unit in accordance with the device description given in the
 * Motorola catalog pg 4-298.
 */

/**
 * Front Page (4-298):
 *
 * 8-Bit Microprocessing Unit
 *
 * The MC6809E is a revolutionary high performance 8-bit mircoprocessor which supports modern
 * programming techniques such as position independence, reentrancy, and modular programming.
 *
 * This third-generation addition to the M6800 family has major architectural improvements which
 * include additional registers, instructions and addressing modes.__data__
 *
 * The basic instructions of any computer are greatly enhanced by the presence of powerful addresing
 * modes. The MC6809E has the most complete set of addressing modes available on any 8-bi
 * microprocessor today.
 *
 * The MC6809E has hardware and software features which make it an ideal processor for higher level
 * language execution or standard controller applications. External clock inputs are provided to
 * allow synchronization with peripherals, systems or other MPUs.
 *
 * MC6800 COMPATIBLE
 *
 * + Hardware - Interfaces with All M6800 Peripherals
 * + Software - Upward Source Code Compatible Instruction Set and Addressing Modes
 *
 * ARCHITECTURAL FEATURES
 *
 * + Two 16-bit Index Registers
 * + Two 16-bit Indexable Stack Pointers
 * + Two 8--bit Accumulators can be Concatenated to Form One 16-Bit Accumulator,
 * + Direct Page Register Allows Direct Addressing Throughout Memory
 *
 * HARDWARE FEATURES
 *
 * + External Clock Inputs, E and Q, Allow Synchronization
 * + TSC Input Controls Internal Bus Buffers
 * + LIC Indicates Opcode Fetch
 * + AVMA Allows Efficient Use of Common Resources in A Multiprocessor System
 * + BUSY is a Status Line for Multiprocessing
 * + Fast Interrupt Request Input Stacks Only Condition Code Register and Program Counter
 * + Interrupt Acknowledge Output Allows Vectoring By Devices
 * + SYNC Acknowledge Output Allows for Synchronization to External Event
 * + Single Bus-Cycle RESET
 * + Single 5-Volt Supply Operation
 * + NMI Inhibited After RESET Until Until After First Load of Stack Pointer
 * + Early Address Valid Allows Use With Slower Memories
 * + Early Write-Data for Dynamic Memories
 *
 * SOFTWARE FEATURES
 *
 * + 10 Addressing Modes
 *     + M6800 Upward Compatible Addressing Modes
 *     + Direct Addressing Anywhere in Memory Map
 *     + Long Relative Branches
 *     + Program Counter Relative
 *     + True Indirect Addressing
 *     + Expanded Indexed Addressing:
 *         + 0, 5, 8, or 16-bit Constant Offsets
 *         + 8, or 16-bit Accumulator Offsets
 *         + Auto-Increment/Decrement by 1 or 2
 * + Improved Stack Manipulation
 * + 1464 Instructions with Unique Addressing Modes
 * + 8 x 8 Unsigned Multiply
 * + 16-bit Arithmetic
 * + Transfer/Exchange All Registers
 * + Push/Pull Any Registers or Any Set of Registers
 * + Load Effective Address
 */

/**
 * PIN ASSIGNMENT
 *
 *       ---\__/---           PIN DESCRIPTIONS
 *  VSS [| 1   40 |] HALT
 *       |        |           POWER (VSS, VCC)
 *  NMI [| 2   39 |] TSC          Two pins are used to supply power to the part: VSS is ground or O
 *       |        |               volts, while VCC is +5.0 V ±5%
 *  IRQ [| 3   38 |] LIC
 *       |        |           ADDRESS BUS (A0-A15)
 * FIRQ [| 4   37 |] RESET        Sixteen pins are used to output address information from the MPU
 *       |        |               onto the Address Bus. When the processor does not require the bus
 *   BS [| 5   36 |] AVMA         for a data transfer, it will output address FFFF (16), R/W = 1,
 *       |        |               and BS = 0; this is a"dummy access"or VMA cycle. All address bus
 *   BA [| 6   35 |] Q            drivers are made high-impedance when output Bus Available (BA) is
 *       |        |               high or when TSC is asserted. Each pin will drive one Schottky TTL
 *  VCC [| 7   34 |] E            load or four LS TTL loads, and 90 pF.
 *       |        |
 *   A0 [| 8   33 |] BUSY     DATA BUS (D0-D7)
 *       |        |               These eight pins provide communication with the system
 *   A1 [| 9   32 |] R/W          bi-directional data bus. Each pin will drive one Schottky TTL load
 *       |        |               or four LS TTL loads, and 130 pF.
 *   A2 [| 10  31 |] D0
 *       |        |           READ/WRITE (R/W)
 *   A3 [| 11  30 |] D1           This signal indicates the direction of data transfer on the data
 *       |        |               bus. A low indicates that the MPU is writing data onto the data
 *   A4 [| 12  29 |] D2           bus. R/W is made high impedance when BA is high or when TSC is
 *       |        |               asserted.
 *   A5 [| 13  28 |] D3
 *       |        |           RESET
 *   A6 [| 14  27 |] D4           A low level on this Schmitt-trigger input for greater than one bus
 *       |        |               cycle will reset the MPU, as shown in Figure 7. The Reset vectors
 *   A7 [| 15  26 |] D5           are fetched from locations FFFE (16) and FFFF (16) (Table 1) when
 *       |        |               Interrupt Acknowledge is true, (BA • BS = 1). During initial
 *   A8 [| 16  25 |] D6           power-on, the Reset line should be held low until the clock input
 *       |        |               signals are fully operational.
 *   A9 [| 17  24 |] D7
 *       |        |               Because the MC6809E Reset pin has a Schmitt-trigger input with a
 *  A10 [| 18  23 |] A15          threshold voltage higher than that of standard peripherals, a
 *       |        |               simple R/C network may be used to reset the entire system. This
 *  A11 [| 19  22 |] A14          higher threshold voltage ensures that all peripherals are out of
 *       |        |               the reset state before the Processor.
 *  A12 [| 20  21 |] A13
 *       ----------
 *
 * HALT
 *     A low level on this input pin will cause the MPU to stop running at the end of the present
 *     instruction and remain halted indefinitely without loss of data. When halted, the BA output
 *     is driven high indicating the buses are high-impedance. BS is also high which indicates the
 *     processor is in the Halt state. While halted, the MPU will not respond to external real-time
 *     requests (FIRO, IRQ) although NMI or RESET will be latched for later response. During the
 *     Halt state Q and E should continue to run normally. A halted state (BA • BS = 1) can be
 *     achieved by pulling HALT low while RESET is still low. See Figure 8.
 *
 * BUS AVAILABLE, BUS STATUS (BA, BS)
 *     The Bus Available output is an indication of an internal control signal which makes the MOS
 *     buses of the MPU high impedance. When BA goes low, a dead cycle will elapse before the MPU
 *     acquires the bus. BA will not be asserted when TSC is active, thus allowing dead cycle
 *     consistency.
 *
 *     The Bus Status output signal, when decoded with BA, represents the MPU state (valid with
 *     leading edge of Q).
 *
 *     ----------------------------------------------
 *     | MPU State |           MPU State            |
 *     -------------           Definition           |
 *     |  BA | BS  |                                |
 *     ----------------------------------------------
 *     |  0  |  0  | Normal (Running)               |
 *     |  0  |  1  | Interrupt or RESET Acknowledge |
 *     |  1  |  0  | SYNC Acknowledge               |
 *     |  1  |  1  | HALT Acknowledge               |
 *     ----------------------------------------------
 *
 *     Interrupt Acknowledge is indicated during both cycles of a hardware-vector-fetch (RESET, NMI,
 *     FIRQ, IRQ, SWI, SWl2, SWl3). This signal, plus decoding of the lower four address lines, can
 *     provide the user with an indication of which interrupt level is being serviced and allow
 *     vectoring by device. See Table 1.
 *
 *     TABLE 1 - MEMORY MAP FOR INTERRUPT VECTORS
 *
 *     ------------------------------------------------------------------
 *     | Memory Map For Vector Locations |       Interrupt Vector       |
 *     -----------------------------------          Description         |
 *     |       MS       |       LS       |                              |
 *     ------------------------------------------------------------------
 *     |      FFFE      |      FFFF      |            RESET             |
 *     ------------------------------------------------------------------
 *     |      FFFC      |      FFFD      |             NMI              |
 *     ------------------------------------------------------------------
 *     |      FFFA      |      FFFB      |             SWI              |
 *     ------------------------------------------------------------------
 *     |      FFF8      |      FFF9      |             IRQ              |
 *     ------------------------------------------------------------------
 *     |      FFF6      |      FFF7      |            FIRQ              |
 *     ------------------------------------------------------------------
 *     |      FFF4      |      FFF5      |            SWI2              |
 *     ------------------------------------------------------------------
 *     |      FFF2      |      FFF3      |            SWI3              |
 *     ------------------------------------------------------------------
 *     |      FFF0      |      FFF1      |           Reserved           |
 *     ------------------------------------------------------------------
 *
 *     Sync Acknowledge is indicated while the MPU is waiting for external synchronization on an
 *     interrupt line.
 *
 *     Halt/Acknowledge is indicated when the MC6809E is in a Halt condition.
 *
 * NON MASKABLE INTERRUPT (NMI)*
 *     A negative transition on this input requests that a non-maskable interrupt sequence be
 *     generated. A non-maskable interrupt cannot be inhibited by the program, and also has a higher
 *     priority than FIRO, IRO or software interrupts. During recognition of an NMI, the entire
 *     machine state is saved on the hardware stack. After reset, an NM! will not be recognized
 *     until the first program load of the Hardware Stack Pointer (S). The pulse width of NMI low
 *     must be at least one E cycle. If the NMI input does not meet the minimum set up with respect
 *     to Q, the interrupt will not be recognized until the next cycle. See Figure 9.
 *
 * FAST-INTERRUPT REQUEST (FIRQ)*
 *     A low level on this input pin will initiate a fast interrupt sequence, provided its mask bit
 *     (F) in the CC is clear. This sequence has priority over the standard Interrupt Request (IRQ),
 *     and is fast in the sense that it stacks only the contents of the condition code register and
 *     the program counter. The interrupt service routine should clear the source of the interrupt
 *     before doing an RTI. See Figure 10.
 *
 * INTERRUPT REQUEST (IRQ)*
 *     A low level input on this pin will initiate an Interrupt Request sequence provided the mask
 *     bit (I) in the CC is clear. Since IRQ stacks the entire machine state it provides a slower
 *     response to interrupts than FIRQ. IRQ also has a lower priority than FIRQ. Again, the
 *     interrupt service routine should clear the source of the interrupt before doing an RTI. See
 *     Figure 9.
 *
 * *NMI, FIRQ, and IRQ requests are sampled on the falling edge of Q. One cycle is required for
 * synchronization before these interrupts are recognized. The pending interrupts will not be
 * serviced until completion of the current instruction unless a SYNC or CWAI condition is present.
 * If IRQ and FIRQ do not remain low until completion of the current instruction they may not be
 * recognized. However, NMI is latched and need only remain low for one cycle. No interrupts are
 * recognized or latched between the falling edge of RESET and the rising edge of BS indicating
 * RESET acknowledge. See RESET sequence in the MPU flowchart in Figure 15.
 *
 * CLOCK INPUTS E, Q
 *     E and Q are the clock signals required by the MC6809E. Q must lead E; that is, a transition
 *     on Q must be followed by a similar transition on E after a minimum delay. Addresses will be
 *     valid from the MPU, t(AD) after the falling edge of E, and data will be latched from the bus
 *     by the falling edge of E. While the Q input is fully TTL compatible, the E input directly
 *     drives internal MOS circuitry and, thus, requires a high level above normal TTL levels. This
 *     approach minimizes clock skew inherent with an internal buffer. Timing and waveforms tor E
 *     and Q are shown in Figure 2 while Figure 11 shows a simple clock generator tor the MC6809E.
 *
 * BUSY
 *     Busy will be high for the read and modify cycles of a read-modify-write instruction and
 *     during the access of the first byte of a double-byte operation (e.g., LDX, STD, ADDD). Busy
 *     is also high during the first byte of any indirect or other vector fetch (e.g., jump
 *     extended, SWI indirect etc.).
 *
 *     In a multi-processor system, busy indicates the need to defer the rearbitration of the next
 *     bus cycle to insure the integrity of the above operations. This difference provides the
 *     indivisible memory access required tor a "test-and-set" primitive, using any one of several
 *     read-modify-write instructions.
 *
 *     Busy does not become active during PSH or PUL operations. A typical read-modify-write
 *     instruction (ASL) is shown in Figure 12. Timing information is given in Figure 13. Busy is
 *     valid t(CD) after the rising edge of Q.
 *
 * AVMA
 *     AVMA is the Advanced VMA signal and indicates that the MPU will use the bus in the following
 *     bus cycle. The predictive nature of the AVMA signal allows efficient shared-bus multiprocessor
 *     systems. AVMA is LOW when the MPU is in either a HALT or SYNC state. AVMA is valid t(CD)
 *     after the rising edge of Q.
 *
 * LIC
 *     LIC (Last Instruction Cycle) is HIGH during the last cycle of every instruction, and its
 *     transition from HIGH to LOW will indicate that the first byte of an opcode will be latched at
 *     the end of the present bus cycle. LIC will be HIGH when the MPU is Halted at the end of an
 *     instruction, (i.e., not in CWAI or RESET) in SYNC state or while stacking during interrupts.
 *     LIC is valid t(CD) after the rising edge of Q.
 *
 * TSC
 *     TSC (Three-State Control) will cause MOS address, data, and R/W buffers to assume a
 *     high-impedance state. The control signals (BA, BS, BUSY, AVMA and LIC) will not go to the
 *     high-impedance state. TSC is intended to allow a single bus to be shared with other bus
 *     masters (processors or OMA controllers).
 *
 *     While E is low, TSC controls the address buffers and R/W directly. The data bus buffers
 *     during a write operation are in a high-impedance state until Q rises at which time, if TSC is
 *     true, they will remain in a high-impedance state. It TSC is held beyond the rising edge of E,
 *     then it will be internally latched, keeping the bus drivers in a high-impedance state for the
 *     remainder of the bus cycle. See Figure 14.
 */

/**
 * "Programming Model"
 *
 * From 4-301: As shown in Figure 5, the MC6809E adds three registers to the set available in the
 * MC6800. The added registers include a Direct Page Register, the User Stack pointer and a second
 * Index Register.
 * 15                           0
 * ------------------------------  \                      7                      0
 * |     X - Index Register     |  |                      ------------------------
 * ------------------------------  |                      | Direct Page Register |
 * |     Y - Index Register     |  |                      ------------------------
 * ------------------------------  |- Pointer Registers
 * |   U - User Stack Pointer   |  |                      7    Condition Code Register    0
 * ------------------------------  |                      ---------------------------------
 * | S - Hardware Stack Pointer |  |                      | E | F | H | I | N | Z | V | C |
 * ------------------------------  /                      ---------------------------------
 * |       Program Counter      |                           |   |   |   |   |   |   |   |
 * ------------------------------  \                   Entire   |   |   |   |   |   |   Carry
 * |      A      |      B       |  |- Accumulators              |   |   |   |   |   |
 * ------------------------------  /                    FIRQ Mask   |   |   |   |   Overflow
 * \____________________________/                                   |   |   |   |
 *               |                                         Half Carry   |   |   Zero
 *               D                                                      |   |
 *                                                               IRQ Mask   Negative
 * ACCUMULATORS (A, B, D)
 *
 * The A and B registers are general purpose accumulators which are used for arithmetic calculations
 * and manipulation of data.
 *
 * Certain instructions concatenate the A and B registers to form a single 16-bit accumulator. This
 * is referred to as the D Register, and is formed with the A Register as the most significant byte.
 *
 * DIRECT PAGE REGISTER (DP)
 *
 * The Direct Page Register of the MC6809E serves to enhance the Direct Addressing Mode. The content
 * of this register appears at the higher address outputs (A8-A15) during direct addressing
 * instruction execution. This allows the direct mode to be used at any place in memory, under
 * program control. To ensure M6800 compatibility, all bits of this register are cleared during
 * Processor Reset.
 *
 * INDEX REGISTERS (X, Y)
 *
 * The Index Registers are used in indexed mode of addressing. The 16-bit address in this register
 * takes part in the calculation of effective addresses. This address may be used to point to data
 * directly or may be modifed by an optional constant or register offset. During some indexed modes,
 * the contents of the index register are incremented or decremented to point to the next item of
 * tabular type data. All four pointer register (X, Y, U, S) may be used as index registers.
 *
 * STACK POINTER (U, S)
 *
 * The Hardware Stack Pointer (S) is used automatically by the processor during subroutine calls and
 * interrupts. The User Stack Pointer (U) is controlled exclusively by the programmer thus allowing
 * arguments to be passed to and from subroutines with ease. The U-register is frequently used as a
 * stack marker. Both Stack Pointers have the same indexed mode addressing capabilities as the X and
 * Y registers, but also support Push and Pull instructions. This allows the MC6809E to be used
 * efficiently as a stack processor, greatly enhancing its ability to support higher level languages
 * and modular programming.
 *
 * NOTE: The stack pointers of the MC6B09E point to the top of the stack, in contrast to the MC6800
 *       stack pointer, which pointed to the next free location on stack.
 *
 * PROGRAM COUNTER
 *
 * The Program Counter is used by the processor to point to the address of the next instruction to
 * be executed by the processor. Relative Addressing is provided allowing the Program Counter to be
 * used like an index register in some situations.
 *
 * CONDITION CODE REGISTER
 *
 * The Condition Code Register defines the state of the processor at any given time.
 *
 * CONDITION CODE REGISTER DESCRIPTION
 *
 * BIT 0 (C) - Bit O is the carry flag, and is usually the carry from the binary ALU. C is also used
 *             to represent a 'borrow' from subtract like instructions (CMP, NEG, SUB, SBC) and is
 *             the complement of the carry from the binary ALU.
 *
 * BIT 1 (V) - Bit 1 is the overflow flag, and is set to a one by an operation which causes a signed
 *             two's complement arithmetic overflow. This overflow is detected in an operation in
 *             which the carry from the MSB in the ALU does not match the carry from the MSB-1.
 *
 * BIT 2 (Z) - Bit 2 is the zero flag, and is set to a one if the result of the previous operation
 *             was identically zero.
 *
 * BIT 3 (N) - Bit 3 is the negative flag, which contains exactly the value of the MSB of the result
 *             of the preceding operation. Thus, a negative two's-complement result will leave N set
 *             to a one.
 *
 * BIT 4 (I) - Bit 4 is the IRQ mask bit. The processor will not recognize interrupts from the IRQ
 *             line if this bit is set to a one. NMI, FIRQ, IRQ, RESET, and SWI all set I to a one;
 *             SWl2 and SWl3 do not affect I.
 *
 * BIT 5 (H) - Bit 5 is the half-carry bit, and is used to indicate a carry from bit 3 in the ALU as
 *             a result of an 8-bit addition only (ADC or ADD). This bit is used by the DAA
 *             instruction to perform a BCD decimal add adjust operation. The state of this flag is
 *             undefined in all subtract-like instructions.
 *
 * BIT 6 (F) - Bit 6 is the FIRQ mask bit. The processor will not recognize interrupts from the FIRQ
 *             line if this bit is a one. NMI, FIRQ, SWI, and RESET all set F to a one. IRQ, SWl2
 *             and SWl3 do not affect F.
 *
 * BIT 7 (E) - Bit 7 is the entire flag, and when set to a one indicates that the complete machine
 *             state (all the registers) was stacked, as opposed to the subset state (PC and CC).
 *             The E bit of the stacked CC is used on a return from interrupt (RTI) to determine the
 *             extent of the unstacking. Therefore, the current E left in the Condition Code
 *             Register represents past action.
 */

/**
 * MPU OPERATION
 *
 * During normal operation, the MPU fetches an instruction from memory and then executes the
 * requested function. This sequence begins after RESET and is repeated indefinitely unless altered
 * by a special instruction or hardware occurrence. Software instructions that alter normal MPU
 * operation are: SWI, SWl2, SWI3, CWAI, RTI and SYNC. An interrupt or HALT input can also alter the
 * normal execution of instructions. Figure 15 is the flow chart for the MC6809E.
 */

/**
 * ADDRESSING MODES (4-312)
 *
 * The basic instructions of any computer are greatly enhanced by the presence of powerful
 * addressing modes. The MC6809E has the most complete set of addressing modes available on any
 * microcomputer today. For example, the MC6809E has 59 basic instructions; however, it recognizes
 * 1464 different variations of instructions and addressing modes. The addressing modes support
 * modern programming techniques. The following addressing modes are available on the MC6809E:
 *
 * + Inherent (Includes Accumulator)
 * + Immediate
 * + Extended
 *     + Extended Indirect
 * + Direct
 * + Register
 * + Indexed
 *     + Zero-Offset
 *     + Constant Offset
 *     + Accumulator Offset
 *     + Auto Increment/Decrement
 *     + Indexed Indirect
 * + Relative
 *     + Short/Long Relative Branching
 *     + Program Counter Relative Addressing
 *
 * INHERENT (INCLUDES ACCUMULATOR)
 *
 *     In this addressing mode, the opcode of the instruction contains all the address information
 * necessary. Examples of Inherent Addressing are: ABX, DAA, SWI, ASRA, and CLRB.
 *
 * IMMEDIATE ADDRESSING
 *
 *     In Immediate Addressing, the effective address of the data is the location immediately
 * following the opcode {i.e., the data to be used in the instruction immediately follows the opcode
 * of the instruction). The MC6809E uses both 8 and 16-bit immediate values depending on the size of
 * argument specified by the opcode. Examples of instructions with Immediate Addressing are:
 *
 *     LDA #$20
 *     LDX #$FOOO
 *     LDY #CAT
 *
 * NOTE:  # signifies Immediate addressing, $ signifies hexa- decimal value to the MC6809 assembler.
 *
 * EXTENDED ADDRESSING
 *
 *     In Extended Addressing, the contents of the two bytes immediately following the opcode fully
 * specify the 16-bit effective address used by the instruction. Note that the address generated by
 * an extended instruction defines an absolute address and is not position independent. Examples of
 * Extended Addressing include:
 *
 *     LDA CAT
 *     STX MOUSE
 *     LDD $2000
 *
 * EXTENDED INDIRECT
 *
 *     As a special case of indexed addressing {discussed below), one level of indirection may be
 * added to Extended Addressing. In Extended Indirect, the two bytes following the postbyte of an
 * Indexed instruction contain the address of the data.
 *
 *     LDA [CAT]
 *     LDX [$FFFE]
 *     STU [DOG]
 *
 * DIRECT ADDRESSING
 *
 *     Direct addressing is similar to extended addressing except that only one byte of address
 * follows the opcode. This byte specifies the lower 8 bits of the address to be used. The upper 8
 * bits of the address are supplied by the direct page register. Since only one byte of address is
 * required in direct addressing, this mode requires less memory and executes faster than extended
 * addressing. Of course, only 256 locations (one page) can be accessed without redefining the
 * contents of the DP register. Since the DP register is set to $00 on Reset, direct addressing on
 * the MC6809E is upward compatible with direct addressing on the M6800. Indirection is not allowed
 * in direct addressing. Some examples of direct addressing are:
 *
 *     LDA where DP = $00
 *     LDB where DP = $10
 *     LDD <CAT
 *
 * NOTE: < is an assembler directive which forces direct addressing.
 *
 * REGISTER ADDRESSING
 *
 *     Some opcodes are followed by a byte that defines a register or set of registers to be used by
 * the instruction. This is called a postbyte. Some examples of register addressing are:
 *
 *     TFR    X, Y          Transfers X into Y
 *     EXG    A, B          Exchanges A with B
 *     PSHS   A, B, X, Y    Push Y, X, B, and A onto S stack
 *     PULU   X, Y, D       Pull D, X, ad Y from U stack
 *
 * INDEXED ADDRESSING
 *
 *     In all indexed addressing, one of the pointer registers {X, Y, U, S, and sometimes PC) is
 * used in a calculation of the effective address of the operand to be used by the instruction. Five
 * basic types of indexing are available and are discussed below. The postbyte of an indexed
 * instruction specifies the basic type and variation of the addressing mode as well as the pointer
 * register to be used. Figure 16 lists the legal formats for the postbyte. Table 2 gives the
 * assembler form and the number of cycles and bytes added to the basic values for indexed
 * addressing for each variation.
 *
 * FIGURE 16 - INDEXED ADDRESSING POSTBYTE REGISTER BIT ASSIGNMENTS
 *
 * ------------------------------------------------------------
 * |    Post-Byte Register Bit     |         Indexed          |
 * ---------------------------------        Addressing        |
 * | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |           Mode           |
 * ------------------------------------------------------------
 * | 0 | R | R | d | d | d | d | d |  EA = ,R + 5 Bit Offset  |
 * ------------------------------------------------------------
 * | 1 | R | R | 0 | 0 | 0 | 0 | 0 |           ,R+            |
 * ------------------------------------------------------------
 * | 1 | R | R | i | 0 | 0 | 0 | 1 |           ,R++           |
 * ------------------------------------------------------------
 * | 1 | R | R | 0 | 0 | 0 | 1 | 0 |            ,-R           |
 * ------------------------------------------------------------
 * | 1 | R | R | i | 0 | 0 | 1 | 1 |           ,--R           |
 * ------------------------------------------------------------
 * | 1 | R | R | i | 0 | 1 | 0 | 0 |    EA = ,R +0 Offset     |
 * ------------------------------------------------------------
 * | 1 | R | R | i | 0 | 1 | 0 | 1 |   EA = ,R + ACCB Offset  |
 * ------------------------------------------------------------
 * | 1 | R | R | i | 0 | 1 | 1 | 0 |   EA = ,R + ACCA Offset  |
 * ------------------------------------------------------------
 * | 1 | R | R | i | 1 | 0 | 0 | 0 |   EA = ,R + 8 Bit Offset |
 * ------------------------------------------------------------
 * | 1 | R | R | i | 1 | 0 | 0 | 1 |  EA = ,R + 16 Bit Offset |
 * ------------------------------------------------------------
 * | 1 | R | R | i | 1 | 0 | 1 | 1 |    EA = ,R + D Offset    |
 * ------------------------------------------------------------
 * | 1 | x | x | i | 1 | 1 | 0 | 0 |  EA = ,PC + 9 Bit Offset |
 * ------------------------------------------------------------
 * | 1 | x | x | i | 1 | 1 | 0 | 1 | EA = ,PC + 16 Bit Offset |
 * ------------------------------------------------------------
 * | 1 | R | R | i | 1 | 1 | 1 | 1 |      EA = [,Address]     |
 * ------------------------------------------------------------
 *     \______/\__/\_______________/
 *        |     |          |
 *        |     |      Addressing Mode Field
 *        |     |
 *        |  Indirect Field (Sign bit when b(7) = 0)
 *        |
 *     Register Field: RR
 *
 *     00 = X  01 = Y            x = Don't Care  d = Offset Bit
 *     10 = U  11 = S            i = 0 Not Indirect
 *                                   1 Indirect
 *
 * TABLE 2 - INDEXED ADDRESSING MODE
 *
 * ----------------------------------------------------------------------------------------------------------------
 * |                            |                   |          Non Indirect        |          Indirect            |
 * |            Type            |        Forms      ---------------------------------------------------------------
 * |                            |                   | Assembler | Postbyte | + | + | Assembler | Postbyte | + | + |
 * |                            |                   |    Form   |  OP Code | ~ | # |    Form   |  OP Code | ~ | # |
 * ----------------------------------------------------------------------------------------------------------------
 * | Constant Offset From R     | No Offset         |     ,R    | 1RR00100 | 0 | 0 |    [,R]   | 1RR10100 | 3 | 0 |
 * | (2's Complement Offsets)   -----------------------------------------------------------------------------------
 * |                            | 5 Bit Offset      |    n, R   | 0RRnnnnn | 1 | 0 |       defaults to 8-bit      |
 * |                            -----------------------------------------------------------------------------------
 * |                            | 8 Bit Offset      |    n, R   | 1RR01000 | 1 | 1 |   [n, R]  | 1RR11000 | 4 | 1 |
 * |                            -----------------------------------------------------------------------------------
 * |                            | 16 Bit Offset     |    n, R   | 1RR01001 | 4 | 2 |   [n, R]  | 1RR11001 | 7 | 2 |
 * ----------------------------------------------------------------------------------------------------------------
 * | Accumulator Offset From R  | A Register Offset |    A, R   | 1RR00110 | 1 | 0 |   [A, R]  | 1RR10110 | 4 | 0 |
 * | (2's Complement Offsets)   -----------------------------------------------------------------------------------
 * |                            | B Register Offset |    B, R   | 1RR00101 | 1 | 0 |   [B, R]  | 1RR10101 | 4 | 0 |
 * |                            -----------------------------------------------------------------------------------
 * |                            | D Register Offset |    D, R   | 1RR01011 | 4 | 0 |   [D, R]  | 1RR11011 | 7 | 0 |
 * ----------------------------------------------------------------------------------------------------------------
 * | Auto Increment/Decrement R | Increment By 1    |     ,R+   | 1RR00000 | 2 | 0 |         not allowed          |
 * |                            -----------------------------------------------------------------------------------
 * |                            | Increment By 2    |    ,R++   | 1RR00001 | 3 | 0 |   [,R++]  | 1RR10001 | 6 | 0 |
 * |                            -----------------------------------------------------------------------------------
 * |                            | Decrement By 1    |     ,-R   | 1RR00010 | 2 | 0 |         not allowed          |
 * |                            -----------------------------------------------------------------------------------
 * |                            | Decrement By 2    |    ,--R   | 1RR00011 | 3 | 0 |   [,--R]  | 1RR10011 | 6 | 0 |
 * ----------------------------------------------------------------------------------------------------------------
 * | Constant Offset From PC    | 8 Bit Offset      |   n, PCR  | 1xx01100 | 1 | 1 |  [n, PCR] | 1xx11100 | 4 | 1 |
 * | (2's Complement Offsets)   -----------------------------------------------------------------------------------
 * |                            | 16 Bit Offset     |   n, PCR  | 1xx01101 | 5 | 2 |  [n, PCR] | 1xx11101 | 8 | 2 |
 * ----------------------------------------------------------------------------------------------------------------
 * | Extended Indirect          | 16 Bit Address    |     -     |    --    | - | - |     [n]   | 10011111 | 5 | 2 |
 * ----------------------------------------------------------------------------------------------------------------
 *
 * R = X, Y, U, or S    RR: 00 = X  01 = Y  10 = U  11 = S
 *
 * x = Don't Care       + ~ and + # indicate the number of additional cycles and bytes respectively for the
 *                                  particular indexing variation.
 *
 * Zero-Offset Indexed - In this mode, the selected pointer register contains the effective address
 *     of the data to be used by the instruction. This is the fastest indexing mode.
 *
 *     Examples are:
 *
 *     LDD 0, X
 *     LDA ,S
 *
 * Constant Offset Indexed - In this mode, a two's-complement offset and the contents of one of the
 *     pointer registers are added to form the effective address of the operand. The pointer
 *     register's initial content is unchanged by the addition.
 *
 *     Three sizes of offsets are available:
 *
 *     5-bit  (-16 to +15)
 *     8-bit  (-128 to +127)
 *     16-bit (-32768 to +32767)
 *
 *     The two's complement 5-bit offset is included in the postbyte and, therefore, is most
 *     efficient in use of bytes and cycles. The two's complement 8-bit offset is contained in a
 *     single byte following the postbyte. The two's complement 16-bit offset is in the two bytes
 *     following the postbyte. In most cases the programmer need not be concerned with the size of
 *     this offset since the assembler will select the optimal size automatically.
 *
 *     Examples of constant-offset indexing are:
 *
 *     LDA 23,X
 *     LDX -2,S
 *     LDY 300,X
 *     LDU CAT,Y
 *
 * Accumulator-Offset Indexed - This mode is similar to constant offset indexed except that the
 *     two's-complement value in one of the accumulators (A, B or D) and the contents of one of the
 *     pointer registers are added to form the effective address of the operand. The contents of
 *     both the accumulator and the pointer register are unchanged by the addition. The postbyte
 *     specifies which accumulator to use as an offset and no additional bytes are required. The
 *     advantage of an accumulator offset is that the value of the offset can be calculated by a
 *     program at run-time.
 *
 *     Some examples are:
 *
 *     LDA  B,Y
 *     LDX  D,Y
 *     LEAX B,X
 *
 * Auto Increment/Decrement Indexed - In the auto increment addressing mode, the pointer register
 *     contains the address of the operand. Then, after the pointer register is used it is
 *     incremented by one or two. This addressing mode is useful in stepping through tables, moving
 *     data, or for the creation of software stacks. In auto decrement, the pointer register is
 *     decremented prior to use as the address of the data. The use of auto decrement is similar to
 *     that of auto increment; but the tables, etc., are scanned from the high to low addresses. The
 *     size of the increment/decrement can be either one or two to allow for tables of either 8- or
 *     16-bit data to be accessed and is selectable by the programmer. The pre-decrement,
 *     post-increment nature of these modes allow them to be used to create additional software
 *     stacks that behave identically to the U and S stacks.
 *
 *     Some examples of the auto increment/decrement addressing modes are:
 *
 *     LDA ,X+
 *     STD ,Y++
 *     LDB ,-Y
 *     LDX ,--S
 *
 *     Care should be taken in performing operations on 16-bit pointer registers (X, Y, U, S) where
 *     the same register is used to calculate the effective address.
 *
 *     Consider the following instruction:
 *
 *     STX 0,X++ (X initialized to 0)
 *
 *     The desired result is to store a 0 in locations $0000 and $0001 then increment X to point to
 *     $0002. In reality, the following occurs:
 *
 *     0   -> temp      calculate the EA; temp is a holding register
 *     X+2 -> X         perform autoincrement
 *     X   -> (temp)    do store operation
 *
 * INDEXED INDIRECT
 *
 * INDEXED INDIRECT
 *
 *     All of the indexing modes with the exception of auto increment/decrement by one, or a ±5-bit
 * offset may have an additional level of indirection specified. In indirect addressing, the
 * effective address is contained at the location specified by the contents of the Index Register
 * plus any offset. In the example below, the A accumulator is loaded indirectly using an effective
 * address calculated from the Index Register and an offset.
 *
 * Before Execution
 *
 * A=XX (don't care)
 * X = $FOOO
 *
 * $0100 LDA [$10,X]    EA is now $F010
 *
 * $F010 $Fl            $F150 is now the new EA
 * $F011 $50
 *
 * $F150 $AA
 *
 * After Execution
 *
 * A = $AA (Actual Data Loaded)
 * X = $FOOO
 *
 * All modes of indexed indirect are included except those which are meaningless (e.g., auto
 * increment/decrement by 1 indirect). Some examples of indexed indirect are:
 *
 * LDA [,Xl
 * LDD [10,Sl]
 * LDA [B,Y]
 * LDD [,X++]
 *
 * RELATIVE ADDRESSING
 *
 *     The byte(s) following the branch opcode is (are) treated as a signed offset which may be
 * added to the program counter. If the branch condition is true then the calculated address (PC +
 * signed offset) is loaded into the program counter. Program execution continues at the new
 * location as indicated by the PC; short (1 byte offset) and long (2 bytes offset) relative
 * addressing modes are available. All of memory can be reached in long relative addressing as an
 * effective address interpreted modulo 2^16. Some examples of relative ad- dressing are:
 *
 *        BEQ  CAT    (short)
 *        BGT  DOG    (short)
 * CAT    LBEQ RAT    (long)
 * DOG    LBGT RABBIT (long)
 *      *
 *      *
 * RAT    NOP
 * RABBIT NOP
 *
 * PROGRAM COUNTER RELATIVE
 *
 *     The PC can be used as the pointer register with 8 or 16-bit signed offsets. As in relative
 * addressing, the offset is added to the current PC to create the effective address. The effective
 * address is then used as the address of the operand or data. Program Counter Relative Addressing
 * is used for writing position independent programs. Tables related to a particular routine will
 * maintain the same relationship after the routine is moved, if referenced relative to the Program
 * Counter. Examples are:
 *
 * LDA  CAT,   PCR
 * LEAX TABLE, PCR
 *
 * Since program counter relative is a type of indexing, an additional level of indirection is
 * available.
 *
 * LDA [CAT, PCR]
 * LDU [DOG, PCR]
 */

/**
 * MC6809E INSTRUCTION SET
 *
 *     The instruction set of the MC6809E is similar to that of the MC6800 and is upward compatible
 * at the source code level. The number of opcodes has been reduced from 72 to 59, but because of
 * the expanded architecture and additional addressing modes, the number of available opcodes (with
 * different addressing modes) has risen from 197 to 1464.
 *
 * Some of the new instructions are described in detail below:
 *
 * PSHU/PSHS
 *
 *     The push instructions have the capability of pushing onto either the hardware stack (S) or
 * user stack (U) any single register, or set of registers with a single instruction.
 *
 * PULU/PULS
 *
 *     The pull instructions have the same capability of the push instruction, in reverse order.
 *
 * The byte immediately following the push or pull opcode determines which register or registers are
 * to be pushed or pulled. The actual PUSH/PULL sequence is fixed; each bit defines a unique
 * register to push or pull, as shown below.
 *
 * PUSH/PULL POST BYTE        STACKING ORDER
 * -----------------            PULL ORDER
 * | | | | | | | | |                 |
 * -----------------                 V
 *  | | | | | | | |
 *  | | | | | | | CCR                CC
 *  | | | | | | |                    A
 *  | | | | | | A                    B
 *  | | | | | |                      DP
 *  | | | | | B                     X Hi
 *  | | | | |                       X Lo
 *  | | | | DPR                     Y Hi
 *  | | | |                         Y Lo
 *  | | | X                        U/S Hi
 *  | | |                          U/S Lo
 *  | | Y                          PC Hi
 *  | |                            PC Lo
 *  | S/U
 *  |                                ∧
 *  PC                               |
 *                              PUSH ORDER
 *
 *                          INCREASING MEMORY
 *                                   |
 *                                   V
 *
 * TFR/EXG
 *
 *     Within the MC6809E, any register may be transferred to or exchanged with another of
 * like-size; i.e., 8-bit to 8-bit or 16-bit to 16-bit. Bits 4-7 of postbyte define the source
 * register, while bits 0-3 represent the destination register. These are denoted as follows:
 *
 * TRANSFER/EXCHANGE POST BYTE        REGISTER FIELD
 * ---------------------------        0000 D (AB)    1000 A
 * | SOURCE  |  DESTINATION  |        0001 X         1001 B
 * ---------------------------        0010 Y         1010 CCR
 *                                    0011 U         1011 DPR
 *                                    0100 S
 *                                    0101 PC
 *
 * NOTE: All other combinations are undefined and INVALID
 *
 * LEAX/LEAY/LEAU/LEAS
 *
 *     The LEA (Load Effective Address) works by calculating the effective address used in an
 * indexed instruction and stores that address value, rather than the data at that address, in a
 * pointer register. This makes all the features of the internal addressing hardware available to
 * the programmer. Some of the implications of this instruction are illustrated in Table 3.
 *
 * The LEA instruction also allows the user to access data and tables in a position independent
 * manner. For example:
 *
 * LEAX MSG1, PCR
 * LBSR PDATA             (Print message routine)
 *   *
 *   *
 * MSG1 FCC   'MESSSAGE'
 *
 * This sample program prints: 'MESSAGE'. By writing MSG1, PCR, the assembler computes the distance
 * between the present address and MSG1. This result is placed as a constant into the LEAX
 * instruction which will be indexed from the PC value at the time of execution. No matter where the
 * code is located, when it is executed, the computed offset from the PC will put the absolute
 * address of MSG1 into the X pointer register. This code is totally position independent.
 *
 * The LEA instructions are very powerful and use an internal holding register (temp). Care must be
 * exercised when using the LEA instructions with the autoincrement and autodecrement addressing
 * modes due to the sequence of internal operations. The LEA internal sequence is outlined as
 * follows:
 *
 * LEA a , b+    (any of the 16-bit pointer registers X, Y, U, or S may be substituted for a, and b)
 *
 * 1. b -> temp    (calculate the EA)
 * 2. b+1 -> b     (modify bm, postincrement)
 * 3. temp -> a    (load a)
 *
 * LEA a, -b
 *
 * 1. b-1 -> temp  (calculate EA with predecrement)
 * 2. b-1 -> b     (modify bm, predecrement)
 * 3. temp -> a    (load a)
 *
 * Autoincrement-by-two and autodecrement-by-two instructions work similarly. Note that LEAX ,X+
 * does not change X, however LEAX ,-X does decrement X. LEAX 1,X should be used to increment X by
 * one.
 *
 * TABLE 3 - LEA EXAMPLES
 * --------------------------------------------------------------
 * | Instruction |  Operation   |            Comment            |
 * --------------------------------------------------------------
 * | LEAX 10, X  | X + 10 -> X  | Adds 5-bit constant 10 to X   |
 * --------------------------------------------------------------
 * | LEAX 500, X | X + 500 -> X | Adds 16-bit constant 500 to X |
 * --------------------------------------------------------------
 * | LEAY A, Y   | Y + A -> Y   | Adds 8-bit A accumulator to Y |
 * --------------------------------------------------------------
 * | LEAU -10, U | U - 10 -> U  | Subtracts 10 from U           |
 * --------------------------------------------------------------
 * | LEAS -10, S | S - 10 -> S  | Used to reserve area on stack |
 * --------------------------------------------------------------
 * | LEAS 10, S  | S + 10 -> S  | Used to 'clean up' stack      |
 * --------------------------------------------------------------
 * | LEAX 5, S   | S + 5 -> X   | Transfers as well as adds     |
 * --------------------------------------------------------------
 *
 * MUL
 *
 *     Multiplies the unsigned binary numbers in the A and B ac- cumulator and places the unsigned
 * result into the 16-bit D accumulator. This unsigned multiply also allows multiple-precision
 * multiplications.
 *
 * Long And Short Relative Branches
 *
 *     The MC6809E has the capability of program counter relative branching throughout the entire
 * memory map. In this mode, if the branch is to be taken, the 8 or 16-bit signed offset is added to
 * the value of the program counter to be used as the effective address·. This allows the program to
 * branch anywhere in the 64K memory map. Position independent code can be easily generated through
 * the use of relative branching. Both short (8-bit) and long (16-bit) branches are available.
 *
 * SYNC
 *
 *     After encountering a Sync instruction, the MPU enters a Sync state, stops processing
 * instructions and waits for an interrupt. If the pending interrupt is non-maskable (NMI) or
 * maskable (FIRQ, IRQ) with its mask bit (F or I) clear, the processor will clear the Sync state
 * and perform the normal interrupt stacking and service routine. Since FIRQ and IRQ are not
 * edge-triggered, a low level with a minimum duration of three bus cycles is required to assure
 * that the interrupt will be taken. If the pending interrupt is maskable (FIRQ, IRQ) with its mask
 * bit (F or I) set, the processor will clear the Sync state and continue processing by executing
 * the next inline instruction. Figure 17 depicts Sync timing.
 *
 * Software Interrupts
 *
 *     A Software Interrupt is an instruction which will cause an interrupt, and its associated
 * vector fetch. These Software Interrupts are useful in operating system calls, software debugging,
 * trace operations, memory mapping, and software development systems. Three levels of SWI are
 * available on this MC6809E, and are prioritized in the following order: SWI, SWl2, SWl3.
 *
 * 16-Bit Operation
 *
 *     The MC6809E has the capability of processing 16-bit data. These instructions include loads,
 * stores, compares, adds, subtracts, transfers, exchanges, pushes and pulls.
 */

/**
 * CYCLE-BY-CYCLE OPERATION
 *
 *     The address bus cycle-by-cycle performance chart illustrates the memory-access sequence
 * corresponding to each possible instruction and addressing mode in the MC6809E. Each instruction
 * begins with an opcode fetch. While that opcode is being internally decoded, the next program byte
 * is always fetched. (Most instructions will use the next byte, so this technique considerably
 * speeds throughput.) Next, the operation of each opcode will follow the flow chart. VMA is an
 * indication of FFFF (16) on the address bus, R/W = 1 and BS = 0. The following examples illustrate
 * the use of the chart; see Figure 18.
 *
 * Example 1: LBSR (Branch Taken)
 *
 * Before Execution SP = F000                    CYCLE-BY-CYCLE FLOW
 *
 *                   *                Cycle #   Address   Data   R/W   Description
 *                   *
 * $8000           LBSR    CAT          1         8000     17     1    Opcode fetch
 *                   *                  2         8001     20     1    Offset High Byte
 *                   *                  3         8002     00     1    Offset Low Byte
 * $A000    CAT      *                  4         FFFF     *      1    VMA Cycle
 *                                      5         FFFF     *      1    VMA Cycle
 *                                      6         A000     *      1    Computed Branch Address
 *                                      7         FFFF     *      1    VMA Cycle
 *                                      8         EFFF     80     0    Stack High Order Byt of Return Address
 *                                      9         EFFE     03     0    Stack Low Order Byt of Return Address
 *
 * Example 2: DEC (Extended)          Cycle #   Address   Data   R/W   Description
 *
 * $8000    DEC    $A000                1         8000     7A     1    Opcode Fetch
 * $A000    FCB    $80                  2         8001     A0     1    Operand Address, High Byte
 *                                      3         8002     00     1    Operand Address, Low Byte
 *                                      4         FFFF     *      1    VMA Cycle
 *                                      5         A000     80     1    Read the Data
 *                                      6         FFFF     *      1    VMA Cycle
 *                                      7         A000     7F     0    Store the Decremented Data
 *
 *                                      * The data bus has the data at that particular address.
 */

/**
 * MC6809E INSTRUCTION SET TABLES
 *
 *     The instructions of the MC6809E have been broken down into five different categories. They
 * are as follows:
 *
 * * 8-Bit operation (Table 4)
 * * 16-Bit operation (Table 5)
 * * Index register/stack pointer instructions (Table 6)
 * * Relative branches (long or short) (Table 7)
 * * Miscellaneous instructions (Table 8)
 *
 * Hexadecimal values for the instructions are given in Table 9.
 *
 * TABLE 4 - 8-BIT ACCUMULATOR AND MEMORY INSTRUCTIONS
 * ------------------------------------------------------------------------
 * | Mnemonic(s)     | Operation                                          |
 * ------------------------------------------------------------------------
 * | ADCA, ADCB      | Add memory to accumulator with carry               |
 * ------------------------------------------------------------------------
 * | ADDA, ADDB      | Add memory to accumulator                          |
 * ------------------------------------------------------------------------
 * | ANDA, ANDB      | And memory with accumulator                        |
 * ------------------------------------------------------------------------
 * | ASL, ASLA, ASLB | Arithmetic shift of accumulator or memory left     |
 * ------------------------------------------------------------------------
 * | ASR, ASRA, ASRB | Arithmetic shift of accumulator or memory right    |
 * ------------------------------------------------------------------------
 * | BITA, BITB      | Bit test memory with accumulator                   |
 * ------------------------------------------------------------------------
 * | CLR, CLRA, CLRB | Clear accumulator or memory location               |
 * ------------------------------------------------------------------------
 * | CMPA, CMPB      | Compare memory from accumulator                    |
 * ------------------------------------------------------------------------
 * | COM, COMA, COMB | Complement accumulator or memory location          |
 * ------------------------------------------------------------------------
 * | DAA             | Decimal adjust A accumulator                       |
 * ------------------------------------------------------------------------
 * | DEC, DECA, DECB | Decrement accumulator or memory location           |
 * ------------------------------------------------------------------------
 * | EORA, EORB      | Exclusive or memory with accumulator               |
 * ------------------------------------------------------------------------
 * | EXG R1, R2      | Exchange R1 with R2 (R1, R2 = A, B, CC, DP)        |
 * ------------------------------------------------------------------------
 * | INC, INCA, INCB | Increment accumulator or memory location           |
 * ------------------------------------------------------------------------
 * | LDA, LDB        | Load accumulator from memory                       |
 * ------------------------------------------------------------------------
 * | LSL, LSLA, LSLB | Logical shift left accumulator or memory location  |
 * ------------------------------------------------------------------------
 * | LSR, LSRA, LSRB | Logical shift right accumulator or memory location |
 * ------------------------------------------------------------------------
 * | MUL             | Unsigned multiply (A x B -> D)                     |
 * ------------------------------------------------------------------------
 * | NEG, NEGA, NEGB | Negate accumulator or memory                       |
 * ------------------------------------------------------------------------
 * | ORA, ORB        | Or memory with accumulator                         |
 * ------------------------------------------------------------------------
 * | ROL, ROLA, ROLB | Rotate accumulator or memory left                  |
 * ------------------------------------------------------------------------
 * | ROR, RORA, RORB | Rotate accumulator or memory right                 |
 * ------------------------------------------------------------------------
 * | SBCA, SBCB      | Subtract memory from accumulator with borrow       |
 * ------------------------------------------------------------------------
 * | STA, STB        | Store accumulator to memory                        |
 * ------------------------------------------------------------------------
 * | SUBA, SUBB      | Subtract memory from accumulator                   |
 * ------------------------------------------------------------------------
 * | TST, TSTA, TSTB | Test accumulator or memory location                |
 * ------------------------------------------------------------------------
 * | TFR R1, R2      | Transfer R1 to R2 (R1, R2 = A, B, CC, DP)          |
 * ------------------------------------------------------------------------
 *
 * NOTE: A, B, CC, or DP may be pushed to (pulled from) either stack with
 *       PSHS, PSHU (PULS, PULU) instructions.
 *
 *
 * TABLE 5 - 16-BIT ACCUMULATOR AND MEMORY INSTRUCTIONS
 * --------------------------------------------------------------
 * | Mnemonic(s) | Operation                                    |
 * --------------------------------------------------------------
 * | ADDD        | Add memory to D accumulator                  |
 * --------------------------------------------------------------
 * | CMPD        | Compare memory from D accumulator            |
 * --------------------------------------------------------------
 * | EXG D, R    | Exchange D with X, Y, S, U, or PC            |
 * --------------------------------------------------------------
 * | LDD         | Load D accumulator from memory               |
 * --------------------------------------------------------------
 * | SEX         | Sign Extend B accumulator into A accumulator |
 * --------------------------------------------------------------
 * | STD         | Store D accumulator to memory                |
 * --------------------------------------------------------------
 * | SUBD        | Subtract memory from D accumulator           |
 * --------------------------------------------------------------
 * | TFR D, R    | Transfer D to X, Y, S, U, or PC              |
 * --------------------------------------------------------------
 * | TFR R, D    | Transfer X, Y, S, U, or PC to D              |
 * --------------------------------------------------------------
 *
 * NOTE: D may be pushed (pulled) to either stack with PSHS, PSHU (PULS, PULU) instructions
 *
 *
 * TABLE 6 - INDEX REGISTER/STACK POINTER INSTRUCTIONS
 * --------------------------------------------------------------------------
 * | Instruction | Description                                              |
 * --------------------------------------------------------------------------
 * | CMPS, CMPU  | Compare memory from stack pointer                        |
 * --------------------------------------------------------------------------
 * | CMPX, CMPY  | Compare memory from index register                       |
 * --------------------------------------------------------------------------
 * | EXG R1, R2  | Exchange D, X, Y, S, U, or PC, with D, X, Y, S, U, or PC |
 * --------------------------------------------------------------------------
 * | LEAS, LEAU  | Load effective address into stack pointer                |
 * --------------------------------------------------------------------------
 * | LEAX, LEAY  | Load effective address into index register               |
 * --------------------------------------------------------------------------
 * | LDS, LDU    | Load stack pointer from memory                           |
 * --------------------------------------------------------------------------
 * | LDX, LDY    | Load index register from memory                          |
 * --------------------------------------------------------------------------
 * | PSHS        | Push A, B, CC, DP, D, X, Y, U, or PC onto hardware stack |
 * --------------------------------------------------------------------------
 * | PSHU        | Push A, B, CC, DP, D, X, Y, U, or PC onto user stack     |
 * --------------------------------------------------------------------------
 * | PULS        | Pull A, B, CC, DP, D, X, Y, U, or PC from hardware stack |
 * --------------------------------------------------------------------------
 * | PULU        | Pull A, B, CC, DP, D, X, Y, U, or PC from user stack     |
 * --------------------------------------------------------------------------
 * | STS, STU    | Store stack pointer to memory                            |
 * --------------------------------------------------------------------------
 * | STX, STY    | Store index register to memory                           |
 * --------------------------------------------------------------------------
 * | TFR R1, R2  | Transfer D, X, Y, S, U or PC to D, X, Y, S, U or PC      |
 * --------------------------------------------------------------------------
 * | ABX         | Add B accumulator to X (unsigned)                        |
 * --------------------------------------------------------------------------
 *
 *
 * TABLE 7 - BRANCH INSTRUCTIONS
 * --------------------------------------------------------
 * | Instruction | Description                            |
 * --------------------------------------------------------
 * |                   SIMPLE BRANCHES                    |
 * --------------------------------------------------------
 * | BEQ, LBEQ | Branch if equal                          |
 * --------------------------------------------------------
 * | BNE, LBNE | Branch if not equal                      |
 * --------------------------------------------------------
 * | BMI, LBMI | Branch if minus                          |
 * --------------------------------------------------------
 * | BPL, LBPL | Branch if plus                           |
 * --------------------------------------------------------
 * | BCS, LBCS | Branch if carry set                      |
 * --------------------------------------------------------
 * | BCC, LBCC | Branch if carry clear                    |
 * --------------------------------------------------------
 * | BVS, LBVS | Branch if overflow set                   |
 * --------------------------------------------------------
 * | BVC, LBVC | Branch if overflow clear                 |
 * --------------------------------------------------------
 * |                   SIGNED BRANCHES                    |
 * --------------------------------------------------------
 * | BGT, LBGT | Branch if greater (signed)               |
 * --------------------------------------------------------
 * | BVS, LBVS | Branch if invalid 2's complement result  |
 * --------------------------------------------------------
 * | BGE, LBGE | Branch if greater than or equal (signed) |
 * --------------------------------------------------------
 * | BEQ, LBEQ | Branch if equal                          |
 * --------------------------------------------------------
 * | BNE, LBNE | Branch if not equal                      |
 * --------------------------------------------------------
 * | BLE, LBLE | Branch if less than or equal (signed)    |
 * --------------------------------------------------------
 * | BVC, LBVC | Branch if valid 2's complement result    |
 * --------------------------------------------------------
 * | BLT, LBLT | Branch if less than (signed)             |
 * --------------------------------------------------------
 * |                  UNSIGNED BRANCHES                   |
 * --------------------------------------------------------
 * | BHI, LBHI | Branch if higher (unsigned)              |
 * --------------------------------------------------------
 * | BCC, LBCC | Branch if higher or same (unsigned)      |
 * --------------------------------------------------------
 * | BHS, LBHS | Branch if higher or same (unsigned)      |
 * --------------------------------------------------------
 * | BEQ, LBEQ | Branch if equal                          |
 * --------------------------------------------------------
 * | BNE, LBNE | Branch if not equal                      |
 * --------------------------------------------------------
 * | BLS, LBLS | Branch if lower or same (unsigned)       |
 * --------------------------------------------------------
 * | BCS, LBCS | Branch if lower (unsigned)               |
 * --------------------------------------------------------
 * | BLO, LBLO | Branch if lower (unsigned)               |
 * --------------------------------------------------------
 * |                    OTHER BRANCHES                    |
 * --------------------------------------------------------
 * | BSR, LBSR | Branch to subroutine                     |
 * --------------------------------------------------------
 * | BRA, LBRA | BRanch always                            |
 * --------------------------------------------------------
 * | BRN, LBRN | Branch never                             |
 * --------------------------------------------------------
 *
 *
 * TABLE 8 - MISCELLANEOUS INSTRUCTIONS
 * --------------------------------------------------------------------------
 * | Instruction     | Description                                          |
 * --------------------------------------------------------------------------
 * | ANDCC           | AND condition code register                          |
 * --------------------------------------------------------------------------
 * | CWAI            | AND condition code register, then wait for interrupt |
 * --------------------------------------------------------------------------
 * | NOP             | No operation                                         |
 * --------------------------------------------------------------------------
 * | ORCC            | OR condition code register                           |
 * --------------------------------------------------------------------------
 * | JMP             | Jump                                                 |
 * --------------------------------------------------------------------------
 * | JSR             | Jump to subroutine                                   |
 * --------------------------------------------------------------------------
 * | RTI             | Return from interrupt                                |
 * --------------------------------------------------------------------------
 * | RTS             | Return from subroutine                               |
 * --------------------------------------------------------------------------
 * | SWI, SWI2, SWI3 | Software interrupt (absolute indirect)               |
 * --------------------------------------------------------------------------
 * | SYNC            | Synchronize with interrupt line                      |
 * --------------------------------------------------------------------------
 *
 *
 * TABLE 9 - HEXADECIMAL VALUES OF MACHINE CODES
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------    LEGEND:
 * | 00 | NEG        |  Direct   | 6    | 2  |
 * | 01 | *          |     |     |      |    |    ~ Number of MPU cycles (less possible push pull or indexed-mode cycles)
 * | 02 | *          |     |     |      |    |    # Number of program bytes
 * | 03 | COM        |     |     | 6    | 2  |    * Denotes unused opcode
 * | 04 | LSR        |     |     | 6    | 2  |
 * | 05 | *          |     |     |      |    |    NOTE: All unused opcodes are both undefined and illegal
 * | 06 | ROR        |     |     | 6    | 2  |
 * | 07 | ASR        |     |     | 6    | 2  |
 * | 08 | ASL, LSL   |     |     | 6    | 2  |
 * | 09 | ROL        |     |     | 6    | 2  |
 * | 0A | DEC        |     |     | 6    | 2  |
 * | 0B | *          |     |     |      |    |
 * | 0C | INC        |     |     | 6    | 2  |
 * | 0D | TST        |     |     | 6    | 2  |
 * | 0E | JMP        |     V     | 3    | 2  |
 * | 0F | CLR        |  Direct   | 6    | 2  |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | 10 | Page 2                             |
 * | 11 | Page 3                             |
 * -------------------------------------------
 * | 12 | NOP        | Inherent  | 2    | 1  |
 * | 13 | SYNC       | Inherent  | >= 4 | 1  |
 * | 14 | *          |           |      |    |
 * | 15 | *          |           |      |    |
 * | 16 | LBRA       | Relative  | 5    | 3  |
 * | 17 | LBSR       | Relative  | 9    | 3  |
 * | 18 | *          |           |      |    |
 * | 19 | DAA        | Inherent  | 2    | 1  |
 * | 1A | ORCC       | Immediate | 3    | 2  |
 * | 1B | *          |           |      |    |
 * | 1C | ANDCC      | Immediate | 3    | 2  |
 * | 1D | SEX        | Inherent  | 2    | 1  |
 * | 1E | EXG        |     V     | 8    | 2  |
 * | 1F | TFR        | Inherent  | 6    | 2  |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | 20 | BRA        | Relative  | 3    | 2  |
 * | 21 | BRN        |     |     | 3    | 2  |
 * | 22 | BHI        |     |     | 3    | 2  |
 * | 23 | BLS        |     |     | 3    | 2  |
 * | 24 | BHS, BCC   |     |     | 3    | 2  |
 * | 25 | BLO, BCS   |     |     | 3    | 2  |
 * | 26 | BNE        |     |     | 3    | 2  |
 * | 27 | BEQ        |     |     | 3    | 2  |
 * | 28 | BVC        |     |     | 3    | 2  |
 * | 29 | BVS        |     |     | 3    | 2  |
 * | 2A | BPL        |     |     | 3    | 2  |
 * | 2B | BMI        |     |     | 3    | 2  |
 * | 2C | BGE        |     |     | 3    | 2  |
 * | 2D | BLT        |     |     | 3    | 2  |
 * | 2E | BGT        |     V     | 3    | 2  |
 * | 2F | BLE        | Relative  | 3    | 2  |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | 30 | LEAX       |  Indexed  | 4+   | 2+ |
 * | 31 | LEAY       |     |     | 4+   | 2+ |
 * | 32 | LEAS       |     V     | 4+   | 2+ |
 * | 33 | LEAU       |  Indexed  | 4+   | 2+ |
 * | 34 | PSHS       | Inherent  | 5+   | 2  |
 * | 35 | PULS       |     |     | 5+   | 2  |
 * | 36 | PSHU       |     |     | 5+   | 2  |
 * | 37 | PULU       |     |     | 5+   | 2  |
 * | 38 | *          |     |     |      |    |
 * | 39 | RTS        |     |     | 5    | 1  |
 * | 3A | ABX        |     |     | 3    | 1  |
 * | 3B | RTI        |     |     | 6/15 | 1  |
 * | 3C | CWAI       |     |     | >=20 | 2  |
 * | 3D | MUL        |     |     | 11   | 1  |
 * | 3E | *          |     V     |      |    |
 * | 3F | SWI        | Inherent  | 19   | 1  |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | 40 | NEGA       | Inherent  | 2    | 1  |
 * | 41 | *          |     |     |      |    |
 * | 42 | *          |     |     |      |    |
 * | 43 | COMA       |     |     | 2    | 1  |
 * | 44 | LSRA       |     |     | 2    | 1  |
 * | 45 | *          |     |     |      |    |
 * | 46 | RORA       |     |     | 2    | 1  |
 * | 47 | ASRA       |     |     | 2    | 1  |
 * | 48 | ASLA, LSLA |     |     | 2    | 1  |
 * | 49 | ROLA       |     |     | 2    | 1  |
 * | 4A | DECA       |     |     | 2    | 1  |
 * | 4B | *          |     |     |      |    |
 * | 4C | INCA       |     |     | 2    | 1  |
 * | 4D | TSTA       |     |     | 2    | 1  |
 * | 4E | *          |     V     |      |    |
 * | 4F | CLRA       | Inherent  | 2    | 1  |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | 50 | NEGB       | Inherent  | 2    | 1  |
 * | 51 | *          |     |     |      |    |
 * | 52 | *          |     |     |      |    |
 * | 53 | COMB       |     |     | 2    | 1  |
 * | 54 | LSRB       |     |     | 2    | 1  |
 * | 55 | *          |     |     |      |    |
 * | 56 | RORB       |     |     | 2    | 1  |
 * | 57 | ASRB       |     |     | 2    | 1  |
 * | 58 | ASLB, LSLB |     |     | 2    | 1  |
 * | 59 | ROLB       |     |     | 2    | 1  |
 * | 5A | DECB       |     |     | 2    | 1  |
 * | 5B | *          |     |     |      |    |
 * | 5C | INCB       |     |     | 2    | 1  |
 * | 5D | TSTB       |     |     | 2    | 1  |
 * | 5E | *          |     V     |      |    |
 * | 5F | CLRB       | Inherent  | 2    | 1  |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | 60 | NEG        |  Indexed  | 6+   | 2+ |
 * | 61 | *          |     |     |      |    |
 * | 62 | *          |     |     |      |    |
 * | 63 | COM        |     |     | 6+   | 2+ |
 * | 64 | LSR        |     |     | 6+   | 2+ |
 * | 65 | *          |     |     |      |    |
 * | 66 | ROR        |     |     | 6+   | 2+ |
 * | 67 | ASR        |     |     | 6+   | 2+ |
 * | 68 | ASL, LSL   |     |     | 6+   | 2+ |
 * | 69 | ROL        |     |     | 6+   | 2+ |
 * | 6A | DEC        |     |     | 6+   | 2+ |
 * | 6B | *          |     |     |      |    |
 * | 6C | INC        |     |     | 6+   | 2+ |
 * | 6D | TST        |     |     | 6+   | 2+ |
 * | 6E | JMP        |     V     | 3+   | 2+ |
 * | 6F | CLR        |  Indexed  | 6+   | 2+ |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | 70 | NEG        |  Extended | 7    | 3  |
 * | 71 | *          |     |     |      |    |
 * | 72 | *          |     |     |      |    |
 * | 73 | COM        |     |     | 7    | 3  |
 * | 74 | LSR        |     |     | 7    | 3  |
 * | 75 | *          |     |     |      |    |
 * | 76 | ROR        |     |     | 7    | 3  |
 * | 77 | ASR        |     |     | 7    | 3  |
 * | 78 | ASL, LSL   |     |     | 7    | 3  |
 * | 79 | ROL        |     |     | 7    | 3  |
 * | 7A | DEC        |     |     | 7    | 3  |
 * | 7B | *          |     |     |      |    |
 * | 7C | INC        |     |     | 7    | 3  |
 * | 7D | TST        |     |     | 7    | 3  |
 * | 7E | JMP        |     V     | 4    | 3  |
 * | 7F | CLR        |  Extended | 7    | 3  |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | 80 | SUBA       | Immediate | 2    | 2  |
 * | 81 | CMPA       |     |     | 2    | 2  |
 * | 82 | SBCA       |     |     | 2    | 2  |
 * | 83 | SUBD       |     |     | 4    | 3  |
 * | 84 | ANDA       |     |     | 2    | 2  |
 * | 85 | BITA       |     |     | 2    | 2  |
 * | 86 | LDA        |     |     | 2    | 2  |
 * | 87 | *          |     |     |      |    |
 * | 88 | EORA       |     |     | 2    | 2  |
 * | 89 | ADCA       |     |     | 2    | 2  |
 * | 8A | ORA        |     |     | 2    | 2  |
 * | 8B | ADDA       |     V     | 2    | 2  |
 * | 8C | CMPX       | Immediate | 4    | 3  |
 * | 8D | BSR        | Relative  | 7    | 2  |
 * | 8E | LDX        | Immediate | 3    | 3  |
 * | 8F | *          |           |      |    |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | 90 | SUBA       |  Direct   | 4    | 2  |
 * | 91 | CMPA       |     |     | 4    | 2  |
 * | 92 | SBCA       |     |     | 4    | 2  |
 * | 93 | SUBD       |     |     | 6    | 2  |
 * | 94 | ANDA       |     |     | 4    | 2  |
 * | 95 | BITA       |     |     | 4    | 2  |
 * | 96 | LDA        |     |     | 4    | 2  |
 * | 97 | STA        |     |     | 4    | 2  |
 * | 98 | EORA       |     |     | 4    | 2  |
 * | 99 | ADCA       |     |     | 4    | 2  |
 * | 9A | ORA        |     |     | 4    | 2  |
 * | 9B | ADDA       |     |     | 4    | 2  |
 * | 9C | CMPX       |     |     | 6    | 2  |
 * | 9D | JSR        |     |     | 7    | 2  |
 * | 9E | LDX        |     V     | 5    | 2  |
 * | 9F | STX        |  Direct   | 5    | 2  |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | A0 | SUBA       |  Indexed  | 4+   | 2+ |
 * | A1 | CMPA       |     |     | 4+   | 2+ |
 * | A2 | SBCA       |     |     | 4+   | 2+ |
 * | A3 | SUBD       |     |     | 6+   | 2+ |
 * | A4 | ANDA       |     |     | 4+   | 2+ |
 * | A5 | BITA       |     |     | 4+   | 2+ |
 * | A6 | LDA        |     |     | 4+   | 2+ |
 * | A7 | STA        |     |     | 4+   | 2+ |
 * | A8 | EORA       |     |     | 4+   | 2+ |
 * | A9 | ADCA       |     |     | 4+   | 2+ |
 * | AA | ORA        |     |     | 4+   | 2+ |
 * | AB | ADDA       |     |     | 4+   | 2+ |
 * | AC | CMPX       |     |     | 6+   | 2+ |
 * | AD | JSR        |     |     | 7+   | 2+ |
 * | AE | LDX        |     V     | 5+   | 2+ |
 * | AF | STX        |  Indexed  | 5+   | 2+ |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | B0 | SUBA       |  Extended | 5    | 3  |
 * | B1 | CMPA       |     |     | 5    | 3  |
 * | B2 | SBCA       |     |     | 5    | 3  |
 * | B3 | SUBD       |     |     | 7    | 3  |
 * | B4 | ANDA       |     |     | 5    | 3  |
 * | B5 | BITA       |     |     | 5    | 3  |
 * | B6 | LDA        |     |     | 5    | 3  |
 * | B7 | STA        |     |     | 5    | 3  |
 * | B8 | EORA       |     |     | 5    | 3  |
 * | B9 | ADCA       |     |     | 5    | 3  |
 * | BA | ORA        |     |     | 5    | 3  |
 * | BB | ADDA       |     |     | 5    | 3  |
 * | BC | CMPX       |     |     | 7    | 3  |
 * | BD | JSR        |     |     | 8    | 3  |
 * | BE | LDX        |     V     | 6    | 3  |
 * | BF | STX        |  Extended | 6    | 3  |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | C0 | SUBB       | Immediate | 2    | 2  |
 * | C1 | CMPB       |     |     | 2    | 2  |
 * | C2 | SBCB       |     |     | 2    | 2  |
 * | C3 | ADDD       |     |     | 4    | 3  |
 * | C4 | ANDB       |     |     | 2    | 2  |
 * | C5 | BITB       |     |     | 2    | 2  |
 * | C6 | LDB        |     |     | 2    | 2  |
 * | C7 | *          |     |     |      |    |
 * | C8 | EORB       |     |     | 2    | 2  |
 * | C9 | ADCB       |     |     | 2    | 2  |
 * | CA | ORB        |     |     | 2    | 2  |
 * | CB | ADDB       |     |     | 2    | 2  |
 * | CC | LDD        |     |     | 3    | 3  |
 * | CD | *          |     V     |      |    |
 * | CE | LDU        | Immediate | 3    | 3  |
 * | CF | *          |           |      |    |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | D0 | SUBB       |  Direct   | 4    | 2  |
 * | D1 | CMPB       |     |     | 4    | 2  |
 * | D2 | SBCB       |     |     | 4    | 2  |
 * | D3 | ADDD       |     |     | 6    | 2  |
 * | D4 | ANDB       |     |     | 4    | 2  |
 * | D5 | BITB       |     |     | 4    | 2  |
 * | D6 | LDB        |     |     | 4    | 2  |
 * | D7 | STB        |     |     | 4    | 2  |
 * | D8 | EORB       |     |     | 4    | 2  |
 * | D9 | ADCB       |     |     | 4    | 2  |
 * | DA | ORB        |     |     | 4    | 2  |
 * | DB | ADDB       |     |     | 4    | 2  |
 * | DC | LDD        |     |     | 5    | 2  |
 * | DD | STD        |     |     | 5    | 2  |
 * | DE | LDU        |     V     | 5    | 2  |
 * | DF | STU        |  Direct   | 5    | 2  |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | E0 | SUBB       |  Indexed  | 4+   | 2+ |
 * | E1 | CMPB       |     |     | 4+   | 2+ |
 * | E2 | SBCB       |     |     | 4+   | 2+ |
 * | E3 | ADDD       |     |     | 6+   | 2+ |
 * | E4 | ANDB       |     |     | 4+   | 2+ |
 * | E5 | BITB       |     |     | 4+   | 2+ |
 * | E6 | LDB        |     |     | 4+   | 2+ |
 * | E7 | STB        |     |     | 4+   | 2+ |
 * | E8 | EORB       |     |     | 4+   | 2+ |
 * | E9 | ADCB       |     |     | 4+   | 2+ |
 * | EA | ORB        |     |     | 4+   | 2+ |
 * | EB | ADDB       |     |     | 4+   | 2+ |
 * | EC | LDD        |     |     | 5+   | 2+ |
 * | ED | STD        |     |     | 5+   | 2+ |
 * | EE | LDU        |     V     | 5+   | 2+ |
 * | EF | STU        |  Indexed  | 5+   | 2+ |
 * -------------------------------------------
 * | OP | Mnem       |   Mode    | ~    | #  |
 * -------------------------------------------
 * | F0 | SUBB       |  Extended | 5    | 3  |
 * | F1 | CMPB       |     |     | 5    | 3  |
 * | F2 | SBCB       |     |     | 5    | 3  |
 * | F3 | ADDD       |     |     | 7    | 3  |
 * | F4 | ANDB       |     |     | 5    | 3  |
 * | F5 | BITB       |     |     | 5    | 3  |
 * | F6 | LDB        |     |     | 5    | 3  |
 * | F7 | STB        |     |     | 5    | 3  |
 * | F8 | EORB       |     |     | 5    | 3  |
 * | F9 | ADCB       |     |     | 5    | 3  |
 * | FA | ORB        |     |     | 5    | 3  |
 * | FB | ADDB       |     |     | 5    | 3  |
 * | FC | LDD        |     |     | 6    | 3  |
 * | FD | STD        |     |     | 6    | 3  |
 * | FE | LDU        |     V     | 6    | 3  |
 * | FF | STU        |  Extended | 6    | 3  |
 * -------------------------------------------
 *
 * Page 2 and 3 Machine Codes
 * ---------------------------------------------
 * |  OP  | Mnem       |   Mode    | ~    | #  |
 * ---------------------------------------------
 * | 1021 | LBRN       | Relative  | 5    | 4  |
 * | 1022 | LBHI       |     |     | 5(6) | 4  |
 * | 1023 | LBLS       |     |     | 5(6) | 4  |
 * | 1024 | LBHS, LBCC |     |     | 5(6) | 4  |
 * | 1025 | LBCS, LBLO |     |     | 5(6) | 4  |
 * | 1026 | LBNE       |     |     | 5(6) | 4  |
 * | 1027 | LBEQ       |     |     | 5(6) | 4  |
 * | 1028 | LBVC       |     |     | 5(6) | 4  |
 * | 1029 | LBVS       |     |     | 5(6) | 4  |
 * | 102A | LBPL       |     |     | 5(6) | 4  |
 * | 102B | LBMI       |     |     | 5(6) | 4  |
 * | 102C | LBGE       |     |     | 5(6) | 4  |
 * | 102D | LBLT       |     |     | 5(6) | 4  |
 * | 102E | LBGT       |     V     | 5(6) | 4  |
 * | 102F | LBLE       | Relative  | 5(6) | 4  |
 * | 103F | SWI2       | Inherent  | 20   | 2  |
 * ---------------------------------------------
 * |  OP  | Mnem       |   Mode    | ~    | #  |
 * ---------------------------------------------
 * | 1083 | CMPD       | Immediate | 5    | 4  |
 * | 108C | CMPY       |     V     | 5    | 4  |
 * | 108E | LDY        | Immediate | 4    | 4  |
 * | 1093 | CMPD       |  Direct   | 7    | 3  |
 * | 109C | CMPY       |     |     | 7    | 3  |
 * | 109E | LDY        |     V     | 6    | 3  |
 * | 109F | STY        |  Direct   | 6    | 3  |
 * | 10A3 | CMPD       |  Indexed  | 7+   | 3+ |
 * | 10AC | CMPY       |     |     | 7+   | 3+ |
 * | 10AE | LDY        |     V     | 6+   | 3+ |
 * | 10AF | STY        |  Indexed  | 6+   | 3+ |
 * | 10B3 | CMPD       |  Extended | 8    | 4  |
 * | 10BC | CMPY       |     |     | 8    | 4  |
 * | 10BE | LDY        |     V     | 7    | 4  |
 * | 10BF | STY        |  Extended | 7    | 4  |
 * | 10CE | LDS        | Immediate | 4    | 4  |
 * ---------------------------------------------
 * |  OP  | Mnem       |   Mode    | ~    | #  |
 * ---------------------------------------------
 * | 10DE | LDS        |  Direct   | 6    | 3  |
 * | 10DF | STS        |  Direct   | 6    | 3  |
 * | 10EE | LDS        |  Indexed  | 6+   | 3+ |
 * | 10EF | STS        |  Indexed  | 6+   | 3+ |
 * | 10FE | LDS        |  Extended | 7    | 4  |
 * | 10FF | STS        |  Extended | 7    | 4  |
 * | 113F | SWI3       | Inherent  | 20   | 2  |
 * | 1183 | CMPU       | Immediate | 5    | 4  |
 * | 118C | CMPS       | Immediate | 7    | 3  |
 * | 1193 | CMPU       |  Direct   | 7    | 3  |
 * | 119C | CMPS       |  Direct   | 7    | 3  |
 * | 11A3 | CMPU       |  Indexed  | 7+   | 3+ |
 * | 11AC | CMPS       |  Indexed  | 7+   | 3+ |
 * | 11B3 | CMPU       |  Extended | 8    | 4  |
 * | 11BC | CMPS       |  Extended | 8    | 4  |
 * ---------------------------------------------
 *
 *
 * FIGURE 20 - PROGRAMMING AID
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * |             |        |                              Addressing Modes                                  |                                   |   |   |   |   |   |
 * |             |        ----------------------------------------------------------------------------------                                   |   |   |   |   |   |
 * |             |        |    Immediate    |    Direct    |    Indexed     |   Extended   |   Inherent    |                                   | 5 | 3 | 2 | 1 | 0 |
 * |             |        ----------------------------------------------------------------------------------                                   ---------------------
 * | Instruction | Forms  |  Op  | ~    | # |  Op  | ~ | # |  Op  | ~  | #  |  Op  | ~ | # |  Op  | ~  | # |      Description                  | H | N | Z | V | C |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | ABX         |        |      |      |   |      |   |   |      |    |    |      |   |   |  3A  | 3  | 1 | B + X -> X (Unsigned)             | * | * | * | * | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | ADC         | ADCA   |  89  |  2   | 2 |  99  | 4 | 2 |  A9  | 4+ | 2+ |  B9  | 5 | 3 |      |    |   | A + M + C -> A                    | | | | | | | | | | |
 * |             | ADCB   |  C9  |  2   | 2 |  D9  | 4 | 2 |  E9  | 4+ | 2+ |  F9  | 5 | 3 |      |    |   | B + M + C -> B                    | | | | | | | | | | |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | ADD         | ADDA   |  8B  |  2   | 2 |  9B  | 4 | 2 |  AB  | 4+ | 2+ |  BB  | 5 | 3 |      |    |   | A + M -> A                        | | | | | | | | | | |
 * |             | ADDB   |  CB  |  2   | 2 |  DB  | 4 | 2 |  EB  | 4+ | 2+ |  FB  | 5 | 3 |      |    |   | B + M -> B                        | | | | | | | | | | |
 * |             | ADDD   |  C3  |  4   | 3 |  D3  | 6 | 2 |  E3  | 6+ | 2+ |  F3  | 7 | 3 |      |    |   | D + M : M + 1 -> D                | * | | | | | | | | |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | AND         | ANDA   |  84  |  2   | 2 |  94  | 4 | 2 |  A4  | 4+ | 2+ |  B4  | 5 | 3 |      |    |   | A ∧ M -> A                        | * | | | | | 0 | * |
 * |             | ANDB   |  C4  |  2   | 2 |  D4  | 4 | 2 |  E4  | 4+ | 2+ |  F4  | 5 | 3 |      |    |   | B ∧ M -> B                        | * | | | | | 0 | * |
 * |             | ANDCC  |  1C  |  3   | 2 |      |   |   |      |    |    |      |   |   |      |    |   | CC ∧ |MM -> CC                    |   |   |   |   | 7 |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | ASL         | ASLA   |      |      |   |      |   |   |      |    |    |      |   |   |  48  | 2  | 1 | See Diagram 1 Below               | 8 | | | | | | | | |
 * |             | ASLB   |      |      |   |      |   |   |      |    |    |      |   |   |  58  | 2  | 1 |                                   | 8 | | | | | | | | |
 * |             | ASL    |      |      |   |  08  | 6 | 2 |  68  | 6+ | 2+ |  78  | 7 | 3 |      |    |   |                                   | 8 | | | | | | | | |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | ASR         | ASRA   |      |      |   |      |   |   |      |    |    |      |   |   |  47  | 2  | 1 | See Diagram 2 Below               | 8 | | | | | * | | |
 * |             | ASRB   |      |      |   |      |   |   |      |    |    |      |   |   |  57  | 2  | 1 |                                   | 8 | | | | | * | | |
 * |             | ASR    |      |      |   |  07  | 6 | 2 |  67  | 6+ | 2+ |  77  | 7 | 3 |      |    |   |                                   | 8 | | | | | * | | |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | BIT         | BITA   |  85  |  2   | 2 |  95  | 4 | 2 |  A5  | 4+ | 2+ |  B5  | 5 | 3 |      |    |   | Bit Test A (M ∧ A)                | * | | | | | 0 | * |
 * |             | BITB   |  C5  |  2   | 2 |  D5  | 4 | 2 |  E5  | 4+ | 2+ |  F5  | 5 | 3 |      |    |   | Bit Test B (M ∧ B)                | * | | | | | 0 | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | CLR         | CLRA   |      |      |   |      |   |   |      |    |    |      |   |   |  4F  | 2  | 1 | 0 -> A                            | * | 0 | 1 | 0 | 0 |
 * |             | CLRB   |      |      |   |      |   |   |      |    |    |      |   |   |  5F  | 2  | 1 | 0 -> B                            | * | 0 | 1 | 0 | 0 |
 * |             | CLR    |      |      |   |  0F  | 6 | 2 |  6F  | 6+ | 2+ |  7F  | 7 | 3 |      |    |   | 0 -> M                            | * | 0 | 1 | 0 | 0 |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | CMP         | CMPA   |  81  |  2   | 2 |  91  | 4 | 2 |  A1  | 4+ | 2+ |  B1  | 5 | 3 |      |    |   | Compare M from A                  | 8 | | | | | | | | |
 * |             | CMPB   |  C1  |  2   | 2 |  D1  | 4 | 2 |  E1  | 4+ | 2+ |  F1  | 5 | 3 |      |    |   | Compare M from B                  | 8 | | | | | | | | |
 * |             | CMPD   | 1083 |  5   | 4 | 1093 | 7 | 3 | 10A3 | 7+ | 3+ | 10B3 | 8 | 4 |      |    |   | Compare M : M + 1 from D          | * | | | | | | | | |
 * |             | CMPS   | 118C |  5   | 4 | 119C | 7 | 3 | 11AC | 7+ | 3+ | 11BC | 8 | 4 |      |    |   | Compare M : M + 1 from S          | * | | | | | | | | |
 * |             | CMPU   | 1183 |  5   | 4 | 1193 | 7 | 3 | 11A3 | 7+ | 3+ | 11B3 | 8 | 4 |      |    |   | Compare M : M + 1 from U          | * | | | | | | | | |
 * |             | CMPX   |  8C  |  4   | 3 |  9C  | 6 | 2 |  AC  | 6+ | 2+ |  BC  | 7 | 3 |      |    |   | Compare M : M + 1 from X          | * | | | | | | | | |
 * |             | CMPY   | 108C |  5   | 4 | 109C | 7 | 3 | 10AC | 7+ | 3+ | 10BC | 8 | 4 |      |    |   | Compare M : M + 1 from Y          | * | | | | | | | | |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | COM         | COMA   |      |      |   |      |   |   |      |    |    |      |   |   |  43  | 2  | 1 | (A) -> A                          | * | | | | | 0 | 1 |
 * |             | COMB   |      |      |   |      |   |   |      |    |    |      |   |   |  53  | 2  | 1 | (B) -> B                          | * | | | | | 0 | 1 |
 * |             | COM    |      |      |   |  03  | 6 | 2 |  63  | 6+ | 2+ |  73  | 7 | 3 |      |    |   | (M) -> M                          | * | | | | | 0 | 1 |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | CWAI        |        |  3C  | >=20 | 2 |      |   |   |      |    |    |      |   |   |      |    |   | CC ∧ |MM -> CC Wait for interrupt |   |   |   |   | 7 |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | DAA         |        |      |      |   |      |   |   |      |    |    |      |   |   |  19  | 2  | 1 | Decimal Adjust A                  | * | | | | | 0 | | |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | DEC         | DECA   |      |      |   |      |   |   |      |    |    |      |   |   |  4A  | 2  | 1 | A - 1 -> A                        | * | | | | | | | * |
 * |             | DECB   |      |      |   |      |   |   |      |    |    |      |   |   |  5A  | 2  | 1 | B - 1 -> B                        | * | | | | | | | * |
 * |             | DEC    |      |      |   |  0A  | 6 | 2 |  6A  | 6+ | 2+ |  7A  | 7 | 3 |      |    |   | M - 1 -> M                        | * | | | | | | | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | EOR         | EORA   |  88  |  2   | 2 |  98  | 4 | 2 |  A8  | 4+ | 2+ |  B8  | 5 | 3 |      |    |   | A ⊻ M -> A                        | * | | | | | 0 | * |
 * |             | EORB   |  C8  |  2   | 2 |  D8  | 4 | 2 |  E8  | 4+ | 2+ |  F8  | 5 | 3 |      |    |   | B ⊻ M -> B                        | * | | | | | 0 | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | EXG         | R1, R2 |      |      |   |      |   |   |      |    |    |      |   |   |  1E  | 8  | 2 | R1 <-> R2 (2)                     | * | * | * | * | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | INC         | INCA   |      |      |   |      |   |   |      |    |    |      |   |   |  4C  | 2  | 1 | A + 1 -> A                        | * | | | | | | | * |
 * |             | INCB   |      |      |   |      |   |   |      |    |    |      |   |   |  5C  | 2  | 1 | B + 1 -> B                        | * | | | | | | | * |
 * |             | INC    |      |      |   |  0C  | 6 | 2 |  6C  | 6+ | 2+ |  7C  | 7 | 3 |      |    |   | M + 1 -> M                        | * | | | | | | | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | JMP         |        |      |      |   |  0E  | 3 | 2 |  6E  | 3+ | 2+ |  7E  | 4 | 3 |      |    |   | EA (3) -> PC                      | * | * | * | * | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | JSR         |        |      |      |   |  9D  | 7 | 2 |  AD  | 7+ | 2+ |  BD  | 8 | 3 |      |    |   | Jump to Subroutine                | * | * | * | * | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | LD          | LDA    |  86  |  2   | 2 |  96  | 4 | 2 |  A6  | 4+ | 2+ |  B6  | 5 | 3 |      |    |   | M -> A                            | * | | | | | 0 | * |
 * |             | LDB    |  C6  |  2   | 2 |  D6  | 4 | 2 |  E6  | 4+ | 2+ |  F6  | 5 | 3 |      |    |   | M -> B                            | * | | | | | 0 | * |
 * |             | LDD    |  CC  |  3   | 3 |  DC  | 5 | 2 |  EC  | 5+ | 2+ |  FC  | 6 | 3 |      |    |   | M : M + 1 -> D                    | * | | | | | 0 | * |
 * |             | LDS    | 10CE |  4   | 4 | 10DE | 6 | 3 | 10EE | 6+ | 3+ | 10FE | 7 | 4 |      |    |   | M : M + 1 -> S                    | * | | | | | 0 | * |
 * |             | LDU    |  CE  |  3   | 3 |  DE  | 5 | 2 |  EE  | 5+ | 2+ |  FE  | 6 | 3 |      |    |   | M : M + 1 -> U                    | * | | | | | 0 | * |
 * |             | LDX    |  8E  |  3   | 3 |  9E  | 5 | 2 |  AE  | 5+ | 2+ |  BE  | 6 | 3 |      |    |   | M : M + 1 -> X                    | * | | | | | 0 | * |
 * |             | LDY    | 108E |  4   | 4 | 109E | 6 | 3 | 10AE | 6+ | 3+ | 10BE | 7 | 4 |      |    |   | M : M + 1 => Y                    | * | | | | | 0 | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | LEA         | LEAS   |      |      |   |      |   |   |  32  | 4+ | 2+ |      |   |   |      |    |   | EA (3) -> S                       | * | * | * | * | * |
 * |             | LEAU   |      |      |   |      |   |   |  33  | 4+ | 2+ |      |   |   |      |    |   | EA (3) -> U                       | * | * | * | * | * |
 * |             | LEAX   |      |      |   |      |   |   |  30  | 4+ | 2+ |      |   |   |      |    |   | EA (3) -> X                       | * | * | | | * | * |
 * |             | LEAY   |      |      |   |      |   |   |  31  | 4+ | 2+ |      |   |   |      |    |   | EA (3) -> Y                       | * | * | | | * | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | LSL         | LSLA   |      |      |   |      |   |   |      |    |    |      |   |   |  48  | 2  | 1 | See Diagram 3 Below               | * | | | | | | | | |
 * |             | LSLB   |      |      |   |      |   |   |      |    |    |      |   |   |  58  | 2  | 1 |                                   | * | | | | | | | | |
 * |             | LSL    |      |      |   |  08  | 6 | 2 |  68  | 6+ | 2+ |  78  | 7 | 3 |      |    |   |                                   | * | | | | | | | | |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | LSR         | LSRA   |      |      |   |      |   |   |      |    |    |      |   |   |  44  | 2  | 1 | See Diagram 4 Below               | * | 0 | | | * | | |
 * |             | LSRB   |      |      |   |      |   |   |      |    |    |      |   |   |  54  | 2  | 1 |                                   | * | 0 | | | * | | |
 * |             | LSR    |      |      |   |  04  | 6 | 2 |  64  | 6+ | 2+ |  74  | 7 | 3 |      |    |   |                                   | * | 0 | | | * | | |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | MUL         |        |      |      |   |      |   |   |      |    |    |      |   |   |  3D  | 11 | 1 | A x B -> D (Unsigned)             | * | * | | | * | 9 |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | NEG         | NEGA   |      |      |   |      |   |   |      |    |    |      |   |   |  40  | 2  | 1 | (A) + 1 -> A                      | 8 | | | | | | | | |
 * |             | NEGB   |      |      |   |      |   |   |      |    |    |      |   |   |  50  | 2  | 1 | (B) + 1 -> B                      | 8 | | | | | | | | |
 * |             | NEG    |      |      |   |  00  | 6 | 2 |  60  | 6+ | 2+ |  70  | 7 | 3 |      |    |   | (M) + 1 -> M                      | 8 | | | | | | | | |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | NOP         |        |      |      |   |      |   |   |      |    |    |      |   |   |  12  | 2  | 1 | No Operation                      | * | * | * | * | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | OR          | ORA    |  8A  |  2   | 2 |  9A  | 4 | 2 |  AA  | 4+ | 2+ |  BA  | 5 | 3 |      |    |   | A ∨ M -> A                        | * | | | | | 0 | * |
 * |             | ORB    |  CA  |  2   | 2 |  DA  | 4 | 2 |  EA  | 4+ | 2+ |  FA  | 5 | 3 |      |    |   | B ∨ M -> B                        | * | | | | | 0 | * |
 * |             | ORCC   |  1A  |  3   | 2 |      |   |   |      |    |    |      |   |   |      |    |   | CC ∨ |MM -> CC                    |   |   |   | 7 |   |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | PSH         | PSHS   |  34  | 5+(4)| 2 |      |   |   |      |    |    |      |   |   |      |    |   | Push Registers on S Stack         | * | * | * | * | * |
 * |             | PSHU   |  36  | 5+(4)| 2 |      |   |   |      |    |    |      |   |   |      |    |   | Push Registers on U Stack         | * | * | * | * | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | PUL         | PULS   |  35  | 5+(4)| 2 |      |   |   |      |    |    |      |   |   |      |    |   | Pull Registers from S Stack       | * | * | * | * | * |
 * |             | PULU   |  37  | 5+(4)| 2 |      |   |   |      |    |    |      |   |   |      |    |   | Pull Registers from U Stack       | * | * | * | * | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | ROL         | ROLA   |      |      |   |      |   |   |      |    |    |      |   |   |  49  | 2  | 1 | See Diagram 5 Below               | * | | | | | | | | |
 * |             | ROLB   |      |      |   |      |   |   |      |    |    |      |   |   |  59  | 2  | 1 |                                   | * | | | | | | | | |
 * |             | ROL    |      |      |   |  09  | 6 | 2 |  69  | 6+ | 2+ |  79  | 7 | 3 |      |    |   |                                   | * | | | | | | | | |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | ROR         | RORA   |      |      |   |      |   |   |      |    |    |      |   |   |  46  | 2  | 1 | See Diagram 6 Below               | * | | | | | * | | |
 * |             | RORB   |      |      |   |      |   |   |      |    |    |      |   |   |  56  | 2  | 1 |                                   | * | | | | | * | | |
 * |             | ROR    |      |      |   |  06  | 6 | 2 |  66  | 6+ | 2+ |  76  | 7 | 3 |      |    |   |                                   | * | | | | | * | | |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | RTI         |        |      |      |   |      |   |   |      |    |    |      |   |   |  3B  |6/15| 1 | Return From Interrupt             |   |   |   |   | 7 |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | RTS         |        |      |      |   |      |   |   |      |    |    |      |   |   |  39  | 5  | 1 | Return From Subroutine            | * | * | * | * | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | SBC         | SBCA   |  82  |  2   | 2 |  92  | 4 | 2 |  A2  | 4+ | 2+ |  B2  | 5 | 3 |      |    |   | A - M - C -> A                    | 8 | | | | | | | | |
 * |             | SBCB   |  C2  |  2   | 2 |  D2  | 4 | 2 |  E2  | 4+ | 2+ |  F2  | 5 | 3 |      |    |   | B - M - C -> B                    | 8 | | | | | | | | |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | SEX         |        |      |      |   |      |   |   |      |    |    |      |   |   |  1D  | 2  | 1 | Sign Extend B into A              | * | | | | | 0 | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | ST          | STA    |      |      |   |  97  | 4 | 2 |  A7  | 4+ | 2+ |  B7  | 5 | 3 |      |    |   | A -> M                            | * | | | | | 0 | * |
 * |             | STB    |      |      |   |  D7  | 4 | 2 |  E7  | 4+ | 2+ |  F7  | 5 | 3 |      |    |   | B -> M                            | * | | | | | 0 | * |
 * |             | STD    |      |      |   |  DD  | 5 | 2 |  ED  | 5+ | 2+ |  FD  | 6 | 3 |      |    |   | D -> M : M + 1                    | * | | | | | 0 | * |
 * |             | STS    |      |      |   | 10DF | 6 | 3 | 10EF | 6+ | 3+ | 10FF | 7 | 4 |      |    |   | S -> M : M + 1                    | * | | | | | 0 | * |
 * |             | STU    |      |      |   |  DF  | 5 | 2 |  EF  | 5+ | 2+ |  FF  | 6 | 3 |      |    |   | U -> M : M + 1                    | * | | | | | 0 | * |
 * |             | STX    |      |      |   |  9F  | 5 | 2 |  AF  | 5+ | 2+ |  BF  | 6 | 3 |      |    |   | X -> M : M + 1                    | * | | | | | 0 | * |
 * |             | STY    |      |      |   | 109F | 6 | 3 | 10AF | 6+ | 3+ | 10BF | 7 | 4 |      |    |   | Y -> M : M + 1                    | * | | | | | 0 | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | SUB         | SUBA   |  80  |  2   | 2 |  90  | 4 | 2 |  A0  | 4+ | 2+ |  B0  | 5 | 3 |      |    |   | A - M -> A                        | 8 | | | | | | | | |
 * |             | SUBB   |  C0  |  2   | 2 |  D0  | 4 | 2 |  E0  | 4+ | 2+ |  F0  | 5 | 3 |      |    |   | B - M -> B                        | 8 | | | | | | | | |
 * |             | SUBD   |  83  |  4   | 3 |  93  | 6 | 2 |  A3  | 6+ | 2+ |  B3  | 7 | 3 |      |    |   | D - M : M + 1 -> D                | * | | | | | | | | |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | SWI         | SWI(6) |      |      |   |      |   |   |      |    |    |      |   |   |  3F  | 19 | 1 | Software Interrupt 1              | * | * | * | * | * |
 * |             | SWI(6) |      |      |   |      |   |   |      |    |    |      |   |   | 103F | 20 | 2 | Software Interrupt 2              | * | * | * | * | * |
 * |             | SWI(6) |      |      |   |      |   |   |      |    |    |      |   |   | 113F | 20 | 1 | Software Interrupt 3              | * | * | * | * | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | SYNC        |        |      |      |   |      |   |   |      |    |    |      |   |   |  13  | >=4| 1 | Synchronize to Interrupt          | * | * | * | * | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | TFR         | R1, R2 |      |      |   |      |   |   |      |    |    |      |   |   |  1F  | 6  | 2 | R1 -> R2 (2)                      | * | * | * | * | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 * | TST         | TSTA   |      |      |   |      |   |   |      |    |    |      |   |   |  4D  | 2  | 1 | Test A                            | * | | | | | 0 | * |
 * |             | TSTB   |      |      |   |      |   |   |      |    |    |      |   |   |  5D  | 2  | 1 | Test B                            | * | | | | | 0 | * |
 * |             | TST    |      |      |   |  0D  | 6 | 2 |  6D  | 6+ | 2+ |  7D  | 7 | 3 |      |    |   | Test M                            | * | | | | | 0 | * |
 * -----------------------------------------------------------------------------------------------------------------------------------------------------------------
 *
 *
 * Diagram 1                              Diagram 3                              Diagram 5
 *                   <------                                <------                    ________________________________
 * A \   ___    _________________         A \   ___    _________________         A \   |   ___    _________________   |
 * B |-  | | <- | | | | | | | | | <- 0    B |-  | | <- | | | | | | | | | <- 0    B |-  --- | | <- | | | | | | | | | <--
 * M /   ---    -----------------         M /   ---    -----------------         M /       ---    -----------------
 *        C     b7             b0                C     b7             b0                    C     b7             b0
 *
 *
 * Diagram 2                              Diagram 4                              Diagram 6
 *       _____       ------>                                ------>                    ________________________________
 * A \   |   _________________    ___     A \        _________________    ___    A \   |   ________________________   |
 * B |-  --> | | | | | | | | | -> | |     B |-  0 -> | | | | | | | | | -> | |    B |-  --> | | -> | | | | | | | | | ---
 * M /       -----------------    ---     M /        -----------------    ---    M /       ---    -----------------
 *           b7             b0     C                 b7             b0     C                C     b7             b0
 *
 * Legend:                          (M) Complement of M             | Test and set if true, cleared otherwise
 *
 * OP Operation Code (Hexadecimal)   -> Transfer Into               * Not Affected
 *
 *  ~ Number of MPU Cycles            H Half-carry (from bit 3)    CC Condition Code Register
 *
 *  # Number of Program Bytes         N Negative (sign bit)         : Concatenation
 *
 *  + Arithmetic Plus                 Z Zero Result                 ∨ Logical Or
 *
 *  - Arithmetic Minus                V Overflow, 2's Complement    ∧ Logical And
 *
 *  * Multiply                        C Carry from ALU              ⊻ Logical Excluside Or
 *
 * NOTES: 1. This column gives a base cycle and byte count. To obtain total count, add the values
 *           obtained from teh INDEXED ADDRESSING MODE table, Table 2.
 *
 *        2. R1 and R2 may be any pair of 8 bit or any pair of 16 bit registers.
 *               The 8 bit registers are: A, B, CC, DP
 *               The 16 bit registers are: X, Y, U, S, D, PC
 *
 *        3. EA is the effective address.
 *
 *        4. The PSH and PUL instrutions requires 5 cycles plus 1 cycle for each byte pushed or
 *           pulled.
 *
 *        5. 5(6) means: 5 cycles if branch not taken, 6 cycles if taken (Branch instructions).
 *
 *        6. SWI sets I and F bits. SWI2 and SWI3 do not affect I and F.
 *
 *        7. Conditions Codes set as a direct result of the instruction.
 *
 *        8. Value of half-carry flag is undefined.
 *
 *        9. Special Case - Carry set if b7 is SET.
 *
 *
 * Branch Instructions
 * -------------------------------------------------------------------------------------------
 * |             |       | Addressing Modes |                            |   |   |   |   |   |
 * |             |       ---------------------------------------------------------------------
 * |             |       |     Relative     |                            | 5 | 3 | 2 | 1 | 0 |
 * |             |       ---------------------------------------------------------------------
 * | Instruction | Forms |  Op  |  ~   | #  | Description                | H | N | Z | V | C |
 * -------------------------------------------------------------------------------------------
 * | BCC         | BCC   |  24  |  3   | 2  | Branch C = 0               | * | * | * | * | * |
 * |             | LBCC  | 1024 | 5(6) | 4  | Long Branch C = 0          | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BCS         | BCS   |  25  |  3   | 2  | Branch C = 1               | * | * | * | * | * |
 * |             | LBCS  | 1025 | 5(6) | 4  | Long Branch C = 1          | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BEQ         | BEQ   |  27  |  3   | 2  | Branch Z = 1               | * | * | * | * | * |
 * |             | LBEQ  | 1027 | 5(6) | 4  | Long Branch Z = 0          | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BGE         | BGE   |  2C  |  3   | 2  | Branch >= Zero             | * | * | * | * | * |
 * |             | LBGE  | 102C | 5(6) | 4  | Long Branch >= Zero        | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BGT         | BGT   |  2E  |  3   | 2  | Branch > Zero              | * | * | * | * | * |
 * |             | LBGT  | 102E | 5(6) | 4  | Long Branch > Zero         | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BHI         | BHI   |  22  |  3   | 2  | Branch Higher              | * | * | * | * | * |
 * |             | LBHI  | 1022 | 5(6) | 4  | Long Branch Higher         | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BHS         | BHS   |  24  |  3   | 2  | Branch Higher or Same      | * | * | * | * | * |
 * |             | LBHS  | 1024 | 5(6) | 4  | Long Branch Higher or Same | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BLE         | BLE   |  2F  |  3   | 2  | Branch <= Zero             | * | * | * | * | * |
 * |             | LBLE  | 102F | 5(6) | 4  | Long Branch <= Zero        | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BLO         | BLO   |  25  |  3   | 2  | Branch Lower               | * | * | * | * | * |
 * |             | LBLO  | 1025 | 5(6) | 4  | Long Branch Lower          | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BLS         | BLS   |  23  |  3   | 2  | Branch Lower or Same       | * | * | * | * | * |
 * |             | LBLS  | 1023 | 5(6) | 4  | Long Branch Lower or Same  | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BLT         | BLT   |  2D  |  3   | 2  | Branch < Zero              | * | * | * | * | * |
 * |             | LBLT  | 102D | 5(6) | 4  | Long Branch < Zero         | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BMI         | BMI   |  2B  |  3   | 2  | Branch Minus               | * | * | * | * | * |
 * |             | LBMI  | 102B | 5(6) | 4  | Long Branch Minus          | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BNE         | BNE   |  26  |  3   | 2  | Branch Z = 0               | * | * | * | * | * |
 * |             | LBNE  | 1026 | 5(6) | 4  | Long Branch Z = 0          | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BPL         | BPL   |  2A  |  3   | 2  | Branch Plus                | * | * | * | * | * |
 * |             | LBPL  | 102A | 5(6) | 4  | Long Branch Plus           | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BRA         | BRA   |  20  |  3   | 2  | Branch Always              | * | * | * | * | * |
 * |             | LBRA  |  16  |  5   | 3  | Long Branch Always         | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BRN         | BRN   |  21  |  3   | 2  | Branch Never               | * | * | * | * | * |
 * |             | LBRN  | 1021 |  5   | 4  | Long Branch Never          | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BSR         | BSR   |  8D  |  7   | 2  | Branch to Subroutine       | * | * | * | * | * |
 * |             | LBSR  |  17  |  9   | 3  | Long Branch to Subroutine  | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BVC         | BVC   |  28  |  3   | 2  | Branch V = 0               | * | * | * | * | * |
 * |             | LBVC  | 1028 | 5(6) | 4  | Long Branch V = 0          | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 * | BVS         | BVS   |  29  |  3   | 2  | Branch V = 1               | * | * | * | * | * |
 * |             | LBVS  | 1029 | 5(6) | 4  | Long Branch V = 1          | * | * | * | * | * |
 * -------------------------------------------------------------------------------------------
 *
 *
 * SIMPLE BRANCHES
 * -----------------------    NOTES:
 * |      |  OP  | ~ | # |
 * -----------------------    1. All conditional branches have both short and long variations.
 * | BRA  |  20  | 3 | 2 |
 * -----------------------    2. All short branches are 2 bytes and require 3 cycles.
 * | LBRA |  16  | 5 | 3 |
 * -----------------------    3. All conditional long branches are formed by prefixing the short
 * | BRN  |  21  | 3 | 2 |       branch opcode with $10 and using a 16-bit destination offset.
 * -----------------------
 * | LBRN | 1021 | 5 | 4 |    4. All conditional long branches require 4 bytes and 6 cycles if the
 * -----------------------       branch is taken or 5 cycles if the branch is not taken.
 * | BSR  |  8D  | 7 | 2 |
 * -----------------------    5. 5(6) Means: 5 cycles if branch not taken, 6 cycles if taken.
 * | LBSR |  17  | 9 | 3 |
 * -----------------------
 *
 * SIMPLE CONDITIONAL BRANCHES (Notes 1-4)    SIGNED CONDITIONAL BRANCHES (Notes 1-4)
 * -----------------------------------        ------------------------------------
 * | Test  |  True | OP | False | OP |        |  Test  |  True | OP | False | OP |
 * -----------------------------------        ------------------------------------
 * | N = 1 |  BMI  | 2B |  BPL  | 2A |        | r > m  |  BGT  | 2E |  BLE  | 2F |
 * -----------------------------------        ------------------------------------
 * | Z = 1 |  BEQ  | 27 |  BNE  | 26 |        | r >= m |  BGE  | 2C |  BLT  | 2D |
 * -----------------------------------        ------------------------------------
 * | V = 1 |  BVS  | 29 |  BVC  | 28 |        | r = m  |  BEQ  | 27 |  BNE  | 26 |
 * -----------------------------------        ------------------------------------
 * | C = 1 |  BCS  | 25 |  BCC  | 24 |        | r <= m |  BLE  | 2F |  BGT  | 2E |
 * -----------------------------------        ------------------------------------
 *                                            | r < m  |  BLT  | 2D |  BGE  | 2C |
 *                                            ------------------------------------
 *
 * UNSIGNED CONDITIONAL BRANCHES (Notes 1-4)
 * ------------------------------------
 * |  Test  |  True | OP | False | OP |
 * ------------------------------------
 * | r > m  |  BHI  | 22 |  BLS  | 23 |
 * ------------------------------------
 * | r >= m |  BHS  | 24 |  BLO  | 25 |
 * ------------------------------------
 * | r = m  |  BEQ  | 27 |  BNE  | 26 |
 * ------------------------------------
 * | r <= m |  BLS  | 23 |  BHI  | 22 |
 * ------------------------------------
 * | r < m  |  BLO  | 25 |  BHS  | 24 |
 * ------------------------------------
 */

#ifndef MC68B09E_H
#define MC68B09E_H

#include <cstdint>
#include <vector>

#include <Godot.hpp>
#include <Reference.hpp>
#include <Ref.hpp>

#include "PinsState.hpp"
#include "ConditionCodeRegister.hpp"

#include "Tcc1014.hpp"

namespace godot
{
	class Mc68b09e : public Reference
	{
		GODOT_CLASS(Mc68b09e, Reference)

	private:
		/**
		 * PUSH/PULL POST BYTE
		 *
		 * See PULU/PULS
		 *
		 * PUSH/PULL POST BYTE
		 * -----------------
		 * | | | | | | | | |
		 * -----------------
		 *  | | | | | | | |
		 *  | | | | | | | CCR
		 *  | | | | | | |
		 *  | | | | | | A
		 *  | | | | | |
		 *  | | | | | B
		 *  | | | | |
		 *  | | | | DPR
		 *  | | | |
		 *  | | | X
		 *  | | |
		 *  | | Y
		 *  | |
		 *  | S/U
		 *  |
		 *  PC
		 */
		union PushPullPostByte
		{
			std::uint8_t byte;

			struct
			{
				bool condition_code_register : 1;
				bool a_accumulator : 1;
				bool b_accumulator : 1;
				bool direct_page_register : 1;
				bool x_index_register : 1;
				bool y_index_register : 1;
				bool s_u_stack_pointer_register : 1;
				bool program_counter_register : 1;
			} bits;
		};

		/**
		 * TRANSFER/EXCHANGE POST BYTE
		 *
		 * See TFR/EXG
		 *
		 * TRANSFER/EXCHANGE POST BYTE        REGISTER FIELD
		 * ---------------------------        0000 D (AB)    1000 A
		 * | SOURCE  |  DESTINATION  |        0001 X         1001 B
		 * ---------------------------        0010 Y         1010 CCR
		 *                                    0011 U         1011 DPR
		 *                                    0100 S
		 *                                    0101 PC
		 */
		enum class TransferExchangePostByte
		{
			D_REGISTER,
			X_INDEX_REGISTER,
			Y_INDEX_REGISTER,
			USER_STACK_POINTER_REGISTER,
			HARDWARE_STACK_POINTER_REGISTER,
			PROGRAM_COUNTER_REGISTER,
			A_ACCUMULATOR = 0x08,
			B_ACCUMULATOR,
			CONDITION_CODE_REGISTER,
			DIRECT_PAGE_REGISTER
		};

		/**
		 * INDEXED ADDRESSING POSTBYTE REGISTER BIT
		 *
		 * See FIGURE 16
		 *
		 * ------------------------------------------------------------
		 * |    Post-Byte Register Bit     |         Indexed          |
		 * ---------------------------------        Addressing        |
		 * | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |           Mode           |
		 * ------------------------------------------------------------
		 * | 0 | R | R | d | d | d | d | d |  EA = ,R + 5 Bit Offset  |
		 * ------------------------------------------------------------
		 * | 1 | R | R | 0 | 0 | 0 | 0 | 0 |           ,R+            |
		 * ------------------------------------------------------------
		 * | 1 | R | R | i | 0 | 0 | 0 | 1 |           ,R++           |
		 * ------------------------------------------------------------
		 * | 1 | R | R | 0 | 0 | 0 | 1 | 0 |            ,-R           |
		 * ------------------------------------------------------------
		 * | 1 | R | R | i | 0 | 0 | 1 | 1 |           ,--R           |
		 * ------------------------------------------------------------
		 * | 1 | R | R | i | 0 | 1 | 0 | 0 |    EA = ,R +0 Offset     |
		 * ------------------------------------------------------------
		 * | 1 | R | R | i | 0 | 1 | 0 | 1 |   EA = ,R + ACCB Offset  |
		 * ------------------------------------------------------------
		 * | 1 | R | R | i | 0 | 1 | 1 | 0 |   EA = ,R + ACCA Offset  |
		 * ------------------------------------------------------------
		 * | 1 | R | R | i | 1 | 0 | 0 | 0 |   EA = ,R + 8 Bit Offset |
		 * ------------------------------------------------------------
		 * | 1 | R | R | i | 1 | 0 | 0 | 1 |  EA = ,R + 16 Bit Offset |
		 * ------------------------------------------------------------
		 * | 1 | R | R | i | 1 | 0 | 1 | 1 |    EA = ,R + D Offset    |
		 * ------------------------------------------------------------
		 * | 1 | x | x | i | 1 | 1 | 0 | 0 |  EA = ,PC + 9 Bit Offset |
		 * ------------------------------------------------------------
		 * | 1 | x | x | i | 1 | 1 | 0 | 1 | EA = ,PC + 16 Bit Offset |
		 * ------------------------------------------------------------
		 * | 1 | R | R | i | 1 | 1 | 1 | 1 |      EA = [,Address]     |
		 * ------------------------------------------------------------
		 *     \______/\__/\_______________/
		 *        |     |          |
		 *        |     |      Addressing Mode Field
		 *        |     |
		 *        |  Indirect Field (Sign bit when b(7) = 0)
		 *        |
		 *     Register Field: RR
		 *
		 *     00 = X  01 = Y            x = Don't Care  d = Offset Bit
		 *     10 = U  11 = S            i = 0 Not Indirect
		 *                                   1 Indirect
		 */
		union IndexedAddressingPostByte
		{
			std::uint8_t byte;

			struct
			{
				std::uint8_t addressing_mode_field : 4;
				bool indirect_field : 1;
				std::uint8_t register_field : 2;
				bool five_bit_offset : 1;
			} index;

			struct
			{
				std::uint8_t five_bit_offset : 5;
				int register_field : 2;
				std::uint8_t is_offset : 1;
			} five_bit_offset;
		};

		enum class RegisterField : std::uint8_t
		{
			X_INDEX_REGISTER,
			Y_INDEX_REGISTER,
			USER_STACK_POINTER_REGISTER,
			HARDWARE_STACK_POINTER_REGISTER
		};

		enum class AddressingMode : std::uint8_t
		{
			INCREMENT_BY_1,
			INCREMENT_BY_2,
			DECREMENT_BY_1,
			DECREMENT_BY_2,
			NO_OFFSET,
			B_ACCUMULATOR_OFFSET,
			A_ACCUMULATOR_OFFSET,
			EIGHT_BIT_OFFSET = 0x8,
			SIXTEEN_BIT_OFFSET,
			D_ACCUMULATOR_OFFSET = 0xB,
			CONSTANT_PROGRAM_COUNTER_8_BIT_OFFSET,
			CONSTANT_PROGRAM_COUNTER_16_BIT_OFFSET,
			EXTENDED_INDIRECT = 0xF,
		};

		/**
		 * PROGRAMMING MODEL
		 *
		 * See 4-301
		 *
		 * 15                           0
		 * ------------------------------  \                      7                      0
		 * |     X - Index Register     |  |                      ------------------------
		 * ------------------------------  |                      | Direct Page Register |
		 * |     Y - Index Register     |  |                      ------------------------
		 * ------------------------------  |- Pointer Registers
		 * |   U - User Stack Pointer   |  |                      7    Condition Code Register    0
		 * ------------------------------  |                      ---------------------------------
		 * | S - Hardware Stack Pointer |  |                      | E | F | H | I | N | Z | V | C |
		 * ------------------------------  /                      ---------------------------------
		 * |       Program Counter      |                           |   |   |   |   |   |   |   |
		 * ------------------------------  \                   Entire   |   |   |   |   |   |   Carry
		 * |      A      |      B       |  |- Accumulators              |   |   |   |   |   |
		 * ------------------------------  /                    FIRQ Mask   |   |   |   |   Overflow
		 * \____________________________/                                   |   |   |   |
		 *               |                                         Half Carry   |   |   Zero
		 *               D                                                      |   |
		 *                                                               IRQ Mask   Negative
		 */

		Ref<ConditionCodeRegister> condition_code_register;

		union DirectPageRegister
		{
			std::uint16_t value;

			struct
			{
				std::uint8_t dummy;
				std::uint8_t reg;
			} container;
		};

		DirectPageRegister direct_page_register = {0};

		union Accumulator
		{
			std::uint16_t d;

			struct
			{
				std::uint8_t b;
				std::uint8_t a;
			} registers;
		};

		Accumulator accumulator_register = {0};

		std::uint16_t x_index_register = 0;
		std::uint16_t y_index_register = 0;
		std::uint16_t user_stack_pointer_register = 0;
		std::uint16_t hardware_stack_pointer_register = 0;
		std::uint16_t program_counter_register = 0;

		float cycle_counter = 0;
		bool cwai = false;
		bool syncing = false;

		std::uint8_t negate(std::uint8_t byte);
		std::uint8_t complement(std::uint8_t byte);

		std::uint8_t logical_shift_right(std::uint8_t byte);
		std::uint8_t rotate_right(std::uint8_t byte);
		std::uint8_t arithmetic_shift_right(std::uint8_t byte);

		std::uint8_t rotate_left(std::uint8_t byte);
		std::uint8_t arithmetic_shift_left(std::uint8_t byte);

		std::uint8_t decrement(std::uint8_t byte);
		std::uint8_t increment(std::uint8_t byte);

		void test(std::uint8_t byte);
		void clear();

		std::uint16_t subtract(std::uint16_t minuend, std::uint16_t subtrahend);
		std::uint8_t subtract(std::uint8_t minuend, std::uint8_t subtrahend);

		std::uint16_t add(std::uint16_t augend, std::uint16_t addend);
		std::uint8_t add(std::uint8_t augend, std::uint8_t addend);

		void push_onto_hardware_stack(std::uint8_t byte);
		std::uint8_t pull_from_hardware_stack();

		void push_onto_user_stack(std::uint8_t byte);
		std::uint8_t pull_from_user_stack();

		void push_program_counter_register();
		void push_machine_state();
		void pull_machine_state();

		void compare(std::uint16_t slot, std::uint16_t post);
		void compare(std::uint8_t slot, std::uint8_t post);

		void store_register(std::uint16_t address, std::uint16_t data);
		void store_register(std::uint16_t address, std::uint8_t data);

		void condition_register(std::uint16_t data);
		void condition_register(std::uint8_t data);

		void exchange_registers(TransferExchangePostByte source, TransferExchangePostByte destination);
		std::uint16_t read_from_register(TransferExchangePostByte source);
		void write_to_register(TransferExchangePostByte destination, std::uint16_t data);

		std::uint8_t read(std::uint16_t address);
		std::uint16_t read2(std::uint16_t address);

		std::uint8_t read_and_advance_program_counter();
		std::uint16_t read2_and_advance_program_counter();

		void write(std::uint16_t address, std::uint8_t byte);
		void write2(std::uint16_t address, std::uint16_t bytes);

		void branch_relative_offset();
		void long_branch_relative_offset();

		std::uint16_t retrieve_direct_address();
		std::uint16_t retrieve_effective_address();

		std::uint16_t retrieve_register(RegisterField field);
		std::uint16_t retrieve_and_increment_register(RegisterField field, std::uint16_t amount);
		std::uint16_t retrieve_and_decrement_register(RegisterField field, std::uint16_t amount);

	public:
		Ref<PinsState> pins_state;
		Ref<Tcc1014> tcc1014;

		/**
		 * VECTOR TABLE
		 *
		 * See TABLE 1 - MEMORY MAP FOR INTERRUPT VECTORS
		 *
		 * ------------------------------------------------------------------
		 * | Memory Map For Vector Locations |       Interrupt Vector       |
		 * -----------------------------------          Description         |
		 * |       MS       |       LS       |                              |
		 * ------------------------------------------------------------------
		 * |      FFFE      |      FFFF      |            RESET             |
		 * ------------------------------------------------------------------
		 * |      FFFC      |      FFFD      |             NMI              |
		 * ------------------------------------------------------------------
		 * |      FFFA      |      FFFB      |             SWI              |
		 * ------------------------------------------------------------------
		 * |      FFF8      |      FFF9      |             IRQ              |
		 * ------------------------------------------------------------------
		 * |      FFF6      |      FFF7      |            FIRQ              |
		 * ------------------------------------------------------------------
		 * |      FFF4      |      FFF5      |            SWI2              |
		 * ------------------------------------------------------------------
		 * |      FFF2      |      FFF3      |            SWI3              |
		 * ------------------------------------------------------------------
		 * |      FFF0      |      FFF1      |           Reserved           |
		 * ------------------------------------------------------------------
		 */
		enum class VectorTable : std::uint16_t
		{
			RESERVED = 0xFFF0,
			SOFTWARE_INTERRUPT_3 = 0xFFF2,
			SOFTWARE_INTERRUPT_2 = 0xFFF4,
			FAST_INTERRUPT_REQUEST = 0xFFF6,
			INTERRUPT_REQUEST = 0xFFF8,
			SOFTWARE_INTERRUPT = 0xFFFA,
			NON_MASKABLE_INTERRUPT = 0xFFFC,
			RESET = 0xFFFE,
		};

		/**
		 * OPCODE DEFINITIONS
		 *
		 * See TABLE 9 - HEXADECIMAL VALUES OF MACHINE CODES
		 */
		enum class Opcodes : std::uint8_t
		{
			NEG_DIRECT,
			COM_DIRECT = 0x3,
			LSR_DIRECT,
			ROR_DIRECT = 0x6,
			ASR_DIRECT,
			ASL_DIRECT,
			ROL_DIRECT,
			DEC_DIRECT,
			INC_DIRECT = 0xC,
			TST_DIRECT,
			JMP_DIRECT,
			CLR_DIRECT,
			PAGE2,
			PAGE3,
			NOP_INHERENT,
			SYNC_INHERENT,
			LBRA_RELATIVE = 0x16,
			LBSR_RELATIVE,
			DAA_INHERENT = 0x19,
			ORCC_IMMEDIATE,
			ANDCC_IMMEDIATE = 0x1C,
			SEX_INHERENT,
			EXG_INHERENT,
			TFR_INHERENT,
			BRA_RELATIVE,
			BRN_RELATIVE,
			BHI_RELATIVE,
			BLS_RELATIVE,
			BHS_RELATIVE,
			BLO_RELATIVE,
			BNE_RELATIVE,
			BEQ_RELATIVE,
			BVC_RELATIVE,
			BVS_RELATIVE,
			BPL_RELATIVE,
			BMI_RELATIVE,
			BGE_RELATIVE,
			BLT_RELATIVE,
			BGT_RELATIVE,
			BLE_RELATIVE,
			LEAX_INDEXED,
			LEAY_INDEXED,
			LEAS_INDEXED,
			LEAU_INDEXED,
			PSHS_INHERENT,
			PULS_INHERENT,
			PSHU_INHERENT,
			PULU_INHERENT,
			RTS_INHERENT = 0x39,
			ABX_INHERENT,
			RTI_INHERENT,
			CWAI_INHERENT,
			MUL_INHERENT,
			RESET,
			SWI_INHERENT,
			NEGA_INHERENT,
			COMA_INHERENT = 0x43,
			LSRA_INHERENT,
			RORA_INHERENT = 0x46,
			ASRA_INHERENT,
			ASLA_INHERENT,
			ROLA_INHERENT,
			DECA_INHERENT,
			INCA_INHERENT = 0x4C,
			TSTA_INHERENT,
			CLRA_INHERENT = 0x4F,
			NEGB_INHERENT,
			COMB_INHERENT = 0x53,
			LSRB_INHERENT,
			RORB_INHERENT = 0x56,
			ASRB_INHERENT,
			ASLB_INHERENT,
			ROLB_INHERENT,
			DECB_INHERENT,
			INCB_INHERENT = 0x5C,
			TSTB_INHERENT,
			CLRB_INHERENT = 0x5F,
			NEG_INDEXED,
			COM_INDEXED = 0x63,
			LSR_INDEXED,
			ROR_INDEXED = 0x66,
			ASR_INDEXED,
			ASL_INDEXED,
			ROL_INDEXED,
			DEC_INDEXED,
			INC_INDEXED = 0x6C,
			TST_INDEXED,
			JMP_INDEXED,
			CLR_INDEXED,
			NEG_EXTENDED,
			COM_EXTENDED = 0x73,
			LSR_EXTENDED,
			ROR_EXTENDED = 0x76,
			ASR_EXTENDED,
			ASL_EXTENDED = 0x78,
			ROL_EXTENDED,
			DEC_EXTENDED,
			INC_EXTENDED = 0x7C,
			TST_EXTENDED,
			JMP_EXTENDED,
			CLR_EXTENDED,
			SUBA_IMMEDIATE,
			CMPA_IMMEDIATE,
			SBCA_IMMEDIATE,
			SUBD_IMMEDIATE,
			ANDA_IMMEDIATE,
			BITA_IMMEDIATE,
			LDA_IMMEDIATE,
			EORA_IMMEDIATE = 0x88,
			ADCA_IMMEDIATE,
			ORA_IMMEDIATE,
			ADDA_IMMEDIATE,
			CMPX_IMMEDIATE,
			BSR_RELATIVE,
			LDX_IMMEDIATE,
			SUBA_DIRECT = 0x90,
			CMPA_DIRECT,
			SBCA_DIRECT,
			SUBD_DIRECT,
			ANDA_DIRECT,
			BITA_DIRECT,
			LDA_DIRECT,
			STA_DIRECT,
			EORA_DIRECT,
			ADCA_DIRECT,
			ORA_DIRECT,
			ADDA_DIRECT,
			CMPX_DIRECT,
			JSR_DIRECT,
			LDX_DIRECT,
			STX_DIRECT,
			SUBA_INDEXED,
			CMPA_INDEXED,
			SBCA_INDEXED,
			SUBD_INDEXED,
			ANDA_INDEXED,
			BITA_INDEXED,
			LDA_INDEXED,
			STA_INDEXED,
			EORA_INDEXED,
			ADCA_INDEXED,
			ORA_INDEXED,
			ADDA_INDEXED,
			CMPX_INDEXED,
			JSR_INDEXED,
			LDX_INDEXED,
			STX_INDEXED,
			SUBA_EXTENDED,
			CMPA_EXTENDED,
			SBCA_EXTENDED,
			SUBD_EXTENDED,
			ANDA_EXTENDED,
			BITA_EXTENDED,
			LDA_EXTENDED,
			STA_EXTENDED,
			EORA_EXTENDED,
			ADCA_EXTENDED,
			ORA_EXTENDED,
			ADDA_EXTENDED,
			CMPX_EXTENDED,
			JSR_EXTENDED,
			LDX_EXTENDED,
			STX_EXTENDED,
			SUBB_IMMEDIATE,
			CMPB_IMMEDIATE,
			SBCB_IMMEDIATE,
			ADDD_IMMEDIATE,
			ANDB_IMMEDIATE,
			BITB_IMMEDIATE,
			LDB_IMMEDIATE,
			EORB_IMMEDIATE = 0xC8,
			ADCB_IMMEDIATE,
			ORB_IMMEDIATE,
			ADDB_IMMEDIATE,
			LDD_IMMEDIATE,
			LDU_IMMEDIATE = 0xCE,
			SUBB_DIRECT = 0xD0,
			CMPB_DIRECT,
			SBCB_DIRECT,
			ADDD_DIRECT,
			ANDB_DIRECT,
			BITB_DIRECT,
			LDB_DIRECT,
			STB_DIRECT,
			EORB_DIRECT,
			ADCB_DIRECT,
			ORB_DIRECT,
			ADDB_DIRECT,
			LDD_DIRECT,
			STD_DIRECT,
			LDU_DIRECT,
			STU_DIRECT,
			SUBB_INDEXED,
			CMPB_INDEXED,
			SBCB_INDEXED,
			ADDD_INDEXED,
			ANDB_INDEXED,
			BITB_INDEXED,
			LDB_INDEXED,
			STB_INDEXED,
			EORB_INDEXED,
			ADCB_INDEXED,
			ORB_INDEXED,
			ADDB_INDEXED,
			LDD_INDEXED,
			STD_INDEXED,
			LDU_INDEXED,
			STU_INDEXED,
			SUBB_EXTENDED,
			CMPB_EXTENDED,
			SBCB_EXTENDED,
			ADDD_EXTENDED,
			ANDB_EXTENDED,
			BITB_EXTENDED,
			LDB_EXTENDED,
			STB_EXTENDED,
			EORB_EXTENDED,
			ADCB_EXTENDED,
			ORB_EXTENDED,
			ADDB_EXTENDED,
			LDD_EXTENDED,
			STD_EXTENDED,
			LDU_EXTENDED,
			STU_EXTENDED,
			// Page 2 is 0x10xx
			LBRN_RELATIVE = 0x21,
			LBHI_RELATIVE,
			LBLS_RELATIVE,
			LBHS_RELATIVE,
			LBCS_RELATIVE,
			LBNE_RELATIVE,
			LBEQ_RELATIVE,
			LBVC_RELATIVE,
			LBVS_RELATIVE,
			LBPL_RELATIVE,
			LBMI_RELATIVE,
			LBGE_RELATIVE,
			LBLT_RELATIVE,
			LBGT_RELATIVE,
			LBLE_RELATIVE,
			SWI2_INHERENT = 0x3F,
			CMPD_IMMEDIATE = 0x83,
			CMPY_IMMEDIATE = 0x8C,
			LDY_IMMEDIATE = 0x8E,
			CMPD_DIRECT = 0x93,
			CMPY_DIRECT = 0x9C,
			LDY_DIRECT = 0x9E,
			STY_DIRECT,
			CMPD_INDEXED = 0xA3,
			CMPY_INDEXED = 0xAC,
			LDY_INDEXED = 0xAE,
			STY_INDEXED,
			CMPD_EXTENDED = 0xB3,
			CMPY_EXTENDED = 0xBC,
			LDY_EXTENDED = 0xBE,
			STY_EXTENDED,
			LDS_IMMEDIATE = 0xCE,
			LDS_DIRECT = 0xDE,
			STS_DIRECT,
			LDS_INDEXED = 0xEE,
			STS_INDEXED,
			LDS_EXTENDED = 0xFE,
			STS_EXTENDED,
			// and Page 3 is 0x11xx
			SWI3_INHERENT = 0x3F,
			CMPU_IMMEDIATE = 0x83,
			CMPS_IMMEDIATE = 0x8C,
			CMPU_DIRECT = 0x93,
			CMPS_DIRECT = 0x9C,
			CMPU_INDEXED = 0xA3,
			CMPS_INDEXED = 0xAC,
			CMPU_EXTENDED = 0xB3,
			CMPS_EXTENDED = 0xBC
		};

		// C++ Specific Definitions

		Mc68b09e();

		~Mc68b09e();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		// Class Definitions

		Ref<PinsState> get_pins_state();
		Ref<ConditionCodeRegister> get_condition_code_register();

		void set_debug(bool new_value);

		void reset();

		void latch_nmi();
		void handle_nmi();

		void latch_irq(bool delay);
		void handle_irq();

		void latch_firq();
		void handle_firq();

		float execute(float num_cycles_to_run, std::int64_t num_instructions_to_execute);

		void set_direct_page_register(std::uint8_t new_register);
		std::uint8_t get_direct_page_register();

		void set_a_accumulator(std::uint8_t new_register);
		std::uint8_t get_a_accumulator();

		void set_b_accumulator(std::uint8_t new_register);
		std::uint8_t get_b_accumulator();

		void set_d_accumulator(std::uint16_t new_register);
		std::uint16_t get_d_accumulator();

		void set_cwai(bool new_cwai);
		bool get_cwai();

		void set_syncing(bool new_syncing);
		bool get_syncing();
	};
}

#endif