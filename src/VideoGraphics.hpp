/**
 * From the Motorola Reference Sheet / Catalog for the MC6847(Y)
 *
 * MC6847/MC6847Y VIDEO DISPLAY GENERATOR (VDG)
 *
 *     The video display generator (VDG) provides a means of interfacing the M6800 microprocessor
 * family (or similar products) to a standard col- or or black and white NTSC television receiver.
 * Applications of the VDG include video games, process control displays, home computers,
 * education, communications, and graphics applications.
 *     The VDG reads data from memory and produces a video signal which will allow the generation
 * of alphanumeric or graphic displays. The generated video signal may be modulated to either
 * channel 3 or 4 by using the compatible MC1372 (TV chroma and video modulator). This modulated
 * signal is suitable for reception by a standard unmodified television receiver. A typical TV game
 * is shown in Figure 1.
 *
 * + Compatible with the M6800 Family, the M68000 Family, and Other Microprocessor Families
 * + Generates Four Different Alphanumeric Display Modes, Two Semi- graphic Modes, and Eight
 *   Graphic Display Modes
 * + The Alphanumeric Modes Display 32 Characters Per Line by 16 Lines Using Either the Internal
 *   ROM or an External Character Generator
 * + Alphanumeric and Sem1graph1c Modes May Be Mixed on a Char- acter-by-Character Basis
 * + Alphanumeric Modes Support Selectable Inverse on a Character- by-Character Basis
 * + Internal ROM May Be Mask Programmed with a Custom Pattern
 * + Full Graphic Modes Offer 64 x 64, 128 x 64, 128 x 96, 128 x 192, or 256 x 192 Densities
 * + Full Graphic Modes Use One of Two 4-Color Sets or One of Two 2-Color Sets
 * + Compatible with the MC1372 and MC1373 Modulators Via Y, R-Y lef,A), and B-Y lef,B) Interface
 * + Compatible with the MC6883 (74LS783) Synchronous-Address Multi- plexer
 * + Available in Either an Interlace (NTSC Standard) or Non-interlace Version
 *
 * (Edited) Figure 13 - Display Area Timing
 *
 * ----------------------------------------------------------------
 * |   Vertical Blanking 13 H Lines   |                           |
 * ----------------------------------------------------           |
 * |      Top Border 25 H Lines       |               |           |
 * ----------------------------------------           |           |
 * |  Active Display Area 192 H Lines |   |           |           |
 * |                                  |   |           |           |
 * |                                  |   | 192 Lines | 242 Lines | 262 (1/2) Lines
 * |              Falling Edge of FS* |   |           |           |
 * |                                 \|   V           |           |
 * ----------------------------------------           |           |
 * |     Bottom Border 26 H Lines     |               V           |
 * ----------------------------------------------------           |
 * |    Vertical Retrace 6 H Lines    |                           V
 * ----------------------------------------------------------------
 *
 * Figure 14 - Typical Format of the Television Screen Border
 *
 * (Black in all alpha/semigraphic modes. Green or buff (off-white) in all graphic modes.)
 *
 * ---------------------------------------------------------------
 * |                                                             |
 * |    -----------------------------------------------------    |
 * |    |                                                   |    |
 * |    |  <---- 256 Dots ---->                             |    |
 * |    |                                                   |    |
 * |    |    |                                              |    |
 * |    |    |                                              |    |
 * |    |    | 192 Dots         DISPLAY AREA                |    |
 * |    |    |           (49152 dots, all under VDG         |    |
 * |    |    V           control in all modes. Each         |    |
 * |    |               one of up to 8 colors when on       |    |
 * |    |                    depending on mode.)            |    |
 * |    |                                                   |    |
 * |    -----------------------------------------------------    |
 * |                                                             |
 * ---------------------------------------------------------------
 *
 * VIDEO DISPLAY GENERATOR DESCRIPTION
 *
 *     The MC6847/MC6847Y video display generators provide a simple interface for display of
 * digital information on a color monitor or standard color/black and white television receiver.
 *     Television transmissions in North and South America and Japan conform to the National
 * Television System Committee (NTSC) standards. This system is based on a field repetition rate of
 * 60 fields per second. There are 525 interlaced lines per frame or one-half this number per
 * field.
 *     The MC6847 scans one field of 262 lines 60 times per second. The MC6847 non-interlace VDG is
 * recommended for use in systems {i.e., TV games and personal computers) where absolute NTSC
 * compatibility is not required. If NTSC compatibility is required, perhaps for caption overlays
 * on broad-case signals, then the MC6847Y interlace VDG is recommended.
 *
 *     NOTE: A system with the MC6847 VDG and the MC1372 video modulator forms a transmitter,
 * transmitting at 61.2 MHz (channel 3) or 67.25 MHz (channel 4) depending on component values
 * chosen. This being a Class I TV device, care must be taken to meet FCC requirements Part 15,
 * Subpart H. However, if the composite video output from the MC1372 were to drive the television
 * directly, Section 15.7 of the FCC spec1f1cation must be adhered to.
 *
 * SIGNAL DESCRIPTION
 *
 * DISPLAY ADDRESS OUTPUT LINES IDA0-DA121
 *
 *     Thirteen address lines are used by the VDG to scan the display memory as shown in Figures
 * 4-7. The starting address of the display memory is located at the upper left corner of the
 * display screen. As the television sweeps from the left to right and top to bottom, the VDG
 * increments the RAM display address. The timing for two accesses starting at the beginning of the
 * line is shown in Figure 6. These lines are TTL compatible and may be forced into a
 * high-impedance state whenever MS (pin 12) goes low. AO-A3 change during the active display area.
 * A4 changes during the active display area in the alphanumerics, semigraphics, CG2, CG3, CG6, and
 * RG6 modes. A5-A 12 do not toggle within the active display area but instead, ripple through the
 * address during border and blanking time.
 *
 * DATA INPUTS IDD0-DD71
 *
 *     Eight TTL compatible data lines ;,re used to input data from RAM to be processed by the VDG.
 * The data is then interpreted and transformed into luminance (Y) and chroma outputs (¢A and ¢8).
 *
 * ...
 *
 * VIDEO OUTPUTS (¢A, ¢B, Y, CHB) - These four analog outputs are used to transfer luminance and
 * color information to a standard NTSC color television receiver, either via the MC1372 RF
 * modulator or via drivers directly into Y, ¢A, cps television video inputs (see Figures 10, 11,
 * and 12).
 *
 *     Luminance (Yl - This six level analog output contains composite sync, blanking and four
 * levels of video luminance.
 *
 *     ¢A - This three level analog output is used in combination with ¢B and Y outputs to specify
 * one of eight colors.
 *
 *     ¢B - This four level output is used in combination with ¢A and Y outputs to specify one of
 * eight colors. Additionally, one analog level is used to specify the time of the color burst
 * reference signal.
 *
 *     Chroma Bias (CHB) - This pin is an analog output and provides a DC reference corresponding
 * to the quiescent value of ¢A and 4-18. CHB is used to guarantee good thermal tracking and
 * minimize the variation between the MC1372 and MC6847. This pin, when pulled low, resets certain
 * registers within the chip In a user's system, this pin should not normally be used as an input.
 * It is used mainly to enhance test capabilities within the factory.
 *
 * SYNCHRONIZING INPUTS (MS, CLK)
 *
 *     THREE-STATE CONTROL - (MS) is a TTL compatible input which, when low, forces the VDG address
 * lines into a high-impedance state, as shown in Figure 9. This may be done to allow other devices
 * (such as an MPUl to address the display memory (RAM).
 *
 *     CLOCK (CLK) - The VDG clock input (CLK) requires a 3.579545 MHz (standaro color burst) TV
 * crystal frequency square wave. The duty cycle of this clock must be between 45 and 55% since it
 * controls the width of alternate dots on the television screen. The MC1372 RF modulator may be
 * used to supply the 3.579545 MHz clock and has provisions for a duty cycle adjustment. The VDG
 * will power-up using either the rising or falling edge of the clock. The dotted line on the CLK
 * signal in Figure 4 indicates this characteristic of latching in data on either clock edge.
 *
 * SYNCHRONIZING OUTPUTS (FS, HS, RPI
 *
 *     Three TTL compatible outputs provide circuits, exterior to the VDG, with timing references
 * to the following internal VDG states:
 *
 *     FIELD SYNC (FS) - The high-to-low transition ol the FS output coincides with 1he end of
 * active display area (see Figure 8). During this time interval, an MPU may have total access to
 * the display RAM without causing undesired flicker on the screen. The low-to-high transition of
 * FS coincides with the trailing edge of the vertical synchronization pulse.
 *
 *     HORIZONTAL SYNC (HSI - The HS pulse coincides with the horizontal synchronization pulse
 * furnished to the television receiver by the VDG (see Figure 7) The high-to- low transition of
 * the HS output coincides with the leading edge of the horizontal synchronization pulse and the
 * low-to- high transItIon coincides with the trailing edge.
 *
 *     ROW PRESET (RP) - If desired, an external character generator ROM may be used with the VDG.
 * However, an external four bit counter must be added to supply row addresses. The counter is
 * clocked by the HS signal and is cleared by the RP signal. RP pulses occur In all alphanumeric
 * and semigraphics modes; no pulses are output in the full graphic modes. RP occurs after the
 * first valid 12 lines. Therefore, use an FS clocked prelcadable counter such as a 74LS161 as
 * shown in Figures 7, 14, and 23
 *
 * MODE CONTROL LINES INPUT (A/G, A/S, INT/EXT, GMO, GM1, GM2, CSS, INV)
 *
 *     Eight TTL compatible inputs are used to control the operating mode of the VDG. A/S, INT/EXT,
 * CSS, and INV may be changed on a character-by-character basis. The CSS pin is used to select
 * between two possible alphanumeric colors when the VDG is in the alphanumeric mode and between
 * two color sets when the VDG is in the Semigraphics 6 or full graphic modes. Table 1 illustrates
 * the various modes that can be obtained using the mode control lines. There are two different
 * types of memory access concerning these modes, they are a short and a long access cycle, which
 * differ by a shift of one full 3.58 MHz cycle. One of the differences between these access times,
 * in the short access time frame, is a shift of one full 3.58 MHz cycle from the corresponding
 * normal long access time frame, as shown in Figure 6. The modes using short access times read
 * memory twice as often as the long access modes.
 *
 * Table 1 - MODE CONTROL LINES (INPUTS)
 *
 * ---------------------------------------------------------------------------------------------------------
 * | A/G | A/S | INT/EXT | INV | GM2 | GM1 | GM0 |     Alpha/Graphic Mode Select             | # of Colors |
 * ---------------------------------------------------------------------------------------------------------
 * |  0  |  0  |    0    |  0  |  X  |  X  |  X  | Internal Alphanumerics                    |      2      |
 * |  0  |  0  |    0    |  1  |  X  |  X  |  X  | Internal Alphanumerics Inverted           |             |
 * |  0  |  0  |    1    |  0  |  X  |  X  |  X  | External Alphanumerics                    |             |
 * |  0  |  0  |    1    |  1  |  X  |  X  |  X  | External Alphanumerics Inverted           |             |
 * ---------------------------------------------------------------------------------------------------------
 * |  0  |  1  |    0    |  X  |  X  |  X  |  X  | Semigraphics 4 (SG4)                      |      8      |
 * |  0  |  1  |    1    |  X  |  X  |  X  |  X  | Semigraphics 6 (SG6)                      |      8      |
 * ---------------------------------------------------------------------------------------------------------
 * |  1  |  X  |    X    |  X  |  0  |  0  |  0  | 64 x 64 Color Graphics One (CG1)          |      4      |
 * |  1  |  X  |    X    |  X  |  0  |  0  |  1  | 128 x 64 Resolution Graphics One (RG1)    |      2      |
 * |  1  |  X  |    X    |  X  |  0  |  1  |  0  | 128 x 64 Color Graphics Two (CG2)         |      4      |
 * |  1  |  X  |    X    |  X  |  0  |  1  |  1  | 128 x 96 Resolution Graphics Two (RG2)    |      2      |
 * |  1  |  X  |    X    |  X  |  1  |  0  |  0  | 128 x 96 Color Graphics Three (CG3)       |      4      |
 * |  1  |  X  |    X    |  X  |  1  |  0  |  1  | 128 x 192 Resolution Graphics Three (RG3) |      2      |
 * |  1  |  X  |    X    |  X  |  1  |  1  |  0  | 128 x 192 Color Graphics Six (CG6)        |      4      |
 * |  1  |  X  |    X    |  X  |  1  |  1  |  1  | 256 x 192 Resolution Graphics Six (RG6)   |      2      |
 * ---------------------------------------------------------------------------------------------------------
 *
 * OPERATION OF THE VDG
 *
 *     A simplified block diagram of the VDG is shown in Figure 17a and a detailed block diagram is
 * shown in Figure 17b.
 *     The externally generated 3.58 MHz color burst clock drives the VDG. Referring to Figures 11
 * and 12, note that the horizontal screen span from blanking to blanking is 193.1 clock periods
 * (=53.95 μ.s). The display window is offset from the left-hand edge by 283 periods and lasts for
 * 128 periods {35.75 μ.s) Of the 242 lines on the vertical screen from blanking to blanking, 192
 * lines are used for the display. The display window is offset from the top by 25 lines. Under the
 * constraint of the master clock, the smallest display element possible for the VOG is half period
 * of the 3.58 MHz clock wide by one scan line high. All other display elements are multiples of
 * this basic size.
 *
 * DISPLAY MEMORY ADDRESS DRIVERS
 *
 *     The address drivers normally drive the video refresh address into the display memory so
 * characters may be displayed on the CRT. When the memory select pin {MS) is pulled low by an
 * external decoder, the driver outputs go to a h1gh-1rnpedance state so external three-state
 * drivers may switch the MPU produced address onto the display memory address bus; the MPU may
 * directly manipulate data in the display memory
 *
 * VIDEO TIMING AND CONTROL
 *
 *     This subsystem of the VDG 1ncludes the mode decoding,timing generation, and associated row
 * counter logic, and uses the 3.58 MHz color frequency to generate horizontal and vertical timing
 * information (via linear shift register counters), which the video and chroma encoder uses to
 * generate color video information The horizontal timing for the VDG 1s summarized in Figure 7.
 * Ten and one-half cycles of the 3.58 MHz subcarrier are transm1tted on the back porch of every
 * horizontal blanking period. This color burst is suppressed during vertical sync and equalizing
 * intervals. Color burst is also suppressed in the most dense two color graphic modes. This leads
 * to some interesting rainbow effects on the display which 1s frequency and pattern dependent. The
 * vertical timing for the VDG is given in Figure 18. Vertical retrace is initiated by the
 * luminance signal being brought to the blanking level. The vertical blanking period begins with
 * three lines of equalizing pulses followed by three lines of serrated vertical sync pulses
 * followed by three more lines of equalizing pulses. The remaining vertical blanking period
 * contains the normal horizontal sync pulses. The equalizing and serra- tion pulses are at half
 * line frequency. Notice the difference in spacing between the last horizontal sync pulse and the
 * first equalizing pulse in even and odd fields. lt is the half line difference between fields
 * that produces the interlaced picture in a frame. Vertical timing between fielcs for the
 * non-interlaced VDG, on the other hand, is identical. The equalizing and serration pulses are,
 * however, at the horizontal frequency.
 *     The 3.58 MHz color frequency is also used to clock the video shift register load counter.
 * This counter and the video shift clock inhibit circuitry derive the dot-clock for the output of
 * the video shift registers and the load signals for the video shift registers' input latches. The
 * vertical and horizontal address counters generate the addresses for the external display memory.
 *
 * INTERNAL CHARACTER GENERATOR ROM
 *
 *     Since many uses of the VDG will involve the display of alphanumeric data, a
 * character-generator ROM is included on the chip. This ROM will generate 64 standard 5x 7 dot
 * matrix characters from standard 6-bit ASCII input. A standard character set is included in the
 * MC6847 although the ROM is custom programmable.
 *
 * INTERNAL/EXTERNAL CHARACTER GENERATOR MULTIPLEXER
 *
 *     The internal/external multiplexer allows the use of either the internal ROM or an external
 * character generator. This multiplexer may be switched on a character-by-character basis to allow
 * mixed internal and external characters on the CRT. The external character may be any desired
 * dot-pattern in the standarU 8x12 one-character display matrix, thus allowing the maximum 256x192
 * screen density.
 *
 * VIDEO AND COLOR SUBSYSTEM
 *
 *     The 8-b1t output of the internal/external multiplexer 1s serialized in an 8-bit shift
 * register clocked at the dot-clock frequency.
 *     The luminance information from the shift register is summed with the horizontal and vertical
 * sync signals to produce a composite video signal less the chrominance 1nformat1on, called Y. The
 * luminance signal, Y, and the two chrominance outputs, A {R-Y) and B (B-Y), can be combined
 * (modulated) by an MC1372 into a composite video signal with color. Figures 8, 9, 10, and 16 show
 * the re!ationshtp between the luminance and chrominance signals and the resultant color.
 *
 * DISPLAY MODES
 *
 *     There are two major display modes in the VDG. Major mode 1 contains four alphanumeric and
 * two limited graphic modes. Major mode 2 contains eight graphic modes. Of these, four are full
 * color graphic and four restricted color graphic modes. The mode selection for the VDG is
 * summarized in Table 2. The mnemonics of these fourteen modes are explained in the following
 * sections.
 *     ln major mode 1 the display window 1s divided into 32 columns by 16 character element rows
 * thus requiring 512 bytes of memory. Each character element is 8 half periods by 12 scan lines in
 * size as shown in Figure 19. The area outside the display window is black.
 *     The VDG has a built-in character generator ROM containing the 64 ASCII characters In a 5x7
 * format (see Figure 20) The 5x7 character font is positioned two columns to the right and three
 * rows down within the 8x12 character element. Six bits of the 8-bit data word are typically used
 * for the internal ASCII character generator. The remaining two bits may be used to implement
 * inverse video, color switching, or external character generator ROM selection on a
 * character-by-character basis. For those who wish to display lower case letters, special
 * characters, or even limited-graphics, an external ROM may be used. If such external ROM is used,
 * all of the 8x12 picture elements, or pixels, in the character element can be utilized.
 * Characters may be either green on a dark green background or orange on a dark orange background,
 * depending on the state of the CSS pin. The invert pin can be used to display dark characters on
 * a bright background.
 *     The two limited graphic modes are Semigraphics 4 and Sernigraph1cs 6. In Semigraphics 4, the
 * 8x12 dot character block is divided into four pixels (each pixel is four half-clocks by six scan
 * lines). The four low-order bits (DD0-003) of each incoming byte of data select one of sixteen
 * possible illumination patterns while the next three bits {004-006) determine the color of the
 * illuminated elements. The most significant bit is unused. Figure 21 shows the color and pattern
 * selections. In Semigraphics 6 the 8x12 dot character block is divided into six pixels, each four
 * half-clocks by four scan lines. The six low-order bits of each byte of incoming data select one
 * of 64 possible illumination patterns while the CSS input and the high-order data bits (0D6-0D7)
 * determine the color of the illuminated elements.
 *     The display window in major mode 2 (full graphics) has a less rigorous format than in major
 * mode 1. The display elements vary from one scan line to three scan lines in height. The length
 * of the display element is either eight or sixteen half-periods wide. Each display element is
 * divided into four or eight pixels. The former corresponds to a full color mode while the latter
 * a restricted color mode, like the semigraphics modes, represents illumination data. When it is
 * high the pixel is illuminated with the color chosen by the color set select (CSS) pin. When it
 * is low the pixel Is black. In the full color modes, pairs of data bits choose one of four colors
 * in one of two color sets defined by the CSS pin. Depending on the state of the CSS pin, the area
 * outside the display window is either green or buff. The display formats and color selection for
 * this major mode are summarized in Figure 19.
 *
 *     THE 64x64 COLOR GRAPHICS ONE (CG1) MODE - The 64x64 color graphics mode generates a display
 * matrix of 64 elements wide by 64 elements high. Each element may be one of four colors. A 1k x 8
 * display memory is required. The display RAM is accessed 16 times per horizontal line. Each pixel
 * equals four half-clocks by three scan lines.
 *
 *     THE 128x64 RESOLUTION GRAPHICS ONE (RGl) MODE - The 128x64 graphics mode generates a matrix
 * 128 elements wide by 64 elements high. Each element may be either ON or OFF. However, the entire
 * display may be one of two colors, selected bv using the color set select pin. A 1k x 8 display
 * memory is required. The display RAM is accessed 16 times per horizontal line. Each pixel equals
 * two half-clocks by three scan lines.
 *
 *     THE 128x64 COLOR GRAPHICS TWO (CG2) MODE - The 128x64 color graphics mode generates a
 * display matrix 128 elements wide by 64 elements high. Each element may be one of four colors. A
 * 2k x 8 display memory is required. The display RAM is accessed 32 times per horizontal line.
 * Each pixel equals two half-clocks by three scan lines.
 *
 *     THE 128 x 96 RESOLUTION GRAPHICS TWO (RG2) MODE - The 128x96 graphics mode generates a
 * display matrix 128 elements wide by 96 elements high. Each element may be either ON or OFF.
 * However, the entire display may be one of two colors selected by using the color set select pin.
 * A 1.5k x 8 display memory is required. The display RAM is accessed 16 times per horizontal line.
 * Each pixel equals two half-clocks by two scan lines.
 *
 *     THE 128x 96 COLOR GRAPHICS THREE (CG3) MODE - The 128x96 color graphics mode generates a
 * display 128 elements wide by 96 elements high. Each element may be one of four colors. A 3k x 8
 * display memory is required. The display RAM is accessed 32 times per horizontal line. Each pixel
 * equals two half-clocks by two scan lines.
 *
 *     THE 128 x 192 RESOLUTION GRAPHICS THREE (RG3) MODE - The 128x192 graphics mode generates a
 * display matrix 128 elements wide by 192 elements high. Each element may be either ON or OFF, but
 * the ON element may be one of two colors selected with the color set select pin. A 3k x 8 display
 * memory is required. The display RAM is accessed 16 times per horizontal line. Each pixel equals
 * two half-clocks by one scan line.
 *
 *     THE 128x 192 COLOR GRAPHICS SIX (CG6) MODE The 128x192 color graphics mode generates a
 * display 128 elements wide by 192 elements high. Each element may be one of four colors. A 6k x 8
 * display memory is required. The display RAM is accessed 32 times per horizontal line. Each pixel
 * equals two half-clocks by one scan line.
 *
 *     THE 256x 192 RESOLUTION GRAPHICS SIX (RG6) MODE - The 256x193 graphics mode generates a
 * display 256 elements wide by 192 elements high. Each element may be either ON or OFF, but the ON
 * element may be one of two colors selected with the color set select pin. A 6k x 8 display memory
 * is required. The display RAM is accessed 32 times per horizontal line. Each pixel equals one
 * half-clock by one scan line.
 *
 * Table 2 - SUMMARY OF MAJOR MODES
 *
 * MAjoe Mode 1 - Alpha Modes
 *
 * -----------------------------------------------------------------------------------------------------------------------------
 * |     Title     |  Memory |     Display Elements     | Colors |     Title      |  Memory |    Display Elements     | Colors |
 * -----------------------------------------------------------------------------------------------------------------------------
 * | Alphanumerics | 512 x 8 |  2 -> |  |<-->| 5        |    2   | Semigraphics 4 | 512 x 8 |                         |   8    |
 * |   (Internal)  |         |       -----------  ---   |        |                |         |  -----------            |        |
 * |               |         |       |         |   |    |        |                |         |  |    |    |            |        |
 * |               |         |  ---  |  ------ |   |    |        |                |         |  |    |    |            |        |
 * |               |         |   |   |  |    | |   |    |        |                |         |  -----------            |        |
 * |               |         | 7 |   |  |    | |   | 12 |        |                |         |  |    |XXXX| <- Element |        |
 * |               |         |   V   |  |    | |   |    |        |                |         |  |    |XXXX|            |        |
 * |               |         |  ---  |  ------ |   |    |        |                |         |  -----------            |        |
 * |               |         |   2   |         |   V    |        |                |         |                         |        |
 * |               |         |  ---  -----------  ---   |        |                |         |                         |        |
 * |               |         |                          |        |                |         |                         |        |
 * | Alphanumerics |         |            8             |        | Semigraphics 6 | 512 x 8 |                         |        |
 * |   (External)  | 512 x 8 |       |<------->|        |    2   |                |         |  -----------            |    4   |
 * |               |         |       -----------  ---   |        |                |         |  |    |    |            |        |
 * |               |         |       |         |   |    |        |                |         |  |    |    |            |        |
 * |               |         |       |         |   |    |        |                |         |  -----------            |        |
 * |               |         |       |         |   |    |        |                |         |  |    |XXXX| <- Element |        |
 * |               |         |       |         |   | 12 |        |                |         |  |    |XXXX|            |        |
 * |               |         |       |         |   |    |        |                |         |  -----------            |        |
 * |               |         |       |         |   |    |        |                |         |  |    |    |            |        |
 * |               |         |       |         |   |    |        |                |         |  |    |    |            |        |
 * |               |         |       |         |   V    |        |                |         |  -----------            |        |
 * |               |         |       -----------  ---   |        |                |         |                         |        |
 * |               |         |                          |        |                |         |                         |        |
 * -----------------------------------------------------------------------------------------------------------------------------
 *
 * Major Mode 2 - Graphics Modes
 *
 * ---------------------------------------------------------------------------
 * |         Title         |  Memory  | Colors |          Comments           |
 * ---------------------------------------------------------------------------
 * | 64x64 Color Graphic   |  1k x 8  |    4   | Matrix 64x64 Elements       |
 * | 128x64 Graphics*      |  1k x 8  |    2   | Matrix 128 Elements Wide by |
 * | 128x64 Color Graphic  |  2k x 8  |    4   |   64 Elements High          |
 * | 128x96 Graphics*      | 1.5k x 8 |    2   | Matrix 128 Elements Wide by |
 * | 128x96 Color Graphic  |  3k x 8  |    4   |   96 Elements High          |
 * | 128x192 Graphics*     |  3k x 8  |    2   | Matrix 128 Elements Wide by |
 * | 128x192 Color Graphic |  6k x 8  |    4   |   192 Elements High         |
 * | 256x192 Graphics*     |  6k x 8  |    2   | Matrix 256 Elements Wide by |
 * |                       |          |        |   192 Elements High         |
 * ---------------------------------------------------------------------------
 *     * Graphics mode turns on or off each element. The color may be one of two.
 *
 * Table 3 - DETAILED DESCRIPTION OF VDG MODES (Excerpts)
 *
 * Alphanumerics Internal G/A 0 S/A 0
 *
 * ---------------------------------------------------------------------------
 * | CSS | INV | Character Color | Background | Border |    VDG Data Bus     |
 * ---------------------------------------------------------------------------
 * |  0  |  0  | Green           | Black      | Black  |  _________________  |
 * |     |  1  | Black           | Green      |        |  | | | | | | | | |  |
 * |  1  |  0  | Orange          | Black      | Black  |  -----------------  |
 * |     |  1  | Black           | Orange     |        |   --- -----------   |
 * |     |     |                 |            |        |    |   ASCII Code   |
 * |     |     |                 |            |        |  Extra              |
 * ---------------------------------------------------------------------------
 *
 * Semigraphics Four G/A 0 S/A 1
 *
 * ------------------------------------------------------------------------------------------------------------------------------
 * |                 Color                  |              TV Screen              |                                             |
 * --------------------------------------------------------------------------------                VDG Data Bus                 |
 * | L | C2 | C1 | C0 | Background | Border |    Display Mode     |     Detail    |                                             |
 * ------------------------------------------------------------------------------------------------------------------------------
 * | 0 | X  | X  | X  | Black      | Black  | 64 Display elements |               |                                             |
 * | 1 | 0  | 0  | 0  | Green      |        | per row             |  -----------  |                                             |
 * | 1 | 0  | 0  | 1  | Yellow     |        |                     |  |    |    |  |                                             |
 * | 1 | 0  | 1  | 0  | Blue       |        | 32 Display elements |  | L3 | L2 |  |  -----------------------------------------  |
 * | 1 | 0  | 1  | 1  | Red        |        | in rows             |  |    |    |  |  |    | C2 | C1 | C0 | L3 | L2 | L1 | L0 |  |
 * | 1 | 1  | 0  | 0  | Buff       |        |                     |  -----------  |  -----------------------------------------  |
 * | 1 | 1  | 0  | 1  | Cyan       |        |                     |  |    |    |  |                                             |
 * | 1 | 1  | 1  | 0  | Magenta    |        |                     |  | L1 | L0 |  |                                             |
 * | 1 | 1  | 1  | 1  | Orange     |        |                     |  |    |    |  |                                             |
 * |   |    |    |    |            |        |                     |  -----------  |                                             |
 * |   |    |    |    |            |        |                     |      4x6      |                                             |
 * ------------------------------------------------------------------------------------------------------------------------------
 *
 * 64 x 64 (Color Graphics One) G/A 1 GM2 0 GM1 0 GM0 0
 *
 * ----------------------------------------------------------------------------------------------------------------------------------
 * |              Color               |                   TV Screen                   |                                             |
 * ------------------------------------------------------------------------------------                VDG Data Bus                 |
 * | CSS | C1 | C0 |  Color  | Border |    Display Mode     |         Detail          |                                             |
 * ----------------------------------------------------------------------------------------------------------------------------------
 * | 0   | 0  | 0  | Green   | Green  | 64 Display elements |                         |                                             |
 * |     | 0  | 1  | Yellow  |        | per row             |                         |                                             |
 * |     | 1  | 0  | Blue    |        |                     |  ---------------------  |  -----------------------------------------  |
 * |     | 1  | 1  | Red     |        | 64 Display elements |  | E3 | E2 | E1 | E0 |  |  | C1 | C0 | C1 | C0 | C1 | C0 | C1 | C0 |  |
 * | 1   | 0  | 0  | Buff    | Buff   | in rows             |  ---------------------  |  -----------------------------------------  |
 * |     | 0  | 1  | Cyan    |        |                     |           4x3           |                                             |
 * |     | 1  | 0  | Magenta |        |                     |                         |                                             |
 * |     | 1  | 1  | Orange  |        |                     |                         |                                             |
 * ----------------------------------------------------------------------------------------------------------------------------------
 *
 * 128 x 64 (Resolution Graphics One) G/A 1 GM2 0 GM1 0 GM0 1
 *
 * ----------------------------------------------------------------------------------------------------------------------------------------------
 * |          Color           |                            TV Screen                              |                                             |
 * ------------------------------------------------------------------------------------------------                VDG Data Bus                 |
 * | CSS | L | Color | Border |     Display Mode     |                   Detail                   |                                             |
 * ----------------------------------------------------------------------------------------------------------------------------------------------
 * | 0   | 0 | Black | Green  | 128 Display elements |                                            |                                             |
 * |     | 1 | Green |        | per row              | -----------------------------------------  |  -----------------------------------------  |
 * | 1   | 0 | Black | Buff   |                      | | L7 | L6 | L5 | L4 | L3 | L2 | L1 | L0 |  |  | L7 | L6 | L5 | L4 | L3 | L2 | L1 | L0 |  |
 * |     | 1 | Buff  |        | 64 Display elements  | -----------------------------------------  |                                             |
 * |     |   |       |        | in rows              |                    3x3                     |                                             |
 * ----------------------------------------------------------------------------------------------------------------------------------------------
 *
 * 128 x 64 (Color Graphics Two) G/A 1 GM2 0 GM1 1 GM0 0
 *
 * -----------------------------------------------------------------------------------------------------------------------------------
 * |              Color               |                    TV Screen                   |                                             |
 * -------------------------------------------------------------------------------------                VDG Data Bus                 |
 * | CSS | C1 | C0 |  Color  | Border |    Display Mode      |         Detail          |                                             |
 * -----------------------------------------------------------------------------------------------------------------------------------
 * | 0   | 0  | 0  | Green   | Green  | 128 Display elements |                         |                                             |
 * |     | 0  | 1  | Yellow  |        | per row              |                         |                                             |
 * |     | 1  | 0  | Blue    |        |                      |  ---------------------  |  -----------------------------------------  |
 * |     | 1  | 1  | Red     |        | 64 Display elements  |  | E3 | E2 | E1 | E0 |  |  | C1 | C0 | C1 | C0 | C1 | C0 | C1 | C0 |  |
 * | 1   | 0  | 0  | Buff    | Buff   | in rows              |  ---------------------  |  -----------------------------------------  |
 * |     | 0  | 1  | Cyan    |        |                      |           3x3           |                                             |
 * |     | 1  | 0  | Magenta |        |                      |                         |                                             |
 * |     | 1  | 1  | Orange  |        |                      |                         |                                             |
 * -----------------------------------------------------------------------------------------------------------------------------------
 *
 * 128 x 96 (Resolution Graphics Two) G/A 1 GM2 0 GM1 1 GM0 1
 *
 * -----------------------------------------
 * | L7 | L6 | L5 | L4 | L3 | L2 | L1 | L0 |
 * -----------------------------------------
 *                    2x2
 *
 * 128 x 96 (Color Graphics Three) G/A 1 GM2 1 GM1 0 GM0 0
 *
 * ---------
 * | | | | |  2x2
 * ---------
 * E3     E0
 *
 * 128 x 192 (Resolution Graphics Three) G/A 1 GM2 1 GM1 0 GM0 1
 *
 * -----------------
 * | | | | | | | | |  2x1
 * -----------------
 * L7             L0
 *
 * 128 x 192 (Graphics Mode Six) G/A 1 GM2 1 GM1 1 GM0 0
 *
 * ---------
 * | | | | |  2x1
 * ---------
 * E3     E0
 *
 * 256 x 192 (Resolution Graphics Six) G/A 1 GM2 1 GM1 1 GM0 1
 *
 * -----------------
 * | | | | | | | | |  1x1
 * -----------------
 * L7             L0
 *
 */

/**
 * From the TANDY Service Manual for the Color Computer 3 (26-3334)
 *
 * ...
 *
 * FF98:       Video Mode Register
 *
 *        BIT 7 = BP          0 = alphanumeric, 1 = bit plane
 *        BIT 6   -
 *        BIT 5 = BPI         1 = Burst phase inverted
 *        BIT 4 = MOCH        1 = monochrome (on composite)
 *        BIT 3 = H50         1 = 50 Hz vertical sync
 *        BIT 2 = LPR2        Lines per row (See table below)
 *        BIT 1 = LPR1        Lines per row (See table below)
 *        BIT 0 = LPR0        Lines per row (See table below)
 *
 * -------
 * |
 * | Alternate Color Set
 * |
 * |     Setting bit 5 of dedicated address FF98 invokes the alternate color set. In this set all
 * | the same colors are available; they are just specified by a different color code. Enabling the
 * | alternate color set has the effect of shifting all the colors, except gray, half way around the
 * | hue wheel in Fig. 2-4. Thus the hue angle specifies a different color than with the normal
 * | color set. The purpose of the alternate color set is to simulate the original artifact effect
 * | on a TV. With the original CoCo, sometimes the artifact colors would be of one set and other
 * | times of another set.
 * |
 * - From Assembly Language Programming For The COCO 3 by Laurence A. Tepolt
 *
 * ------------------------------------------------------------
 * | LPR2 | LPR1 | LPR0 | Lines per character row             |
 * ------------------------------------------------------------
 * |  0   |  0   |  0   | one        (Graphics modes)         |
 * |  0   |  0   |  1   | two        (CoCo 1 and CoCo 2 only) |
 * |  0   |  1   |  0   | three      (CoCo 1 and CoCo 2 only) |
 * |  0   |  1   |  1   | eight                               |
 * |  1   |  0   |  0   | nine                                |
 * |  1   |  0   |  1   | (reserved)                          |
 * |  1   |  1   |  0   | twelve     (CoCo 1 and CoCo 2 only) |
 * |  1   |  1   |  1   | (reserved)                          |
 * ------------------------------------------------------------
 *
 * FF99:       Video Resolution Register
 *
 *        BIT 7   -
 *        BIT 6 = LPF1        Lines per field (See table below)    [VRES1]
 *        BIT 5 = LPF0        Lines per field                      [VRES0]
 *        BIT 4 = HRES2       Horizontal resolution (See Video resolution on page 17)
 *        BIT 3 = HRES1       Horizontal resolution
 *        BIT 2 = HRES0       Horizontal resolution
 *        BIT 1 = CRES1       Color resolution (See Video resolution)
 *        BIT 0 = CRES0       Color resolution
 *
 * ---------------------------------
 * | LPF1 | LPF0 | Lines per field |
 * ---------------------------------
 * |  0   |  0   |       192       |
 * |  0   |  1   |       200       |
 * |  1   |  0   |     Reserved    |
 * |  1   |  1   |       225       |
 * ---------------------------------
 *
 * From page 18:
 *
 * VIDEO RESOLUTION
 *
 * The combination of HRES and CRES bits determine the resolution of the screen. The following
 * resolutions are supported:
 *
 * Alphanumerics: BP = 0, CoCo = 0
 *
 * --------------------------------------------------------
 * |              |       |       |       |       |       |
 * | \    RES bit |       |       |       |       |       |
 * |     \        | HRES2 | HRES1 | HRES0 | CRES1 | CRES0 |
 * |         \    |       |       |       |       |       |
 * |  MODE      \ |       |       |       |       |       |
 * |              |       |       |       |       |       |
 * --------------------------------------------------------
 * |              |       |       |       |       |       |
 * | 32 character |   0   |   -   |   0   |   -   |   1   |
 * |              |       |       |       |       |       |
 * | 40 character |   0   |   -   |   1   |   -   |   1   |
 * |              |       |       |       |       |       |
 * | 80 character |   1   |   -   |   1   |   -   |   1   |
 * --------------------------------------------------------
 *
 * Graphics: BP = 1, CoCo = 0
 *
 * -----------------------------------------------------------
 * | Pixels | Colors | HRES2 | HRES1 | HRES0 | CRES1 | CRES0 |
 * -----------------------------------------------------------
 * |  640   |   4    |   1   |   1   |   1   |   0   |   1   |
 * |  640   |   2    |   1   |   0   |   1   |   0   |   0   |
 * -----------------------------------------------------------
 * |  512   |   4    |   1   |   1   |   0   |   0   |   1   |
 * |  512   |   2    |   1   |   0   |   0   |   0   |   0   |
 * -----------------------------------------------------------
 * |  320   |   16   |   1   |   1   |   1   |   1   |   0   |
 * |  320   |   4    |   1   |   0   |   1   |   0   |   1   |
 * -----------------------------------------------------------
 * |  256   |   16   |   1   |   1   |   0   |   1   |   0   |
 * |  256   |   4    |   1   |   0   |   0   |   0   |   1   |
 * |  256   |   2    |   0   |   1   |   0   |   0   |   0   |
 * -----------------------------------------------------------
 * |  160   |   16   |   1   |   0   |   1   |   1   |   0   |
 * -----------------------------------------------------------
 *
 * In addition to the above modes, the previous CoCo modes are available.
 *
 * -------------------------------------------------------------
 * |                          | MC6883 (SAM) |                 |
 * |                          | DISPLAY MODE |                 |
 * |                          |              | Reg. FF22       |
 * |                          |   V2 V1 V0   | 7  6  5  4  3   |
 * -------------------------------------------------------------
 * | Alphanumerics            |   0  0  0    | 0  X  X  0  CSS |
 * | Alphanumerics Inverted   |   0  0  0    | 0  X  X  0  CSS |
 * -------------------------------------------------------------
 * | Semigraphics - 4         |   0  0  0    | 0  X  X  0  X   |
 * -------------------------------------------------------------
 * | 64 x 64 Color Graphics   |   0  0  1    | 1  0  0  0  CSS |
 * -------------------------------------------------------------
 * | 128 x 64 Graphics        |   0  0  1    | 1  0  0  1  CSS |
 * | 128 x 64 Color Graphics  |   0  1  0    | 1  0  1  0  CSS |
 * -------------------------------------------------------------
 * | 128 x 96 Graphics        |   0  1  1    | 1  0  1  1  CSS |
 * | 128 x 96 Color Graphics  |   1  0  0    | 1  1  0  0  CSS |
 * -------------------------------------------------------------
 * | 128 x 192 Graphics       |   1  0  1    | 1  1  0  1  CSS |
 * | 128 x 192 Color Graphics |   1  1  0    | 1  1  1  0  CSS |
 * -------------------------------------------------------------
 * | 256 x 192 Graphics       |   1  1  0    | 1  1  1  1  CSS |
 * -------------------------------------------------------------
 *
 * FF9A:       Border Register (All bits are 0 for CoCo 1 and CoCo 2 compatibility)
 *
 *        BIT 7   -
 *        BIT 6   -
 *        BIT 5 = RED1        Most significant red bit
 *        BIT 4 = GRN1        Most significant green bit
 *        BIT 3 = BLU1        Most significant blue bit
 *        BIT 2 = RED0        Least significant red bit
 *        BIT 1 = GRN0        Least significant green bit
 *        BIT 0 = BLU0        Least significant blue bit
 *
 * FF9B:       Reserved
 *
 * FF9C:       Vertical Scroll Register
 *
 *        BIT 7   -
 *        BIT 6   -
 *        BIT 5   -
 *        BIT 4   -
 *        BIT 3 = VSC3        (Vert. Scroll)
 *        BIT 2 = VSC2
 *        BIT 1 = VSC1
 *        BIT 0 = VSC0
 *
 *        NOTE: In the CoCo mode, the VSC's must be initialized to 0F hex.
 *
 * FF9D:       Vertical Offset 1 Register
 *
 *        BIT 7 = Y18         (Vert. Offset)
 *        BIT 6 = Y17
 *        BIT 5 = Y16
 *        BIT 4 = Y15
 *        BIT 3 = Y14
 *        BIT 2 = Y13
 *        BIT 1 = Y12
 *        BIT 0 = Y11
 *
 * FF9E:       Vertical Offset 0 Register
 *
 *        BIT 7 = Y10         (Vert. Offset)
 *        BIT 6 = Y9
 *        BIT 5 = Y8
 *        BIT 4 = Y7
 *        BIT 3 = Y6
 *        BIT 2 = Y5
 *        BIT 1 = Y4
 *        BIT 0 = Y3
 *
 *        NOTE: In CoCo mode, Y15 - Y9 are not effective, and are controlled by SAM bits F6 - F0.
 *              Also in CoCo mode, Y18 - Y16 should be 1, all others 0.
 *
 * -------
 * |
 * | Vertical Offset Registers 0 & 1
 * |
 * |     The starting address of a buffer area is indicated to the ACVC via these registers. This is
 * | done by writing the upper sixteen bits (Y18-Y3) of the starting physical address into these
 * | registers. Y18-Y11 are written to vertical offset 1 and Y10-Y3 are written to vertical offset
 * | 0. Thus, a buffer is limited to starting on an 8-bit boundary, or the starting address is
 * | limited to the binary form: xxx xxxx xxxx xxxx x000
 * |
 * - From Assembly Language Programming For The COCO 3 by Laurence A. Tepolt
 *
 * FF9F:       Horizontal Offset 0 Register
 *
 *        BIT 7 = HVEN        Horizontal Virtual Enable
 *        BIT 6 = X6          Horizontal Offset address
 *        BIT 5 = X5          Horizontal Offset address
 *        BIT 4 = X4          Horizontal Offset address
 *        BIT 3 = X3          Horizontal Offset address
 *        BIT 2 = X2          Horizontal Offset address
 *        BIT 1 = X1          Horizontal Offset address
 *        BIT 0 = X0          Horizontal Offset address
 *
 *        NOTE: HVEN enables a horizontal screen width of 128 bytes regardless of the HRES bits and
 *              CRES bits selected. This will allow a "virtual" screen somewhat larger than the
 *              displayed screen. The user can move the "window" (the displayed screen) by means of
 *              the horizontal offset bits. In character mode, the screen width is 128 characters
 *              regardless of attribute (or 64, if double-wide is selected).
 */

#ifndef VIDEOGRAPHICS_H
#define VIDEOGRAPHICS_H

#include <vector>

#include <Godot.hpp>
#include <Reference.hpp>
#include <StreamPeerBuffer.hpp>
#include <PoolArrays.hpp>
#include <Ref.hpp>

#include "Memory.hpp"
#include "Mc68b09e.hpp"
#include "Tcc1014.hpp"
#include "InitializationRegister0.hpp"
#include "VideoModeRegister.hpp"
#include "VideoResolutionRegister.hpp"
#include "VerticalScrollRegister.hpp"
#include "VerticalOffset.hpp"
#include "HorizontalOffsetRegister.hpp"
#include "ColorPaletteData.hpp"
#include "SAMControlRegisters.hpp"
#include "VDGControl.hpp"

namespace godot
{
	class VideoGraphics : public Reference
	{
		GODOT_CLASS(VideoGraphics, Reference)

	private:
	public:
		/**
		 * We're going to pretend we have optimal conditions and run at 60 fps, even if CoCo3 didn't run
		 * at exactly 60 fps.
		 */
		static const float constexpr FRAMERATE = 60.0;
		// NTSC Color Burst in Hertz
		static const float constexpr COLOR_BURST = (315.0 / 88.0);
		// material suggests 0.89 MHz, but a more accurate reprsentation comes from the technical specs
		static const float constexpr CYCLES_PER_SECOND_089 = (COLOR_BURST / 4) * 1000000;
		static const float constexpr CYCLES_PER_SECOND_178 = CYCLES_PER_SECOND_089 * 2;
		static const float constexpr CYCLES_PER_FRAME_089 = CYCLES_PER_SECOND_089 / FRAMERATE;
		static const float constexpr CYCLES_PER_FRAME_178 = CYCLES_PER_SECOND_178 / FRAMERATE;
		// NTSC ideal lines per screen
		static const float constexpr LINES_PER_SCREEN = 262.0;
		static const float constexpr CYCLES_PER_LINE_089 = CYCLES_PER_FRAME_089 / LINES_PER_SCREEN;
		static const float constexpr CYCLES_PER_LINE_178 = CYCLES_PER_FRAME_178 / LINES_PER_SCREEN;

		static const int TARGET_BYTE_PER_PIXEL = 4;

		Ref<Memory> memory;
		Ref<Mc68b09e> processor;
		Ref<Tcc1014> tcc1014;

		Ref<ColorPaletteData> color_palette_data;

		Ref<StreamPeerBuffer> target_screen_buffer;

		float cpu_drift = 0.0;

		// C++ Specific Definitions

		VideoGraphics();

		~VideoGraphics();

		// Godot Specific Definitions

		static void _register_methods();

		void _init(); // our initializer called by Godot

		void _on_initialization_register_0_coco_compatibility_enabled_toggled(bool new_value);

		void _on_video_resolution_register_set(std::int64_t new_value);

		void _on_video_mode_register_set(bool new_value);

		void _on_vdg_control_register_set(std::int64_t new_value);

		void _on_sam_control_registers_display_mode_control_set(std::int64_t new_value);

		// Class Definitions

		Ref<Memory> get_memory();
		Ref<Mc68b09e> get_processor();
		Ref<InitializationRegister0> get_initialization_register_0();
		Ref<VideoModeRegister> get_video_mode_register();
		Ref<VideoResolutionRegister> get_video_resolution_register();
		Ref<VerticalScrollRegister> get_vertical_scroll_register();
		Ref<VerticalOffset> get_vertical_offset_registers();
		Ref<HorizontalOffsetRegister> get_horizontal_offset_register();
		Ref<Monitor> get_monitor();
		Ref<SAMControlRegisters> get_sam_control_registers();
		Ref<VDGControl> get_vdg_control_register();
		Ref<ColorPaletteData> get_color_palette_data();

		void set_tcc1014(Ref<Tcc1014> new_value);

		int get_horizontal_pixels();
		int get_vertical_pixels();
		int get_colors();

		int get_character_columns();
		int get_lines_per_row();

		int get_vertical_border();
		int get_horizontal_border();

		int get_buffer_width();

		void update_graphics_mode();

		void render_frame();

		PoolByteArray get_image_data();

		void scanline();
	};
}

#endif