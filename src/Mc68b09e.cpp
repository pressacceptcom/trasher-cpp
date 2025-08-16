#include "Mc68b09e.hpp"

// from VCC
#define OverflowTest_8(carry, post_byte, temporary, reg) carry ^ (((post_byte ^ temporary ^ reg) >> 7) & 1)
#define OverflowTest_16(carry, post_byte, temporary, reg) carry ^ (((post_byte ^ temporary ^ reg) >> 15) & 1)

using namespace godot;

// C++ Specific

Mc68b09e::Mc68b09e()
{
}

Mc68b09e::~Mc68b09e()
{
}

// Godot Specific

void Mc68b09e::_register_methods()
{
	// Methods
	register_method("get_pins_state", &Mc68b09e::get_pins_state);
	register_method("get_condition_code_register", &Mc68b09e::get_condition_code_register);
	register_method("reset", &Mc68b09e::reset);
	register_method("latch_nmi", &Mc68b09e::latch_nmi);
	register_method("latch_irq", &Mc68b09e::latch_irq);
	register_method("latch_firq", &Mc68b09e::latch_firq);
	register_method("execute", &Mc68b09e::execute);

	// Properties
	register_property<Mc68b09e, std::uint8_t>("direct_page_register", &Mc68b09e::set_direct_page_register, &Mc68b09e::get_direct_page_register, 0);
	register_property<Mc68b09e, std::uint8_t>("a_accumulator", &Mc68b09e::set_a_accumulator, &Mc68b09e::get_a_accumulator, 0);
	register_property<Mc68b09e, std::uint8_t>("b_accumulator", &Mc68b09e::set_b_accumulator, &Mc68b09e::get_b_accumulator, 0);
	register_property<Mc68b09e, std::uint16_t>("d_accumulator", &Mc68b09e::set_d_accumulator, &Mc68b09e::get_d_accumulator, 0);
	register_property<Mc68b09e, bool>("cwai", &Mc68b09e::set_cwai, &Mc68b09e::get_cwai, 0);
	register_property<Mc68b09e, bool>("syncing", &Mc68b09e::set_syncing, &Mc68b09e::get_syncing, 0);
}

void Mc68b09e::_init()
{
	// initialize any variables here
	pins_state = Ref<PinsState>(PinsState::_new());
	condition_code_register = Ref<ConditionCodeRegister>(ConditionCodeRegister::_new());

	condition_code_register->set_irq_mask(true);
	condition_code_register->set_firq_mask(true);
}

// Class Specific

/**
 * NEG    Negate
 * Operation: M' <- 0 - M (i.e. M' <- ~M + 1)
 * Condition Codes:
 *     H: Undefined
 *     N: Set if bit 7 of result is set
 *     Z: Set if all bits of result are Clear
 *     V: Set if the original operand was 10000000
 *     C: Set if the operation did not cause a cary from bit 7 in the ALU.
 * Description:
 *     Replaces the operand with its two's complement. The C-flag represents a borrow and is set
 *     inverse to the resulting binary carry. Not that 80 (16) is replaced by itself and only in
 *     this case is V Set. The value 00 (16) is also replaced by itself, and only in this case is C
 *     cleared.
 */
inline std::uint8_t Mc68b09e::negate(std::uint8_t byte)
{
	byte = (~byte) + 1;

	condition_code_register->set_negative(byte & 0x80);
	condition_code_register->set_zero(byte == 0);
	condition_code_register->set_overflow(byte == 0x80);
	condition_code_register->set_carry(byte > 0);

	return byte;
}

/**
 * COM    Complement
 * Operation: M' (R') <- 0 + ~M (~R)
 * Condition Codes:
 *     H: Not Affected
 *     N: Set if bit 7 of the result is Set
 *     Z: Set if all bits of the result are Clear
 *     V: Cleared
 *     C: Set
 * Description:
 *     Replaces the contents of M or ACCX with its one's complement (also called the logical
 *     complement). The carry flag is set for 6800 compatibility.
 * Comments:
 *     When operating on unsigned values, only BEQ and MBE branches can be expected to behave
 *     properly. When operating on two's complement values, all signed branches are available.
 */
inline std::uint8_t Mc68b09e::complement(std::uint8_t byte)
{
	byte = ~byte;

	condition_code_register->set_negative(byte & 0x80);
	condition_code_register->set_zero(byte == 0);
	condition_code_register->set_overflow(false);
	condition_code_register->set_carry(true);

	return byte;
}

/**
 * LSR    Logical shift right
 * Operation:
 *      _________________    _____
 * 0 -> | | | | | | | | | -> | C |
 *      -----------------    -----
 *      b7             b0
 *
 * Condition Codes:
 *     H: Not Affected
 *     N: Cleared
 *     Z: Set if all bits of the result are Clear
 *     V: Not Affected
 *     C: Loaded with bit 0 of the original operand
 * Description:
 *     Performs a logical shift right on the operand. Shifts a zero into bit 7 and bit 0 into the
 *     carry flag. The 6800 processor also affects the V flag.
 */
inline std::uint8_t Mc68b09e::logical_shift_right(std::uint8_t byte)
{
	condition_code_register->set_negative(false);
	condition_code_register->set_carry(byte & 0x01);

	byte >>= 1;

	condition_code_register->set_zero(byte == 0);

	return byte;
}

/**
 * ROR    Rotate right
 * Operation:
 *       _____
 *  -----| C |-----
 *  |    -----    |
 * _________________
 * | | | | | | | | |
 * -----------------
 * b7      ->     b0
 *
 * Condition Codes:
 *     H: Not Affected
 *     N: Set if bit 7 of the result is Set
 *     Z: Set if all bits of the result are Clear
 *     V: Not Affected
 *     C: Loaded with bit 0 of the original operand
 * Description:
 *     Rotates all bits of the operand right one place through the carry flag; this is a nine-bit
 *     rotation. The 6800 processor also affects the V flag.
 */
inline std::uint8_t Mc68b09e::rotate_right(std::uint8_t byte)
{
	bool temp_carry = byte & 0x01;
	byte >>= 1;

	if (condition_code_register->bits.carry)
	{
		byte |= 0x80;
	}

	condition_code_register->set_carry(temp_carry);
	condition_code_register->set_negative(byte & 0x80);
	condition_code_register->set_zero(byte == 0);

	return byte;
}

/**
 * ASR    Arithmetic shift right
 * Operation:
 * _____
 * |   |
 * |  _________________    _____
 * -->| | | | | | | | | -> | C |
 *    -----------------    -----
 *    b7             b0
 *
 * Condition Codes:
 *     H: Undefined
 *     N: Set if bit 7 of the result is set
 *     Z: Set if all bits of result are clear
 *     V: Not affected
 *     C: Loaded with bit 0 of the original operand
 * Description:
 *     Shifts all bits of the operand right one place. Bit 7 is held constant. Bit 0 is shifted
 *     into the carry flag. The 6800/01/02/03/08 processors do affect the V flag.
 */
inline std::uint8_t Mc68b09e::arithmetic_shift_right(std::uint8_t byte)
{
	condition_code_register->set_carry(byte & 0x01);

	byte = (byte & 0x80) | (byte >> 1);

	condition_code_register->set_negative(byte & 0x80);
	condition_code_register->set_zero(byte == 0);

	return byte;
}

/**
 * ROL    Rotate left
 * Operation:
 *       _____
 *  -----| C |-----
 *  |    -----    |
 * _________________
 * | | | | | | | | |
 * -----------------
 * b7      <-     b0
 *
 * Condition Codes:
 *     H: Not Affected
 *     N: Set if bit 7 of the result is Set
 *     Z: Set if all bits of the result are Clear
 *     V: Loaded with the result of (b7 ^ b6) of the original operand.
 *     C: Loaded with bit 7 of the original operand
 * Description:
 *     Rotate all bits of the operand one place left through the carry flag; this is a 9-bit
 *     rotation.
 */
inline std::uint8_t Mc68b09e::rotate_left(std::uint8_t byte)
{
	bool temp_carry = byte & 0x80;

	condition_code_register->set_overflow(((byte & 0x80) >> 1) ^ (byte & 0x40));

	byte <<= 1;

	if (condition_code_register->bits.carry)
	{
		byte |= 0x01;
	}

	condition_code_register->set_carry(temp_carry);
	condition_code_register->set_negative(byte & 0x80);
	condition_code_register->set_zero(byte == 0);

	return byte;
}

/**
 * ASL    Arithmetic shift left
 * Operation:
 * _____    _________________
 * | C | <- | | | | | | | | | <- 0
 * -----    -----------------
 *          b7      <-     b0
 *
 * Condition Codes:
 *     H: Undefined
 *     N: Set if bit 7 of the result is set
 *     Z: Set if all bits of the result are clear
 *     V: Loaded with the result of b7 ^ b0 of the original operand.
 *     C: Loaded with bit 7 of the original operand.
 * Description:
 *     Shifts all bits of the operand one place to the left. Bit 0 is loaded with a zero. Bit 7 of
 *     the operand is shifted into the carry flag.
 */
inline std::uint8_t Mc68b09e::arithmetic_shift_left(std::uint8_t byte)
{
	condition_code_register->set_carry(byte & 0x80);
	condition_code_register->set_overflow(((byte & 0x80) >> 1) ^ (byte & 0x40));

	byte <<= 1;

	condition_code_register->set_negative(byte & 0x80);
	condition_code_register->set_zero(byte == 0);

	return byte;
}

/**
 * DEC    Decrement
 * Operation: M' (R') <- M-1 (R-1)
 * Condition Codes:
 *     H: Not Affected
 *     N: Set if bit 7 of result is Set
 *     Z: Set if all bits of result are Clear
 *     V: Set if the original operand was 10000000
 *        From Assembly Language Programing for the Color Computer by Laurence Tepolt:
 *            Set if an overflow or underflow occurred; cleared otherwise. This applies only to
 *            signed binary.
 *     C: Not affected
 * Description:
 *     Subtract one from the operand. The carry flag is not affected, thus allowing DEC to be a
 *     loopcounter in multiple-precision computations.
 * Comments:
 *     When operating on unsigned values only BEQ and BNE branches can be expected to behave
 *     consistently. When operating on two's complement values, all signed branches are available.
 */
inline std::uint8_t Mc68b09e::decrement(std::uint8_t byte)
{
	condition_code_register->set_overflow(byte == 0x80);

	byte--;

	condition_code_register->set_negative(byte & 0x80);
	condition_code_register->set_zero(byte == 0);

	return byte;
}

/**
 * INC    Increment
 * Operation: M' (R') <- M + 1 (R + 1)
 * Condition Codes:
 *     H: Not Affected
 *     N: Set if bit 7 of the result is Set
 *     Z: Set if all bits of the result are Clear
 *     V: Set if the original operand was 01111111
 *     C: Not Affected
 * Description:
 *     Add one to the operand. The carry flag is not affected, thus allowing INC to be used as a
 *     loop-counter in multiple-precision computations.
 * Comments:
 *     When operating on unsigned values, only the BEQ and BNE branches can be expected to behave
 *     consistently. When operating on two's complement values, all signed branches are correctly
 *     available.
 */
inline std::uint8_t Mc68b09e::increment(std::uint8_t byte)
{
	condition_code_register->set_overflow(byte == 0x7F);

	byte++;

	condition_code_register->set_negative(byte & 0x80);
	condition_code_register->set_zero(byte == 0);

	return byte;
}

/**
 * TST    Test
 * Operation: TEMP <- M - 0
 * Condition Codes:
 *     H: Not Affected
 *     N: Set if bit 7 of the result is Set
 *     Z: Set if all bits of the result are Clear
 *     V: Cleared
 *     C: Not Affected
 * Description:
 *     Set condition code flags N and Z according to the contents of M, and clear the V flag. The
 *     6800 processor clears the C flag.
 * Comments:
 *     The TST instruction provides only minimum information when testing unsigned values; since no
 *     unsigned value is less than zero, BLO and BLS have no utility. While BHI could be used after
 *     TST, it provides exactly the same control as BNE, which is preferred. The signed branches
 *     are available.
 */
inline void Mc68b09e::test(std::uint8_t byte)
{
	condition_code_register->set_negative(byte & 0x80);
	condition_code_register->set_zero(byte == 0);
	condition_code_register->set_overflow(false);
}

/**
 * CLR    Clear
 * Operation: TEMP <- M
 *            M <- 00 (16)
 * Condition CodeS:
 *     H: Not Affected
 *     N: Cleared
 *     Z: Set
 *     V: Cleared
 *     C: Cleared
 * Description:
 *     ACCX or M is loaded with 00000000. The C-flag is cleared for 6800 compatibility.
 */
inline void Mc68b09e::clear()
{
	condition_code_register->set_negative(false);
	condition_code_register->set_zero(true);
	condition_code_register->set_overflow(false);
	condition_code_register->set_carry(false);
}

/**
 * SUB    Subtract memory from register
 * Operation: R' <- R - M
 * Condition Codes:
 *     H: Undefined
 *     N: Set if bit 7 (15) of the result is Set
 *     Z: Set if all bits of the result are Clear
 *     V: Set if the operation caused an 8 (16)-bit two's complement overflow
 *     C: Set if the operation did not cause a carry from bit 7 in the ALU
 * Description:
 *     Subtracts the value in M from the contents of an 8 (16)-bit register. The C flag represents
 *     a borrow and is set inverse to the resulting carry.
 */
inline std::uint16_t Mc68b09e::subtract(std::uint16_t minuend, std::uint16_t subtrahend)
{
	std::uint16_t difference = minuend - subtrahend;

	condition_code_register->set_negative(difference & 0x8000);
	condition_code_register->set_zero(difference == 0);
	condition_code_register->set_carry(subtrahend > minuend);
	condition_code_register->set_overflow(
		OverflowTest_16(
			condition_code_register->bits.carry,
			subtrahend,
			difference,
			minuend));

	return difference;
}

/**
 * SUB    Subtract memory from register
 * Operation: R' <- R - M
 * Condition Codes:
 *     H: Undefined
 *     N: Set if bit 7 (15) of the result is Set
 *     Z: Set if all bits of the result are Clear
 *     V: Set if the operation caused an 8 (16)-bit two's complement overflow
 *     C: Set if the operation did not cause a carry from bit 7 in the ALU
 * Description:
 *     Subtracts the value in M from the contents of an 8 (16)-bit register. The C flag represents
 *     a borrow and is set inverse to the resulting carry.
 */
inline std::uint8_t Mc68b09e::subtract(std::uint8_t minuend, std::uint8_t subtrahend)
{
	std::uint8_t difference = minuend - subtrahend;

	condition_code_register->set_negative(difference & 0x80);
	condition_code_register->set_zero(difference == 0);
	condition_code_register->set_carry(subtrahend > minuend);
	condition_code_register->set_overflow(
		OverflowTest_8(
			condition_code_register->bits.carry,
			subtrahend,
			difference,
			minuend));

	return difference;
}

/**
 * ADD    Add memory into register
 * Operation: R' <- R + M
 * Condition Codes:
 *     H: Set if the operation caused a carry from bit 3 in the ALU.
 *     N: Set if bit 7 (15) of the result is set.
 *     Z: Set if all bits of the result are clear.
 *     V: Set if the operation caused an 8 (16) -bit two's complement arithmetic
 *        overflow.
 *     C: Set if the operation caused a carry from bit 7 (15) in the ALU
 * Description:
 *     Adds the memory byte into an 8 (16) - bit register
 */
inline std::uint16_t Mc68b09e::add(std::uint16_t augend, std::uint16_t addend)
{
	std::uint32_t sum = augend + addend;

	// half carry from VCC
	condition_code_register->set_half_carry(((augend ^ addend ^ sum) & 0x10));
	condition_code_register->set_negative(sum & 0x8000);
	condition_code_register->set_zero(sum == 0);
	condition_code_register->set_carry(sum & 0x10000);
	condition_code_register->set_overflow(
		OverflowTest_16(
			condition_code_register->bits.carry,
			addend,
			sum,
			augend));

	return static_cast<std::uint16_t>(sum);
}

/**
 * ADD    Add memory into register
 * Operation: R' <- R + M
 * Condition Codes:
 *     H: Set if the operation caused a carry from bit 3 in the ALU.
 *     N: Set if bit 7 (15) of the result is set.
 *     Z: Set if all bits of the result are clear.
 *     V: Set if the operation caused an 8 (16) -bit two's complement arithmetic
 *        overflow.
 *     C: Set if the operation caused a carry from bit 7 (15) in the ALU
 * Description:
 *     Adds the memory byte into an 8 (16) - bit register
 */
inline std::uint8_t Mc68b09e::add(std::uint8_t augend, std::uint8_t addend)
{
	std::uint16_t sum = augend + addend;

	// half carry from VCC
	condition_code_register->set_half_carry(((augend ^ addend ^ sum) & 0x10));
	condition_code_register->set_negative(sum & 0x80);
	condition_code_register->set_zero(sum == 0);
	condition_code_register->set_carry(sum & 0x100);
	condition_code_register->set_overflow(
		OverflowTest_8(
			condition_code_register->bits.carry,
			addend,
			sum,
			augend));

	return static_cast<std::uint8_t>(sum);
}

/**
 * SP' <- SP - 1, (SP) <- Byte
 */
inline void Mc68b09e::push_onto_hardware_stack(std::uint8_t byte)
{
	hardware_stack_pointer_register--;
	write(hardware_stack_pointer_register, byte);
}

/**
 * ret <- (SP), SP' <- SP + 1
 */
inline std::uint8_t Mc68b09e::pull_from_hardware_stack()
{
	std::uint8_t ret = read(hardware_stack_pointer_register);
	hardware_stack_pointer_register++;
	return ret;
}

/**
 * UP' <- UP - 1, (UP) <- Byte
 */
inline void Mc68b09e::push_onto_user_stack(std::uint8_t byte)
{
	user_stack_pointer_register--;
	write(user_stack_pointer_register, byte);
}

/**
 * ret <- (UP), UP' <- UP + 1
 */
inline std::uint8_t Mc68b09e::pull_from_user_stack()
{
	std::uint8_t ret = read(user_stack_pointer_register);
	user_stack_pointer_register++;
	return ret;
}

/**
 * SP' <- SP - 1, (SP) <- PCL
 * SP' <- SP - 1, (SP) <- PCH
 */
inline void Mc68b09e::push_program_counter_register()
{
	push_onto_hardware_stack(static_cast<std::uint8_t>(program_counter_register));
	push_onto_hardware_stack(static_cast<std::uint8_t>(program_counter_register >> 8));
}

/**
 * SP' <- SP - 1, (SP) <- PCL
 * SP' <- SP - 1, (SP) <- PCH
 * SP' <- SP - 1, (SP) <- USL
 * SP' <- SP - 1, (SP) <- USH
 * SP' <- SP - 1, (SP) <- IYL
 * SP' <- SP - 1, (SP) <- IYH
 * SP' <- SP - 1, (SP) <- IXL
 * SP' <- SP - 1, (SP) <- IXH
 * SP' <- SP - 1, (SP) <- DPR
 * SP' <- SP - 1, (SP) <- ACCA
 * SP' <- SP - 1, (SP) <- ACCB
 * SP' <- SP - 1, (SP) <- CCR
 */
inline void Mc68b09e::push_machine_state()
{
	push_program_counter_register();

	push_onto_hardware_stack(static_cast<std::uint8_t>(user_stack_pointer_register));
	push_onto_hardware_stack(static_cast<std::uint8_t>(user_stack_pointer_register >> 8));

	push_onto_hardware_stack(static_cast<std::uint8_t>(y_index_register));
	push_onto_hardware_stack(static_cast<std::uint8_t>(y_index_register >> 8));

	push_onto_hardware_stack(static_cast<std::uint8_t>(x_index_register));
	push_onto_hardware_stack(static_cast<std::uint8_t>(x_index_register >> 8));

	push_onto_hardware_stack(direct_page_register.container.reg);

	push_onto_hardware_stack(accumulator_register.registers.b);
	push_onto_hardware_stack(accumulator_register.registers.a);

	push_onto_hardware_stack(condition_code_register->slot);
}

/**
 * NOTE: Does NOT pull CCR, nor PC
 *
 * ACCA <- (SP), SP' <- SP + 1
 * ACCB <- (SP), SP' <- SP + 1
 * DPR <- (SP), SP' <- SP + 1
 * IXH <- (SP), SP' <- SP + 1
 * IXL <- (SP), SP' <- SP + 1
 * IYH <- (SP), SP' <- SP + 1
 * IYL <- (SP), SP' <- SP + 1
 * USH <- (SP), SP' <- SP + 1
 * USL <- (SP), SP' <- SP + 1
 */
inline void Mc68b09e::pull_machine_state()
{
	accumulator_register.registers.a = pull_from_hardware_stack();
	accumulator_register.registers.b = pull_from_hardware_stack();

	direct_page_register.container.reg = pull_from_hardware_stack();

	x_index_register = pull_from_hardware_stack();
	x_index_register <<= 8;
	x_index_register |= pull_from_hardware_stack();

	y_index_register = pull_from_hardware_stack();
	y_index_register <<= 8;
	y_index_register |= pull_from_hardware_stack();

	user_stack_pointer_register = pull_from_hardware_stack();
	user_stack_pointer_register <<= 8;
	user_stack_pointer_register |= pull_from_hardware_stack();
}

/**
 * CMP    Compare memory from a register
 * Operation: TEMP <- R - M(:M+1)
 * Condition Codes:
 *     H: Undefined
 *     N: Set if bit 7 (15) of the result is Set.
 *     Z: Set if all bits of the result are Clear
 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
 * Description:
 *     Compares the contents of M from teh contents of the specified register and sets appropriate
 *     condition codes. Neither M nor R is modified. The C flag represents a borrow and is set
 *     inverse to the resulting binary carry.
 */
inline void Mc68b09e::compare(std::uint16_t slot, std::uint16_t post)
{
	std::uint16_t difference = slot - post;

	condition_code_register->set_negative(difference & 0x8000);
	condition_code_register->set_zero(difference == 0);
	condition_code_register->set_carry(post > slot);
	condition_code_register->set_overflow(
		OverflowTest_16(
			condition_code_register->bits.carry,
			post,
			difference,
			slot));
}

/**
 * CMP    Compare memory from a register
 * Operation: TEMP <- R - M(:M+1)
 * Condition Codes:
 *     H: Undefined
 *     N: Set if bit 7 (15) of the result is Set.
 *     Z: Set if all bits of the result are Clear
 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
 * Description:
 *     Compares the contents of M from teh contents of the specified register and sets appropriate
 *     condition codes. Neither M nor R is modified. The C flag represents a borrow and is set
 *     inverse to the resulting binary carry.
 */
inline void Mc68b09e::compare(std::uint8_t slot, std::uint8_t post)
{
	std::uint8_t difference = slot - post;

	condition_code_register->set_negative(difference & 0x80);
	condition_code_register->set_zero(difference == 0);
	condition_code_register->set_carry(post > slot);
	condition_code_register->set_overflow(
		OverflowTest_8(
			condition_code_register->bits.carry,
			post,
			difference,
			slot));
}

/**
 * ST    Store register into memory
 * Operation: M'(:M+1') <- R
 * Condition Codes:
 *     H: Not Affected
 *     N: Set if bit 7 (15) of stored data was Set
 *     Z: Set if all bits of stored data are Clear
 *     V: Cleared
 *     C: Not Affected
 * Description:
 *     Writes the contents of an MPU register into a memory location.
 */
inline void Mc68b09e::store_register(std::uint16_t address, std::uint16_t data)
{
	write2(address, data);
	condition_register(data);
}

/**
 * ST    Store register into memory
 * Operation: M'(:M+1') <- R
 * Condition Codes:
 *     H: Not Affected
 *     N: Set if bit 7 (15) of stored data was Set
 *     Z: Set if all bits of stored data are Clear
 *     V: Cleared
 *     C: Not Affected
 * Description:
 *     Writes the contents of an MPU register into a memory location.
 */
inline void Mc68b09e::store_register(std::uint16_t address, std::uint8_t data)
{
	write(address, data);
	condition_register(data);
}

/**
 * Set the condition code register based on the contents of an 16-bit register.
 */
inline void Mc68b09e::condition_register(std::uint16_t data)
{
	condition_code_register->set_negative(data & 0x8000);
	condition_code_register->set_zero(data == 0);
	condition_code_register->set_overflow(false);
}

/**
 * Set the condition code register based on the contents of an 8-bit register.
 */
inline void Mc68b09e::condition_register(std::uint8_t data)
{
	condition_code_register->set_negative(data & 0x80);
	condition_code_register->set_zero(data == 0);
	condition_code_register->set_overflow(false);
}

/**
 * NOTE: Does not check sizes
 *
 * EXG    Exchange registers
 * Operation: R1 <-> R2
 * Condition Codes: Not Affected (unless one of the registres is CCR)
 * Description:
 *     Bits 3-0 of the immediate byte of the instruction define one register, while bits 7-4 define
 *     the other, as follows:
 *
 *     0000 = A:B          1000 = A
 *     0001 = X            1001 = B
 *     0010 = Y            1010 = CCR
 *     0011 = US           1011 = DPR
 *     0100 = SP           1100 = Undefined
 *     0101 = PC           1101 = Undefined
 *     0110 = Undefined    1110 = Undefined
 *     0111 = Undefined    1111 = Undefined
 *
 *     Registers may only be exchanged with registers of like size; i.e., 8-bit with 8-bit, or 16
 *     with 16.
 */
inline void Mc68b09e::exchange_registers(TransferExchangePostByte source, TransferExchangePostByte destination)
{
	std::uint16_t temp = read_from_register(destination);
	write_to_register(destination, read_from_register(source));
	write_to_register(source, temp);
}

inline std::uint16_t Mc68b09e::read_from_register(TransferExchangePostByte source)
{
	switch (source)
	{
	case TransferExchangePostByte::D_REGISTER:
		return accumulator_register.d;

	case TransferExchangePostByte::X_INDEX_REGISTER:
		return x_index_register;

	case TransferExchangePostByte::Y_INDEX_REGISTER:
		return y_index_register;

	case TransferExchangePostByte::USER_STACK_POINTER_REGISTER:
		return user_stack_pointer_register;

	case TransferExchangePostByte::HARDWARE_STACK_POINTER_REGISTER:
		return hardware_stack_pointer_register;

	case TransferExchangePostByte::PROGRAM_COUNTER_REGISTER:
		return program_counter_register;

	case TransferExchangePostByte::A_ACCUMULATOR:
		return static_cast<std::uint16_t>(accumulator_register.registers.a);

	case TransferExchangePostByte::B_ACCUMULATOR:
		return static_cast<std::uint16_t>(accumulator_register.registers.b);

	case TransferExchangePostByte::CONDITION_CODE_REGISTER:
		return static_cast<std::uint16_t>(condition_code_register->slot);

	case TransferExchangePostByte::DIRECT_PAGE_REGISTER:
		return static_cast<std::uint16_t>(direct_page_register.container.reg);
	}

	return 0;
}

inline void Mc68b09e::write_to_register(TransferExchangePostByte destination, std::uint16_t data)
{
	switch (destination)
	{
	case TransferExchangePostByte::D_REGISTER:
		accumulator_register.d = data;
		break;

	case TransferExchangePostByte::X_INDEX_REGISTER:
		x_index_register = data;
		break;

	case TransferExchangePostByte::Y_INDEX_REGISTER:
		y_index_register = data;
		break;

	case TransferExchangePostByte::USER_STACK_POINTER_REGISTER:
		user_stack_pointer_register = data;
		break;

	case TransferExchangePostByte::HARDWARE_STACK_POINTER_REGISTER:
		hardware_stack_pointer_register = data;
		break;

	case TransferExchangePostByte::PROGRAM_COUNTER_REGISTER:
		program_counter_register = data;
		break;

	case TransferExchangePostByte::A_ACCUMULATOR:
		accumulator_register.registers.a = static_cast<std::uint8_t>(data);
		break;

	case TransferExchangePostByte::B_ACCUMULATOR:
		accumulator_register.registers.b = static_cast<std::uint8_t>(data);
		break;

	case TransferExchangePostByte::CONDITION_CODE_REGISTER:
		condition_code_register->set_register(static_cast<std::uint8_t>(data));
		break;

	case TransferExchangePostByte::DIRECT_PAGE_REGISTER:
		direct_page_register.container.reg = static_cast<std::uint8_t>(data);
		break;
	}
}

inline std::uint8_t Mc68b09e::read(std::uint16_t address)
{
	return tcc1014->get_byte_at(address);
}

inline std::uint16_t Mc68b09e::read2(std::uint16_t address)
{
	return tcc1014->get_bytes_at(address);
}

/**
 *  Read one byte at PC and advance PC
 */
inline std::uint8_t Mc68b09e::read_and_advance_program_counter()
{
	std::uint8_t ret = read(program_counter_register);
	program_counter_register++;
	return ret;
}

/**
 *  Reads two bytes at PC and advance PC
 */
inline std::uint16_t Mc68b09e::read2_and_advance_program_counter()
{
	std::uint16_t ret = read2(program_counter_register);
	program_counter_register += 2;
	return ret;
}

inline void Mc68b09e::write(std::uint16_t address, std::uint8_t byte)
{
	tcc1014->set_byte_at(address, byte);
}

inline void Mc68b09e::write2(std::uint16_t address, std::uint16_t bytes)
{
	tcc1014->set_bytes_at(address, bytes);
}

/**
 * Offset the program counter by a twos-complement signed byte
 */
inline void Mc68b09e::branch_relative_offset()
{
	std::int8_t post = static_cast<std::int8_t>(read(program_counter_register));
	program_counter_register += post;
}

/**
 * Offset the program counter by a twos-complement signed word (2 bytes)
 */
inline void Mc68b09e::long_branch_relative_offset()
{
	std::int16_t post = static_cast<std::int8_t>(read2(program_counter_register));
	program_counter_register += post;
}

/**
 * NOTE: Advances program counter
 *
 * Compute the direct address by concatenating the DPR with the value at PC
 */
inline std::uint16_t Mc68b09e::retrieve_direct_address()
{
	return direct_page_register.value | read_and_advance_program_counter();
}

/**
 * NOTE: Advances PC
 *
 * Calculate the effective address indicated by the byte read from the program counter in accordance
 * with these charts:
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
 * SEE INDEXED ADDRESSING section at beginning of file (taken from MC68B09E Catalog Entry)
 */
inline std::uint16_t Mc68b09e::retrieve_effective_address()
{
	std::int8_t offset_8;
	std::int16_t offset_16;

	IndexedAddressingPostByte post;
	post.byte = read_and_advance_program_counter();

	if (post.five_bit_offset.is_offset)
	{

		std::int8_t offset = post.five_bit_offset.five_bit_offset;
		offset = (offset << 3) / 8;
		cycle_counter++;
		return retrieve_register((RegisterField)post.five_bit_offset.register_field) + offset;
	}
	else
	{
		if (post.index.indirect_field)
		{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"

			// INDIRECT
			switch (static_cast<AddressingMode>(post.index.addressing_mode_field))
			{

			case AddressingMode::INCREMENT_BY_2:
				cycle_counter += 6;
				return read2(retrieve_and_increment_register((RegisterField)post.index.register_field, 2));

			case AddressingMode::DECREMENT_BY_2:
				cycle_counter += 6;
				return read2(retrieve_and_decrement_register((RegisterField)post.index.register_field, 2));

			case AddressingMode::NO_OFFSET:
				cycle_counter += 3;
				return read2(retrieve_register((RegisterField)post.index.register_field));

			case AddressingMode::B_ACCUMULATOR_OFFSET:
				cycle_counter += 4;
				offset_8 = static_cast<std::int8_t>(accumulator_register.registers.b);
				return read2(retrieve_register((RegisterField)post.index.register_field) + offset_8);

			case AddressingMode::A_ACCUMULATOR_OFFSET:
				cycle_counter += 4;
				offset_8 = static_cast<std::int8_t>(accumulator_register.registers.a);
				return read2(retrieve_register((RegisterField)post.index.register_field) + offset_8);

			case AddressingMode::EIGHT_BIT_OFFSET:
				cycle_counter += 4;
				offset_8 = static_cast<std::int8_t>(read_and_advance_program_counter());
				return read2(retrieve_register((RegisterField)post.index.register_field) + offset_8);

			case AddressingMode::SIXTEEN_BIT_OFFSET:
				cycle_counter += 7;
				offset_16 = static_cast<std::int16_t>(read2_and_advance_program_counter());
				return read2(retrieve_register((RegisterField)post.index.register_field) + offset_16);

			case AddressingMode::D_ACCUMULATOR_OFFSET:
				cycle_counter += 7;
				offset_16 = static_cast<std::int16_t>(accumulator_register.d);
				return read2(retrieve_register((RegisterField)post.index.register_field) + offset_16);

			case AddressingMode::CONSTANT_PROGRAM_COUNTER_8_BIT_OFFSET:
				cycle_counter += 4;
				offset_8 = static_cast<std::int8_t>(read_and_advance_program_counter());
				return read2(program_counter_register + offset_8);

			case AddressingMode::CONSTANT_PROGRAM_COUNTER_16_BIT_OFFSET:
				cycle_counter += 8;
				offset_16 = static_cast<std::int16_t>(read2_and_advance_program_counter());
				return read2(program_counter_register + offset_16);

			case AddressingMode::EXTENDED_INDIRECT:
				cycle_counter += 8;
				return read2(read2_and_advance_program_counter());
			}

#pragma GCC diagnostic pop
		}
		else
		{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"

			switch (static_cast<AddressingMode>(post.index.addressing_mode_field))
			{

			case AddressingMode::INCREMENT_BY_1:
				cycle_counter += 2;
				return retrieve_and_increment_register((RegisterField)post.index.register_field, 1);

			case AddressingMode::INCREMENT_BY_2:
				cycle_counter += 3;
				return retrieve_and_increment_register((RegisterField)post.index.register_field, 2);

			case AddressingMode::DECREMENT_BY_1:
				cycle_counter += 2;
				return retrieve_and_decrement_register((RegisterField)post.index.register_field, 1);

			case AddressingMode::DECREMENT_BY_2:
				cycle_counter += 3;
				return retrieve_and_decrement_register((RegisterField)post.index.register_field, 2);

			case AddressingMode::NO_OFFSET:
				return retrieve_register((RegisterField)post.index.register_field);

			case AddressingMode::B_ACCUMULATOR_OFFSET:
				cycle_counter += 1;
				offset_8 = static_cast<std::int8_t>(accumulator_register.registers.b);
				return retrieve_register((RegisterField)post.index.register_field) + offset_8;

			case AddressingMode::A_ACCUMULATOR_OFFSET:
				cycle_counter += 1;
				offset_8 = static_cast<std::int8_t>(accumulator_register.registers.a);
				return retrieve_register((RegisterField)post.index.register_field) + offset_8;

			case AddressingMode::EIGHT_BIT_OFFSET:
				cycle_counter += 1;
				offset_8 = static_cast<std::int8_t>(read_and_advance_program_counter());
				return retrieve_register((RegisterField)post.index.register_field) + offset_8;

			case AddressingMode::SIXTEEN_BIT_OFFSET:
				cycle_counter += 4;
				offset_16 = static_cast<std::int16_t>(read2_and_advance_program_counter());
				return retrieve_register((RegisterField)post.index.register_field) + offset_16;

			case AddressingMode::D_ACCUMULATOR_OFFSET:
				cycle_counter += 4;
				offset_16 = static_cast<std::int16_t>(accumulator_register.d);
				return retrieve_register((RegisterField)post.index.register_field) + offset_16;

			case AddressingMode::CONSTANT_PROGRAM_COUNTER_8_BIT_OFFSET:
				cycle_counter += 1;
				offset_8 = static_cast<std::int8_t>(read_and_advance_program_counter());
				return program_counter_register + offset_8;

			case AddressingMode::CONSTANT_PROGRAM_COUNTER_16_BIT_OFFSET:
				cycle_counter += 5;
				offset_16 = static_cast<std::int16_t>(read2_and_advance_program_counter());
				return program_counter_register + offset_16;
			}

#pragma GCC diagnostic pop
		}
	}

	return 0;
}

inline std::uint16_t Mc68b09e::retrieve_register(RegisterField field)
{
	switch (field)
	{

	case RegisterField::X_INDEX_REGISTER:
		return x_index_register;

	case RegisterField::Y_INDEX_REGISTER:
		return y_index_register;

	case RegisterField::USER_STACK_POINTER_REGISTER:
		return user_stack_pointer_register;

	case RegisterField::HARDWARE_STACK_POINTER_REGISTER:
		return hardware_stack_pointer_register;
	}

	return 0;
}

inline std::uint16_t Mc68b09e::retrieve_and_increment_register(RegisterField field, std::uint16_t amount)
{
	std::uint16_t ret;

	switch (field)
	{

	case RegisterField::X_INDEX_REGISTER:
		ret = x_index_register;
		x_index_register += amount;
		break;

	case RegisterField::Y_INDEX_REGISTER:
		ret = y_index_register;
		y_index_register += amount;
		break;

	case RegisterField::USER_STACK_POINTER_REGISTER:
		ret = user_stack_pointer_register;
		user_stack_pointer_register += amount;
		break;

	case RegisterField::HARDWARE_STACK_POINTER_REGISTER:
		ret = hardware_stack_pointer_register;
		hardware_stack_pointer_register += amount;
		break;
	}

	return ret;
}

inline std::uint16_t Mc68b09e::retrieve_and_decrement_register(RegisterField field, std::uint16_t amount)
{
	std::uint16_t ret;

	switch (field)
	{

	case RegisterField::X_INDEX_REGISTER:
		x_index_register -= amount;
		return x_index_register;

	case RegisterField::Y_INDEX_REGISTER:
		y_index_register -= amount;
		return y_index_register;

	case RegisterField::USER_STACK_POINTER_REGISTER:
		user_stack_pointer_register -= amount;
		return user_stack_pointer_register;

	case RegisterField::HARDWARE_STACK_POINTER_REGISTER:
		hardware_stack_pointer_register -= amount;
		return hardware_stack_pointer_register;
	}

	return 0;
}

Ref<PinsState> Mc68b09e::get_pins_state()
{
	return pins_state;
}

Ref<ConditionCodeRegister> Mc68b09e::get_condition_code_register()
{
	return condition_code_register;
}

void Mc68b09e::set_debug(bool new_value)
{
	condition_code_register->debug = new_value;
	pins_state->debug = new_value;
}

void Mc68b09e::reset()
{
	accumulator_register.d = 0;
	x_index_register = 0;
	y_index_register = 0;
	user_stack_pointer_register = 0;
	hardware_stack_pointer_register = 0;

	condition_code_register->set_register(0);
	condition_code_register->set_irq_mask(true);
	condition_code_register->set_firq_mask(true);
	direct_page_register.container.reg = 0;

	// Interrupt or RESET acknowledge
	pins_state->set_ba(false);
	pins_state->set_bs(false);

	syncing = false;
	cwai = false;

	program_counter_register = static_cast<std::uint16_t>(VectorTable::RESET);
}

void Mc68b09e::latch_nmi()
{
	pins_state->set_nmi(true);
}

void Mc68b09e::handle_nmi()
{
	if (!cwai)
	{
		condition_code_register->set_entire(true);

		push_machine_state();
	}

	condition_code_register->set_irq_mask(true);
	condition_code_register->set_firq_mask(true);

	// Interrupt or RESET acknowledge
	pins_state->set_ba(false);
	pins_state->set_bs(true);
	pins_state->set_nmi(false);

	program_counter_register = read2(static_cast<std::uint16_t>(VectorTable::NON_MASKABLE_INTERRUPT));
}

void Mc68b09e::latch_irq(bool delay)
{
	pins_state->delay_irq = delay;
	pins_state->set_irq(true);
}

void Mc68b09e::handle_irq()
{
	if (!condition_code_register->bits.irq_mask)
	{
		if (!cwai)
		{
			condition_code_register->set_entire(true);

			push_machine_state();
		}

		condition_code_register->set_irq_mask(true);

		// Interrupt or RESET acknowledge
		pins_state->set_ba(false);
		pins_state->set_bs(true);

		program_counter_register = read2(static_cast<std::uint16_t>(VectorTable::INTERRUPT_REQUEST));
	}

	pins_state->set_irq(false);
}

void Mc68b09e::latch_firq()
{
	pins_state->set_firq(true);
}

void Mc68b09e::handle_firq()
{
	if (!condition_code_register->bits.firq_mask)
	{
		if (!cwai)
		{
			condition_code_register->set_entire(false);

			push_program_counter_register();
			push_onto_hardware_stack(condition_code_register->slot);
		}

		condition_code_register->set_irq_mask(true);
		condition_code_register->set_firq_mask(true);

		// Interrupt or RESET acknowledge
		pins_state->set_ba(false);
		pins_state->set_bs(true);

		program_counter_register = read2(static_cast<std::uint16_t>(VectorTable::FAST_INTERRUPT_REQUEST));
	}

	pins_state->set_firq(false);
}

float Mc68b09e::execute(float num_cycles_to_run, std::int64_t num_instructions_to_execute)
{
	std::uint8_t opcode;
	std::uint16_t address;
	std::uint8_t byte;
	std::int8_t offset;
	std::uint64_t num_instructions = 0;

	// DAA
	std::uint8_t most_significant_nibble;
	std::uint8_t least_significant_nibble;
	std::uint16_t addition;

	// EXG
	std::uint8_t source;
	std::uint8_t destination;

	// PSH
	PushPullPostByte postbyte;

	float ret;

	if (pins_state->halt)
	{
		pins_state->set_ba(true);
		pins_state->set_bs(true);

		return 0.0;
	}

	pins_state->set_ba(false);
	pins_state->set_bs(false);

	while ((!num_instructions_to_execute && cycle_counter < num_cycles_to_run) || (num_instructions_to_execute && num_instructions < num_instructions_to_execute))
	{
		if (pins_state->nmi)
		{
			handle_nmi();
		}

		if (pins_state->firq)
		{
			handle_firq();
		}

		if (pins_state->irq && !pins_state->delay_irq)
		{
			handle_irq();
		}

		if (syncing)
		{
			return 0.0;
		}

		// opcode information taken from The MC6809 Cookbook by Carl D. Warren

		opcode = read_and_advance_program_counter();

		switch (static_cast<Opcodes>(opcode))
		{

		/**
		 * NEG    Negate
		 * Source Form: NEG Q
		 * Operation: M' <- 0 - M (i.e. M' <- ~M + 1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of result is set
		 *     Z: Set if all bits of result are Clear
		 *     V: Set if the original operand was 10000000
		 *     C: Set if the operation did not cause a cary from bit 7 in the ALU.
		 * Description:
		 *     Replaces the operand with its two's complement. The C-flag represents a borrow
		 *     and is set inverse to the resulting binary carry. Not that 80 (16) is replaced by
		 *     itself and only in this case is V Set. The value 00 (16) is also replaced by
		 *     itself, and only in this case is C cleared.
		 *
		 * NEG - Direct - Opcode: 00 - MPU Cycles: 6 - No of bytes: 2
		 */
		case Opcodes::NEG_DIRECT:
			cycle_counter += 6;
			address = retrieve_direct_address();
			write(address, negate(read(address)));
			break;

		/**
		 * COM    Complement
		 * Source Form: COM Q; COMA; COMB
		 * Operation: M' (R') <- 0 + ~M (~R)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Set
		 * Description:
		 *     Replaces the contents of M or ACCX with its one's complement (also called the
		 *     logical complement). The carry flag is set for 6800 compatibility.
		 * Comments:
		 *     When operating on unsigned values, only BEQ and MBE branches can be expected to
		 *     behave properly. When operating on two's complement values, all signed branches
		 *     are available.
		 *
		 * COM - Direct - Opcode: 03 - MPY Cycles: 6 - No of bytes: 2
		 */
		case Opcodes::COM_DIRECT:
			cycle_counter += 6;
			address = retrieve_direct_address();
			write(address, complement(read(address)));
			break;

		/**
		 * LSR    Logical shift right
		 * Source Form: LSR Q; LSRA; LSRB
		 * Operation:
		 *      _________________    _____
		 * 0 -> | | | | | | | | | -> | C |
		 *      -----------------    -----
		 *      b7             b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Cleared
		 *     Z: Set if all bits of the result are Clear
		 *     V: Not Affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Performs a logical shift right on the operand. Shifts a zero into bit 7 and bit 0
		 *     into the carry flag. The 6800 processor also affects the V flag.
		 *
		 * LSR - Direct - Opcode: 04 - MPU Cycles: 6 - No of bytes: 2
		 */
		case Opcodes::LSR_DIRECT:
			cycle_counter += 6;
			address = retrieve_direct_address();
			write(address, logical_shift_right(read(address)));
			break;

		/**
		 * ROR    Rotate right
		 * Source Form: ROR Q; RORA; RORB
		 * Operation:
		 *       _____
		 *  -----| C |-----
		 *  |    -----    |
		 * _________________
		 * | | | | | | | | |
		 * -----------------
		 * b7      ->     b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Not Affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Rotates all bits of the operand right one place through the carry flag; this is
		 *     a nine-bit rotation. The 6800 processor also affects the V flag.
		 *
		 * ROR - Direct - Opcode: 06 - MPU Cycles: 6 - No of bytes: 2
		 */
		case Opcodes::ROR_DIRECT:
			cycle_counter += 6;
			address = retrieve_direct_address();
			write(address, rotate_right(read(address)));
			break;

		/**
		 * ASR    Arithmetic shift right
		 * Source Form: ASR Q
		 * Operation:
		 * _____
		 * |   |
		 * |  _________________    _____
		 * -->| | | | | | | | | -> | C |
		 *    -----------------    -----
		 *    b7             b0
		 *
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set
		 *     Z: Set if all bits of result are clear
		 *     V: Not affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Shifts all bits of the operand right one place. Bit 7 is held constant. Bit 0 is
		 *     shifted into the carry flag. The 6800/01/02/03/08 processors do affect the V
		 *     flag.
		 *
		 * ASR - Direct - Opcode: 07 - MPU Cycles: 6 - No of bytes: 2
		 */
		case Opcodes::ASR_DIRECT:
			cycle_counter += 6;
			address = retrieve_direct_address();
			write(address, arithmetic_shift_right(read(address)));
			break;

		/**
		 * ASL    Arithmetic shift left
		 * Source Form: ASL Q
		 * Operation:
		 * _____    _________________
		 * | C | <- | | | | | | | | | <- 0
		 * -----    -----------------
		 *          b7      <-     b0
		 *
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set
		 *     Z: Set if all bits of the result are clear
		 *     V: Loaded with the result of b7 ^ b0 of the original operand.
		 *     C: Loaded with bit 7 of the original operand.
		 * Description:
		 *     Shifts all bits of the operand one place to the left. Bit 0 is loaded with a
		 *     zero. Bit 7 of the operand is shifted into the carry flag.
		 *
		 * ASL - Direct - Opcode: 08 - MPU Cycles: 6 - No of bytes: 2
		 */
		case Opcodes::ASL_DIRECT:
			cycle_counter += 6;
			address = retrieve_direct_address();
			write(address, arithmetic_shift_left(read(address)));
			break;

		/**
		 * ROL    Rotate left
		 * Source Form: ROL Q; ROLA; ROLB
		 * Operation:
		 *       _____
		 *  -----| C |-----
		 *  |    -----    |
		 * _________________
		 * | | | | | | | | |
		 * -----------------
		 * b7      <-     b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Loaded with the result of (b7 ^ b6) of the original operand.
		 *     C: Loaded with bit 7 of the original operand
		 * Description:
		 *     Rotate all bits of the operand one place left through the carry flag; this is a
		 *     9-bit rotation.
		 *
		 * ROL - Direct - Opcode: 09 - MPU Cycles: 6 - No of bytes: 2
		 */
		case Opcodes::ROL_DIRECT:
			cycle_counter += 6;
			address = retrieve_direct_address();
			write(address, rotate_left(read(address)));
			break;

		/**
		 * DEC    Decrement
		 * Source Form: DEC Q; DECA; DECB
		 * Operation: M' (R') <- M-1 (R-1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Set if the original operand was 10000000
		 *     C: Not affected
		 * Description:
		 *     Subtract one from the operand. The carry flag is not affected, thus allowing DEC
		 *     to be a loopcounter in multiple-precision computations.
		 * Comments:
		 *     When operating on unsigned values only BEQ and BNE branches can be expected to
		 *     behave consistently. When operating on two's complement values, all signed
		 *     branches are available.
		 *
		 * DEC - Direct - Opcode: 0A - MPU Cycles: 6 - No of bytes: 2
		 */
		case Opcodes::DEC_DIRECT:
			cycle_counter += 6;
			address = retrieve_direct_address();
			write(address, decrement(read(address)));
			break;

		/**
		 * INC    Increment
		 * Source Form: INC Q; INCA, INCB
		 * Operation: M' (R') <- M + 1 (R + 1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the original operand was 01111111
		 *     C: Not Affected
		 * Description:
		 *     Add one to the operand. The carry flag is not affected, thus allowing INC to be
		 *     used as a loop-counter in multiple-precision computations.
		 * Comments:
		 *     When operating on unsigned values, only the BEQ and BNE branches can be expected
		 *     to behave consistently. When operating on two's complement values, all signed
		 *     branches are correctly available.
		 *
		 * INC - Direct - Opcode: 0C - MPU Cycles: 6 - No of bytes: 2
		 */
		case Opcodes::INC_DIRECT:
			cycle_counter += 6;
			address = retrieve_direct_address();
			write(address, increment(read(address)));
			break;

		/**
		 * TST    Test
		 * Source Form: TST Q; TSTA; TSTB
		 * Operation: TEMP <- M - 0
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Set condition code flags N and Z according to the contents of M, and clear the V
		 *     flag. The 6800 processor clears the C flag.
		 * Comments:
		 *     The TST instruction provides only minimum information when testing unsigned
		 *     values; since no unsigned value is less than zero, BLO and BLS have no utility.
		 *     While BHI could be used after TST, it provides exactly the same control as BNE,
		 *     which is preferred. The signed branches are available.
		 *
		 * TST - Direct - Opcode: 0D - MPU Cycles: 6 - No of bytes: 2
		 */
		case Opcodes::TST_DIRECT:
			cycle_counter += 6;
			test(read(retrieve_direct_address()));
			break;

		/**
		 * JMP    Jump to effective address
		 * Source Form: JMP
		 * Operation: PC <- EA
		 * Condition Codes: Not Affected
		 * Description:
		 *     Program control is transferred to the location equivalent to the effective
		 *     address
		 *
		 * JMP - Direct - Opcode: 0E - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::JMP_DIRECT:
			cycle_counter += 3;
			program_counter_register = direct_page_register.value | read(program_counter_register);
			break;

		/**
		 * CLR    Clear
		 * Source Form: CLR Q, CLRA, CLRB
		 * Operation: TEMP <- M
		 *            M <- 00 (16)
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Cleared
		 *     Z: Set
		 *     V: Cleared
		 *     C: Cleared
		 * Description:
		 *     ACCX or M is loaded with 00000000. The C-flag is cleared for 6800 compatibility.
		 *
		 * CLR - Direct - Opcode: 0F - MPU Cycles: 6 - No of bytes: 2
		 */
		case Opcodes::CLR_DIRECT:
			cycle_counter += 6;
			write(retrieve_direct_address(), 0);
			clear();
			break;

		case Opcodes::PAGE2:
			opcode = read_and_advance_program_counter();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"

			switch (static_cast<Opcodes>(opcode))
			{

			/**
			 * BRN    Branch never
			 * Source Form: BRN DD; LBRN DDDD
			 * Operation: TEMP <- MI
			 * Condition Codes: Not Affected
			 * Description:
			 *     Does not cause a branch. This instruction is essentially a NO-OP, but has
			 *     a bit pattern logically related to BRA.
			 *
			 * LBRN - Relative - Opcode: 1021 - MPU Cycles: 5 - No of bytes: 4
			 */
			case Opcodes::LBRN_RELATIVE:
				cycle_counter += 5;
				program_counter_register += 2;
				break;

			/**
			 * BHI    Branch if higher
			 * Source Forms: BHI DD; LBHI DDDD
			 * Operaiton: TEMP <- MI
			 *            if C ∨ Z = 0 then PC <- PC + TEMP
			 * Condition Codes: Not affected
			 * Description:
			 *     Causes a branch if the previous operation caused neither a carry nor a
			 *     zero result.
			 * Comments:
			 *     Used after a subtract or compare operation on unsigned binary values,
			 *     this instruction will "branch" if the register was higher than the memory
			 *     operand. Not useful, in general, after INC/DEC, LD/ST, TST/CLR/COM.
			 *
			 * LBHI - Relative - Opcode: 1022 - MPU Cycles: 5(6) - No of bytes: 4
			 */
			case Opcodes::LBHI_RELATIVE:
				cycle_counter += 5;

				if (!condition_code_register->bits.carry && !condition_code_register->bits.zero)
				{
					long_branch_relative_offset();
					cycle_counter += 1;
				}

				program_counter_register += 2;
				break;

			/**
			 * BLS    Branch on lower or same
			 * Source Forms: BLS DD; LBLS DDDD
			 * Operation: TEMP <- MI
			 *            if C ∨ Z = 1 then PC <- PC + TEMP
			 * Condition Codes: Not affected
			 * Description:
			 *     Causes a branch if the previous operation caused either a carry or a zero
			 *     result.
			 * Comments:
			 *     Used after a subtract or compare operation on unsigned binary values,
			 *     this instruction will "branch" if the register was lower than or the same
			 *     as the memory operand. Not useful, in general, after INC/DEC, LD/ST,
			 *     TST/CLR/COM.
			 *
			 * LBLS - Relative - Opcode: 1023 - MPU Cycles: 5(6) - No of bytes: 4
			 */
			case Opcodes::LBLS_RELATIVE:
				cycle_counter += 5;

				if (condition_code_register->bits.carry || condition_code_register->bits.zero)
				{
					long_branch_relative_offset();
					cycle_counter += 1;
				}

				program_counter_register += 2;
				break;

			/**
			 * BCC    Branch on carry clear
			 * Source Form: BCC DD; LBCC DDDD
			 * Operation: TEMP <- MI
			 *            if C = 0 then PC <- PC + TEMP
			 * Condition Codes: Not affected
			 * Description:
			 *     Tests the state of the C bit and causes a branch if C is clear.
			 * Comments:
			 *     When used after a subtract or compare on unsigned binary values, this
			 *     instruction could be called "branch" if the register was higher or the
			 *     same as the memory operand.
			 *
			 * Comments (BHS):
			 *     ... This is a duplicate assembly-language mnemonic for the single machine
			 *     instruction BCC. Not useful, in general, after INC/DEC, LD/ST,
			 *     TST/CLR/COM.
			 *
			 * LBCC - Relative - Opcode: 1024 - MPU Cycles: 5(6) - No of bytes: 4
			 */
			case Opcodes::LBHS_RELATIVE:
				cycle_counter += 6;

				if (!condition_code_register->bits.carry)
				{
					long_branch_relative_offset();
					cycle_counter += 1;
				}

				program_counter_register += 2;
				break;

			/**
			 * BCS    Branch on carry set
			 * Source Form: BCS DD; LBCS DDDD
			 * Operation: TEMP <- MI
			 *            if C = 1 then PC <- PC + TEMP
			 * Condition Codes: Not affected
			 * Description:
			 *     Tests the state of the C bit and causes a branch if C is set.
			 * Comments:
			 *     When used after a subtract or compare, on unsigned binary values, this
			 *     instruction could be called "branch" if the register was lower than the
			 *     memory operand.
			 * Comments on BLO:
			 *     Note that this is a duplicate assembly-language mnemonic for the single
			 *     machine instruction BCS. Not useful, in general, after INC/DEC, LD/ST,
			 *     TST/CLR/COM.
			 *
			 * LBCS - Relative - Opcode: 1025 - MPU Cycles: 5(6) - No of bytes: 4
			 */
			case Opcodes::LBCS_RELATIVE:
				cycle_counter += 5;

				if (condition_code_register->bits.carry)
				{
					long_branch_relative_offset();
					cycle_counter += 1;
				}

				program_counter_register += 2;
				break;

			/**
			 * BNE    Branch not equal
			 * Source Forms: BNE DD; LBNE DDDD
			 * Operation: TEMP <- MI
			 *            if Z = 0 then PC <- PC + TEMP
			 * Condition Codes: Not Affected
			 * Description:
			 *     Tests the state of the Z bit and causes a branch if the Z bit is clear.
			 * Comments:
			 *     Used after a subtract or compare operation on any binary values, this
			 *     instruction will "branch if the register is (or would be) not equal to
			 *     the memory operand."
			 *
			 * LBNE - Relative - Opcode: 1026 - MPU Cycles: 5(6) - No of bytes: 4
			 */
			case Opcodes::LBNE_RELATIVE:
				cycle_counter += 5;

				if (!condition_code_register->bits.zero)
				{
					long_branch_relative_offset();
					cycle_counter += 1;
				}

				program_counter_register += 2;
				break;

			/**
			 * BEQ    Branch on equal
			 * Source Forms: BEQ DD; LBEQ DDDD;
			 * Operation: TEMP <- MI
			 *            if Z = 1 then PC <- PC + TEMP
			 * Condition Codes: Not affected.
			 * Description:
			 *     Tests the state of the Z bit and causes a branch if the Z bit is set.
			 * Comments:
			 *     Used after a subtract or compare operation, this instruction will branch
			 *     if the compared values - signed or unsigned - were exactly the same.
			 *
			 * LBEQ - Relative - Opcode: 1027 - MPU Cycles: 5(6) - No of bytes: 4
			 */
			case Opcodes::LBEQ_RELATIVE:
				cycle_counter += 5;

				if (!condition_code_register->bits.zero)
				{
					long_branch_relative_offset();
					cycle_counter += 1;
				}

				program_counter_register += 2;
				break;

			/**
			 * BVC    Branch on overflow clear
			 * Source Form: BVC DD; LBVC DDDD
			 * Operation: TEMP <- MI
			 *            if V = 0 then PC <- PC + TEMP
			 * Condition Codes: Not Affected
			 * Description:
			 *     Tests the state of the V bit and causes a branch if the V bit is clear.
			 *     That is, branch if the two's complement result was valid.
			 * Comments:
			 *     Used after an operation on two's complement binary values, this
			 *     instruction will "branch if there was no overflow."
			 *
			 * LBVC - Relative - Opcode: 1028 - MPU Cycles 5(6) - No of bytes: 4
			 */
			case Opcodes::LBVC_RELATIVE:
				cycle_counter += 5;

				if (!condition_code_register->bits.overflow)
				{
					long_branch_relative_offset();
					cycle_counter += 1;
				}

				program_counter_register += 2;
				break;

			/**
			 * BVS    Branch on overflow set
			 * Source Form: BVS DD; LBVS DDDD
			 * Operation: TEMP <- MI
			 *            if V = 1 then PC <- PC + TEMP
			 * Condition Codes: Not Affected
			 * Description:
			 *     Tests the state of the V bit and causes a branch if the V bit is set.
			 *     That is, branch if the two's complement result was invalid.
			 * Comments:
			 *     Used after an operation on two's complement binary values, this
			 *     instruction will "branch if there was an overflow." This instruction is
			 *     also used after ASL or LSL to detect binary floating-point normalization.
			 *
			 * LBVS - Relative - Opcode: 1029 - MPU Cycles: 5(6) - No of bytes: 4
			 */
			case Opcodes::LBVS_RELATIVE:
				cycle_counter += 5;

				if (condition_code_register->bits.overflow)
				{
					long_branch_relative_offset();
					cycle_counter += 1;
				}

				program_counter_register += 2;
				break;

			/**
			 * BPL    Branch if plus
			 * Source Form: BPL DD; LBPL DDDD
			 * Operation: TEMP <- MI
			 *            if N = 0 then PC <- PC + TEMP
			 * Condition Codes: Not Affected
			 * Description:
			 *     Tests the state of the N bit and causes a branch if N is clear. That is,
			 *     branch if the sign of the two's complement result is positive.
			 * Comments:
			 *     Used after an operation on two's complement binary values, this
			 *     instruction will "branch if the possibly invalid result is positive."
			 *
			 * LBPL - Relative - Opcode: 102A - MPU Cycles: 5(6) - No of bytes: 4
			 */
			case Opcodes::LBPL_RELATIVE:
				cycle_counter += 5;

				if (!condition_code_register->bits.negative)
				{
					long_branch_relative_offset();
					cycle_counter += 1;
				}

				program_counter_register += 2;
				break;

			/**
			 * BMI    Branch on minus
			 * Source Form: BMI DD; LBMI DDDD
			 * Operation: TEMP <- MI
			 *            if N = 1 then PC <- PC + TEMP
			 * Condition Codes: Not Affected
			 * Description:
			 *     Tests the state of the N bit and causes a branch if N is set. That is,
			 *     branch if the sign of the two's complement result is negative.
			 * Comments:
			 *     Used after an operation on two's complement binary values, this
			 *     instruction will "branch if the (possibly invalid) result is minus."
			 *
			 * LBMI - Relative - Opcode: 102B - MPU Cycles: 5(6) - No of bytes: 4
			 */
			case Opcodes::LBMI_RELATIVE:
				cycle_counter += 5;

				if (condition_code_register->bits.negative)
				{
					long_branch_relative_offset();
					cycle_counter += 1;
				}

				program_counter_register += 2;
				break;

			/**
			 * BGE    Branch on greater than or equal to zero
			 * Source Forms: BGE DD; LBGE DDDD;
			 * Operation: TEMP <- MI
			 *            if N ^ V = 0 then PC <- PC + TEMP
			 * Condition Codes: Not affected
			 * Description:
			 *     Causes a branch if N and V are either both set or both clear. For
			 *     example, branch if the sign of a valid two's complement result is, or
			 *     would be, positive.
			 * Comments:
			 *     Used after a subtract or compare operation on two's complement values,
			 *     this instruction will branch if the register was greater than or equal to
			 *     the memory operand.
			 *
			 * LBGE - Relative - Opcode: 102C - MPU Cycles: 5(6) - No of bytes: 4
			 */
			case Opcodes::LBGE_RELATIVE:
				cycle_counter += 5;

				if (condition_code_register->bits.negative == condition_code_register->bits.overflow)
				{
					long_branch_relative_offset();
					cycle_counter += 1;
				}

				program_counter_register += 2;
				break;

			/**
			 * BLT    Branch on less than zero
			 * Source Forms: BLT DD; LBLT DDDD;
			 * Operation: Temp <- MI
			 *            if N ^ V = 1 then PC <- PC + TEMP
			 * Condition Codes: Not affected
			 * Description:
			 *     Causes a branch if either, but not both, of the N or V bits is 1. That
			 *     is, branch if the sign of a valid two's complement result is - or would
			 *     be - negative.
			 * Comments:
			 *     Used after a subtract or compare operation on two's complement binary
			 *     values, this instruction will "branch if the register was less than the
			 *     memory operand."
			 *
			 * LBLT - Relative - Opcode: 102D - MPU Cycles: 5(6) - No of bytes: 4
			 */
			case Opcodes::LBLT_RELATIVE:
				cycle_counter += 5;

				if (condition_code_register->bits.negative != condition_code_register->bits.overflow)
				{
					long_branch_relative_offset();
					cycle_counter += 1;
				}

				program_counter_register += 2;
				break;

			/**
			 * BGT    Branch on greater
			 * Source Forms: BGT DD; LBGT DDDD;
			 * Operation: TEMP <- MI
			 *            if Z ∨ (N ^ V) = 0 then PC <- PC + TEMP
			 * Condition Codes: Not affected
			 * Description:
			 *     Causes a branch if (N and V are either both set or both clear) and Z is
			 *     clear. In other words, branch if the sign of a valid two's complemet
			 *     result is, or would be, positive and non-zero.
			 * Comments:
			 *     Used after a subtract or compare operation on two's complement values,
			 *     this instruction will "branch" if the register was greater than the
			 *     memory operand.
			 *
			 * LBGT - Relative - Opcode: 102E - MPU Cycles: 5(6) - No of bytes: 4
			 */
			case Opcodes::LBGT_RELATIVE:
				cycle_counter += 5;

				if (!condition_code_register->bits.zero && condition_code_register->bits.negative == condition_code_register->bits.overflow)
				{
					long_branch_relative_offset();
					cycle_counter += 1;
				}

				program_counter_register += 2;
				break;

			/**
			 * BLE    Branch on less than or equal to zero
			 * Source Form: BLE DD; LBLE DDDD;
			 * Operation: TEMP <- MI
			 *            if Z ∨ (N ^ V) = 1 then PC = PC + TEMP
			 * Condition Codes: Not Affected
			 * Description:
			 *     Causes a branch if the "Exclusive OR" of the N and V bits is 1 or if
			 *     Z = 1. That is, branch if the sign of a valid two's complement result is
			 *     - or would be - negative.
			 * Comments:
			 *     Used after a subtract or compare operation on two's complement values,
			 *     this instruction will "branch" if the register was less then or equal to
			 *     the memory operand.
			 *
			 * LBLE - Relative - Opcode: 102F - MPU Cycles: 5(6) - No of bytes: 4
			 */
			case Opcodes::LBLE_RELATIVE:
				cycle_counter += 5;

				if (condition_code_register->bits.zero || condition_code_register->bits.negative != condition_code_register->bits.overflow)
				{
					long_branch_relative_offset();
					cycle_counter += 1;
				}

				program_counter_register += 2;
				break;

			/**
			 * SWI2    Software Interrupt 2
			 * Source Form: SWI2
			 * Operation: Set E (entire state saved)
			 *            SP' <- SP - 1, (SP) <- PCL
			 *            SP' <- SP - 1, (SP) <- PCH
			 *            SP' <- SP - 1, (SP) <- USL
			 *            SP' <- SP - 1, (SP) <- USH
			 *            SP' <- SP - 1, (SP) <- IYL
			 *            SP' <- SP - 1, (SP) <- IYH
			 *            SP' <- SP - 1, (SP) <- IXL
			 *            SP' <- SP - 1, (SP) <- IXH
			 *            SP' <- SP - 1, (SP) <- DPR
			 *            SP' <- SP - 1, (SP) <- ACCB
			 *            SP' <- SP - 1, (SP) <- ACCA
			 *            SP' <- SP - 1, (SP) <- CCR
			 *            PC' <- (FFF4):(FFF5)
			 * Condition Codes: Not Affected
			 * Description:
			 *     All of the MPU registers are pushed onto the hardware stack (excepting
			 *     only the hardware stack pointer itself), and control is transferred
			 *     through the SWI2 vector. SWI2 is available to the end user and must not
			 *     be used in packaged software.
			 *
			 *     SWI2 DOES NOT AFFECT I AND F BITS
			 *
			 * SWI2 - Inherent - Opcode: 103F - MPU Cycles: 20 - No of bytes: 2
			 */
			case Opcodes::SWI2_INHERENT:
				cycle_counter += 20;

				condition_code_register->set_entire(true);

				push_machine_state();
				program_counter_register = read2(static_cast<std::uint16_t>(VectorTable::SOFTWARE_INTERRUPT_2));
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPD - Immediate - Opcode: 1083 - MPU Cycles: 5 - No of bytes: 4
			 */
			case Opcodes::CMPD_IMMEDIATE:
				cycle_counter += 5;
				compare(accumulator_register.d, read2_and_advance_program_counter());
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPY - Immediate - Opcode: 108C - MPU Cycles: 5 - No of bytes: 4
			 */
			case Opcodes::CMPY_IMMEDIATE:
				cycle_counter += 5;
				compare(y_index_register, read2_and_advance_program_counter());
				break;

			/**
			 * LD    Load register from memory
			 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
			 * Operation: R' <- M(:M+1)
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 (15) of loaded data is Set
			 *     Z: Set if all bits of loaded data are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Load the contents of the addressed memory into the register.
			 *
			 * LDY - Immediate - Opcode: 108E - MPU Cycles: 4 - No of bytes: 4
			 */
			case Opcodes::LDY_IMMEDIATE:
				cycle_counter += 5;
				y_index_register = read2_and_advance_program_counter();
				condition_register(y_index_register);
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPD - Direct - Opcode: 1093 - MPU Cycles: 7 - No of bytes: 3
			 */
			case Opcodes::CMPD_DIRECT:
				cycle_counter += 7;
				compare(accumulator_register.d, read2(retrieve_direct_address()));
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPY - Direct - Opcode: 109C - MPU Cycles: 7 - No of bytes: 3
			 */
			case Opcodes::CMPY_DIRECT:
				cycle_counter += 7;
				compare(y_index_register, read2(retrieve_direct_address()));
				break;

			/**
			 * LD    Load register from memory
			 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
			 * Operation: R' <- M(:M+1)
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 (15) of loaded data is Set
			 *     Z: Set if all bits of loaded data are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Load the contents of the addressed memory into the register.
			 *
			 * LDY - Direct - Opcode: 109E - MPU Cycles: 6 - No of bytes: 3
			 */
			case Opcodes::LDY_DIRECT:
				cycle_counter += 6;
				y_index_register = read2(retrieve_direct_address());
				condition_register(y_index_register);
				break;

			/**
			 * ST    Store register into memory
			 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
			 * Operation: M'(:M+1') <- R
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 (15) of stored data was Set
			 *     Z: Set if all bits of stored data are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Writes the contents of an MPU register into a memory location.
			 *
			 * STY - Direct - Opcode: 109F - MPU Cycles: 6 - No of bytes: 3
			 */
			case Opcodes::STY_DIRECT:
				cycle_counter += 6;
				store_register(retrieve_direct_address(), y_index_register);
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPD - Indexed - Opcode: 10A3 - MPU Cycles: 7+ - No of bytes: 3+
			 */
			case Opcodes::CMPD_INDEXED:
				cycle_counter += 7;
				compare(accumulator_register.d, read2(retrieve_effective_address()));
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPY - Indexed - Opcode: 10AC - MPU Cycles: 7+ - No of bytes: 3+
			 */
			case Opcodes::CMPY_INDEXED:
				cycle_counter += 7;
				compare(y_index_register, read2(retrieve_effective_address()));
				break;

			/**
			 * LD    Load register from memory
			 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
			 * Operation: R' <- M(:M+1)
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 (15) of loaded data is Set
			 *     Z: Set if all bits of loaded data are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Load the contents of the addressed memory into the register.
			 *
			 * LDY - Indexed - Opcode: 10AE - MPU Cycles: 6+ - No of bytes: 3+
			 */
			case Opcodes::LDY_INDEXED:
				cycle_counter += 6;
				y_index_register = read2(retrieve_effective_address());
				condition_register(y_index_register);
				break;

			/**
			 * ST    Store register into memory
			 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
			 * Operation: M'(:M+1') <- R
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 (15) of stored data was Set
			 *     Z: Set if all bits of stored data are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Writes the contents of an MPU register into a memory location.
			 *
			 * STY - Indexed - Opcode: 10AF - MPU Cycles: 6+ - No of bytes: 3+
			 */
			case Opcodes::STY_INDEXED:
				cycle_counter += 6;
				store_register(retrieve_effective_address(), y_index_register);
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPD - Extended - Opcode: 10B3 - MPU Cycles: 8 - No of bytes: 4
			 */
			case Opcodes::CMPD_EXTENDED:
				cycle_counter += 8;
				compare(accumulator_register.d, read2(read2_and_advance_program_counter()));
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPY - Extended - Opcode: 10BC - MPU Cycles: 8 - No of bytes: 4
			 */
			case Opcodes::CMPY_EXTENDED:
				cycle_counter += 8;
				compare(y_index_register, read2(read2_and_advance_program_counter()));
				break;

			/**
			 * LD    Load register from memory
			 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
			 * Operation: R' <- M(:M+1)
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 (15) of loaded data is Set
			 *     Z: Set if all bits of loaded data are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Load the contents of the addressed memory into the register.
			 *
			 * LDY - Extended - Opcode: 10BE - MPU Cycles: 7 - No of bytes: 4
			 */
			case Opcodes::LDY_EXTENDED:
				cycle_counter += 7;
				y_index_register = read2(read2_and_advance_program_counter());
				condition_register(y_index_register);
				break;

			/**
			 * ST    Store register into memory
			 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
			 * Operation: M'(:M+1') <- R
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 (15) of stored data was Set
			 *     Z: Set if all bits of stored data are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Writes the contents of an MPU register into a memory location.
			 *
			 * STY - Extended - Opcode: 10DF - MPU Cycles: 7 - No of bytes: 4
			 */
			case Opcodes::STY_EXTENDED:
				cycle_counter += 7;
				store_register(read2_and_advance_program_counter(), y_index_register);
				break;

			/**
			 * LD    Load register from memory
			 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
			 * Operation: R' <- M(:M+1)
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 (15) of loaded data is Set
			 *     Z: Set if all bits of loaded data are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Load the contents of the addressed memory into the register.
			 *
			 * LDS - Immediate - Opcode: 10CE - MPU Cycles: 4 - No of bytes: 4
			 */
			case Opcodes::LDS_IMMEDIATE:
				cycle_counter += 4;
				hardware_stack_pointer_register = read2_and_advance_program_counter();
				condition_register(hardware_stack_pointer_register);
				break;

			/**
			 * LD    Load register from memory
			 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
			 * Operation: R' <- M(:M+1)
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 (15) of loaded data is Set
			 *     Z: Set if all bits of loaded data are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Load the contents of the addressed memory into the register.
			 *
			 * LDS - Direct - Opcode: 10DE - MPU Cycles: 6 - No of bytes: 3
			 */
			case Opcodes::LDS_DIRECT:
				cycle_counter += 6;
				hardware_stack_pointer_register = read2(retrieve_direct_address());
				condition_register(hardware_stack_pointer_register);
				break;

			/**
			 * ST    Store register into memory
			 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
			 * Operation: M'(:M+1') <- R
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 (15) of stored data was Set
			 *     Z: Set if all bits of stored data are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Writes the contents of an MPU register into a memory location.
			 *
			 * STS - Direct - Opcode: 10DF - MPU Cycles: 6 - No of bytes: 3
			 */
			case Opcodes::STS_DIRECT:
				cycle_counter += 6;
				store_register(retrieve_direct_address(), hardware_stack_pointer_register);
				break;

			/**
			 * LD    Load register from memory
			 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
			 * Operation: R' <- M(:M+1)
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 (15) of loaded data is Set
			 *     Z: Set if all bits of loaded data are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Load the contents of the addressed memory into the register.
			 *
			 * LDS - Indexed - Opcode: 10EE - MPU Cycles: 6+ - No of bytes: 3+
			 */
			case Opcodes::LDS_INDEXED:
				cycle_counter += 6;
				hardware_stack_pointer_register = read2(retrieve_effective_address());
				condition_register(hardware_stack_pointer_register);
				break;

			/**
			 * ST    Store register into memory
			 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
			 * Operation: M'(:M+1') <- R
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 (15) of stored data was Set
			 *     Z: Set if all bits of stored data are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Writes the contents of an MPU register into a memory location.
			 *
			 * STS - Indexed - Opcode: 10EF - MPU Cycles: 6+ - No of bytes: 3+
			 */
			case Opcodes::STS_INDEXED:
				cycle_counter += 6;
				store_register(retrieve_effective_address(), hardware_stack_pointer_register);
				break;

			/**
			 * LD    Load register from memory
			 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
			 * Operation: R' <- M(:M+1)
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 (15) of loaded data is Set
			 *     Z: Set if all bits of loaded data are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Load the contents of the addressed memory into the register.
			 *
			 * LDS - Extended - Opcode: 10FE - MPU Cycles: 7 - No of bytes: 4
			 */
			case Opcodes::LDS_EXTENDED:
				cycle_counter += 7;
				hardware_stack_pointer_register = read2(read2_and_advance_program_counter());
				condition_register(hardware_stack_pointer_register);
				break;

			/**
			 * ST    Store register into memory
			 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
			 * Operation: M'(:M+1') <- R
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 (15) of stored data was Set
			 *     Z: Set if all bits of stored data are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Writes the contents of an MPU register into a memory location.
			 *
			 * STS - Extended - Opcode: 10FF - MPU Cycles: 7 - No of bytes: 4
			 */
			case Opcodes::STS_EXTENDED:
				cycle_counter += 7;
				store_register(read2_and_advance_program_counter(), hardware_stack_pointer_register);
				break;
			}
			break;

#pragma GCC diagnostic pop

		case Opcodes::PAGE3:
			opcode = read_and_advance_program_counter();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"

			switch (static_cast<Opcodes>(opcode))
			{

			/**
			 * SWI3    Software Interrupt 3
			 * Source Form: SWI3
			 * Operation: Set E (entire state saved)
			 *            SP' <- SP - 1, (SP) <- PCL
			 *            SP' <- SP - 1, (SP) <- PCH
			 *            SP' <- SP - 1, (SP) <- USL
			 *            SP' <- SP - 1, (SP) <- USH
			 *            SP' <- SP - 1, (SP) <- IYL
			 *            SP' <- SP - 1, (SP) <- IYH
			 *            SP' <- SP - 1, (SP) <- IXL
			 *            SP' <- SP - 1, (SP) <- IXH
			 *            SP' <- SP - 1, (SP) <- DPR
			 *            SP' <- SP - 1, (SP) <- ACCB
			 *            SP' <- SP - 1, (SP) <- ACCA
			 *            SP' <- SP - 1, (SP) <- CCR
			 *            PC' <- (FFF2):(FFF3)
			 * Condition Codes: Not Affected
			 * Description:
			 *     All of the MPU registers are pushed onto the hardware stack (excepting
			 *     only the hardware stack pointer itself), and control is transferred
			 *     through the SWI3 vector.
			 *
			 *     SWI3 DOES NOT AFFECT I AND F BITS
			 *
			 * SWI3 - Inherent - Opcode: 113F - MPU Cycles: 20 - No of bytes: 2
			 */
			case Opcodes::SWI3_INHERENT:
				cycle_counter += 20;

				condition_code_register->set_entire(true);

				push_machine_state();
				program_counter_register = read2(static_cast<std::uint16_t>(VectorTable::SOFTWARE_INTERRUPT_3));
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPU - Immediate - Opcode: 1183 - MPU Cycles: 5 - No of bytes: 4
			 */
			case Opcodes::CMPU_IMMEDIATE:
				cycle_counter += 5;
				compare(user_stack_pointer_register, read2_and_advance_program_counter());
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPS - Immediate - Opcode: 118C - MPU Cycles: 5 - No of bytes: 4
			 */
			case Opcodes::CMPS_IMMEDIATE:
				cycle_counter += 5;
				compare(hardware_stack_pointer_register, read2_and_advance_program_counter());
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPU - Direct - Opcode: 1193 - MPU Cycles: 7 - No of bytes: 3
			 */
			case Opcodes::CMPU_DIRECT:
				cycle_counter += 7;
				compare(user_stack_pointer_register, read2(retrieve_direct_address()));
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPS - Direct - Opcode: 119C - MPU Cycles: 7 - No of bytes: 3
			 */
			case Opcodes::CMPS_DIRECT:
				cycle_counter += 7;
				compare(hardware_stack_pointer_register, read2(retrieve_direct_address()));
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPU - Indexed - Opcode: 11A3 - MPU Cycles: 7+ - No of bytes: 3+
			 */
			case Opcodes::CMPU_INDEXED:
				cycle_counter += 7;
				compare(user_stack_pointer_register, read2(retrieve_effective_address()));
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPS - Indexed - Opcode: 11AC - MPU Cycles: 7+ - No of bytes: 3+
			 */
			case Opcodes::CMPS_INDEXED:
				cycle_counter += 7;
				compare(hardware_stack_pointer_register, read2(retrieve_effective_address()));
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPU - Extended - Opcode: 11B3 - MPU Cycles: 8 - No of bytes: 4
			 */
			case Opcodes::CMPU_EXTENDED:
				cycle_counter += 8;
				compare(user_stack_pointer_register, read2(read2_and_advance_program_counter()));
				break;

			/**
			 * CMP    Compare memory from a register
			 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
			 * Operation: TEMP <- R - M(:M+1)
			 * Condition Codes:
			 *     H: Undefined
			 *     N: Set if bit 7 (15) of the result is Set.
			 *     Z: Set if all bits of the result are Clear
			 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
			 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
			 * Description:
			 *     Compares the contents of M from teh contents of the specified register
			 *     and sets appropriate condition codes. Neither M nor R is modified. The C
			 *     flag represents a borrow and is set inverse to the resulting binary
			 *     carry.
			 *
			 * CMPS - Extended - Opcode: 11BC - MPU Cycles: 8 - No of bytes: 4
			 */
			case Opcodes::CMPS_EXTENDED:
				cycle_counter += 8;
				compare(hardware_stack_pointer_register, read2(read2_and_advance_program_counter()));
				break;
			}
			break;

#pragma GCC diagnostic pop

		/**
		 * NOP    No operation
		 * Source Form: NOP
		 * Condition CodeS: Not Affected
		 * Description:
		 *     This is a single-byte instruction that causes only the program counter to be
		 *     incremented. No other registers or memory contents are affected.
		 *
		 * NOP - Inherent - Opcode: 12 - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::NOP_INHERENT:
			cycle_counter += 2;
			break;

		/**
		 * SYNC    Synchronize to external event
		 * Source Form: SYNC
		 * Operation: Stop processing instructions
		 * Condition CodeS: Unaffected
		 * Description:
		 *     Whan a SYNC instruction is executed, the MPU enters a SYNCING state, stops
		 *     processing instructions and waits on an interrupt. When an interrupt occurs, the
		 *     SYNCING state is cleared and processing continues. IF the interrupt is enabled,
		 *     and the interrupt lasts 3 cycles or more, the processor will perform the
		 *     interrupt routine. If the interrupt is masked or is shorter than 3 cycles long,
		 *     the processor simply continues to the next instruction (without stacking
		 *     registers). While SYNCING, the address and data buses are tri-state.
		 * Comments:
		 *     This instruction provides software synchronization with a hardware process.
		 *     Consider the high-speed acquisition of data:
		 *
		 *           FOR DATA
		 *     FAST  SYNC         WAIT FOR DATA
		 *           <--------------------- interrupt!
		 *           LDA  DISC    DATA FORM DISC AND CLEAR INTERRUPT
		 *           STA  ,X+     PUT IN BUFFER
		 *           DECB         COUNT IT, DONE?
		 *           BNE  FAST    GO AGAIN IF NOT.
		 *
		 *     The SYNCING state is cleared by any interrupt, and any enabled interrupt will
		 *     probably destroy the transfer (this may be used to provide MPU response to an
		 *     emergency condition).
		 *
		 *     The same connection used for interrupt-driven I/O service may thus be used for
		 *     high-speed data transfers by setting the interrupt mask and using SYNC.
		 *
		 * SYNC - Inherent - Opcode: 13 - MPU Cycles >= 2 - No of bytes: 1
		 */
		case Opcodes::SYNC_INHERENT:
			cycle_counter = num_cycles_to_run;

			// Interrupt or RESET acknowledge
			pins_state->set_ba(true);
			pins_state->set_bs(false);

			syncing = true;
			break;

		/**
		 * BRA    Branch always
		 * Source Forms: BRA DD; LBRA DDDD
		 * Operation: TEMP <- MI
		 *            PC <- PC + TEMP
		 * Condition CodeS: Not Affected
		 * Description:
		 *     Causes an unconditional branch.
		 *
		 * LBRA - Relative - Opcode: 16 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::LBRA_RELATIVE:
			cycle_counter += 5;
			long_branch_relative_offset();
			program_counter_register += 2;
			break;

		/**
		 * BSR    Branch to subroutine
		 * Source Forms: BSR DD; LBSR DDDD
		 * Operation: TEMP <- MI
		 *            SP' <- SP - 1, (SP) <- PCL
		 *            SP' <- SP - 1, (SP) <- PCH
		 *            PC <- PC + TEMP
		 * Condition Codes: Not Affected
		 * Description:
		 *     The program counter is pushed onto the stack. The program counter is then loaded
		 *     with the sum of the program counter and the memory immediate offset.
		 *
		 * LBSR - Relative - Opcode: 17 - MPU Cycles: 9 - No of bytes: 3
		 */
		case Opcodes::LBSR_RELATIVE:
			cycle_counter += 9;
			address = read2_and_advance_program_counter();
			push_program_counter_register();
			program_counter_register = address;
			break;

		/**
		 * DA    Decimal Addition Adjust
		 * Source Form: DAA
		 * Operation: ACCA' <- Acca + CF(MSN):CF(LSN)
		 *            where CF is a Correction Factor, as follows:
		 *
		 *            The CF for each nybble (BCD Digit) is determined separately, and is either
		 *            6 or 0.
		 *
		 *            Least Significant Nybble: CF(LSN) = 6 if 1) H = 1
		 *                                                     or 2) LSN > 9
		 *            Most Sifnigicant Nybble: CF(MSN) = 6 if 1) C = 1
		 *                                                    or 2) MSN > 9
		 *                                                    or 3) MSN > 8 and LSN > 9
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if MSB of result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Not defined
		 *     C: Set if the operation caused a carry from bit 7 in the ALU, or if the carry
		 *        flag was Set before the operation.
		 * Description:
		 *     The sequence of a single-byte add instruction on ACCA (either ADDA or ADCA) and a
		 *     following DAA instruction results in a BCD addition with appropriate carry flag.
		 *     Both values to be added must be in proper BCD form (each nybble such that 0 <=
		 *     nybble <= 9). Multiple-precision additions must add the carry generated by this
		 *     DA into the next higher digit during the add operation immediately prior to the
		 *     next DA.
		 *
		 * DAA - Inherent - Opcode: 19 - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::DAA_INHERENT:
			cycle_counter += 2;

			most_significant_nibble = (accumulator_register.registers.a >> 4);
			least_significant_nibble = accumulator_register.registers.a & 0xF;
			addition = 0;

			if (condition_code_register->bits.half_carry || least_significant_nibble > 9)
			{
				addition |= 0x06;
			}

			if (condition_code_register->bits.carry || most_significant_nibble > 9 || (most_significant_nibble > 8 && least_significant_nibble > 9))
			{
				addition |= 0x60;
			}

			addition += accumulator_register.registers.a;
			accumulator_register.registers.a = addition;

			condition_code_register->set_negative(accumulator_register.registers.a & 0x80);
			condition_code_register->set_zero(accumulator_register.registers.a == 0);
			condition_code_register->set_carry(addition & 0x100);
			break;

		/**
		 * OR    Inclusive OR memory-immediate into CCR
		 * Source Form: ORCC #XX
		 * Operation: R <- R ∨ MI
		 * Condition Codes: CCR' <- CCR ∨ MI
		 * Description:
		 *     Performs an "inclusive OR" operation between the contents of CCR and the contents
		 *     of MI, and the result is placed in CCR. This instruction may be used to Set
		 *     interrupt masks (disable interrupts) or any other flag(s).
		 *
		 * ORCC - Immediate - Opcode: 1A - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::ORCC_IMMEDIATE:
			cycle_counter += 3;
			condition_code_register->or_register(read_and_advance_program_counter());
			break;

		/**
		 * AND    Logical AND immediate memory into CCR
		 * Source Form: ANDCC #XX
		 * Operation: R' <- R ∧ MI
		 * Condition Codes:
		 *     CCR' <- CCR ∧ MI
		 * Description:
		 *     Performs a logical "AND" between the CCR and the MI byte and places the result in
		 *     the CCR.
		 *
		 * ANDCC - Immediate - Opcode: 1C - MPU Cycles: 3 - No of bytes 2
		 */
		case Opcodes::ANDCC_IMMEDIATE:
			cycle_counter += 3;
			condition_code_register->and_register(read_and_advance_program_counter());
			break;

		/**
		 * SEX    Sign extended
		 * Source Form: SEX
		 * Operation: if bit 7 of ACCB is set then ACCA' <- FF (16) else ACCA' <- 00 (16)
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Set if the MSB of the result is Set
		 *     Z: Set if all bits of ACCD are Clear
		 *     V: Not Affected
		 *     C: Not Affected
		 * Description:
		 *     This instruction transformes a two's complement 8-bit value in ACCB into a two's
		 *     complement 16-bit value in the double accumulator.
		 *
		 * SEX - Inherent - Opcode: 1D - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::SEX_INHERENT:
			cycle_counter += 2;

			if (accumulator_register.registers.b & 0x80)
			{
				accumulator_register.registers.a = 0xFF;
			}
			else
			{
				accumulator_register.registers.a = 0x00;
			}

			condition_code_register->set_negative(accumulator_register.d & 0x8000);
			condition_code_register->set_zero(accumulator_register.d == 0);
			break;

		/**
		 * EXG    Exchange registers
		 * Source Form: EXG R1,R2
		 * Operation: R1 <-> R2
		 * Condition Codes: Not Affected (unless one of the registres is CCR)
		 * Description:
		 *     Bits 3-0 of the immediate byte of the instruction define one register, while bits
		 *     7-4 define the other, as follows:
		 *
		 *     0000 = A:B          1000 = A
		 *     0001 = X            1001 = B
		 *     0010 = Y            1010 = CCR
		 *     0011 = US           1011 = DPR
		 *     0100 = SP           1100 = Undefined
		 *     0101 = PC           1101 = Undefined
		 *     0110 = Undefined    1110 = Undefined
		 *     0111 = Undefined    1111 = Undefined
		 *
		 *     Registers may only be exchanged with registers of like size; i.e., 8-bit with
		 *     8-bit, or 16 with 16.
		 *
		 * EXG - Inherent - Opcode: 1E - MPU Cycles: 7 - No of bytes: 2
		 */
		case Opcodes::EXG_INHERENT:
			cycle_counter += 8;
			byte = read_and_advance_program_counter();
			source = byte >> 4;
			destination = byte & 0xF;

			if ((source & 0x8) and (destination & 0x8))
			{
				exchange_registers(static_cast<TransferExchangePostByte>(source), static_cast<TransferExchangePostByte>(destination));
			}
			break;

		/**
		 * TFR    Transfer register to register
		 * Source Form: TFR R1,R2
		 * Operation: R1 <- R2
		 * Condition Codes: Not Affected (unless R2 = CCR)
		 * Description:
		 *     Bits 7-4 of the immediate byte of the instruction define the source register,
		 *     while bits 3-0 define the destination register, as follows:
		 *
		 *     0000 = A:B          1000 = A
		 *     0001 = X            1001 = B
		 *     0010 = Y            1010 = CCR
		 *     0011 = US           1011 = DPR
		 *     0100 = SP           1100 = Undefined
		 *     0101 = PC           1101 = Undefined
		 *     0110 = Undefined    1110 = Undefined
		 *     0111 = Undefined    1111 = Undefined
		 *
		 *     Registers may only be transferred between registers of like size; i.e., 8-bit
		 *     with 8-bit, or 16 with 16.
		 *
		 * TFR - Inherent - Opcode: 1F - MPU Cycles: 7 - No of bytes: 2
		 */
		case Opcodes::TFR_INHERENT:
			cycle_counter += 6;
			byte = read_and_advance_program_counter();
			source = byte >> 4;
			destination = byte & 0xF;

			if ((source & 0x8) and (destination & 0x8))
			{
				write_to_register(static_cast<TransferExchangePostByte>(destination), read_from_register(static_cast<TransferExchangePostByte>(source)));
			}
			break;

		/**
		 * BRA    Branch always
		 * Source Forms: BRA DD; LBRA DDDD
		 * Operation: TEMP <- MI
		 *            PC <- PC + TEMP
		 * Condition CodeS: Not Affected
		 * Description:
		 *     Causes an unconditional branch.
		 *
		 * BRA - Relative - Opcode: 20 - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BRA_RELATIVE:
			cycle_counter += 3;
			offset = static_cast<std::int8_t>(read_and_advance_program_counter());
			program_counter_register += offset;
			break;

		/**
		 * BRN    Branch never
		 * Source Form: BRN DD; LBRN DDDD
		 * Operation: TEMP <- MI
		 * Condition Codes: Not Affected
		 * Description:
		 *     Does not cause a branch. This instruction is essentially a NO-OP, but has a bit
		 *     pattern logically related to BRA.
		 *
		 * BRN - Relative - Opcode: 21 - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BRN_RELATIVE:
			cycle_counter += 3;
			program_counter_register++;
			break;

		/**
		 * BHI    Branch if higher
		 * Source Forms: BHI DD; LBHI DDDD
		 * Operaiton: TEMP <- MI
		 *            if C ∨ Z = 0 then PC <- PC + TEMP
		 * Condition Codes: Not affected
		 * Description:
		 *     Causes a branch if the previous operation caused neither a carry nor a zero
		 *     result.
		 * Comments:
		 *     Used after a subtract or compare operation on unsigned binary values, this
		 *     instruction will "branch" if the register was higher than the memory operand. Not
		 *     useful, in general, after INC/DEC, LD/ST, TST/CLR/COM.
		 *
		 * BHI - Relative - Opcode: 22 - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BHI_RELATIVE:
			cycle_counter += 3;

			if (!condition_code_register->bits.carry && !condition_code_register->bits.zero)
			{
				branch_relative_offset();
			}

			program_counter_register++;
			break;

		/**
		 * BLS    Branch on lower or same
		 * Source Forms: BLS DD; LBLS DDDD
		 * Operation: TEMP <- MI
		 *            if C ∨ Z = 1 then PC <- PC + TEMP
		 * Condition Codes: Not affected
		 * Description:
		 *     Causes a branch if the previous operation caused either a carry or a zero result.
		 * Comments:
		 *     Used after a subtract or compare operation on unsigned binary values, this
		 *     instruction will "branch" if the register was lower than or the same as the
		 *     memory operand. Not useful, in general, after INC/DEC, LD/ST, TST/CLR/COM.
		 *
		 * BLS - Relative - Opcode: 23 - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BLS_RELATIVE:
			cycle_counter += 3;

			if (condition_code_register->bits.carry || condition_code_register->bits.zero)
			{
				branch_relative_offset();
			}

			program_counter_register++;
			break;

		/**
		 * BCC    Branch on carry clear
		 * Source Form: BCC DD; LBCC DDDD
		 * Operation: TEMP <- MI
		 *            if C = 0 then PC <- PC + TEMP
		 * Condition Codes: Not affected
		 * Description:
		 *     Tests the state of the C bit and causes a branch if C is clear.
		 * Comments:
		 *     When used after a subtract or compare on unsigned binary values, this instruction
		 *     could be called "branch" if the register was higher or the same as the memory
		 *     operand.
		 *
		 * Comments (BHS):
		 *     ... This is a duplicate assembly-language mnemonic for the single machine
		 *     instruction BCC. Not useful, in general, after INC/DEC, LD/ST, TST/CLR/COM.
		 *
		 * BCC - Relative - Opcode: 24 - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BHS_RELATIVE:
			cycle_counter += 3;

			if (!condition_code_register->bits.carry)
			{
				branch_relative_offset();
			}

			program_counter_register++;
			break;

		/**
		 * BCS    Branch on carry set
		 * Source Form: BCS DD; LBCS DDDD
		 * Operation: TEMP <- MI
		 *            if C = 1 then PC <- PC + TEMP
		 * Condition Codes: Not affected
		 * Description:
		 *     Tests the state of the C bit and causes a branch if C is set.
		 * Comments:
		 *     When used after a subtract or compare, on unsigned binary values, this
		 *     instruction could be called "branch" if the register was lower than the memory
		 *     operand.
		 * Comments on BLO:
		 *     Note that this is a duplicate assembly-language mnemonic for the single machine
		 *     instruction BCS. Not useful, in general, after INC/DEC, LD/ST, TST/CLR/COM.
		 *
		 * BCS - Relative - Opcode: 25 - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BLO_RELATIVE:
			cycle_counter += 3;

			if (condition_code_register->bits.carry)
			{
				branch_relative_offset();
			}

			program_counter_register++;
			break;

		/**
		 * BNE    Branch not equal
		 * Source Forms: BNE DD; LBNE DDDD
		 * Operation: TEMP <- MI
		 *            if Z = 0 then PC <- PC + TEMP
		 * Condition Codes: Not Affected
		 * Description:
		 *     Tests the state of the Z bit and causes a branch if the Z bit is clear.
		 * Comments:
		 *     Used after a subtract or compare operation on any binary values, this instruction
		 *     will "branch if the register is (or would be) not equal to the memory operand."
		 *
		 * BNE - Relative - Opcode: 26 - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BNE_RELATIVE:
			cycle_counter += 3;

			if (!condition_code_register->bits.zero)
			{
				branch_relative_offset();
			}

			program_counter_register++;
			break;

		/**
		 * BEQ    Branch on equal
		 * Source Forms: BEQ DD; LBEQ DDDD;
		 * Operation: TEMP <- MI
		 *            if Z = 1 then PC <- PC + TEMP
		 * Condition Codes: Not affected.
		 * Description:
		 *     Tests the state of the Z bit and causes a branch if the Z bit is set.
		 * Comments:
		 *     Used after a subtract or compare operation, this instruction will branch if the
		 *     compared values - signed or unsigned - were exactly the same.
		 *
		 * BEQ - Relative - Opcode: 27 - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BEQ_RELATIVE:
			cycle_counter += 3;

			if (condition_code_register->bits.zero)
			{
				branch_relative_offset();
			}

			program_counter_register++;
			break;

		/**
		 * BVC    Branch on overflow clear
		 * Source Form: BVC DD; LBVC DDDD
		 * Operation: TEMP <- MI
		 *            if V = 0 then PC <- PC + TEMP
		 * Condition Codes: Not Affected
		 * Description:
		 *     Tests the state of the V bit and causes a branch if the V bit is clear. That is,
		 *     branch if the two's complement result was valid.
		 * Comments:
		 *     Used after an operation on two's complement binary values, this instruction will
		 *     "branch if there was no overflow."
		 *
		 * BVC - Relative - Opcode: 28 - MPU Cycles 3 - No of bytes: 2
		 */
		case Opcodes::BVC_RELATIVE:
			cycle_counter += 3;

			if (!condition_code_register->bits.overflow)
			{
				branch_relative_offset();
			}

			program_counter_register++;
			break;

		/**
		 * BVS    Branch on overflow set
		 * Source Form: BVS DD; LBVS DDDD
		 * Operation: TEMP <- MI
		 *            if V = 1 then PC <- PC + TEMP
		 * Condition Codes: Not Affected
		 * Description:
		 *     Tests the state of the V bit and causes a branch if the V bit is set. That is,
		 *     branch if the two's complement result was invalid.
		 * Comments:
		 *     Used after an operation on two's complement binary values, this instruction will
		 *     "branch if there was an overflow." This instruction is also used after ASL or LSL
		 *     to detect binary floating-point normalization.
		 *
		 * BVS - Relative - Opcode: 29 - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BVS_RELATIVE:
			cycle_counter += 3;

			if (condition_code_register->bits.overflow)
			{
				branch_relative_offset();
			}

			program_counter_register++;
			break;

		/**
		 * BPL    Branch if plus
		 * Source Form: BPL DD; LBPL DDDD
		 * Operation: TEMP <- MI
		 *            if N = 0 then PC <- PC + TEMP
		 * Condition Codes: Not Affected
		 * Description:
		 *     Tests the state of the N bit and causes a branch if N is clear. That is, branch
		 *     if the sign of the two's complement result is positive.
		 * Comments:
		 *     Used after an operation on two's complement binary values, this instruction will
		 *     "branch if the possibly invalid result is positive."
		 *
		 * BPL - Relative - Opcode: 2A - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BPL_RELATIVE:
			cycle_counter += 3;

			if (!condition_code_register->bits.negative)
			{
				branch_relative_offset();
			}

			program_counter_register++;
			break;

		/**
		 * BMI    Branch on minus
		 * Source Form: BMI DD; LBMI DDDD
		 * Operation: TEMP <- MI
		 *            if N = 1 then PC <- PC + TEMP
		 * Condition Codes: Not Affected
		 * Description:
		 *     Tests the state of the N bit and causes a branch if N is set. That is, branch if
		 *     the sign of the two's complement result is negative.
		 * Comments:
		 *     Used after an operation on two's complement binary values, this instruction will
		 *     "branch if the (possibly invalid) result is minus."
		 *
		 * BMI - Relative - Opcode: 2B - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BMI_RELATIVE:
			cycle_counter += 3;

			if (condition_code_register->bits.negative)
			{
				branch_relative_offset();
			}

			program_counter_register++;
			break;

		/**
		 * BGE    Branch on greater than or equal to zero
		 * Source Forms: BGE DD; LBGE DDDD;
		 * Operation: TEMP <- MI
		 *            if N ^ V = 0 then PC <- PC + TEMP
		 * Condition Codes: Not affected
		 * Description:
		 *     Causes a branch if N and V are either both set or both clear. For example, branch
		 *     if the sign of a valid two's complement result is, or would be, positive.
		 * Comments:
		 *     Used after a subtract or compare operation on two's complement values, this
		 *     instruction will branch if the register was greater than or equal to the memory
		 *     operand.
		 *
		 * BGE - Relative - Opcode: 2C - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BGE_RELATIVE:
			cycle_counter += 3;

			if (condition_code_register->bits.negative == condition_code_register->bits.overflow)
			{
				branch_relative_offset();
			}

			program_counter_register++;
			break;

		/**
		 * BLT    Branch on less than zero
		 * Source Forms: BLT DD; LBLT DDDD;
		 * Operation: Temp <- MI
		 *            if N ^ V = 1 then PC <- PC + TEMP
		 * Condition Codes: Not affected
		 * Description:
		 *     Causes a branch if either, but not both, of the N or V bits is 1. That is, branch
		 *     if the sign of a valid two's complement result is - or would be - negative.
		 * Comments:
		 *     Used after a subtract or compare operation on two's complement binary values,
		 *     this instruction will "branch if the register was less than the memory operand."
		 *
		 * BLT - Relative - Opcode: 2D - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BLT_RELATIVE:
			cycle_counter += 3;

			if (condition_code_register->bits.negative != condition_code_register->bits.overflow)
			{
				branch_relative_offset();
			}

			program_counter_register++;
			break;

		/**
		 * BGT    Branch on greater
		 * Source Forms: BGT DD; LBGT DDDD;
		 * Operation: TEMP <- MI
		 *            if Z ∨ (N ^ V) = 0 then PC <- PC + TEMP
		 * Condition Codes: Not affected
		 * Description:
		 *     Causes a branch if (N and V are either both set or both clear) and Z is clear. In
		 *     other words, branch if the sign of a valid two's complemet result is, or would
		 *     be, positive and non-zero.
		 * Comments:
		 *     Used after a subtract or compare operation on two's complement values, this
		 *     instruction will "branch" if the register was greater than the memory operand.
		 *
		 * BGT - Relative - Opcode: 2E - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BGT_RELATIVE:
			cycle_counter += 3;

			if (!condition_code_register->bits.zero && (condition_code_register->bits.negative == condition_code_register->bits.overflow))
			{
				branch_relative_offset();
			}

			program_counter_register++;
			break;

		/**
		 * BLE    Branch on less than or equal to zero
		 * Source Form: BLE DD; LBLE DDDD;
		 * Operation: TEMP <- MI
		 *            if Z ∨ (N ^ V) = 1 then PC = PC + TEMP
		 * Condition Codes: Not Affected
		 * Description:
		 *     Causes a branch if the "Exclusive OR" of the N and V bits is 1 or if Z = 1. That
		 *     is, branch if the sign of a valid two's complement result is - or would be -
		 *     negative.
		 * Comments:
		 *     Used after a subtract or compare operation on two's complement values, this
		 *     instruction will "branch" if the register was less then or equal to the memory
		 *     operand.
		 *
		 * BLE - Relative - Opcode: 2F - MPU Cycles: 3 - No of bytes: 2
		 */
		case Opcodes::BLE_RELATIVE:
			cycle_counter += 3;

			if (condition_code_register->bits.zero || (condition_code_register->bits.negative != condition_code_register->bits.overflow))
			{
				branch_relative_offset();
			}

			program_counter_register++;
			break;

		/**
		 * LEA    Load effective address
		 * Source Form: LEAX; LEAY; LEAS; LEAU
		 * Operation: R' <- EA
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Not Affected
		 *     Z: LEAX, LEAY: Set if all bits of the result are Clear
		 *        LEAS, LEAU: Not Affected
		 *     V: Not Affected
		 *     C: Not Affected
		 * Description:
		 *     Form the effective address to data using the memory addressing mode. Load that
		 *     address, not the data itself, into the pointer register.
		 *
		 *     LEAX and LEAY affect Z to allow use as counters and for 6800 INX/DEX compatibilty
		 *     LEAU and LEAS do not affect Z to allow for cleaning up the stack while returning
		 *     Z as a parameter to a calling routine, and for 6800 INS/DES compatibility.
		 *
		 * LEAX - Indexed - Opcode: 30 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::LEAX_INDEXED:
			cycle_counter += 4;

			x_index_register = retrieve_effective_address();

			condition_code_register->set_zero(x_index_register == 0);
			break;

		/**
		 * LEA    Load effective address
		 * Source Form: LEAX; LEAY; LEAS; LEAU
		 * Operation: R' <- EA
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Not Affected
		 *     Z: LEAX, LEAY: Set if all bits of the result are Clear
		 *        LEAS, LEAU: Not Affected
		 *     V: Not Affected
		 *     C: Not Affected
		 * Description:
		 *     Form the effective address to data using the memory addressing mode. Load that
		 *     address, not the data itself, into the pointer register.
		 *
		 *     LEAX and LEAY affect Z to allow use as counters and for 6800 INX/DEX compatibilty
		 *     LEAU and LEAS do not affect Z to allow for cleaning up the stack while returning
		 *     Z as a parameter to a calling routine, and for 6800 INS/DES compatibility.
		 *
		 * LEAY - Indexed - Opcode: 31 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::LEAY_INDEXED:
			cycle_counter += 4;

			y_index_register = retrieve_effective_address();

			condition_code_register->set_zero(y_index_register == 0);
			break;

		/**
		 * LEA    Load effective address
		 * Source Form: LEAX; LEAY; LEAS; LEAU
		 * Operation: R' <- EA
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Not Affected
		 *     Z: LEAX, LEAY: Set if all bits of the result are Clear
		 *        LEAS, LEAU: Not Affected
		 *     V: Not Affected
		 *     C: Not Affected
		 * Description:
		 *     Form the effective address to data using the memory addressing mode. Load that
		 *     address, not the data itself, into the pointer register.
		 *
		 *     LEAX and LEAY affect Z to allow use as counters and for 6800 INX/DEX compatibilty
		 *     LEAU and LEAS do not affect Z to allow for cleaning up the stack while returning
		 *     Z as a parameter to a calling routine, and for 6800 INS/DES compatibility.
		 *
		 * LEAS - Indexed - Opcode: 32 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::LEAS_INDEXED:
			cycle_counter += 4;

			hardware_stack_pointer_register = retrieve_effective_address();

			condition_code_register->set_zero(hardware_stack_pointer_register == 0);
			break;

		/**
		 * LEA    Load effective address
		 * Source Form: LEAX; LEAY; LEAS; LEAU
		 * Operation: R' <- EA
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Not Affected
		 *     Z: LEAX, LEAY: Set if all bits of the result are Clear
		 *        LEAS, LEAU: Not Affected
		 *     V: Not Affected
		 *     C: Not Affected
		 * Description:
		 *     Form the effective address to data using the memory addressing mode. Load that
		 *     address, not the data itself, into the pointer register.
		 *
		 *     LEAX and LEAY affect Z to allow use as counters and for 6800 INX/DEX compatibilty
		 *     LEAU and LEAS do not affect Z to allow for cleaning up the stack while returning
		 *     Z as a parameter to a calling routine, and for 6800 INS/DES compatibility.
		 *
		 * LEAU - Indexed - Opcode: 33 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::LEAU_INDEXED:
			cycle_counter += 4;

			user_stack_pointer_register = retrieve_effective_address();

			condition_code_register->set_zero(user_stack_pointer_register == 0);
			break;

		/**
		 * PSHS    Push registers on the hardware stack
		 * Source Form: PSHS register list
		 *              PSHS #Label | PC | U | Y | X | DP | B | A | CC
		 *                          push order ->
		 * Operation:
		 *     if b7 of MI set, then: SP' <- SP - 1, (SP) <- PCL
		 *                            SP' <- SP - 1, (SP) <- PCH
		 *     if b6 of MI set, then: SP' <- SP - 1, (SP) <- USL
		 *                            SP' <- SP - 1, (SP) <- USH
		 *     if b5 of MI set, then: SP' <- SP - 1, (SP) <- IYL
		 *                            SP' <- SP - 1, (SP) <- IYH
		 *     if b4 of MI set, then: SP' <- SP - 1, (SP) <- IXL
		 *                            SP' <- SP - 1, (SP) <- IXH
		 *     if b3 of MI set, then: SP' <- SP - 1, (SP) <- DPR
		 *     if b2 of MI set, then: SP' <- SP - 1, (SP) <- ACCB
		 *     if b1 of MI set, then: SP' <- SP - 1, (SP) <- ACCA
		 *     if b0 of MI set, then: SP' <- SP - 1, (SP) <- CCR
		 * Condition Codes: Not Affected
		 * Description:
		 *     Any, all, any subset, or none of the MPU registers are pushed onto the hardware
		 *     stack, (excepting only the hardware stack pointer itself).
		 *
		 * PSHS - Inherent - Opcode: 34 - MPU Cycles: 5+ - No of bytes: 2
		 */
		case Opcodes::PSHS_INHERENT:
			cycle_counter += 5;
			postbyte.byte = read_and_advance_program_counter();

			if (postbyte.bits.program_counter_register)
			{
				cycle_counter += 2;
				push_program_counter_register();
			}

			if (postbyte.bits.s_u_stack_pointer_register)
			{
				cycle_counter += 2;
				push_onto_hardware_stack(user_stack_pointer_register & 0xFF);
				push_onto_hardware_stack(user_stack_pointer_register >> 8);
			}

			if (postbyte.bits.y_index_register)
			{
				cycle_counter += 2;
				push_onto_hardware_stack(y_index_register & 0xFF);
				push_onto_hardware_stack(y_index_register >> 8);
			}

			if (postbyte.bits.x_index_register)
			{
				cycle_counter += 2;
				push_onto_hardware_stack(x_index_register & 0xFF);
				push_onto_hardware_stack(x_index_register >> 8);
			}

			if (postbyte.bits.direct_page_register)
			{
				cycle_counter += 1;
				push_onto_hardware_stack(direct_page_register.container.reg);
			}

			if (postbyte.bits.b_accumulator)
			{
				cycle_counter += 1;
				push_onto_hardware_stack(accumulator_register.registers.b);
			}

			if (postbyte.bits.a_accumulator)
			{
				cycle_counter += 1;
				push_onto_hardware_stack(accumulator_register.registers.a);
			}

			if (postbyte.bits.condition_code_register)
			{
				cycle_counter += 1;
				push_onto_hardware_stack(condition_code_register->slot);
			}
			break;

		/**
		 * PULS    Pull registers from the hardware stack
		 * Source Form: PULS register list
		 *              PULS #Label | PC | U | Y | X | DP | B | A | CC
		 *                          <- pull order
		 * Operation:
		 *     if b0 of MI set, then: CCR' <- (SP), SP' <- SP + 1
		 *     if b1 of MI set, then: ACCA' <- (SP), SP' <- SP + 1
		 *     if b2 of MI set, then: ACCB' <- (SP), SP' <- SP + 1
		 *     if b3 of MI set, then: DP' <- (SP), SP' <- SP + 1
		 *     if b4 of MI set, then: IXH' <- (SP), SP' <- SP + 1
		 *                            IXL' <- (SP), SP' <- SP + 1
		 *     if b5 of MI set, then: IYH' <- (SP), SP' <- SP + 1
		 *                            IYL' <- (SP), SP' <- SP + 1
		 *     if b6 of MI set, then: USH' <- (SP), SP' <- SP + 1
		 *                            USL' <- (SP), SP' <- SP + 1
		 *     if b7 of MI set, then: PCH' <- (SP), SP' <- SP + 1
		 *                            PCL' <- (SP), SP' <- SP + 1
		 * Condition Codes: May be pulled from stack, otherwise unaffected.
		 * Description:
		 *     Any, all, any subset, or none of the MPU registers are pulled form the hardware
		 *     stack, (excepting only the hardware stack pointer itself). A single register may
		 *     be "pulled" with condition-flags set by loading auto-increment from stack.
		 *     (EX: LDA, S+)
		 *
		 * PULS - Inherent - Opcode: 35 - MPU Cycles: 5+ - No of bytes: 2
		 */
		case Opcodes::PULS_INHERENT:
			cycle_counter += 5;
			postbyte.byte = read_and_advance_program_counter();

			if (postbyte.bits.condition_code_register)
			{
				cycle_counter += 1;

				condition_code_register->set_register(pull_from_hardware_stack());
			}

			if (postbyte.bits.a_accumulator)
			{
				cycle_counter += 1;
				accumulator_register.registers.a = pull_from_hardware_stack();
			}

			if (postbyte.bits.b_accumulator)
			{
				cycle_counter += 1;
				accumulator_register.registers.b = pull_from_hardware_stack();
			}

			if (postbyte.bits.direct_page_register)
			{
				cycle_counter += 1;
				direct_page_register.container.reg = pull_from_hardware_stack();
			}

			if (postbyte.bits.x_index_register)
			{
				cycle_counter += 2;
				x_index_register = static_cast<std::uint16_t>(pull_from_hardware_stack());
				x_index_register <<= 8;
				x_index_register |= pull_from_hardware_stack();
			}

			if (postbyte.bits.y_index_register)
			{
				cycle_counter += 2;
				y_index_register = static_cast<std::uint16_t>(pull_from_hardware_stack());
				y_index_register <<= 8;
				y_index_register |= pull_from_hardware_stack();
			}

			if (postbyte.bits.s_u_stack_pointer_register)
			{
				cycle_counter += 2;
				user_stack_pointer_register = static_cast<std::uint16_t>(pull_from_hardware_stack());
				user_stack_pointer_register <<= 8;
				user_stack_pointer_register |= pull_from_hardware_stack();
			}

			if (postbyte.bits.program_counter_register)
			{
				cycle_counter += 2;
				program_counter_register = static_cast<std::uint16_t>(pull_from_hardware_stack());
				program_counter_register <<= 8;
				program_counter_register |= pull_from_hardware_stack();
			}
			break;

		/**
		 * PSHU    Push registers on the user stack
		 * Source Form: PSHU register list
		 *              PSHU #Label | PC | U | Y | X | DP | B | A | CC
		 *                          push order ->
		 * Operation:
		 *     if b7 of MI set, then: UP' <- UP - 1, (UP) <- PCL
		 *                            UP' <- UP - 1, (UP) <- PCH
		 *     if b6 of MI set, then: UP' <- UP - 1, (UP) <- USL
		 *                            UP' <- UP - 1, (UP) <- USH
		 *     if b5 of MI set, then: UP' <- UP - 1, (UP) <- IYL
		 *                            UP' <- UP - 1, (UP) <- IYH
		 *     if b4 of MI set, then: UP' <- UP - 1, (UP) <- IXL
		 *                            UP' <- UP - 1, (UP) <- IXH
		 *     if b3 of MI set, then: UP' <- UP - 1, (UP) <- DPR
		 *     if b2 of MI set, then: UP' <- UP - 1, (UP) <- ACCB
		 *     if b1 of MI set, then: UP' <- UP - 1, (UP) <- ACCA
		 *     if b0 of MI set, then: UP' <- UP - 1, (UP) <- CCR
		 * Condition CodeS: Not Affected
		 * Description:
		 *     Any, all, any subset, or none of the MPU registers are pushed onto the user stack
		 *     (excepting only the user stack pointer itself).
		 *
		 * PSHU - Inherent - Opcode: 36 - MPU Cycles: 5+ - No of bytes: 2
		 */
		case Opcodes::PSHU_INHERENT:
			cycle_counter += 5;
			postbyte.byte = read_and_advance_program_counter();

			if (postbyte.bits.program_counter_register)
			{
				cycle_counter += 2;
				push_onto_user_stack(program_counter_register & 0xFF);
				push_onto_user_stack(program_counter_register >> 8);
			}

			if (postbyte.bits.s_u_stack_pointer_register)
			{
				cycle_counter += 2;
				push_onto_user_stack(user_stack_pointer_register & 0xFF);
				push_onto_user_stack(user_stack_pointer_register >> 8);
			}

			if (postbyte.bits.y_index_register)
			{
				cycle_counter += 2;
				push_onto_user_stack(y_index_register & 0xFF);
				push_onto_user_stack(y_index_register >> 8);
			}

			if (postbyte.bits.x_index_register)
			{
				cycle_counter += 2;
				push_onto_user_stack(x_index_register & 0xFF);
				push_onto_user_stack(x_index_register >> 8);
			}

			if (postbyte.bits.direct_page_register)
			{
				cycle_counter += 1;
				push_onto_user_stack(direct_page_register.container.reg);
			}

			if (postbyte.bits.b_accumulator)
			{
				cycle_counter += 1;
				push_onto_user_stack(accumulator_register.registers.b);
			}

			if (postbyte.bits.a_accumulator)
			{
				cycle_counter += 1;
				push_onto_user_stack(accumulator_register.registers.a);
			}

			if (postbyte.bits.condition_code_register)
			{
				cycle_counter += 1;
				push_onto_user_stack(condition_code_register->slot);
			}
			break;

		/**
		 * PULU    Pull registers from the user stack
		 * Source Form: PULU register list
		 *              PULU #Label | PC | U | Y | X | DP | B | A | CC
		 *                          <- pull order
		 * Operation:
		 *     if b0 of MI set, then: CCR' <- (UP), UP' <- UP + 1
		 *     if b1 of MI set, then: ACCA' <- (UP), UP' <- UP + 1
		 *     if b2 of MI set, then: ACCB' <- (UP), UP' <- UP + 1
		 *     if b3 of MI set, then: DP' <- (UP), UP' <- UP + 1
		 *     if b4 of MI set, then: IXH' <- (UP), UP' <- UP + 1
		 *                            IXL' <- (UP), UP' <- UP + 1
		 *     if b5 of MI set, then: IYH' <- (UP), UP' <- UP + 1
		 *                            IYL' <- (UP), UP' <- UP + 1
		 *     if b6 of MI set, then: USH' <- (UP), UP' <- UP + 1
		 *                            USL' <- (UP), UP' <- UP + 1
		 *     if b7 of MI set, then: PCH' <- (UP), UP' <- UP + 1
		 *                            PCL' <- (UP), UP' <- UP + 1
		 * Condition Codes: May be pulled from stack, otherwise unaffected.
		 * Description:
		 *     Any, all, any subset, or none of the MPU registers are pulled form the hardware
		 *     stack, (excepting only the hardware stack pointer itself). A single register may
		 *     be "pulled" with condition-flags set by loading auto-increment from stack.
		 *     (EX: LDA, S+)
		 *
		 * PULU - Inherent - Opcode: 37 - MPU Cycles: 5+ - No of bytes: 2
		 */
		case Opcodes::PULU_INHERENT:
			cycle_counter += 5;
			postbyte.byte = read_and_advance_program_counter();

			if (postbyte.bits.condition_code_register)
			{
				cycle_counter += 1;

				condition_code_register->set_register(pull_from_user_stack());
			}

			if (postbyte.bits.a_accumulator)
			{
				cycle_counter += 1;
				accumulator_register.registers.a = pull_from_user_stack();
			}

			if (postbyte.bits.b_accumulator)
			{
				cycle_counter += 1;
				accumulator_register.registers.b = pull_from_user_stack();
			}

			if (postbyte.bits.direct_page_register)
			{
				cycle_counter += 1;
				direct_page_register.container.reg = pull_from_user_stack();
			}

			if (postbyte.bits.x_index_register)
			{
				cycle_counter += 2;
				x_index_register = static_cast<std::uint16_t>(pull_from_user_stack());
				x_index_register <<= 8;
				x_index_register |= pull_from_user_stack();
			}

			if (postbyte.bits.y_index_register)
			{
				cycle_counter += 2;
				y_index_register = static_cast<std::uint16_t>(pull_from_user_stack());
				y_index_register <<= 8;
				y_index_register |= pull_from_user_stack();
			}

			if (postbyte.bits.s_u_stack_pointer_register)
			{
				cycle_counter += 2;
				user_stack_pointer_register = static_cast<std::uint16_t>(pull_from_user_stack());
				user_stack_pointer_register <<= 8;
				user_stack_pointer_register |= pull_from_user_stack();
			}

			if (postbyte.bits.program_counter_register)
			{
				cycle_counter += 2;
				program_counter_register = static_cast<std::uint16_t>(pull_from_user_stack());
				program_counter_register <<= 8;
				program_counter_register |= pull_from_user_stack();
			}
			break;

		/**
		 * RTS    Return from subroutine
		 * Source Form: RTS
		 * Operation: PCH' <- (SP), SP' <- SP + 1
		 *            PCL' <- (SP), SP' <- SP + 1
		 * Condition Codes: Not affected
		 * Description:
		 *     Program control is returned from the subroutine to the calling program. The
		 * return address is pulled from the stack.
		 *
		 * RTS - Inherent - Opcode: 3B - MPU Cycles 6/15 - No of bytes: 1
		 */
		case Opcodes::RTS_INHERENT:
			cycle_counter += 5;
			program_counter_register = static_cast<std::uint16_t>(pull_from_hardware_stack());
			program_counter_register <<= 8;
			program_counter_register |= pull_from_hardware_stack();
			break;

		/**
		 * ABX    Add ACCB into IX
		 * Source Form: ABX
		 * Operation: IX' <- IX + ACCB
		 * Condition Codes: Not affected
		 * Description:
		 *     Add the 8-bit unsigned value in Accumulator B into the X index register.
		 *
		 * ABX - Inherent - Opcode: 3A - MPU Cycles: 3 - No of bytes: 1
		 */
		case Opcodes::ABX_INHERENT:
			cycle_counter += 3;
			x_index_register += accumulator_register.registers.b;
			break;

		/**
		 * RTI    Return from interrupt
		 * Source Form: RTI
		 * Operation: CCR' (SP), SP' <- SP + 1
		 *     if CCR bit E is SET then: ACCA' <- (SP), SP' <- SP + 1
		 *                               ACCB' <- (SP), SP' <- SP + 1
		 *                               DPR' <- (SP), SP' <- SP + 1
		 *                               IXH' <- (SP), SP' <- SP + 1
		 *                               IXL' <- (SP), SP' <- SP + 1
		 *                               IYH' <- (SP), SP' <- SP + 1
		 *                               IYL' <- (SP), SP' <- SP + 1
		 *                               USH' <- (SP), SP' <- SP + 1
		 *                               USL' <- (SP), SP' <- SP + 1
		 *                               PCH' <- (SP), SP' <- SP + 1
		 *                               PCL' <- (SP), SP' <- SP + 1
		 *     if CCR bit E is CLEAR then: PCH' <- (SP), SP' <- SP + 1
		 *                                 PCL' <- (SP), SP' <- SP + 1
		 * Condition Codes: Recovered from stack
		 * Description:
		 *     The saved machine state is recovered from the hardware stack and control is
		 * returned to the interrupted program. If the recovered E bit is CLEAR, it indicates
		 * that only a subset of the machine state was saved (return address and condition
		 * codes) and only that subset is to be recovered.
		 *
		 * RTI - Inherent - Opcode: 3B - MPU Cycles: 6(15) - No of bytes: 1
		 */
		case Opcodes::RTI_INHERENT:
			cycle_counter += 6;

			condition_code_register->set_register(pull_from_hardware_stack());

			if (condition_code_register->bits.entire)
			{
				cycle_counter += 9;
				pull_machine_state();
			}

			program_counter_register = static_cast<std::uint16_t>(pull_from_hardware_stack());
			program_counter_register <<= 8;
			program_counter_register |= pull_from_hardware_stack();
			break;

		/**
		 * CWAI    Clear and wait for interrupt
		 * Source Form: CWAI #$XX
		 * Operation: CCR <- CCR ∧ MI (Possibly clear masks)
		 *            Set E (entire state saved)
		 *            SP' <- SP - 1, (SP) <- PCL    FF = enable neither
		 *            SP' <- SP - 1, (SP) <- PCH    EF = enable IRQ
		 *            SP' <- SP - 1, (SP) <- USL    BF = enable FIRQ
		 *            SP' <- SP - 1, (SP) <- USH    AF = enable both
		 *            SP' <- SP - 1, (SP) <- IYL
		 *            SP' <- SP - 1, (SP) <- IYH
		 *            SP' <- SP - 1, (SP) <- IXL
		 *            SP' <- SP - 1, (SP) <- IXH
		 *            SP' <- SP - 1, (SP) <- DPR
		 *            SP' <- SP - 1, (SP) <- ACCB
		 *            SP' <- SP - 1, (SP) <- ACCA
		 *            SP' <- SP - 1, (SP) <- CCR
		 * Condition Codes: Possible cleared by the immediate byte
		 * Description:
		 *     The CWAI instruction ANDs an immediate byte with the condition code register
		 *     which may clear interrupt maskbit(s). It stacks the entire machine state on the
		 *     hardware stack and then looks for an interrupt. When a nonmasked interrupt
		 *     occurs, no further machine state will be saved before vectoring to the interrupt
		 *     handling routine. This instruction replaced the 6800's CLI WAI sequence, but does
		 *     not tri-state the buses.
		 * Comments:
		 *     An FIRQ interrupt may enter its interrupt handler with its entire machine state
		 *     saved. The RTI will automatically return the entire machine state after testing
		 *    the E bit of the recovered CCR.
		 *
		 * CWAI - Inherent - Opcode: 3C - MPU Cycles: 20 - No of bytes: 2
		 */
		case Opcodes::CWAI_INHERENT:
			cycle_counter = num_cycles_to_run;
			byte = read_and_advance_program_counter();

			condition_code_register->and_register(byte);
			condition_code_register->set_entire(true);

			push_machine_state();
			cwai = true;
			syncing = true;
			break;

		/**
		 * MUL    Multiply accumulators
		 * Source Form: MUL
		 * Operation: ACCA':ACCB' <- ACCA x ACCB
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Not Affected
		 *     Z: Set if all bits of the result are Clear
		 *     V: Not Affected
		 *     C: Set if ACCB bit 7 of result is Set
		 * Description:
		 *     Multiply the unsigned binary numbers in the accumulators and place the result in
		 *     both accumulators. Unsigned multiply allows multiple-precision operations. The
		 *     Carry flag allows rounding the MS byte through the sequence MUL, ADCA #0
		 *
		 * MUL - Inherent - Opcode: 3D - MPU Cycles: 11 - No of bytes: 1
		 */
		case Opcodes::MUL_INHERENT:
			cycle_counter += 11;
			accumulator_register.d = accumulator_register.registers.a * accumulator_register.registers.b;

			condition_code_register->set_zero(accumulator_register.d == 0);
			condition_code_register->set_carry(accumulator_register.d & 0x80);
			break;

		case Opcodes::RESET: // undocumented instruction
			break;

		/**
		 * SWI    Software Interrupt
		 * Source Form: SWI2
		 * Operation: Set E (entire state saved)
		 *            SP' <- SP - 1, (SP) <- PCL
		 *            SP' <- SP - 1, (SP) <- PCH
		 *            SP' <- SP - 1, (SP) <- USL
		 *            SP' <- SP - 1, (SP) <- USH
		 *            SP' <- SP - 1, (SP) <- IYL
		 *            SP' <- SP - 1, (SP) <- IYH
		 *            SP' <- SP - 1, (SP) <- IXL
		 *            SP' <- SP - 1, (SP) <- IXH
		 *            SP' <- SP - 1, (SP) <- DPR
		 *            SP' <- SP - 1, (SP) <- ACCB
		 *            SP' <- SP - 1, (SP) <- ACCA
		 *            SP' <- SP - 1, (SP) <- CCR
		 *            Set I, F (mask interrupts)
		 *            PC' <- (FFFA):(FFFB)
		 * Condition Codes: Not Affected
		 * Description:
		 *     All of the MPU registers are pushed onto the hardware stack (excepting only the
		 *     hardware stack pointer itself), and control is transferred through the SWI
		 *     vector.
		 *
		 *     SWI SETS I AND F BITS
		 *
		 * SWI - Inherent - Opcode: 3F - MPU Cycles: 19 - No of bytes: 1
		 */
		case Opcodes::SWI_INHERENT:
			cycle_counter += 19;

			condition_code_register->set_entire(true);

			push_machine_state();

			condition_code_register->set_irq_mask(true);
			condition_code_register->set_firq_mask(true);

			program_counter_register = read2(static_cast<std::uint16_t>(VectorTable::SOFTWARE_INTERRUPT));
			break;

		/**
		 * NEG   Negate
		 * Source Form: NEG Q
		 * Operation: M' <- 0 - M (i.e. M' <- ~M + 1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of result is set
		 *     Z: Set if all bits of result are Clear
		 *     V: Set if the original operand was 10000000
		 *     C: Set if the operation did not cause a cary from bit 7 in the ALU.
		 * Description:
		 *     Replaces the operand with its two's complement. The C-flag represents a borrow
		 *     and is set inverse to the resulting binary carry. Not that 80 (16) is replaced by
		 *     itself and only in this case is V Set. The value 00 (16) is also replaced by
		 *     itself, and only in this case is C cleared.
		 *
		 * NEGA - Inherent - Opcode: 40 - MPU Cycles 2 - No of bytes: 1
		 */
		case Opcodes::NEGA_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.a = negate(accumulator_register.registers.a);
			break;

		/**
		 * COM    Complement
		 * Source Form: COM Q; COMA; COMB
		 * Operation: M' (R') <- 0 + ~M (~R)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Set
		 * Description:
		 *     Replaces the contents of M or ACCX with its one's complement (also called the
		 *     logical complement). The carry flag is set for 6800 compatibility.
		 * Comments:
		 *     When operating on unsigned values, only BEQ and MBE branches can be expected to
		 *     behave properly. When operating on two's complement values, all signed branches
		 *     are available.
		 *
		 * COMA - Inherent - Opcode: 43 - MPY Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::COMA_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.a = complement(accumulator_register.registers.a);
			break;

		/**
		 * LSR    Logical shift right
		 * Source Form: LSR Q; LSRA; LSRB
		 * Operation:
		 *      _________________    _____
		 * 0 -> | | | | | | | | | -> | C |
		 *      -----------------    -----
		 *      b7             b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Cleared
		 *     Z: Set if all bits of the result are Clear
		 *     V: Not Affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Performs a logical shift right on the operand. Shifts a zero into bit 7 and bit 0
		 *     into the carry flag. The 6800 processor also affects the V flag.
		 *
		 * LSRA - Inherent - Opcode: 44 - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::LSRA_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.a = logical_shift_right(accumulator_register.registers.a);
			break;

		/**
		 * ROR    Rotate right
		 * Source Form: ROR Q; RORA; RORB
		 * Operation:
		 *       _____
		 *  -----| C |-----
		 *  |    -----    |
		 * _________________
		 * | | | | | | | | |
		 * -----------------
		 * b7      ->     b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Not Affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Rotates all bits of the operand right one place through the carry flag; this is
		 *     a nine-bit rotation. The 6800 processor also affects the V flag.
		 *
		 * RORA - Inherent - Opcode: 46 - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::RORA_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.a = rotate_right(accumulator_register.registers.a);
			break;

		/**
		 * ASR    Arithmetic shift right
		 * Source Form: ASR Q
		 * Operation:
		 * _____
		 * |   |
		 * |  _________________    _____
		 * -->| | | | | | | | | -> | C |
		 *    -----------------    -----
		 *    b7             b0
		 *
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set
		 *     Z: Set if all bits of result are clear
		 *     V: Not affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Shifts all bits of the operand right one place. Bit 7 is held constant. Bit 0 is
		 *     shifted into the carry flag. The 6800/01/02/03/08 processors do affect the V
		 *     flag.
		 *
		 * ASRA - Inherent - Opcode: 47 - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::ASRA_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.a = arithmetic_shift_right(accumulator_register.registers.a);
			break;

		/**
		 * ASL    Arithmetic shift left
		 * Source Form: ASL Q
		 * Operation:
		 * _____    _________________
		 * | C | <- | | | | | | | | | <- 0
		 * -----    -----------------
		 *          b7      <-     b0
		 *
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set
		 *     Z: Set if all bits of the result are clear
		 *     V: Loaded with the result of b7 ^ b0 of the original operand.
		 *     C: Loaded with bit 7 of the original operand.
		 * Description:
		 *     Shifts all bits of the operand one place to the left. Bit 0 is loaded with a
		 *     zero. Bit 7 of the operand is shifted into the carry flag.
		 *
		 * ASLA - Inherent - Opcode: 48 - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::ASLA_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.a = arithmetic_shift_left(accumulator_register.registers.a);
			break;

		/**
		 * ROL    Rotate left
		 * Source Form: ROL Q; ROLA; ROLB
		 * Operation:
		 *       _____
		 *  -----| C |-----
		 *  |    -----    |
		 * _________________
		 * | | | | | | | | |
		 * -----------------
		 * b7      <-     b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Loaded with the result of (b7 ^ b6) of the original operand.
		 *     C: Loaded with bit 7 of the original operand
		 * Description:
		 *     Rotate all bits of the operand one place left through the carry flag; this is a
		 *     9-bit rotation
		 *
		 * ROLA - Inherent - Opcode: 49 - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::ROLA_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.a = rotate_left(accumulator_register.registers.a);
			break;

		/**
		 * DEC    Decrement
		 * Source Form: DEC Q; DECA; DECB
		 * Operation: M' (R') <- M-1 (R-1)
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Set if the original operand was 10000000
		 *     C: Not affected
		 * Description:
		 *     Subtract one from the operand. The carry flag is not affected, thus allowing DEC
		 *     to be a loopcounter in multiple-precision computations.
		 * Comments:
		 *     When operating on unsigned values only BEQ and BNE branches can be expected to
		 *     behave consistently. When operating on two's complement values, all signed
		 *     branches are available.
		 *
		 * DECA - Inherent - Opcode: 4A - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::DECA_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.a = decrement(accumulator_register.registers.a);
			break;

		/**
		 * INC    Increment
		 * Source Form: INC Q; INCA, INCB
		 * Operation: M' (R') <- M + 1 (R + 1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the original operand was 01111111
		 *     C: Not Affected
		 * Description:
		 *     Add one to the operand. The carry flag is not affected, thus allowing INC to be
		 *     used as a loop-counter in multiple-precision computations.
		 * Comments:
		 *     When operating on unsigned values, only the BEQ and BNE branches can be expected
		 *     to behave consistently. When operating on two's complement values, all signed
		 *     branches are correctly available.
		 *
		 * INCA - Inherent - Opcode: 4C - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::INCA_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.a = increment(accumulator_register.registers.a);
			break;

		/**
		 * TST    Test
		 * Source Form: TST Q; TSTA; TSTB
		 * Operation: TEMP <- M - 0
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Set condition code flags N and Z according to the contents of M, and clear the V
		 *     flag. The 6800 processor clears the C flag.
		 * Comments:
		 *     The TST instruction provides only minimum information when testing unsigned
		 *     values; since no unsigned value is less than zero, BLO and BLS have no utility.
		 *     While BHI could be used after TST, it provides exactly the same control as BNE,
		 *     which is preferred. The signed branches are available.
		 *
		 * TSTA - Inherent - Opcode: 4D - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::TSTA_INHERENT:
			cycle_counter += 2;
			test(accumulator_register.registers.a);
			break;

		/**
		 * CLR    Clear
		 * Source Form: CLR Q, CLRA, CLRB
		 * Operation: TEMP <- M
		 *            M <- 00 (16)
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Cleared
		 *     Z: Set
		 *     V: Cleared
		 *     C: Cleared
		 * Description:
		 *     ACCX or M is loaded with 00000000. The C-flag is cleared for 6800 compatibility.
		 *
		 * CLRA - Inherent - Opcode: 4F - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::CLRA_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.a = 0;
			clear();
			break;

		/**
		 * NEG   Negate
		 * Source Form: NEG Q
		 * Operation: M' <- 0 - M (i.e. M' <- ~M + 1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of result is set
		 *     Z: Set if all bits of result are Clear
		 *     V: Set if the original operand was 10000000
		 *     C: Set if the operation did not cause a cary from bit 7 in the ALU.
		 * Description:
		 *     Replaces the operand with its two's complement. The C-flag represents a borrow
		 *     and is set inverse to the resulting binary carry. Not that 80 (16) is replaced by
		 *     itself and only in this case is V Set. The value 00 (16) is also replaced by
		 *     itself, and only in this case is C cleared.
		 *
		 * NEGB - Inherent - Opcode: 50 - MPU Cycles 2 - No of bytes: 1
		 */
		case Opcodes::NEGB_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.b = negate(accumulator_register.registers.b);
			break;

		/**
		 * COM    Complement
		 * Source Form: COM Q; COMA; COMB
		 * Operation: M' (R') <- 0 + ~M (~R)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Set
		 * Description:
		 *     Replaces the contents of M or ACCX with its one's complement (also called the
		 *     logical complement). The carry flag is set for 6800 compatibility.
		 * Comments:
		 *     When operating on unsigned values, only BEQ and MBE branches can be expected to
		 *     behave properly. When operating on two's complement values, all signed branches
		 *     are available.
		 *
		 * COMB - Inherent - Opcode: 53 - MPY Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::COMB_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.b = complement(accumulator_register.registers.b);
			break;

		/**
		 * LSR    Logical shift right
		 * Source Form: LSR Q; LSRA; LSRB
		 * Operation:
		 *      _________________    _____
		 * 0 -> | | | | | | | | | -> | C |
		 *      -----------------    -----
		 *      b7             b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Cleared
		 *     Z: Set if all bits of the result are Clear
		 *     V: Not Affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Performs a logical shift right on the operand. Shifts a zero into bit 7 and bit 0
		 *     into the carry flag. The 6800 processor also affects the V flag.
		 *
		 * LSRB - Inherent - Opcode: 54 - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::LSRB_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.b = logical_shift_right(accumulator_register.registers.b);
			break;

		/**
		 * ROR    Rotate right
		 * Source Form: ROR Q; RORA; RORB
		 * Operation:
		 *       _____
		 *  -----| C |-----
		 *  |    -----    |
		 * _________________
		 * | | | | | | | | |
		 * -----------------
		 * b7      ->     b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Not Affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Rotates all bits of the operand right one place through the carry flag; this is
		 *     a nine-bit rotation. The 6800 processor also affects the V flag.
		 *
		 * RORB - Inherent - Opcode: 56 - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::RORB_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.b = rotate_right(accumulator_register.registers.b);
			break;

		/**
		 * ASR    Arithmetic shift right
		 * Source Form: ASR Q
		 * Operation:
		 * _____
		 * |   |
		 * |  _________________    _____
		 * -->| | | | | | | | | -> | C |
		 *    -----------------    -----
		 *    b7             b0
		 *
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set
		 *     Z: Set if all bits of result are clear
		 *     V: Not affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Shifts all bits of the operand right one place. Bit 7 is held constant. Bit 0 is
		 *     shifted into the carry flag. The 6800/01/02/03/08 processors do affect the V
		 *     flag.
		 *
		 * ASRB - Direct - Opcode: 57 - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::ASRB_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.b = arithmetic_shift_right(accumulator_register.registers.b);
			break;

		/**
		 * ASL    Arithmetic shift left
		 * Source Form: ASL Q
		 * Operation:
		 * _____    _________________
		 * | C | <- | | | | | | | | | <- 0
		 * -----    -----------------
		 *          b7      <-     b0
		 *
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set
		 *     Z: Set if all bits of the result are clear
		 *     V: Loaded with the result of b7 ^ b0 of the original operand.
		 *     C: Loaded with bit 7 of the original operand.
		 * Description:
		 *     Shifts all bits of the operand one place to the left. Bit 0 is loaded with a
		 *     zero. Bit 7 of the operand is shifted into the carry flag.
		 *
		 * ASLB - Inherent - Opcode: 58 - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::ASLB_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.b = arithmetic_shift_left(accumulator_register.registers.b);
			break;

		/**
		 * ROL    Rotate left
		 * Source Form: ROL Q; ROLA; ROLB
		 * Operation:
		 *       _____
		 *  -----| C |-----
		 *  |    -----    |
		 * _________________
		 * | | | | | | | | |
		 * -----------------
		 * b7      <-     b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Loaded with the result of (b7 ^ b6) of the original operand.
		 *     C: Loaded with bit 7 of the original operand
		 * Description:
		 *     Rotate all bits of the operand one place left through the carry flag; this is a
		 *     9-bit rotation
		 *
		 * ROLB - Inherent - Opcode: 59 - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::ROLB_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.b = rotate_left(accumulator_register.registers.b);
			break;

		/**
		 * DEC    Decrement
		 * Source Form: DEC Q; DECA; DECB
		 * Operation: M' (R') <- M-1 (R-1)
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Set if the original operand was 10000000
		 *     C: Not affected
		 * Description:
		 *     Subtract one from the operand. The carry flag is not affected, thus allowing DEC
		 *     to be a loopcounter in multiple-precision computations.
		 * Comments:
		 *     When operating on unsigned values only BEQ and BNE branches can be expected to
		 *     behave consistently. When operating on two's complement values, all signed
		 *     branches are available.
		 *
		 * DECB - Inherent - Opcode: 5A - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::DECB_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.b = decrement(accumulator_register.registers.b);
			break;

		/**
		 * INC    Increment
		 * Source Form: INC Q; INCA, INCB
		 * Operation: M' (R') <- M + 1 (R + 1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the original operand was 01111111
		 *     C: Not Affected
		 * Description:
		 *     Add one to the operand. The carry flag is not affected, thus allowing INC to be
		 *     used as a loop-counter in multiple-precision computations.
		 * Comments:
		 *     When operating on unsigned values, only the BEQ and BNE branches can be expected
		 *     to behave consistently. When operating on two's complement values, all signed
		 *     branches are correctly available.
		 *
		 * INCB - Inherent - Opcode: 5C - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::INCB_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.b = increment(accumulator_register.registers.b);
			break;

			/**
			 * TST    Test
			 * Source Form: TST Q; TSTA; TSTB
			 * Operation: TEMP <- M - 0
			 * Condition Codes:
			 *     H: Not Affected
			 *     N: Set if bit 7 of the result is Set
			 *     Z: Set if all bits of the result are Clear
			 *     V: Cleared
			 *     C: Not Affected
			 * Description:
			 *     Set condition code flags N and Z according to the contents of M, and clear the V
			 *     flag. The 6800 processor clears the C flag.
			 * Comments:
			 *     The TST instruction provides only minimum information when testing unsigned
			 *     values; since no unsigned value is less than zero, BLO and BLS have no utility.
			 *     While BHI could be used after TST, it provides exactly the same control as BNE,
			 *     which is preferred. The signed branches are available.
			 *
			 * TSTB - Inherent - Opcode: 5D - MPU Cycles: 2 - No of bytes: 1
			 */
		case Opcodes::TSTB_INHERENT:
			cycle_counter += 2;
			test(accumulator_register.registers.b);
			break;

		/**
		 * CLR    Clear
		 * Source Form: CLR Q, CLRA, CLRB
		 * Operation: TEMP <- M
		 *            M <- 00 (16)
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Cleared
		 *     Z: Set
		 *     V: Cleared
		 *     C: Cleared
		 * Description:
		 *     ACCX or M is loaded with 00000000. The C-flag is cleared for 6800 compatibility.
		 *
		 * CLRB - Inherent - Opcode: 5F - MPU Cycles: 2 - No of bytes: 1
		 */
		case Opcodes::CLRB_INHERENT:
			cycle_counter += 2;
			accumulator_register.registers.b = 0;
			clear();
			break;

		/**
		 * NEG   Negate
		 * Source Form: NEG Q
		 * Operation: M' <- 0 - M (i.e. M' <- ~M + 1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of result is set
		 *     Z: Set if all bits of result are Clear
		 *     V: Set if the original operand was 10000000
		 *     C: Set if the operation did not cause a cary from bit 7 in the ALU.
		 * Description:
		 *     Replaces the operand with its two's complement. The C-flag represents a borrow
		 *     and is set inverse to the resulting binary carry. Not that 80 (16) is replaced by
		 *     itself and only in this case is V Set. The value 00 (16) is also replaced by
		 *     itself, and only in this case is C cleared.
		 *
		 * NEG - Indexed - Opcode: 60 - MPU Cycles 6+ - No of bytes: 2+
		 */
		case Opcodes::NEG_INDEXED:
			cycle_counter += 6;
			address = retrieve_effective_address();
			write(address, negate(read(address)));
			break;

		/**
		 * COM    Complement
		 * Source Form: COM Q; COMA; COMB
		 * Operation: M' (R') <- 0 + ~M (~R)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Set
		 * Description:
		 *     Replaces the contents of M or ACCX with its one's complement (also called the
		 *     logical complement). The carry flag is set for 6800 compatibility.
		 * Comments:
		 *     When operating on unsigned values, only BEQ and MBE branches can be expected to
		 *     behave properly. When operating on two's complement values, all signed branches
		 *     are available.
		 *
		 * COM - Indexed - Opcode: 63 - MPY Cycles: 6+ - No of bytes: 2+
		 */
		case Opcodes::COM_INDEXED:
			cycle_counter += 6;
			address = retrieve_effective_address();
			write(address, complement(read(address)));
			break;

		/**
		 * LSR    Logical shift right
		 * Source Form: LSR Q; LSRA; LSRB
		 * Operation:
		 *      _________________    _____
		 * 0 -> | | | | | | | | | -> | C |
		 *      -----------------    -----
		 *      b7             b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Cleared
		 *     Z: Set if all bits of the result are Clear
		 *     V: Not Affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Performs a logical shift right on the operand. Shifts a zero into bit 7 and bit 0
		 *     into the carry flag. The 6800 processor also affects the V flag.
		 *
		 * LSR - Indexed - Opcode: 64 - MPU Cycles: 6+ - No of bytes: 2+
		 */
		case Opcodes::LSR_INDEXED:
			cycle_counter += 6;
			address = retrieve_effective_address();
			write(address, logical_shift_right(read(address)));
			break;

		/**
		 * ROR    Rotate right
		 * Source Form: ROR Q; RORA; RORB
		 * Operation:
		 *       _____
		 *  -----| C |-----
		 *  |    -----    |
		 * _________________
		 * | | | | | | | | |
		 * -----------------
		 * b7      ->     b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Not Affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Rotates all bits of the operand right one place through the carry flag; this is
		 *     a nine-bit rotation. The 6800 processor also affects the V flag.
		 *
		 * ROR - Indexed - Opcode: 66 - MPU Cycles: 6+ - No of bytes: 2+
		 */
		case Opcodes::ROR_INDEXED:
			cycle_counter += 6;
			address = retrieve_effective_address();
			write(address, rotate_right(read(address)));
			break;

		/**
		 * ASR    Arithmetic shift right
		 * Source Form: ASR Q
		 * Operation:
		 * _____
		 * |   |
		 * |  _________________    _____
		 * -->| | | | | | | | | -> | C |
		 *    -----------------    -----
		 *    b7             b0
		 *
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set
		 *     Z: Set if all bits of result are clear
		 *     V: Not affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Shifts all bits of the operand right one place. Bit 7 is held constant. Bit 0 is
		 *     shifted into the carry flag. The 6800/01/02/03/08 processors do affect the V
		 *     flag.
		 *
		 * ASR - Indexed - Opcode: 67 - MPU Cycles: 6+ - No of bytes: 2+
		 */
		case Opcodes::ASR_INDEXED:
			cycle_counter += 6;
			address = retrieve_effective_address();
			write(address, arithmetic_shift_right(read(address)));
			break;

		/**
		 * ASL    Arithmetic shift left
		 * Source Form: ASL Q
		 * Operation:
		 * _____    _________________
		 * | C | <- | | | | | | | | | <- 0
		 * -----    -----------------
		 *          b7      <-     b0
		 *
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set
		 *     Z: Set if all bits of the result are clear
		 *     V: Loaded with the result of b7 ^ b0 of the original operand.
		 *     C: Loaded with bit 7 of the original operand.
		 * Description:
		 *     Shifts all bits of the operand one place to the left. Bit 0 is loaded with a
		 *     zero. Bit 7 of the operand is shifted into the carry flag.
		 *
		 * ASL - Indexed - Opcode: 68 - MPU Cycles: 6+ - No of bytes: 2+
		 */
		case Opcodes::ASL_INDEXED:
			cycle_counter += 6;
			address = retrieve_effective_address();
			write(address, arithmetic_shift_left(read(address)));
			break;

		/**
		 * ROL    Rotate left
		 * Source Form: ROL Q; ROLA; ROLB
		 * Operation:
		 *       _____
		 *  -----| C |-----
		 *  |    -----    |
		 * _________________
		 * | | | | | | | | |
		 * -----------------
		 * b7      <-     b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Loaded with the result of (b7 ^ b6) of the original operand.
		 *     C: Loaded with bit 7 of the original operand
		 * Description:
		 *     Rotate all bits of the operand one place left through the carry flag; this is a
		 *     9-bit rotation
		 *
		 * ROL - Indexed - Opcode: 69 - MPU Cycles: 6+ - No of bytes: 2+
		 */
		case Opcodes::ROL_INDEXED:
			cycle_counter += 6;
			address = retrieve_effective_address();
			write(address, rotate_left(read(address)));
			break;

		/**
		 * DEC    Decrement
		 * Source Form: DEC Q; DECA; DECB
		 * Operation: M' (R') <- M-1 (R-1)
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Set if the original operand was 10000000
		 *     C: Not affected
		 * Description:
		 *     Subtract one from the operand. The carry flag is not affected, thus allowing DEC
		 *     to be a loopcounter in multiple-precision computations.
		 * Comments:
		 *     When operating on unsigned values only BEQ and BNE branches can be expected to
		 *     behave consistently. When operating on two's complement values, all signed
		 *     branches are available.
		 *
		 * DEC - Indexed - Opcode: 6A - MPU Cycles: 6+ - No of bytes: 2+
		 */
		case Opcodes::DEC_INDEXED:
			cycle_counter += 6;
			address = retrieve_effective_address();
			write(address, decrement(read(address)));
			break;

		/**
		 * INC    Increment
		 * Source Form: INC Q; INCA, INCB
		 * Operation: M' (R') <- M + 1 (R + 1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the original operand was 01111111
		 *     C: Not Affected
		 * Description:
		 *     Add one to the operand. The carry flag is not affected, thus allowing INC to be
		 *     used as a loop-counter in multiple-precision computations.
		 * Comments:
		 *     When operating on unsigned values, only the BEQ and BNE branches can be expected
		 *     to behave consistently. When operating on two's complement values, all signed
		 *     branches are correctly available.
		 *
		 * INC - Indexed - Opcode: 6C - MPU Cycles: 6+ - No of bytes: 2+
		 */
		case Opcodes::INC_INDEXED:
			cycle_counter += 6;
			address = retrieve_effective_address();
			write(address, increment(read(address)));
			break;

		/**
		 * TST    Test
		 * Source Form: TST Q; TSTA; TSTB
		 * Operation: TEMP <- M - 0
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Set condition code flags N and Z according to the contents of M, and clear the V
		 *     flag. The 6800 processor clears the C flag.
		 * Comments:
		 *     The TST instruction provides only minimum information when testing unsigned
		 *     values; since no unsigned value is less than zero, BLO and BLS have no utility.
		 *     While BHI could be used after TST, it provides exactly the same control as BNE,
		 *     which is preferred. The signed branches are available.
		 *
		 * TST - Indexed - Opcode: 6D - MPU Cycles: 6+ - No of bytes: 2+
		 */
		case Opcodes::TST_INDEXED:
			cycle_counter += 6;
			test(read(retrieve_effective_address()));
			break;

		/**
		 * JMP    Jump to effective address
		 * Source Form: JMP
		 * Operation: PC <- EA
		 * Condition Codes: Not Affected
		 * Description:
		 *     Program control is transferred to the location equivalent to the effective
		 *     address
		 *
		 * JMP - Indexed - Opcode: 6E - MPU Cycles: 3+ - No of bytes: 2+
		 */
		case Opcodes::JMP_INDEXED:
			cycle_counter += 3;
			program_counter_register = retrieve_effective_address();
			break;

		/**
		 * CLR    Clear
		 * Source Form: CLR Q, CLRA, CLRB
		 * Operation: TEMP <- M
		 *            M <- 00 (16)
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Cleared
		 *     Z: Set
		 *     V: Cleared
		 *     C: Cleared
		 * Description:
		 *     ACCX or M is loaded with 00000000. The C-flag is cleared for 6800 compatibility.
		 *
		 * CLR - Indexed - Opcode: 7F - MPU Cycles: 7 - No of bytes: 3
		 */
		case Opcodes::CLR_INDEXED:
			cycle_counter += 6;
			write(retrieve_effective_address(), 0);
			clear();
			break;

		/**
		 * NEG   Negate
		 * Source Form: NEG Q
		 * Operation: M' <- 0 - M (i.e. M' <- ~M + 1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of result is set
		 *     Z: Set if all bits of result are Clear
		 *     V: Set if the original operand was 10000000
		 *     C: Set if the operation did not cause a cary from bit 7 in the ALU.
		 * Description:
		 *     Replaces the operand with its two's complement. The C-flag represents a borrow
		 *     and is set inverse to the resulting binary carry. Not that 80 (16) is replaced by
		 *     itself and only in this case is V Set. The value 00 (16) is also replaced by
		 *     itself, and only in this case is C cleared.
		 *
		 * NEG - Extended - Opcode: 70 - MPU Cycles 7 - No of bytes: 3
		 */
		case Opcodes::NEG_EXTENDED:
			cycle_counter += 7;
			address = read2_and_advance_program_counter();
			write(address, negate(read(address)));
			break;

		/**
		 * COM    Complement
		 * Source Form: COM Q; COMA; COMB
		 * Operation: M' (R') <- 0 + ~M (~R)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Set
		 * Description:
		 *     Replaces the contents of M or ACCX with its one's complement (also called the
		 *     logical complement). The carry flag is set for 6800 compatibility.
		 * Comments:
		 *     When operating on unsigned values, only BEQ and MBE branches can be expected to
		 *     behave properly. When operating on two's complement values, all signed branches
		 *     are available.
		 *
		 * COM - Extended - Opcode: 73 - MPY Cycles: 7 - No of bytes: 3
		 */
		case Opcodes::COM_EXTENDED:
			cycle_counter += 7;
			address = read2_and_advance_program_counter();
			write(address, complement(read(address)));
			break;

		/**
		 * LSR    Logical shift right
		 * Source Form: LSR Q; LSRA; LSRB
		 * Operation:
		 *      _________________    _____
		 * 0 -> | | | | | | | | | -> | C |
		 *      -----------------    -----
		 *      b7             b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Cleared
		 *     Z: Set if all bits of the result are Clear
		 *     V: Not Affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Performs a logical shift right on the operand. Shifts a zero into bit 7 and bit 0
		 *     into the carry flag. The 6800 processor also affects the V flag.
		 *
		 * LSR - Extended - Opcode: 74 - MPU Cycles: 7 - No of bytes: 3
		 */
		case Opcodes::LSR_EXTENDED:
			cycle_counter += 7;
			address = read2_and_advance_program_counter();
			write(address, logical_shift_right(read(address)));
			break;

		/**
		 * ROR    Rotate right
		 * Source Form: ROR Q; RORA; RORB
		 * Operation:
		 *       _____
		 *  -----| C |-----
		 *  |    -----    |
		 * _________________
		 * | | | | | | | | |
		 * -----------------
		 * b7      ->     b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Not Affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Rotates all bits of the operand right one place through the carry flag; this is
		 *     a nine-bit rotation. The 6800 processor also affects the V flag.
		 *
		 * ROR - Extended - Opcode: 76 - MPU Cycles: 7 - No of bytes: 3
		 */
		case Opcodes::ROR_EXTENDED:
			cycle_counter += 7;
			address = read2_and_advance_program_counter();
			write(address, rotate_right(read(address)));
			break;

		/**
		 * ASR    Arithmetic shift right
		 * Source Form: ASR Q
		 * Operation:
		 * _____
		 * |   |
		 * |  _________________    _____
		 * -->| | | | | | | | | -> | C |
		 *    -----------------    -----
		 *    b7             b0
		 *
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set
		 *     Z: Set if all bits of result are clear
		 *     V: Not affected
		 *     C: Loaded with bit 0 of the original operand
		 * Description:
		 *     Shifts all bits of the operand right one place. Bit 7 is held constant. Bit 0 is
		 *     shifted into the carry flag. The 6800/01/02/03/08 processors do affect the V
		 *     flag.
		 *
		 * ASR - Extended - Opcode: 77 - MPU Cycles: 7 - No of bytes: 3
		 */
		case Opcodes::ASR_EXTENDED:
			cycle_counter += 7;
			address = read2_and_advance_program_counter();
			write(address, arithmetic_shift_right(read(address)));
			break;

		/**
		 * ASL    Arithmetic shift left
		 * Source Form: ASL Q
		 * Operation:
		 * _____    _________________
		 * | C | <- | | | | | | | | | <- 0
		 * -----    -----------------
		 *          b7      <-     b0
		 *
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set
		 *     Z: Set if all bits of the result are clear
		 *     V: Loaded with the result of b7 ^ b0 of the original operand.
		 *     C: Loaded with bit 7 of the original operand.
		 * Description:
		 *     Shifts all bits of the operand one place to the left. Bit 0 is loaded with a
		 *     zero. Bit 7 of the operand is shifted into the carry flag.
		 *
		 * ASL - Extended - Opcode: 78 - MPU Cycles: 7 - No of bytes: 3
		 */
		case Opcodes::ASL_EXTENDED:
			cycle_counter += 7;
			address = read2_and_advance_program_counter();
			write(address, arithmetic_shift_left(read(address)));
			break;

		/**
		 * ROL    Rotate left
		 * Source Form: ROL Q; ROLA; ROLB
		 * Operation:
		 *       _____
		 *  -----| C |-----
		 *  |    -----    |
		 * _________________
		 * | | | | | | | | |
		 * -----------------
		 * b7      <-     b0
		 *
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Loaded with the result of (b7 ^ b6) of the original operand.
		 *     C: Loaded with bit 7 of the original operand
		 * Description:
		 *     Rotate all bits of the operand one place left through the carry flag; this is a
		 *     9-bit rotation
		 *
		 * ROL - Extended - Opcode: 79 - MPU Cycles: 7 - No of bytes: 3
		 */
		case Opcodes::ROL_EXTENDED:
			cycle_counter += 7;
			address = read2_and_advance_program_counter();
			write(address, rotate_left(read(address)));
			break;

		/**
		 * DEC    Decrement
		 * Source Form: DEC Q; DECA; DECB
		 * Operation: M' (R') <- M-1 (R-1)
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Set if the original operand was 10000000
		 *     C: Not affected
		 * Description:
		 *     Subtract one from the operand. The carry flag is not affected, thus allowing DEC
		 *     to be a loopcounter in multiple-precision computations.
		 * Comments:
		 *     When operating on unsigned values only BEQ and BNE branches can be expected to
		 *     behave consistently. When operating on two's complement values, all signed
		 *     branches are available.
		 *
		 * DEC - Extended - Opcode: 7A - MPU Cycles: 7 - No of bytes: 3
		 */
		case Opcodes::DEC_EXTENDED:
			cycle_counter += 7;
			address = read2_and_advance_program_counter();
			write(address, decrement(read(address)));
			break;

		/**
		 * INC    Increment
		 * Source Form: INC Q; INCA, INCB
		 * Operation: M' (R') <- M + 1 (R + 1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the original operand was 01111111
		 *     C: Not Affected
		 * Description:
		 *     Add one to the operand. The carry flag is not affected, thus allowing INC to be
		 *     used as a loop-counter in multiple-precision computations.
		 * Comments:
		 *     When operating on unsigned values, only the BEQ and BNE branches can be expected
		 *     to behave consistently. When operating on two's complement values, all signed
		 *     branches are correctly available.
		 *
		 * INC - Extended - Opcode: 7C - MPU Cycles: 7 - No of bytes: 3
		 */
		case Opcodes::INC_EXTENDED:
			cycle_counter += 7;
			address = read2_and_advance_program_counter();
			write(address, increment(read(address)));
			break;

		/**
		 * TST    Test
		 * Source Form: TST Q; TSTA; TSTB
		 * Operation: TEMP <- M - 0
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Set condition code flags N and Z according to the contents of M, and clear the V
		 *     flag. The 6800 processor clears the C flag.
		 * Comments:
		 *     The TST instruction provides only minimum information when testing unsigned
		 *     values; since no unsigned value is less than zero, BLO and BLS have no utility.
		 *     While BHI could be used after TST, it provides exactly the same control as BNE,
		 *     which is preferred. The signed branches are available.
		 *
		 * TST - Extended - Opcode: 7D - MPU Cycles: 7 - No of bytes: 3
		 */
		case Opcodes::TST_EXTENDED:
			cycle_counter += 7;
			test(read(read2_and_advance_program_counter()));
			break;

		/**
		 * JMP    Jump to effective address
		 * Source Form: JMP
		 * Operation: PC <- EA
		 * Condition Codes: Not Affected
		 * Description:
		 *     Program control is transferred to the location equivalent to the effective
		 *     address
		 *
		 * JMP - Extended - Opcode: 7E - MPU Cycles: 4 - No of bytes: 3
		 */
		case Opcodes::JMP_EXTENDED:
			cycle_counter += 4;
			program_counter_register = read2(program_counter_register);
			break;

		/**
		 * CLR    Clear
		 * Source Form: CLR Q, CLRA, CLRB
		 * Operation: TEMP <- M
		 *            M <- 00 (16)
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Cleared
		 *     Z: Set
		 *     V: Cleared
		 *     C: Cleared
		 * Description:
		 *     ACCX or M is loaded with 00000000. The C-flag is cleared for 6800 compatibility.
		 *
		 * CLR - Extended - Opcode: 6F - MPU Cycles: 6+ - No of bytes: 2+
		 */
		case Opcodes::CLR_EXTENDED:
			cycle_counter += 7;
			write(read2_and_advance_program_counter(), 0);
			clear();
			break;

		/**
		 * SUB    Subtract memory from register
		 * Source Forms: SUBA P; SUBB P; SUBD P
		 * Operation: R' <- R - M
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16)-bit two's complement overflow
		 *     C: Set if the operation did not cause a carry from bit 7 in the ALU
		 * Description:
		 *     Subtracts the value in M from teh contents of an 8 (16)-bit register. The C flag
		 *     represents a borrow and is set inverse to the resulting carry.
		 *
		 * SUBA - Immediate - Opcode: 80 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::SUBA_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.a = subtract(accumulator_register.registers.a, read_and_advance_program_counter());
			break;

		/**
		 * CMP    Compare memory from a register
		 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
		 * Operation: TEMP <- R - M(:M+1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set.
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
		 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
		 * Description:
		 *     Compares the contents of M from teh contents of the specified register and sets
		 *     appropriate condition codes. Neither M nor R is modified. The C flag represents a
		 *     borrow and is set inverse to the resulting binary carry.
		 *
		 * CMPA - Immediate - Opcode: 81 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::CMPA_IMMEDIATE:
			cycle_counter += 2;
			compare(accumulator_register.registers.a, read_and_advance_program_counter());
			break;

		/**
		 * SBC    Subtract with carry memory into register
		 * Source Form: SBCA P; SBCB P
		 * Operation: R' <- R - M - C
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Subtracts the contents of the carry flag and the memory byte into an 8-bit
		 *     register.
		 *
		 * SBCA - Immediate - Opcode: 82 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::SBCA_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.a = subtract(
				accumulator_register.registers.a,
				read_and_advance_program_counter() + (condition_code_register->bits.carry ? 1 : 0));
			break;

		/**
		 * SUB    Subtract memory from register
		 * Source Forms: SUBA P; SUBB P; SUBD P
		 * Operation: R' <- R - M
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16)-bit two's complement overflow
		 *     C: Set if the operation did not cause a carry from bit 7 in the ALU
		 * Description:
		 *     Subtracts the value in M from teh contents of an 8 (16)-bit register. The C flag
		 *     represents a borrow and is set inverse to the resulting carry.
		 *
		 * SUBD - Immediate - Opcode: 83 - MPU Cycles: 4 - No of bytes: 3
		 */
		case Opcodes::SUBD_IMMEDIATE:
			cycle_counter += 4;
			accumulator_register.d = subtract(accumulator_register.d, read2_and_advance_program_counter());
			break;

		/**
		 * AND    Logical AND memory into register
		 * Source Forms: ANDA P; ANDB P
		 * Operation: R' <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs the logical "AND" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ANDA - Immediate - Opcode: 84 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::ANDA_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.a &= read_and_advance_program_counter();
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * BIT    Bit test
		 * Source Form: BITA P; BITB P
		 * Operation: TEMP <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs the logical "AND" of the contents of ACCX and the contents of M and
		 *     modifies condition codes accordingly. The contents of ACCX or M are not affected.
		 *
		 * BITA - Immediate - Opcode: 85 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::BITA_IMMEDIATE:
			cycle_counter += 2;
			condition_register(static_cast<std::uint8_t>(accumulator_register.registers.a & read_and_advance_program_counter()));
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDA - Immediate - Opcode: 86 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::LDA_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.a = read_and_advance_program_counter();
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * EOR    Exclusive or
		 * Source Forms: EORA P; EORB P
		 * Operation: R' <- R ^ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     The contents of memory is exclusive-ORed into an 8-bit register
		 *
		 * EORA - Immediate - Opcode: 88 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::EORA_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.a ^= read_and_advance_program_counter();
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * ADC    Add with carry memory into register
		 * Source Form: ADCA P; ADCB P
		 * Operation: R' <- R + M + C
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of hte result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Adds the contents of the carry flag and the memory byte into an 8-bit register.
		 *
		 * ADCA - Immediate - Opcode: 89 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::ADCA_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.a = add(
				accumulator_register.registers.a,
				read_and_advance_program_counter() + (condition_code_register->bits.carry ? 1 : 0));
			break;

		/**
		 * OR    Inclusive OR memory into register
		 * Source Form: ORA P; ORB P
		 * Operation: R' <- R ∨ M
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Set if high order bit of result Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs an "inclusive OR" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ORA - Immediate - Opcode: 8A - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::ORA_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.a |= read_and_advance_program_counter();
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * ADD    Add memory into register
		 * Source Forms: ADDA P; ADDB P; ADDD P;
		 * Operation: R' <- R + M
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 (15) of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8 (16) -bit two's complement arithmetic
		 *        overflow.
		 *     C: Set if the operation caused a carry from bit 7 in the ALU
		 * Description:
		 *     Adds the memory byte into an 8 (16) - bit register
		 *
		 * ADDA - Immediate - Opcode: 8B - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::ADDA_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.a = add(accumulator_register.registers.a, read_and_advance_program_counter());
			break;

		/**
		 * CMP    Compare memory from a register
		 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
		 * Operation: TEMP <- R - M(:M+1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set.
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
		 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
		 * Description:
		 *     Compares the contents of M from teh contents of the specified register and sets
		 *     appropriate condition codes. Neither M nor R is modified. The C flag represents a
		 *     borrow and is set inverse to the resulting binary carry.
		 *
		 * CMPX - Immediate - Opcode: 8C - MPU Cycles: 4 - No of bytes: 3
		 */
		case Opcodes::CMPX_IMMEDIATE:
			cycle_counter += 4;
			compare(x_index_register, read2_and_advance_program_counter());
			break;

		/**
		 * BSR    Branch to subroutine
		 * Source Forms: BSR DD; LBSR DDDD
		 * Operation: TEMP <- MI
		 *            SP' <- SP - 1, (SP) <- PCL
		 *            SP' <- SP - 1, (SP) <- PCH
		 *            PC <- PC + TEMP
		 * Condition Codes: Not Affected
		 * Description:
		 *     The program counter is pushed onto the stack. The program counter is then loaded
		 *     with the sum of the program counter and the memory immediate offset.
		 *
		 * BSR - Relative - Opcode: 8D - MPU Cycles: 7 - No of bytes: 2
		 */
		case Opcodes::BSR_RELATIVE:
			cycle_counter += 7;
			offset = static_cast<std::int8_t>(read_and_advance_program_counter());
			push_program_counter_register();
			program_counter_register += offset;
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDX - Immediate - Opcode: 8E - MPU Cycles: 3 - No of bytes: 3
		 */
		case Opcodes::LDX_IMMEDIATE:
			cycle_counter += 3;
			x_index_register = read2_and_advance_program_counter();
			condition_register(x_index_register);
			break;

		/**
		 * SUB    Subtract memory from register
		 * Source Forms: SUBA P; SUBB P; SUBD P
		 * Operation: R' <- R - M
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16)-bit two's complement overflow
		 *     C: Set if the operation did not cause a carry from bit 7 in the ALU
		 * Description:
		 *     Subtracts the value in M from teh contents of an 8 (16)-bit register. The C flag
		 *     represents a borrow and is set inverse to the resulting carry.
		 *
		 * SUBA - Direct - Opcode: 90 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::SUBA_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.a = subtract(
				accumulator_register.registers.a,
				read(retrieve_direct_address()));
			break;

		/**
		 * CMP    Compare memory from a register
		 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
		 * Operation: TEMP <- R - M(:M+1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set.
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
		 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
		 * Description:
		 *     Compares the contents of M from teh contents of the specified register and sets
		 *     appropriate condition codes. Neither M nor R is modified. The C flag represents a
		 *     borrow and is set inverse to the resulting binary carry.
		 *
		 * CMPA - Direct - Opcode: 91 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::CMPA_DIRECT:
			cycle_counter += 4;
			compare(accumulator_register.registers.a, read(retrieve_direct_address()));
			break;

		/**
		 * SBC    Subtract with carry memory into register
		 * Source Form: SBCA P; SBCB P
		 * Operation: R' <- R - M - C
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Subtracts the contents of the carry flag and the memory byte into an 8-bit
		 *     register.
		 *
		 * SBCA - Direct - Opcode: 92 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::SBCA_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.a = subtract(
				accumulator_register.registers.a,
				read(retrieve_direct_address()) + (condition_code_register->bits.carry ? 1 : 0));
			break;

		/**
		 * SUB    Subtract memory from register
		 * Source Forms: SUBA P; SUBB P; SUBD P
		 * Operation: R' <- R - M
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16)-bit two's complement overflow
		 *     C: Set if the operation did not cause a carry from bit 7 in the ALU
		 * Description:
		 *     Subtracts the value in M from teh contents of an 8 (16)-bit register. The C flag
		 *     represents a borrow and is set inverse to the resulting carry.
		 *
		 * SUBD - Direct - Opcode: 93 - MPU Cycles: 6 - No of bytes: 2
		 */
		case Opcodes::SUBD_DIRECT:
			cycle_counter += 6;
			accumulator_register.d = subtract(
				static_cast<std::uint16_t>(accumulator_register.d),
				static_cast<std::uint16_t>(read(retrieve_direct_address())));
			break;

		/**
		 * AND    Logical AND memory into register
		 * Source Forms: ANDA P; ANDB P
		 * Operation: R' <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is set
		 *     Z: Set if all bits of result are clear
		 *     V: Cleared
		 *     C: Not affected
		 * Description:
		 *     Performs the logical "AND" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ANDA - Direct - Opcode: 94 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::ANDA_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.a &= read(retrieve_direct_address());
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * BIT    Bit test
		 * Source Form: BITA P; BITB P
		 * Operation: TEMP <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs the logical "AND" of the contents of ACCX and the contents of M and
		 *     modifies condition codes accordingly. The contents of ACCX or M are not affected.
		 *
		 * BITA - Direct - Opcode: 95 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::BITA_DIRECT:
			cycle_counter += 4;
			condition_register(static_cast<std::uint8_t>(accumulator_register.registers.a & read(retrieve_direct_address())));
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDA - Direct - Opcode: 96 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::LDA_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.a = read(retrieve_direct_address());
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STA - Direct - Opcode: 97 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::STA_DIRECT:
			cycle_counter += 4;
			store_register(retrieve_direct_address(), accumulator_register.registers.a);
			break;

		/**
		 * EOR    Exclusive or
		 * Source Forms: EORA P; EORB P
		 * Operation: R' <- R ^ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     The contents of memory is exclusive-ORed into an 8-bit register
		 *
		 * EORA - Direct - Opcode: 9B - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::EORA_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.a ^= read(retrieve_direct_address());
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * ADC    Add with carry memory into register
		 * Source Form: ADCA P; ADCB P
		 * Operation: R' <- R + M + C
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of hte result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Adds the contents of the carry flag and the memory byte into an 8-bit register.
		 *
		 * ADCA - Direct - Opcode: 99 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::ADCA_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.a = add(
				accumulator_register.registers.a,
				read(retrieve_direct_address()) + (condition_code_register->bits.carry ? 1 : 0));
			break;

		/**
		 * OR    Inclusive OR memory into register
		 * Source Form: ORA P; ORB P
		 * Operation: R' <- R ∨ M
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Set if high order bit of result Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs an "inclusive OR" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ORA - Direct - Opcode: 9A - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::ORA_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.a |= read(retrieve_direct_address());
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * ADD    Add memory into register
		 * Source Forms: ADDA P; ADDB P; ADDD P;
		 * Operation: R' <- R + M
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 (15) of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8 (16) -bit two's complement arithmetic
		 *        overflow.
		 *     C: Set if the operation caused a carry from bit 7 in the ALU
		 * Description:
		 *     Adds the memory byte into an 8 (16) - bit register
		 *
		 * ADDA - Direct - Opcode: 9B - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::ADDA_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.a = add(
				accumulator_register.registers.a,
				read(retrieve_direct_address()));
			break;

		/**
		 * CMP    Compare memory from a register
		 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
		 * Operation: TEMP <- R - M(:M+1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set.
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
		 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
		 * Description:
		 *     Compares the contents of M from teh contents of the specified register and sets
		 *     appropriate condition codes. Neither M nor R is modified. The C flag represents a
		 *     borrow and is set inverse to the resulting binary carry.
		 *
		 * CMPX - Direct - Opcode: 9C - MPU Cycles: 6 - No of bytes: 2
		 */
		case Opcodes::CMPX_DIRECT:
			cycle_counter += 6;
			compare(x_index_register, read2(retrieve_direct_address()));
			break;

		/**
		 * JSR    Jump to subroutine at effective address
		 * Source Form: JSR
		 * Operation: SP' <- SP - 1, (SP) <- PCL
		 *            SP' <- SP - 1, (SP) <- PCH
		 *            PC <- EA
		 * Condition Codes: Not Affected
		 * Description:
		 *     Program control is transferred to the effective address after storing the return
		 *     address on the hardware stack.
		 *
		 * JSR - Direct - Opcode: 9D - MPU Cycles: 7 - No of bytes: 2
		 */
		case Opcodes::JSR_DIRECT:
			cycle_counter += 7;
			address = retrieve_direct_address();
			push_program_counter_register();
			program_counter_register = address;
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDX - Direct - Opcode: 9E - MPU Cycles: 5 - No of bytes: 2
		 */
		case Opcodes::LDX_DIRECT:
			cycle_counter += 5;
			x_index_register = read2(retrieve_direct_address());
			condition_register(x_index_register);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STX - Direct - Opcode: 9F - MPU Cycles: 5 - No of bytes: 2
		 */
		case Opcodes::STX_DIRECT:
			cycle_counter += 5;
			store_register(retrieve_direct_address(), x_index_register);
			break;

		/**
		 * SUB    Subtract memory from register
		 * Source Forms: SUBA P; SUBB P; SUBD P
		 * Operation: R' <- R - M
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16)-bit two's complement overflow
		 *     C: Set if the operation did not cause a carry from bit 7 in the ALU
		 * Description:
		 *     Subtracts the value in M from teh contents of an 8 (16)-bit register. The C flag
		 *     represents a borrow and is set inverse to the resulting carry.
		 *
		 * SUBA - Indexed - Opcode: A0 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::SUBA_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.a = subtract(
				accumulator_register.registers.a,
				read(retrieve_effective_address()));
			break;

		/**
		 * CMP    Compare memory from a register
		 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
		 * Operation: TEMP <- R - M(:M+1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set.
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
		 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
		 * Description:
		 *     Compares the contents of M from teh contents of the specified register and sets
		 *     appropriate condition codes. Neither M nor R is modified. The C flag represents a
		 *     borrow and is set inverse to the resulting binary carry.
		 *
		 * CMPA - Indexed - Opcode: A1 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::CMPA_INDEXED:
			cycle_counter += 5;
			compare(accumulator_register.registers.a, read(retrieve_effective_address()));
			break;

		/**
		 * SBC    Subtract with carry memory into register
		 * Source Form: SBCA P; SBCB P
		 * Operation: R' <- R - M - C
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Subtracts the contents of the carry flag and the memory byte into an 8-bit
		 *     register.
		 *
		 * SBCA - Indexed - Opcode: A2 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::SBCA_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.a = subtract(
				accumulator_register.registers.a,
				read(retrieve_effective_address()) + (condition_code_register->bits.carry ? 1 : 0));
			break;

		/**
		 * SUB    Subtract memory from register
		 * Source Forms: SUBA P; SUBB P; SUBD P
		 * Operation: R' <- R - M
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16)-bit two's complement overflow
		 *     C: Set if the operation did not cause a carry from bit 7 in the ALU
		 * Description:
		 *     Subtracts the value in M from teh contents of an 8 (16)-bit register. The C flag
		 *     represents a borrow and is set inverse to the resulting carry.
		 *
		 * SUBD - Indexed - Opcode: A3 - MPU Cycles: 6+ - No of bytes: 2+
		 */
		case Opcodes::SUBD_INDEXED:
			cycle_counter += 6;
			accumulator_register.d = subtract(
				accumulator_register.d,
				read2(retrieve_effective_address()));
			break;

		/**
		 * AND    Logical AND memory into register
		 * Source Forms: ANDA P; ANDB P
		 * Operation: R' <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is set
		 *     Z: Set if all bits of result are clear
		 *     V: Cleared
		 *     C: Not affected
		 * Description:
		 *     Performs the logical "AND" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ANDA - Indexed - Opcode: A4 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::ANDA_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.a &= read(retrieve_effective_address());
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * BIT    Bit test
		 * Source Form: BITA P; BITB P
		 * Operation: TEMP <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs the logical "AND" of the contents of ACCX and the contents of M and
		 *     modifies condition codes accordingly. The contents of ACCX or M are not affected.
		 *
		 * BITA - Indexed - Opcode: A5 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::BITA_INDEXED:
			cycle_counter += 4;
			condition_register(static_cast<std::uint8_t>(accumulator_register.registers.a & read(retrieve_effective_address())));
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDA - Indexed - Opcode: A6 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::LDA_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.a = read(retrieve_effective_address());
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STA - Indexed - Opcode: A7 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::STA_INDEXED:
			cycle_counter += 4;
			store_register(retrieve_effective_address(), accumulator_register.registers.a);
			break;

		/**
		 * EOR    Exclusive or
		 * Source Forms: EORA P; EORB P
		 * Operation: R' <- R ^ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     The contents of memory is exclusive-ORed into an 8-bit register
		 *
		 * EORA - Indexed - Opcode: A8 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::EORA_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.a ^= read(retrieve_effective_address());
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * ADC    Add with carry memory into register
		 * Source Form: ADCA P; ADCB P
		 * Operation: R' <- R + M + C
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of hte result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Adds the contents of the carry flag and the memory byte into an 8-bit register.
		 *
		 * ADCA - Indexed - Opcode: A9 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::ADCA_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.a = add(
				accumulator_register.registers.a,
				read(retrieve_effective_address()) + (condition_code_register->bits.carry ? 1 : 0));
			break;

		/**
		 * OR    Inclusive OR memory into register
		 * Source Form: ORA P; ORB P
		 * Operation: R' <- R ∨ M
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Set if high order bit of result Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs an "inclusive OR" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ORA - Indexed - Opcode: AA - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::ORA_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.a |= read(retrieve_effective_address());
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * ADD    Add memory into register
		 * Source Forms: ADDA P; ADDB P; ADDD P;
		 * Operation: R' <- R + M
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 (15) of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8 (16) -bit two's complement arithmetic
		 *        overflow.
		 *     C: Set if the operation caused a carry from bit 7 in the ALU
		 * Description:
		 *     Adds the memory byte into an 8 (16) - bit register
		 *
		 * ADDA - Indexed - Opcode: AB - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::ADDA_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.a = add(
				accumulator_register.registers.a,
				read(retrieve_effective_address()));
			break;

		/**
		 * CMP    Compare memory from a register
		 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
		 * Operation: TEMP <- R - M(:M+1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set.
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
		 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
		 * Description:
		 *     Compares the contents of M from teh contents of the specified register and sets
		 *     appropriate condition codes. Neither M nor R is modified. The C flag represents a
		 *     borrow and is set inverse to the resulting binary carry.
		 *
		 * CMPX - Indexed - Opcode: AC - MPU Cycles: 6+ - No of bytes: 2+
		 */
		case Opcodes::CMPX_INDEXED:
			cycle_counter += 6;
			compare(x_index_register, read2(retrieve_effective_address()));
			break;

		/**
		 * JSR    Jump to subroutine at effective address
		 * Source Form: JSR
		 * Operation: SP' <- SP - 1, (SP) <- PCL
		 *            SP' <- SP - 1, (SP) <- PCH
		 *            PC <- EA
		 * Condition Codes: Not Affected
		 * Description:
		 *     Program control is transferred to the effective address after storing the return
		 *     address on the hardware stack.
		 *
		 * JSR - Indexed - Opcode: AD - MPU Cycles: 7+ - No of bytes: 2+
		 */
		case Opcodes::JSR_INDEXED:
			cycle_counter += 7;
			address = retrieve_effective_address();
			push_program_counter_register();
			program_counter_register = address;
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDX - Indexed - Opcode: AE - MPU Cycles: 5+ - No of bytes: 2+
		 */
		case Opcodes::LDX_INDEXED:
			cycle_counter += 5;
			x_index_register = read2(retrieve_effective_address());
			condition_register(x_index_register);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STX - Indexed - Opcode: AF - MPU Cycles: 5+ - No of bytes: 2+
		 */
		case Opcodes::STX_INDEXED:
			cycle_counter += 5;
			store_register(retrieve_effective_address(), x_index_register);
			break;

		/**
		 * SUB    Subtract memory from register
		 * Source Forms: SUBA P; SUBB P; SUBD P
		 * Operation: R' <- R - M
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16)-bit two's complement overflow
		 *     C: Set if the operation did not cause a carry from bit 7 in the ALU
		 * Description:
		 *     Subtracts the value in M from teh contents of an 8 (16)-bit register. The C flag
		 *     represents a borrow and is set inverse to the resulting carry.
		 *
		 * SUBA - Extended - Opcode: B0 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::SUBA_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.a = subtract(
				accumulator_register.registers.a,
				read(read2_and_advance_program_counter()));
			break;

		/**
		 * CMP    Compare memory from a register
		 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
		 * Operation: TEMP <- R - M(:M+1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set.
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
		 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
		 * Description:
		 *     Compares the contents of M from teh contents of the specified register and sets
		 *     appropriate condition codes. Neither M nor R is modified. The C flag represents a
		 *     borrow and is set inverse to the resulting binary carry.
		 *
		 * CMPA - Extended - Opcode: B1 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::CMPA_EXTENDED:
			cycle_counter += 5;
			compare(accumulator_register.registers.a, read(read2_and_advance_program_counter()));
			break;

		/**
		 * SBC    Subtract with carry memory into register
		 * Source Form: SBCA P; SBCB P
		 * Operation: R' <- R - M - C
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Subtracts the contents of the carry flag and the memory byte into an 8-bit
		 *     register.
		 *
		 * SBCA - Extended - Opcode: B2 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::SBCA_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.a = subtract(
				accumulator_register.registers.a,
				read(read2_and_advance_program_counter()) + (condition_code_register->bits.carry ? 1 : 0));
			break;

		/**
		 * SUB    Subtract memory from register
		 * Source Forms: SUBA P; SUBB P; SUBD P
		 * Operation: R' <- R - M
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16)-bit two's complement overflow
		 *     C: Set if the operation did not cause a carry from bit 7 in the ALU
		 * Description:
		 *     Subtracts the value in M from teh contents of an 8 (16)-bit register. The C flag
		 *     represents a borrow and is set inverse to the resulting carry.
		 *
		 * SUBD - Extended - Opcode: B3 - MPU Cycles: 7 - No of bytes: 3
		 */
		case Opcodes::SUBD_EXTENDED:
			cycle_counter += 7;
			accumulator_register.d = subtract(
				accumulator_register.d,
				read2(read2_and_advance_program_counter()));
			break;

		/**
		 * AND    Logical AND memory into register
		 * Source Forms: ANDA P; ANDB P
		 * Operation: R' <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is set
		 *     Z: Set if all bits of result are clear
		 *     V: Cleared
		 *     C: Not affected
		 * Description:
		 *     Performs the logical "AND" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ANDA - Extended - Opcode: B4 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::ANDA_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.a &= read(read2_and_advance_program_counter());
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * BIT    Bit test
		 * Source Form: BITA P; BITB P
		 * Operation: TEMP <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs the logical "AND" of the contents of ACCX and the contents of M and
		 *     modifies condition codes accordingly. The contents of ACCX or M are not affected.
		 *
		 * BITA - Extended - Opcode: B5 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::BITA_EXTENDED:
			cycle_counter += 5;
			condition_register(static_cast<std::uint8_t>(accumulator_register.registers.a & read(read2_and_advance_program_counter())));
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDA - Extended - Opcode: B6 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::LDA_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.a = read(read2_and_advance_program_counter());
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STA - Extended - Opcode: B7 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::STA_EXTENDED:
			cycle_counter += 5;
			store_register(read2_and_advance_program_counter(), accumulator_register.registers.a);
			break;

		/**
		 * EOR    Exclusive or
		 * Source Forms: EORA P; EORB P
		 * Operation: R' <- R ^ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     The contents of memory is exclusive-ORed into an 8-bit register
		 *
		 * EORA - Extended - Opcode: B8 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::EORA_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.a ^= read(read2_and_advance_program_counter());
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * ADC    Add with carry memory into register
		 * Source Form: ADCA P; ADCB P
		 * Operation: R' <- R + M + C
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of hte result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Adds the contents of the carry flag and the memory byte into an 8-bit register.
		 *
		 * ADCA - Extended - Opcode: B9 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::ADCA_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.a = add(
				static_cast<std::uint8_t>(accumulator_register.registers.a),
				static_cast<std::uint8_t>(read(read2_and_advance_program_counter()) + (condition_code_register->bits.carry ? 1 : 0)));
			break;

		/**
		 * OR    Inclusive OR memory into register
		 * Source Form: ORA P; ORB P
		 * Operation: R' <- R ∨ M
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Set if high order bit of result Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs an "inclusive OR" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ORA - Extended - Opcode: BA - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::ORA_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.a |= read(read2_and_advance_program_counter());
			condition_register(accumulator_register.registers.a);
			break;

		/**
		 * ADD    Add memory into register
		 * Source Forms: ADDA P; ADDB P; ADDD P;
		 * Operation: R' <- R + M
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 (15) of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8 (16) -bit two's complement arithmetic
		 *        overflow.
		 *     C: Set if the operation caused a carry from bit 7 in the ALU
		 * Description:
		 *     Adds the memory byte into an 8 (16) - bit register
		 *
		 * ADDA - Extended - Opcode: BB - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::ADDA_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.a = add(
				accumulator_register.registers.a,
				read(read2_and_advance_program_counter()));
			break;

		/**
		 * CMP    Compare memory from a register
		 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
		 * Operation: TEMP <- R - M(:M+1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set.
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
		 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
		 * Description:
		 *     Compares the contents of M from teh contents of the specified register and sets
		 *     appropriate condition codes. Neither M nor R is modified. The C flag represents a
		 *     borrow and is set inverse to the resulting binary carry.
		 *
		 * CMPX - Extended - Opcode: BC - MPU Cycles: 7 - No of bytes: 3
		 */
		case Opcodes::CMPX_EXTENDED:
			cycle_counter += 7;
			compare(x_index_register, read2(read2_and_advance_program_counter()));
			break;

		/**
		 * JSR    Jump to subroutine at effective address
		 * Source Form: JSR
		 * Operation: SP' <- SP - 1, (SP) <- PCL
		 *            SP' <- SP - 1, (SP) <- PCH
		 *            PC <- EA
		 * Condition Codes: Not Affected
		 * Description:
		 *     Program control is transferred to the effective address after storing the return
		 *     address on the hardware stack.
		 *
		 * JSR - Extended - Opcode: BD - MPU Cycles: 8 - No of bytes: 3
		 */
		case Opcodes::JSR_EXTENDED:
			cycle_counter += 8;
			address = read2_and_advance_program_counter();
			push_program_counter_register();
			program_counter_register = address;
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDX - Extended - Opcode: BE - MPU Cycles: 6 - No of bytes: 3
		 */
		case Opcodes::LDX_EXTENDED:
			cycle_counter += 6;
			x_index_register = read2(read2_and_advance_program_counter());
			condition_register(x_index_register);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STX - Extended - Opcode: BF - MPU Cycles: 6 - No of bytes: 3
		 */
		case Opcodes::STX_EXTENDED:
			cycle_counter += 6;
			store_register(read2_and_advance_program_counter(), x_index_register);
			break;

		/**
		 * SUB    Subtract memory from register
		 * Source Forms: SUBA P; SUBB P; SUBD P
		 * Operation: R' <- R - M
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16)-bit two's complement overflow
		 *     C: Set if the operation did not cause a carry from bit 7 in the ALU
		 * Description:
		 *     Subtracts the value in M from teh contents of an 8 (16)-bit register. The C flag
		 *     represents a borrow and is set inverse to the resulting carry.
		 *
		 * SUBB - Immediate - Opcode: C0 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::SUBB_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.b = subtract(
				accumulator_register.registers.b,
				read_and_advance_program_counter());
			break;

		/**
		 * CMP    Compare memory from a register
		 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
		 * Operation: TEMP <- R - M(:M+1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set.
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
		 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
		 * Description:
		 *     Compares the contents of M from teh contents of the specified register and sets
		 *     appropriate condition codes. Neither M nor R is modified. The C flag represents a
		 *     borrow and is set inverse to the resulting binary carry.
		 *
		 * CMPB - Immediate - Opcode: C1 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::CMPB_IMMEDIATE:
			cycle_counter += 2;
			compare(accumulator_register.registers.b, read_and_advance_program_counter());
			break;

		/**
		 * SBC    Subtract with carry memory into register
		 * Source Form: SBCA P; SBCB P
		 * Operation: R' <- R - M - C
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Subtracts the contents of the carry flag and the memory byte into an 8-bit
		 *     register.
		 *
		 * SBCB - Immediate - Opcode: C2 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::SBCB_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.b = subtract(
				accumulator_register.registers.b,
				read_and_advance_program_counter() + (condition_code_register->bits.carry ? 1 : 0));
			break;

		/**
		 * ADD    Add memory into register
		 * Source Forms: ADDA P; ADDB P; ADDD P;
		 * Operation: R' <- R + M
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 (15) of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8 (16) -bit two's complement arithmetic
		 *        overflow.
		 *     C: Set if the operation caused a carry from bit 7 in the ALU
		 * Description:
		 *     Adds the memory byte into an 8 (16) - bit register
		 *
		 * ADDD - Immediate - Opcode: C3 - MPU Cycles: 4 - No of bytes: 3
		 */
		case Opcodes::ADDD_IMMEDIATE:
			cycle_counter += 4;
			accumulator_register.d = add(
				accumulator_register.d,
				read2_and_advance_program_counter());
			break;

		/**
		 * AND    Logical AND memory into register
		 * Source Forms: ANDA P; ANDB P
		 * Operation: R' <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is set
		 *     Z: Set if all bits of result are clear
		 *     V: Cleared
		 *     C: Not affected
		 * Description:
		 *     Performs the logical "AND" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ANDB - Immediate - Opcode: C4 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::ANDB_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.b &= read_and_advance_program_counter();
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * BIT    Bit test
		 * Source Form: BITA P; BITB P
		 * Operation: TEMP <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs the logical "AND" of the contents of ACCX and the contents of M and
		 *     modifies condition codes accordingly. The contents of ACCX or M are not affected.
		 *
		 * BITB - Immediate - Opcode: C5 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::BITB_IMMEDIATE:
			cycle_counter += 2;
			condition_register(static_cast<std::uint8_t>(accumulator_register.registers.b & read_and_advance_program_counter()));
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDB - Immediate - Opcode: C6 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::LDB_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.b = read_and_advance_program_counter();
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * EOR    Exclusive or
		 * Source Forms: EORA P; EORB P
		 * Operation: R' <- R ^ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     The contents of memory is exclusive-ORed into an 8-bit register
		 *
		 * EORB - Immediate - Opcode: C8 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::EORB_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.b ^= read_and_advance_program_counter();
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * ADC    Add with carry memory into register
		 * Source Form: ADCA P; ADCB P
		 * Operation: R' <- R + M + C
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of hte result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Adds the contents of the carry flag and the memory byte into an 8-bit register.
		 *
		 * ADCB - Immediate - Opcode: C9 - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::ADCB_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.b = add(
				static_cast<std::uint8_t>(accumulator_register.registers.b),
				static_cast<std::uint8_t>(read_and_advance_program_counter() + (condition_code_register->bits.carry ? 1 : 0)));
			break;

		/**
		 * OR    Inclusive OR memory into register
		 * Source Form: ORA P; ORB P
		 * Operation: R' <- R ∨ M
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Set if high order bit of result Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs an "inclusive OR" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ORB - Immediate - Opcode: CA - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::ORB_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.b |= read_and_advance_program_counter();
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * ADD    Add memory into register
		 * Source Forms: ADDA P; ADDB P; ADDD P;
		 * Operation: R' <- R + M
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 (15) of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8 (16) -bit two's complement arithmetic
		 *        overflow.
		 *     C: Set if the operation caused a carry from bit 7 in the ALU
		 * Description:
		 *     Adds the memory byte into an 8 (16) - bit register
		 *
		 * ADDB - Immediate - Opcode: CB - MPU Cycles: 2 - No of bytes: 2
		 */
		case Opcodes::ADDB_IMMEDIATE:
			cycle_counter += 2;
			accumulator_register.registers.b = add(
				accumulator_register.registers.b,
				read_and_advance_program_counter());
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDD - Immediate - Opcode: CC - MPU Cycles: 3 - No of bytes: 3
		 */
		case Opcodes::LDD_IMMEDIATE:
			cycle_counter += 3;
			accumulator_register.d = read2_and_advance_program_counter();
			condition_register(accumulator_register.d);
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDU - Immediate - Opcode: CE - MPU Cycles: 3 - No of bytes: 3
		 */
		case Opcodes::LDU_IMMEDIATE:
			cycle_counter += 3;
			user_stack_pointer_register = read2_and_advance_program_counter();
			condition_register(user_stack_pointer_register);
			break;

		/**
		 * SUB    Subtract memory from register
		 * Source Forms: SUBA P; SUBB P; SUBD P
		 * Operation: R' <- R - M
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16)-bit two's complement overflow
		 *     C: Set if the operation did not cause a carry from bit 7 in the ALU
		 * Description:
		 *     Subtracts the value in M from teh contents of an 8 (16)-bit register. The C flag
		 *     represents a borrow and is set inverse to the resulting carry.
		 *
		 * SUBB - Direct - Opcode: D0 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::SUBB_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.b = subtract(
				accumulator_register.registers.b,
				read(retrieve_direct_address()));
			break;

		/**
		 * CMP    Compare memory from a register
		 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
		 * Operation: TEMP <- R - M(:M+1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set.
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
		 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
		 * Description:
		 *     Compares the contents of M from teh contents of the specified register and sets
		 *     appropriate condition codes. Neither M nor R is modified. The C flag represents a
		 *     borrow and is set inverse to the resulting binary carry.
		 *
		 * CMPB - Direct - Opcode: D1 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::CMPB_DIRECT:
			cycle_counter += 4;
			compare(accumulator_register.registers.b, read(retrieve_direct_address()));
			break;

		/**
		 * SBC    Subtract with carry memory into register
		 * Source Form: SBCA P; SBCB P
		 * Operation: R' <- R - M - C
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Subtracts the contents of the carry flag and the memory byte into an 8-bit
		 *     register.
		 *
		 * SBCB - Direct - Opcode: D2 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::SBCB_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.b = subtract(
				accumulator_register.registers.b,
				read(retrieve_direct_address()) + (condition_code_register->bits.carry ? 1 : 0));
			break;

		/**
		 * ADD    Add memory into register
		 * Source Forms: ADDA P; ADDB P; ADDD P;
		 * Operation: R' <- R + M
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 (15) of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8 (16) -bit two's complement arithmetic
		 *        overflow.
		 *     C: Set if the operation caused a carry from bit 7 in the ALU
		 * Description:
		 *     Adds the memory byte into an 8 (16) - bit register
		 *
		 * ADDD - Direct - Opcode: D3 - MPU Cycles: 6 - No of bytes: 2
		 */
		case Opcodes::ADDD_DIRECT:
			cycle_counter += 6;
			accumulator_register.d = add(
				accumulator_register.d,
				read2(retrieve_direct_address()));
			break;

		/**
		 * AND    Logical AND memory into register
		 * Source Forms: ANDA P; ANDB P
		 * Operation: R' <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is set
		 *     Z: Set if all bits of result are clear
		 *     V: Cleared
		 *     C: Not affected
		 * Description:
		 *     Performs the logical "AND" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ANDB - Direct - Opcode: D4 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::ANDB_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.b &= read(retrieve_direct_address());
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * BIT    Bit test
		 * Source Form: BITA P; BITB P
		 * Operation: TEMP <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs the logical "AND" of the contents of ACCX and the contents of M and
		 *     modifies condition codes accordingly. The contents of ACCX or M are not affected.
		 *
		 * BITB - Direct - Opcode: D5 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::BITB_DIRECT:
			cycle_counter += 4;
			condition_register(static_cast<std::uint8_t>(accumulator_register.registers.b & read(retrieve_direct_address())));
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDB - Direct - Opcode: D6 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::LDB_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.b = read(retrieve_direct_address());
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STB - Direct - Opcode: D7 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::STB_DIRECT:
			cycle_counter += 4;
			store_register(retrieve_direct_address(), accumulator_register.registers.b);
			break;

		/**
		 * EOR    Exclusive or
		 * Source Forms: EORA P; EORB P
		 * Operation: R' <- R ^ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     The contents of memory is exclusive-ORed into an 8-bit register
		 *
		 * EORB - Direct - Opcode: D8 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::EORB_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.b ^= read(retrieve_direct_address());
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * ADC    Add with carry memory into register
		 * Source Form: ADCA P; ADCB P
		 * Operation: R' <- R + M + C
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of hte result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Adds the contents of the carry flag and the memory byte into an 8-bit register.
		 *
		 * ADCB - Direct - Opcode: D9 - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::ADCB_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.b = add(
				accumulator_register.registers.b,
				read(retrieve_direct_address()) + (condition_code_register->bits.carry ? 1 : 0));
			break;

		/**
		 * OR    Inclusive OR memory into register
		 * Source Form: ORA P; ORB P
		 * Operation: R' <- R ∨ M
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Set if high order bit of result Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs an "inclusive OR" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ORB - Direct - Opcode: DA - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::ORB_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.b |= read(retrieve_direct_address());
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * ADD    Add memory into register
		 * Source Forms: ADDA P; ADDB P; ADDD P;
		 * Operation: R' <- R + M
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 (15) of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8 (16) -bit two's complement arithmetic
		 *        overflow.
		 *     C: Set if the operation caused a carry from bit 7 in the ALU
		 * Description:
		 *     Adds the memory byte into an 8 (16) - bit register
		 *
		 * ADDB - Direct - Opcode: DB - MPU Cycles: 4 - No of bytes: 2
		 */
		case Opcodes::ADDB_DIRECT:
			cycle_counter += 4;
			accumulator_register.registers.b = add(
				accumulator_register.registers.b,
				read(retrieve_direct_address()));
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDD - Direct - Opcode: DC - MPU Cycles: 5 - No of bytes: 2
		 */
		case Opcodes::LDD_DIRECT:
			cycle_counter += 5;
			accumulator_register.d = read2(retrieve_direct_address());
			condition_register(accumulator_register.d);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STD - Direct - Opcode: DD - MPU Cycles: 5 - No of bytes: 2
		 */
		case Opcodes::STD_DIRECT:
			cycle_counter += 5;
			store_register(retrieve_direct_address(), accumulator_register.d);
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDU - Direct - Opcode: DE - MPU Cycles: 5 - No of bytes: 2
		 */
		case Opcodes::LDU_DIRECT:
			cycle_counter += 5;
			user_stack_pointer_register = read2(retrieve_direct_address());
			condition_register(user_stack_pointer_register);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STU - Direct - Opcode: DF - MPU Cycles: 5 - No of bytes: 2
		 */
		case Opcodes::STU_DIRECT:
			cycle_counter += 5;
			store_register(retrieve_direct_address(), user_stack_pointer_register);
			break;

		/**
		 * SUB    Subtract memory from register
		 * Source Forms: SUBA P; SUBB P; SUBD P
		 * Operation: R' <- R - M
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16)-bit two's complement overflow
		 *     C: Set if the operation did not cause a carry from bit 7 in the ALU
		 * Description:
		 *     Subtracts the value in M from teh contents of an 8 (16)-bit register. The C flag
		 *     represents a borrow and is set inverse to the resulting carry.
		 *
		 * SUBB - Indexed - Opcode: E0 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::SUBB_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.b = subtract(
				accumulator_register.registers.b,
				read(retrieve_effective_address()));
			break;

		/**
		 * CMP    Compare memory from a register
		 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
		 * Operation: TEMP <- R - M(:M+1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set.
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
		 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
		 * Description:
		 *     Compares the contents of M from teh contents of the specified register and sets
		 *     appropriate condition codes. Neither M nor R is modified. The C flag represents a
		 *     borrow and is set inverse to the resulting binary carry.
		 *
		 * CMPB - Indexed - Opcode: E1 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::CMPB_INDEXED:
			cycle_counter += 4;
			compare(accumulator_register.registers.b, read(retrieve_effective_address()));
			break;

		/**
		 * SBC    Subtract with carry memory into register
		 * Source Form: SBCA P; SBCB P
		 * Operation: R' <- R - M - C
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Subtracts the contents of the carry flag and the memory byte into an 8-bit
		 *     register.
		 *
		 * SBCB - Indexed - Opcode: E2 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::SBCB_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.b = subtract(
				accumulator_register.registers.b,
				read(retrieve_effective_address()));
			break;

		/**
		 * ADD    Add memory into register
		 * Source Forms: ADDA P; ADDB P; ADDD P;
		 * Operation: R' <- R + M
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 (15) of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8 (16) -bit two's complement arithmetic
		 *        overflow.
		 *     C: Set if the operation caused a carry from bit 7 in the ALU
		 * Description:
		 *     Adds the memory byte into an 8 (16) - bit register
		 *
		 * ADDD - Indexed - Opcode: E3 - MPU Cycles: 6+ - No of bytes: 2+
		 */
		case Opcodes::ADDD_INDEXED:
			cycle_counter += 6;
			accumulator_register.d = add(
				static_cast<std::uint16_t>(accumulator_register.d),
				static_cast<std::uint16_t>(read(retrieve_effective_address())));
			break;

		/**
		 * AND    Logical AND memory into register
		 * Source Forms: ANDA P; ANDB P
		 * Operation: R' <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is set
		 *     Z: Set if all bits of result are clear
		 *     V: Cleared
		 *     C: Not affected
		 * Description:
		 *     Performs the logical "AND" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ANDB - Indexed - Opcode: E4 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::ANDB_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.b &= read(retrieve_effective_address());
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * BIT    Bit test
		 * Source Form: BITA P; BITB P
		 * Operation: TEMP <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs the logical "AND" of the contents of ACCX and the contents of M and
		 *     modifies condition codes accordingly. The contents of ACCX or M are not affected.
		 *
		 * BITB - Indexed - Opcode: E5 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::BITB_INDEXED:
			cycle_counter += 4;
			condition_register(static_cast<std::uint8_t>(accumulator_register.registers.b & read(retrieve_effective_address())));
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDB - Indexed - Opcode: E6 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::LDB_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.b = read(retrieve_effective_address());
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STB - Indexed - Opcode: E7 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::STB_INDEXED:
			cycle_counter += 4;
			store_register(retrieve_effective_address(), accumulator_register.registers.b);
			break;

		/**
		 * EOR    Exclusive or
		 * Source Forms: EORA P; EORB P
		 * Operation: R' <- R ^ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     The contents of memory is exclusive-ORed into an 8-bit register
		 *
		 * EORB - Indexed - Opcode: E8 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::EORB_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.b ^= read(retrieve_effective_address());
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * ADC    Add with carry memory into register
		 * Source Form: ADCA P; ADCB P
		 * Operation: R' <- R + M + C
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of hte result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Adds the contents of the carry flag and the memory byte into an 8-bit register.
		 *
		 * ADCB - Indexed - Opcode: E9 - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::ADCB_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.b = add(
				accumulator_register.registers.b,
				read(retrieve_effective_address()) + (condition_code_register->bits.carry ? 1 : 0));
			break;

		/**
		 * OR    Inclusive OR memory into register
		 * Source Form: ORA P; ORB P
		 * Operation: R' <- R ∨ M
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Set if high order bit of result Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs an "inclusive OR" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ORB - Indexed - Opcode: EA - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::ORB_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.b |= read(retrieve_effective_address());
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * ADD    Add memory into register
		 * Source Forms: ADDA P; ADDB P; ADDD P;
		 * Operation: R' <- R + M
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 (15) of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8 (16) -bit two's complement arithmetic
		 *        overflow.
		 *     C: Set if the operation caused a carry from bit 7 in the ALU
		 * Description:
		 *     Adds the memory byte into an 8 (16) - bit register
		 *
		 * ADDB - Indexed - Opcode: EB - MPU Cycles: 4+ - No of bytes: 2+
		 */
		case Opcodes::ADDB_INDEXED:
			cycle_counter += 4;
			accumulator_register.registers.b = add(
				accumulator_register.registers.b,
				read(retrieve_effective_address()));
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDD - Indexed - Opcode: EC - MPU Cycles: 5+ - No of bytes: 2+
		 */
		case Opcodes::LDD_INDEXED:
			cycle_counter += 5;
			accumulator_register.d = read2(retrieve_effective_address());
			condition_register(accumulator_register.d);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STD - Indexed - Opcode: ED - MPU Cycles: 5+ - No of bytes: 2+
		 */
		case Opcodes::STD_INDEXED:
			cycle_counter += 5;
			store_register(retrieve_effective_address(), accumulator_register.d);
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDU - Indexed - Opcode: EE - MPU Cycles: 5+ - No of bytes: 2+
		 */
		case Opcodes::LDU_INDEXED:
			cycle_counter += 5;
			user_stack_pointer_register = read2(retrieve_effective_address());
			condition_register(user_stack_pointer_register);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STU - Indexed - Opcode: EF - MPU Cycles: 5+ - No of bytes: 2+
		 */
		case Opcodes::STU_INDEXED:
			cycle_counter += 5;
			store_register(retrieve_effective_address(), user_stack_pointer_register);
			break;

		/**
		 * SUB    Subtract memory from register
		 * Source Forms: SUBA P; SUBB P; SUBD P
		 * Operation: R' <- R - M
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16)-bit two's complement overflow
		 *     C: Set if the operation did not cause a carry from bit 7 in the ALU
		 * Description:
		 *     Subtracts the value in M from teh contents of an 8 (16)-bit register. The C flag
		 *     represents a borrow and is set inverse to the resulting carry.
		 *
		 * SUBB - Extended - Opcode: F0 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::SUBB_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.b = subtract(
				accumulator_register.registers.b,
				read(read2_and_advance_program_counter()));
			break;

		/**
		 * CMP    Compare memory from a register
		 * Source Form: CMPA P; CMPB P; CMPD P; CMPX P; CMPY P; CMPU P; CMPS P
		 * Operation: TEMP <- R - M(:M+1)
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 (15) of the result is Set.
		 *     Z: Set if all bits of the result are Clear
		 *     V: Set if the operation caused an 8 (16) bit two's complement overflow.
		 *     C: Set if the subtraction did not cause a carry from bit 7 in the ALU.
		 * Description:
		 *     Compares the contents of M from the contents of the specified register and sets
		 *     appropriate condition codes. Neither M nor R is modified. The C flag represents a
		 *     borrow and is set inverse to the resulting binary carry.
		 *
		 * CMPB - Extended - Opcode: F1 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::CMPB_EXTENDED:
			cycle_counter += 5;
			compare(accumulator_register.registers.b, read(read2_and_advance_program_counter()));
			break;

		/**
		 * SBC    Subtract with carry memory into register
		 * Source Form: SBCA P; SBCB P
		 * Operation: R' <- R - M - C
		 * Condition Codes:
		 *     H: Undefined
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Subtracts the contents of the carry flag and the memory byte into an 8-bit
		 *     register.
		 *
		 * SBCB - Extended - Opcode: F2 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::SBCB_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.b = subtract(
				accumulator_register.registers.b,
				read(read2_and_advance_program_counter()) + (condition_code_register->bits.carry ? 1 : 0));
			break;

		/**
		 * ADD    Add memory into register
		 * Source Forms: ADDA P; ADDB P; ADDD P;
		 * Operation: R' <- R + M
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 (15) of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8 (16) -bit two's complement arithmetic
		 *        overflow.
		 *     C: Set if the operation caused a carry from bit 7 in the ALU
		 * Description:
		 *     Adds the memory byte into an 8 (16) - bit register
		 *
		 * ADDD - Extended - Opcode: F3 - MPU Cycles: 7 - No of bytes: 3
		 */
		case Opcodes::ADDD_EXTENDED:
			cycle_counter += 7;
			accumulator_register.d = add(
				accumulator_register.d,
				read2(read2_and_advance_program_counter()));
			break;

		/**
		 * AND    Logical AND memory into register
		 * Source Forms: ANDA P; ANDB P
		 * Operation: R' <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is set
		 *     Z: Set if all bits of result are clear
		 *     V: Cleared
		 *     C: Not affected
		 * Description:
		 *     Performs the logical "AND" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ANDB - Extended - Opcode: F4 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::ANDB_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.b &= read(read2_and_advance_program_counter());
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * BIT    Bit test
		 * Source Form: BITA P; BITB P
		 * Operation: TEMP <- R ∧ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of the result is Set
		 *     Z: Set if all bits of the result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs the logical "AND" of the contents of ACCX and the contents of M and
		 *     modifies condition codes accordingly. The contents of ACCX or M are not affected.
		 *
		 * BITB - Extended - Opcode: F5 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::BITB_EXTENDED:
			cycle_counter += 5;
			condition_register(static_cast<std::uint8_t>(accumulator_register.registers.b & read(read2_and_advance_program_counter())));
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDB - Extended - Opcode: F6 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::LDB_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.b = read(read2_and_advance_program_counter());
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STB - Extended - Opcode: F7 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::STB_EXTENDED:
			cycle_counter += 5;
			store_register(read2_and_advance_program_counter(), accumulator_register.registers.b);
			break;

		/**
		 * EOR    Exclusive or
		 * Source Forms: EORA P; EORB P
		 * Operation: R' <- R ^ M
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 of result is Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     The contents of memory is exclusive-ORed into an 8-bit register
		 *
		 * EORB - Extended - Opcode: F8 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::EORB_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.b ^= read(read2_and_advance_program_counter());
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * ADC    Add with carry memory into register
		 * Source Form: ADCA P; ADCB P
		 * Operation: R' <- R + M + C
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 of the result is set.
		 *     Z: Set if all bits of hte result are clear.
		 *     V: Set if the operation caused an 8-bit two's complement arithmetic overflow
		 *     C: Set if the operation caused a carry from bit 7 in the ALU.
		 * Description:
		 *     Adds the contents of the carry flag and the memory byte into an 8-bit register.
		 *
		 * ADCB - Extended - Opcode: F9 - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::ADCB_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.b = add(
				accumulator_register.registers.b,
				read(read2_and_advance_program_counter()) + (condition_code_register->bits.carry ? 1 : 0));
			break;

		/**
		 * OR    Inclusive OR memory into register
		 * Source Form: ORA P; ORB P
		 * Operation: R' <- R ∨ M
		 * Condition CodeS:
		 *     H: Not Affected
		 *     N: Set if high order bit of result Set
		 *     Z: Set if all bits of result are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Performs an "inclusive OR" operation between the contents of ACCX and the
		 *     contents of M and the result is stored in ACCX.
		 *
		 * ORB - Extended - Opcode: FA - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::ORB_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.b |= read(read2_and_advance_program_counter());
			condition_register(accumulator_register.registers.b);
			break;

		/**
		 * ADD    Add memory into register
		 * Source Forms: ADDA P; ADDB P; ADDD P;
		 * Operation: R' <- R + M
		 * Condition Codes:
		 *     H: Set if the operation caused a carry from bit 3 in the ALU.
		 *     N: Set if bit 7 (15) of the result is set.
		 *     Z: Set if all bits of the result are clear.
		 *     V: Set if the operation caused an 8 (16) -bit two's complement arithmetic
		 *        overflow.
		 *     C: Set if the operation caused a carry from bit 7 in the ALU
		 * Description:
		 *     Adds the memory byte into an 8 (16) - bit register
		 *
		 * ADDB - Extended - Opcode: FB - MPU Cycles: 5 - No of bytes: 3
		 */
		case Opcodes::ADDB_EXTENDED:
			cycle_counter += 5;
			accumulator_register.registers.b = add(
				accumulator_register.registers.b,
				read(read2_and_advance_program_counter()));
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDD - Extended - Opcode: FC - MPU Cycles: 6 - No of bytes: 3
		 */
		case Opcodes::LDD_EXTENDED:
			cycle_counter += 6;
			accumulator_register.d = read2(read2_and_advance_program_counter());
			condition_register(accumulator_register.d);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STD - Extended - Opcode: FD - MPU Cycles: 6 - No of bytes: 3
		 */
		case Opcodes::STD_EXTENDED:
			cycle_counter += 6;
			store_register(read2_and_advance_program_counter(), accumulator_register.d);
			break;

		/**
		 * LD    Load register from memory
		 * Source Forms: LDA P; LDB P; LDD P; LDX P; LDY P; LDS P; LDU P
		 * Operation: R' <- M(:M+1)
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of loaded data is Set
		 *     Z: Set if all bits of loaded data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Load the contents of the addressed memory into the register.
		 *
		 * LDU - Extended - Opcode: FE - MPU Cycles: 6 - No of bytes: 3
		 */
		case Opcodes::LDU_EXTENDED:
			cycle_counter += 6;
			user_stack_pointer_register = read2(read2_and_advance_program_counter());
			condition_register(user_stack_pointer_register);
			break;

		/**
		 * ST    Store register into memory
		 * Source Form: STA P; STB P; STD P; STX P; STY P; STS P; STU P
		 * Operation: M'(:M+1') <- R
		 * Condition Codes:
		 *     H: Not Affected
		 *     N: Set if bit 7 (15) of stored data was Set
		 *     Z: Set if all bits of stored data are Clear
		 *     V: Cleared
		 *     C: Not Affected
		 * Description:
		 *     Writes the contents of an MPU register into a memory location.
		 *
		 * STU - Extended - Opcode: FF - MPU Cycles: 6 - No of bytes: 3
		 */
		case Opcodes::STU_EXTENDED:
			cycle_counter += 6;
			store_register(read2_and_advance_program_counter(), user_stack_pointer_register);
			break;
		}

		if (pins_state->irq and pins_state->delay_irq)
		{
			pins_state->delay_irq = false;
			handle_irq();
		}

		num_instructions++;
	}

	ret = num_cycles_to_run - cycle_counter;
	cycle_counter = 0.0;
	return ret;
}

void Mc68b09e::set_direct_page_register(std::uint8_t new_register)
{
	direct_page_register.container.reg = new_register;
}

std::uint8_t Mc68b09e::get_direct_page_register()
{
	return direct_page_register.container.reg;
}

void Mc68b09e::set_a_accumulator(std::uint8_t new_register)
{
	accumulator_register.registers.a = new_register;
}

std::uint8_t Mc68b09e::get_a_accumulator()
{
	return accumulator_register.registers.a;
}

void Mc68b09e::set_b_accumulator(std::uint8_t new_register)
{
	accumulator_register.registers.b = new_register;
}

std::uint8_t Mc68b09e::get_b_accumulator()
{
	return accumulator_register.registers.b;
}

void Mc68b09e::set_d_accumulator(std::uint16_t new_register)
{
	accumulator_register.d = new_register;
}

std::uint16_t Mc68b09e::get_d_accumulator()
{
	return accumulator_register.d;
}

void Mc68b09e::set_cwai(bool new_cwai)
{
	cwai = new_cwai;
}

bool Mc68b09e::get_cwai()
{
	return cwai;
}

void Mc68b09e::set_syncing(bool new_syncing)
{
	syncing = new_syncing;
}

bool Mc68b09e::get_syncing()
{
	return syncing;
}
