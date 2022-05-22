//
//  stack8.h
//  stack8
//
//  Created by Tony Biehl on 12/21/15.
//  Copyright © 2015 Tony Biehl. All rights reserved.
//

#ifndef stack8_hpp
#define stack8_hpp

#include <iostream>
#include <string>
#include <stdio.h>

// enumerated commands and registers for CU and ALU
/*
enum CU_OP{IN, OUT, READ, WRITE, HALT, EXEC, NONE};
enum ALU_OP{ADD, AND, OR, XOR, ROL, ROR, SWAP, CLR, CMR, SIGNEXT, SETSW, SETOPC, EMUL, EDIV, NOP};
 */
enum CU_OP{IN, OUT, READ, WRITE, HALT, EXEC, SETSW, SETOPC, NONE};
enum ALU_OP{ADD, AND, OR, XOR, ROL, ROR, SWAP, CLR, CMR, SIGNEXT, EMUL, EDIV, NOP};
enum REG{AC, LINK, PC, IR, MA, MD, STACK, TEMP, IMINUS1, IONE, IMASK7, IMASK6,
         IZERO, INTRPT, DEVICE, HLT, MQ, OPCODE, SWITCHES, NOREG};
typedef short int word;


// useful constants for memory, alu, and bit masks
const word OFFSET7 = 00177;
const word OFFSET6 = 00077;
const word SIGN = 04000;
const word SIGN7 = 00100;
const word CARRY = 010000;
const word LINK_ON = 00001;
const word LINK_OFF = 00000;
const word BIT_RIGHTMOST = 000001;
const word TOP3BITS = 07000;
const word TOP5BITS = 07600;
const word SCALE = 010000; // scale number to top 12 bits of 24 bit value

const word ADDR_MODE = 00600;  // bits for the addressing mode
const word ADDR_REL = 00000;     // relative addressing mode
const word ADDR_IND = 00200;     // indirect addressing mode
const word ADDR_STK = 00400;     // stack addressing mode
const word ADDR_STI = 00600;     // stack indirect addressing mode

const word ALLBITS = 07777;

const word MAX_MEM = 010000;      // 1 000 000 000 000 or 4096 words


const bool NO = false; // do not set flags
const bool YES = true; // set flags

// stack8 opcodes
const word INS_AND_OR  = 00000; // 000 000 000 000 and (or OR) to ac
const word   OR_BIT    = 00400; // 000 100 000 000 bit to distinguish and from or
const word INS_TAD     = 01000; // 001 000 000 000 two's complement add to ac
const word INS_ISZ     = 02000; // 010 000 000 000 increment and skip on zero
const word INS_DCA     = 03000; // 011 000 000 000 deposit ac to memory and clear ac
const word INS_JMS     = 04000; // 100 000 000 000 jump to subroutine
const word INS_JMP     = 05000; // 101 000 000 000 jump
const word INS_SIO     = 06000; // 110 000 000 000 stack or I/O operations
const word   STACK_OP_BIT = 00400; // bit to indicate statop operations
const word   INS_STACK    = 00300; // sub opcode for stack operations
//const word   STACK_OR     = 00000; //   sub opcode for OR AC with top of stack
const word   STACK_PUSH   = 00100; //   sub opcode for PUSH AC onto stack (offset zero)
const word   STACK_ENTER  = 00100; //   sub opcode for subtracting offset from stack (non-zero)
const word   STACK_POP    = 00200; //   sub opcode for POP AC onto stack
const word   STACK_LEAVE  = 00200; //   sub opcode for adding offset to stack (non-zero)
const word   STACK_RET    = 00300; //   sub opcode for RETURN using stack
const word   IO_DEVICE    = 00170; // I/O device
const word   IO_FUNCTION  = 00007; // I/O function
const word     IO_F0      = 00004; //   f0 operation, move word between AC and device,
                                   //      initiate I/O transfer, and clear device’s ready flag
const word     IO_F1      = 00002; //   f1 operation, clear AC
const word     IO_F2      = 00001; //   f2 operation, skip next instruction if device ready
const word   IO_DEV0      = 00000; // I/O device 0
const word   IO_PPR       = 00001; // high-speed paper tape reader (or file)
const word   IO_PPP       = 00002; // high-speed paper tape punch (or file)
const word   IO_KYB       = 00003; // console keyboard
const word   IO_PRT       = 00004; // console printer (or screen)
const word INS_MIC     = 07000; // 111 000 000 000 micro instructions
const word   MIC1_CLA     = 00200; // 7200, group 1, clear AC
const word   MIC1_CLL     = 00100; // 7100, group 1, clear link
const word   MIC1_CMA     = 00040; // 7040, group 1, complement AC
const word   MIC1_CML     = 00020; // 7020, group 1, complement the Link
const word   MIC1_IAC     = 00001; // 7001, group 1, increment AC
const word   MIC1_RAR     = 00010; // 7010, group 1, rotate AC and Link right
const word   MIC1_RAL     = 00004; // 7004, group 1, rotate AC and Link left
// The BSW combined with rotate bits cause to rotate 2 bits, like so:
const word   MIC1_RTR     = 00012; // 7012, group 1, rotate right twice
const word   MIC1_RTL     = 00006; // 7006, group 1, rotate left twice
const word   MIC1_BSW     = 00002; // 7002, group 1, swap 6-bit “bytes”
const word MIC23_BIT    = 00400; // group 2 & 3 bit
const word   MIC2O_SMA    = 00100; // 7500, group 2 OR, skip on AC < 0 (sign bit of AC is on)
const word   MIC2O_SZA    = 00040; // 7440, group 2 OR, skip on AC == 0 (all bits of AC zero)
const word   MIC2O_SNL    = 00020; // 7420, group 2 OR, skip on Link is on
const word   MIC2O_CLA    = 00200; // 7600, group 2 OR, clear AC
const word   MIC2O_OSR    = 00004; // 7404, group 2 OR, “OR” front-panel switches with AC
const word   MIC2O_HLT    = 00002; // 7402, group 2 OR, halt
const word MIC2_ANDBIT = 00010; // AND group bit
const word   MIC2A_SPA    = 00110; // 7510, group 2 AND, skip on AC >= 0 (sign bit is off)
const word   MIC2A_SNA    = 00050; // 7450, group 2 AND, skip on AC <> 0 (AC not zero)
const word   MIC2A_SZL    = 00030; // 7430, group 2 AND, skip on Link 0
const word   MIC2A_CLA    = 00210; // 7610, group 2 AND, clear AC
const word   MIC2A_MOVSPA = 00014; // 7414, group 2 AND, move SP into the AC
const word   MIC2A_HLT    = 00012; // 7412, group 2 AND, halt
const word MIC3_EXTEND = 00001; // group 3 bit
const word   MIC3_CLA     = 00201; // 7601, group 3, clear AC
const word   MIC3_MQA     = 00101; // 7501, group 3, MQA, Multiplier Quotient Load to Accumulator
                                     // (logical or MQ into AC)
const word   MIC3_MQL     = 00021; // 7421, group 3, MQL, Multiplier Quotient Load (transfer AC to MQ,
                                     // clear AC)
const word   MIC3_LMQ     = 00021; // 7421, group 3, Alternative mnemonic: LMQ
const word   MIC3_CAM     = 00221; // 7621, group 3, CAM, Combine CLA and MQL to clear both AC and MQ
const word   MIC3_CODE    = 00017; // isolate the code and the group3 bit
const word   MIC3_MUY     = 00005; // 7405, group 3, MUY, Multiply next word into MQ and
                                     // put most significant 12 bits into
                                     // AC and least significant 12 bits into MQ
const word   MIC3_DIV     = 00007; // 7407, group 3, DVI, Divide 24 bits (most significant 12 bits in AC
                                     // and least significant 12 bits in MQ) by next word,
                                     // put the quotient into MQ and the remainder into AC.
                                     // Link is 0 for normal divide, 1 for divide overflow
const word   MIC3_NOP     = 00001; // 7401, group 3, NOP, No Operation

/*
------------------------------------------------------------------
                               reg8
------------------------------------------------------------------
 -Register:REG
 -contents:word
------------------------------------------------------------------
 +reg8()
 +reg8(r:REG, c:word)
 +getReg():REG
 +getContents():word
------------------------------------------------------------------
*/

class reg8{
private:
    REG Register;
    word contents;
    bool carry;
public:
    reg8();
    reg8(REG r, word c);
    reg8(REG r, word c, bool cry);
    REG getReg();
    word getContents();
    bool getCarry();
    word operator+();                  // return contents
    reg8& operator+(const reg8);       // add
    reg8& operator&(const reg8);       // and
    reg8& operator|(const reg8);       // or
    reg8& operator^(const reg8);       // xor
    reg8& operator<<(bool link);           // rol
    reg8& operator>>(bool link);           // ror
    reg8& operator~();                 // complement
    reg8& operator!();                 // swap
    void operator*(reg8&);       // mul reg1 * reg2, top 12 bits in reg1, bottom 12 in reg2
    // div has to be done manually since there are 3, not 2, registers active
    reg8& operator*();                // sign extend
    void operator=(const reg8);
    void operator=(const word);
};

/*
------------------------------------------------------------------
                                  stack8
------------------------------------------------------------------
 -pc:word
 -ir:word
 -ac:word
 -ma:word
 -md:word
 -flags:word
 -device:word
 -switches:word
 -temp:word
 -iminus1:word
 -ione:word
 -imask7:word
 -imask6:word
 -izero:word
 -intrpt:word
 -hlt:word
 -mq:word
 -memory[MAX_MEM]:word
------------------------------------------------------------------
 +stack8()
 +clearMemory():void
 +cu(CU_OP): void
 +alu(REG, ALU_OP, REG, REG, bool):word
 +alu(REG, ALU_OP, REG, bool):word
 +alu(ALU_OP, word): word
 -setReg(REG, word): void
 +getReg(REG): word
 +setPC(word): void
 +stringALU_OP(ALU_OP): string
 +stringCU_OP(CU_OP): string
 +convert16(word):word
 reset():void
 fetch(stack_8 &):void
 decode(stack_8 &):void
 execute(stack_8 &):void
 +operator==(r:reg8);
------------------------------------------------------------------
 */
class stack_8 {
private:
    reg8 pc,
         ir,
         opCode,
         ac,
         link,
         ma,
         md,
         stack,
         temp, iminus1, ione, imask7, imask6, izero, intrpt, device, switches, hlt, mq;
    word memory[MAX_MEM];
    void setReg(REG, word); // helper function, take REG and set the value of that register
    reg8 reg;
    
public:
    stack_8();
    void clearMemory();
 //   void cu(CU_OP);
 //   word alu(REG, ALU_OP, REG, REG, bool); // Dest register, ALU op, register1, register2, flag set
 //   word alu(REG, ALU_OP, REG, bool); // Dest register, ALU op, register, flag set
 //   word alu(ALU_OP, word); // ALU op, value switches
    word getReg(REG); // helper function, take REG and return the value of that register
    word getMem(word); // get a word from membory
    void cu(CU_OP);
    void setMem(word, word); // set memory to word
    void setPC(word); // set the PC
    std::string stringALU_OP(ALU_OP);
    std::string stringCU_OP(CU_OP);
    void reset();
    void fetch();
    void decode();
    void execute();
    reg8& operator=(reg8 r);
    reg8& operator==(REG r);
    void getRegs(reg8 []);
    void setRegs(reg8 []);
};

word convert16(word); // to 16 bits for arithmetic
REG nextReg(REG); // bump to next register
#endif /* stack8.h */
