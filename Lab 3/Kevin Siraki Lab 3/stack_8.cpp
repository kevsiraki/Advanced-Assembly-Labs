//
//  stack8.cpp
//  stack8
//
//  Created by Tony Biehl on 12/21/15.
//  Copyright © 2015 Tony Biehl. All rights reserved.
//

#include "stack_8.h"
#include <iomanip>

using namespace std;

reg8::reg8(){
    Register = NOREG;
    contents = 0;
    carry = false;
}

reg8::reg8(REG r, word c){
    Register = r;
    contents = ALLBITS & c;
    carry = false;
}

reg8::reg8(REG r, word c, bool cry){
    Register = r;
    contents = ALLBITS & c;
    carry = cry;
}

REG reg8::getReg(){
    return Register;
}

word reg8::getContents(){
    return contents;
}

bool reg8::getCarry(){
    return carry;
}

word reg8::operator+(){                 // return contents
    return contents;
}

reg8& reg8::operator+(const reg8 r){      // add
    word regAnswer = contents + r.contents;
    if (regAnswer & CARRY) {
        if (carry)
            carry = LINK_OFF;
        else
            carry = LINK_ON;
    }
    reg8 reg = reg8(NOREG, regAnswer & ALLBITS, carry);
    return reg;
}

reg8& reg8::operator&(const reg8 r){      // and
    reg8 reg = reg8(NOREG, contents & r.contents);
    return reg;
}
reg8& reg8::operator|(const reg8 r){       // or
    reg8 reg = reg8(NOREG, contents | r.contents);
    return reg;
}
reg8& reg8::operator^(const reg8 r){       // xor
    reg8 reg = reg8(NOREG, contents ^ r.contents);
    return reg;
}
reg8& reg8::operator<<(bool link){      // rol (pass the link)
    int regAnswer = 0;
    regAnswer = (contents << 1);
    if (link)
        regAnswer++;
    if (contents & CARRY)
        carry = LINK_ON;
    else carry = LINK_OFF;

    reg8 reg = reg8(NOREG, regAnswer & ALLBITS, carry);
    return reg;
}

reg8& reg8::operator>>(bool link){      // ror
    int regAnswer = 0;
    regAnswer = (contents >> 1);
    if (link)
       regAnswer += SIGN;
    if (contents & BIT_RIGHTMOST)
        carry = LINK_ON;
    else carry = LINK_OFF;
    
    reg8 reg = reg8(NOREG, regAnswer & ALLBITS, carry);
    return reg;
}
reg8& reg8::operator~(){                 // complement
    int regAnswer = ~contents;
    reg8 reg = reg8(NOREG, regAnswer & ALLBITS);
    return reg;
}
reg8& reg8::operator!(){                 // swap
    int regAnswer = (contents >> 6) + (contents << 6);
    reg8 reg = reg8(NOREG, regAnswer & ALLBITS);
    return reg;
}
void reg8::operator*(reg8& r)   // mul reg1 * reg2, top 12 bits in reg1, bottom 12 in reg2
{
    int extend = convert16(contents) * convert16(r.contents); // multiply and get 24 bits
    contents = (extend/SCALE) & ALLBITS; // most significant 12 bits go into first register
    r.contents = (extend % SCALE) & ALLBITS; // least significant 12 bits go into regAnswer and second register
}

reg8& reg8::operator*(){                // sign extend
    int regAnswer = (contents & SIGN7 ? contents | TOP5BITS : contents);
    reg8 reg = reg8(NOREG, regAnswer & ALLBITS);
    return reg;
}
void reg8::operator=(const reg8 r){
    if (r.Register == NOREG)
        Register = r.Register;
    contents = r.contents & ALLBITS;
    carry = r.carry & ALLBITS;
}

void reg8::operator=(const word r){
    contents = r & ALLBITS;
}

//===========================================================================================

void stack_8::setPC(word address) // set the PC
{
    setReg(PC, address);
}

word stack_8::getMem(word address) // get a word from membory
{
    if (address < 0 || address > MAX_MEM)
        address = address % MAX_MEM;
    return memory[address];
}

void stack_8::setMem(word address, word w) // set memory to word
{
    if (address < 0 || address > MAX_MEM)
        address = address % MAX_MEM;
    memory[address] = w & ALLBITS;
}

word stack_8::getReg(REG r) // helper function, take REG and return the value of that register
{
    word reg = 0;
    switch (r)
    {
        case AC: reg = +ac;
            break;
        case LINK:
            if (+link != 0)
                reg = 1;
            else reg = 0;
            break;
        case PC: reg = +pc;
            break;
        case IR: reg = +ir;
            break;
        case MA: reg = +ma;
            break;
        case MD: reg = +md;
            break;
        case STACK: reg = +stack;
            break;
        case TEMP: reg = +temp;
            break;
        case IMINUS1: reg = +iminus1;
            break;
        case IONE: reg = +ione;
            break;
        case IMASK7: reg = +imask7;
            break;
        case IMASK6: reg = +imask6;
            break;
        case IZERO: reg = +izero;
            break;
        case DEVICE: reg = +device;
            break;
        case INTRPT: reg = +intrpt;
            break;
        case SWITCHES: reg = +switches;
            break;
        case OPCODE: reg = +opCode;
            break;
        case HLT: reg = +hlt;
            break;
        case MQ: reg = +mq;
            break;
        default:
            cout << "Invalid register to get\n";
            break;
    };
    return reg;
}

void stack_8::setReg(REG r, word reg) // helper function, take REG and set the value of that register
{
    reg = reg & ALLBITS;
    switch (r)
    {
        case AC: ac = reg;
            break;
        case LINK:
            if (reg != 0)
                link = 1;
            else link = 0;
            break;
        case PC: pc = reg;
            break;
        case IR: ir = reg;
            break;
        case MA: ma = reg;
            break;
        case MD: md = reg;
            break;
        case STACK: stack = reg;
            break;
        case TEMP: temp = reg;
            break;
        case INTRPT:
            if (reg != 0)
                intrpt = 1;
            else intrpt = 0;
            break;
        case SWITCHES:
            switches = reg;
            break;
        case HLT:
            if (reg != 0)
                hlt = 1;
            else hlt = 0;
            break;
        case MQ:
            mq = reg;
            break;
        case DEVICE:
            device = reg;
            break;
        default:
            cout << "Invalid register to be set\n";
    }
}

stack_8::stack_8()
{
    clearMemory();
    reset();
}

void stack_8::clearMemory()
{
    for (int addr = 0; addr < MAX_MEM; addr++)
        memory[addr] = 0;
}

void stack_8::cu(CU_OP op)
{
    char ch;
    switch (op)
    {
        case IN:
           // cin.clear();
            cin.get(ch);
            ac = +ac | ch;
            break;
        case OUT:
            ch = +ac;
            cout << ch;
            break;
        case READ: md = memory[+ma];
            break;
        case WRITE: memory[+ma] = +md & ALLBITS;
            break;
        case HALT: setReg(HLT, LINK_ON);
            break;
        case EXEC: setReg(HLT, LINK_OFF);
            break;
        case SETSW: switches = ALLBITS;
            break;
        case SETOPC: opCode = +ir & TOP3BITS;
            break;
        case NONE:
            break;
    }
}

string stack_8::stringALU_OP(ALU_OP op){
    string temp;
    switch (op) {
        case ADD: temp = "ADD";
            break;
        case AND: temp = "AND";
            break;
        case OR: temp = "OR";
            break;
        case XOR: temp = "XOR";
            break;
        case ROL: temp = "ROL";
            break;
        case ROR: temp = "ROR";
            break;
        case SWAP: temp = "SWAP";
            break;
        case CLR:  temp = "CLR";
            break;
        case CMR:  temp = "CMR";
            break;
        case SIGNEXT: temp = "SIGNEXT";
            break;
        case NOP: temp = "NOP";
            break;
        case EMUL: temp = "EMUL";
            break;
        case EDIV: temp = "EDIV";
            break;
        default: temp = "UNKNOWN";
            break;
    }
    return temp;
}

string stack_8::stringCU_OP(CU_OP op) {
    string temp;
    switch (op)
    {
        case IN:
            temp = "IN";
            break;
        case OUT:
            temp = "OUT";
            break;
        case READ:
            temp = "READ";
            break;
        case WRITE:
            temp = "WRITE";
            break;
        case HALT:
            temp = "HALT";
            break;
        case EXEC:
            temp = "EXEC";
            break;
        case SETOPC:
            temp = "SETOPC";
            break;
        case SETSW:
            temp = "SETSW";
            break;
        case NONE:
            temp = "NONE";
            break;
    }
    return temp;
}

void stack_8::fetch() // get the instruction, put in IR, and bump PC
{
    ma = pc; // cpu.alu(MA, NOP, PC, NO);
    pc = +pc + +ione; // cpu.alu(PC, ADD, PC, IONE, NO);
    cu(READ);
    ir = md; //cpu.alu(IR, NOP, MD, NO);
}

void stack_8::decode() // on exit: MA points to operand
{
    cu(SETOPC); // cpu.alu(SETOPC, cpu.getReg(IR));  // get the opcode for the instruction
    word addMode = +ir & ADDR_MODE; // cpu.getReg(IR) & ADDR_MODE;
    word temp = +opCode; // cpu.getReg(OPCODE);
    if (temp == INS_SIO || temp == INS_MIC) // no standard operand
        return;                             // exit
    if (temp == INS_JMP || temp == INS_JMS || // stack mode not defined for JMP and JMS
        temp == INS_AND_OR) // also AND or OR
    {
        if (addMode == ADDR_STK)
            addMode = ADDR_REL;
        if (addMode == ADDR_STI)
            addMode = ADDR_IND;
    }
    reg8 reg = ir & imask7; //cpu.alu(TEMP, AND, IR, IMASK7, NO);
    reg8 tempReg = *reg; // cpu.alu(TEMP, SIGNEXT, TEMP, NO);
    switch (addMode)
    {
        case ADDR_REL:     // relative addressing mode
            ma = tempReg + pc; // cpu.alu(MA, ADD, TEMP, PC, NO);
            break;
        case ADDR_IND:     // indirect addressing mode
            ma = tempReg + pc; //cpu.alu(MA, ADD, TEMP, PC, NO);
            cu(READ); //cpu.cu(READ); // get operand from memory
            ma = md; //cpu.alu(MA, NOP, MD, NO); // copy MD into MA for the indirect
            break;
        case ADDR_STK:     // stack addressing mode
            ma = tempReg + stack; //cpu.alu(MA, ADD, TEMP, STACK, NO);
            break;
        case ADDR_STI:    // stack indirect addressing mode
            ma = tempReg + stack; // cpu.alu(MA, ADD, TEMP, STACK, NO);
            cu(READ); // cpu.cu(READ); // get operand from memory
            ma = md; //cpu.alu(MA, NOP, MD, NO); // copy MD into MA for the stack indirect
            break;
    }
}

void stack_8::execute() // assume decode was done and MA set to operand for instructions TAD to JMP
{
    word irReg = +ir; //cpu.getReg(IR);

    switch (+opCode) // (cpu.getReg(OPCODE))
    {
        case INS_AND_OR:
            // put in code for the AND and OR
            cu(READ);
            if((irReg & OR_BIT)==OR_BIT)
            	ac=md|ac;
            else
            	ac = md & ac;
            break;
        case INS_TAD:
            cout << setbase(8);
            // put in code for the TAD
            cu(READ);
            ac = md + ac;
			break;
        case INS_ISZ:
            // put in code for the ISZ
            cu(READ);
            md = md + ione;
            cu(WRITE);
            if (+md == 0)
            	pc = pc + ione;
			break;
        case INS_DCA:
            // put in code for the DCA
            md = ac;
            cu(WRITE);
            ac = izero;
			break;
        case INS_JMS:
            // put in code for the JMS
            md = pc;
            pc = ma;
            stack = stack + iminus1;
            ma = stack;
            cu(WRITE);
            break;
        case INS_JMP:
            // put in code for the JMP
            pc = ma;
            break;
        case INS_SIO:
            temp = ir & imask7; //cpu.alu(TEMP, AND, IR, IMASK7, NO);
            temp = *temp; // cpu.alu(TEMP, SIGNEXT, TEMP, NO);
            if (irReg & STACK_OP_BIT)
            {
                //word offset6 = irReg & OFFSET6;
                temp = ir & imask6; //cpu.alu(TEMP, AND, IR, IMASK6, NO);
                switch (irReg & INS_STACK)
                {
                        //case STACK_OR: // or AC with top of stack
                        //    cpu.alu(MA, ADD, STACK, TEMP, NO);
                        //    cpu.cu(READ);
                        //    cpu.alu(AC, OR, MD, AC, NO);
                        //    break;
                    case STACK_PUSH: // push AC onto stack
                        if (+temp == 0) { // offset for PUSH must be zero
                            // do PUSH the AC onto stack
                            stack = stack + iminus1; //cpu.alu(STACK, ADD, STACK, IMINUS1, NO); // one more space for AC
                            ma = stack; // cpu.alu(MA, NOP, STACK, NO);
                            md = ac; // cpu.alu(MD, NOP, AC, NO);
                            ac = izero; // cpu.alu(AC, NOP, IZERO, NO);
                            cu(WRITE); //cpu.cu(WRITE);
                        }
                        // following "else if" more of a comment since STACK_PUSH == STACK_ENTER
                        //    only difference is offset is non-zero for STACK_ENTER
                        else if ((irReg & INS_STACK) == STACK_ENTER){
                            temp = ~temp; // cpu.alu(TEMP, CMR, TEMP, NO);   // negate
                            temp = temp + ione; // cpu.alu(TEMP, ADD, IONE, TEMP, NO);
                            stack = stack + temp; // cpu.alu(STACK, ADD, STACK, TEMP, NO); // substract offset
                        }
                        break;
                    case STACK_POP: // pop temp# words off stack then pop AC from stack
                        // following "if" the "(irReg & INS_STACK) == STACK_LEAVE)" part is more of a
                        //    comment since STACK_POP == STACK_LEAVE
                        //    only difference is offset is non-zero for STACK_LEAVE)
                        if (+temp != 0 && ((irReg & INS_STACK) == STACK_LEAVE))
                            stack = stack + temp; // cpu.alu(STACK, ADD, STACK, TEMP, NO); // add offset for LEAVE
                        else { // do POP for the AC
                            ma = stack; // cpu.alu(MA, NOP, STACK, NO);
                            cu(READ); //cpu.cu(READ);
                           ac = md; // cpu.alu(AC, NOP, MD, NO);
                           stack = stack + ione; //cpu.alu(STACK, ADD, STACK, IONE, NO); // remove old AC
                        }
                        break;
                    case STACK_RET: // pop temp# words off then pop PC from stack for return
                        stack = stack + temp; // cpu.alu(STACK, ADD, STACK, TEMP, NO); // add offset
                         ma = stack; // cpu.alu(MA, NOP, STACK, NO);
                        cu(READ); //cpu.cu(READ);
                        stack = stack + ione; // cpu.alu(STACK, ADD, STACK, IONE, NO); // remove old PC
                        pc = md; //cpu.alu(PC, NOP, MD, NO);
                        break;
                }
            }
            else
            {
                temp = ir & imask7; // cpu.alu(TEMP, AND, IR, IMASK7, NO); // Isolate device number from instruction
                temp = temp >> (+link == LINK_ON); //cpu.alu(TEMP, ROR, TEMP, NO);
                temp = temp >> (+link == LINK_ON); //cpu.alu(TEMP, ROR, TEMP, NO);
                temp = temp >> (+link == LINK_ON); //cpu.alu(TEMP, ROR, TEMP, NO);
                device = temp & imask7; // cpu.alu(DEVICE, AND, TEMP, IMASK7, NO); // set device register to device #
                int ioFunction = irReg & IO_FUNCTION;
                //cout << "d=" << device << " if=" << ioFunction << " a=" << cpu.getReg(AC) << endl;
                if(ioFunction & IO_F2) //   f2 operation, skip next instruction if device ready
                    pc = pc + ione; // cpu.alu(PC, ADD, PC, IONE, NO); // assume always ready
                if(ioFunction & IO_F1) //   f1 operation, clear AC
                {
                    if (+device != 0) { // clear AC if not device 0 but is device 3
                        if (+device == IO_KYB)
                            ac = izero; //cpu.alu(AC, NOP, IZERO, NO);
                    }else
                        intrpt = izero; // cpu.alu(INTRPT, NOP, IZERO, NO); // else turn interrupts off
                }
                if (ioFunction & IO_F0) {
                    //   f0 operation, move word between AC and device,
                    //      initiate I/O transfer, and clear device’s ready flag
                    if (+device == IO_DEV0)
                    {
                        intrpt = ione; //cpu.alu(INTRPT, NOP, IONE, NO); // interrupts on
                    }
                    else if (+device == IO_KYB) // read character into AC
                        cu(IN); // cpu.cu(IN);
                    else if (+device == IO_PRT) // print character from AC
                        cu(OUT); // cpu.cu(OUT);
                }
            }
            break;
            
            
        case INS_MIC:
            if ((irReg & MIC23_BIT) == 0)
            { // group 1 microinstructions
                if (irReg & MIC1_CLA) { // 7200, group 1, clear AC
                   ac = izero; // cpu.alu(AC, NOP, IZERO, NO);
                }
                if (irReg & MIC1_CLL) { // 7100, group 1, clear link
                    // put in code for the CLL
					link = izero;
                }
                if (irReg & MIC1_CMA) { // 7040, group 1, complement AC
                    // put in code for the CMA
					ac = ~ac;
                }
                if (irReg & MIC1_CML) { // 7020, group 1, complement the Link
                    // put in code for the CML
					link = ~link;
                }
                if (irReg & MIC1_IAC) { // 7001, group 1, increment AC
                    // put in code for the IAC
					ac = ac + ione;
                }
                if (irReg & MIC1_RAR) { // 7010, group 1, rotate AC and Link right
                    ac = ac >> (+link == 1); //cpu.alu(AC, ROR, AC, YES);
                }
                if (irReg & MIC1_RAL) { // 7004, group 1, rotate AC and Link left
                    ac = ac << (+link == 1); // cpu.alu(AC, ROL, AC, YES);
                }
                // The BSW combined with rotate bits cause to rotate 2 bits, like so:
                if (irReg & MIC1_RTR) { // 7012, group 1, rotate right twice
                    ac = ac >> (+link == 1); //cpu.alu(AC, ROR, AC, YES);
                    ac = ac >> (+link == 1); //cpu.alu(AC, ROR, AC, YES);
                }
                if ((irReg & MIC1_RTL) == MIC1_RTL) { // 7006, group 1, rotate left twice
                    ac = ac<< (+link == 1); //cpu.alu(AC, ROL, AC, YES);
                    ac = ac << (+link == 1); //cpu.alu(AC, ROL, AC, YES);
                }
                if (irReg & MIC1_BSW) { // 7002, group 1, swap 6-bit “bytes”
                    ac = !ac; //cpu.alu(AC, SWAP, AC, NO);
                }
            }
            else
            { // group 2 or 3 microinstructons
                int andGroup = irReg & MIC2_ANDBIT; // check and bit
                int group3 = irReg & MIC3_EXTEND;
                if (group3)
                { // group3
                    if ((irReg & MIC3_CLA) == MIC3_CLA) { // 7601, group 3, clear AC
                        ac = izero; // cpu.alu(AC, CLR, AC, NO);
                    }
                    // 7421, group 3, MQL, Multiplier Quotient Load (transfer AC to MQ,
                    // clear AC). Alternative mnemonic: LMQ
                    if ((irReg & MIC3_MQL) == MIC3_LMQ) {
                        mq = ac; // cpu.alu(MQ, NOP, AC, NO);
                        ac = izero; // cpu.alu(AC, CLR, AC, NO);
                    }
                    // 7501, group 3, MQA, Multiplier Quotient Load to Accumulator
                    // (logical or MQ into AC)
                    if ((irReg & MIC3_MQA) == MIC3_MQA){
                        ac = ac | mq; // cpu.alu(AC, OR, AC, MQ, NO);
                    }
                    // 7405, group 3, MUY, Multiply next word into MQ and put most significant 12 bits into
                    // AC and least significant 12 bits into MQ
                    if ((irReg & MIC3_CODE) == MIC3_MUY) {
                        ma = pc; // cpu.alu(MA, NOP, PC, NO); // need to get next word
                        pc = pc + ione; // cpu.alu(PC, ADD, PC, IONE, NO); // and bump PC past it
                        cu(READ); // cpu.cu(READ);
                        ac = md; // get the value to multiply by
                        ac * mq; //cpu.alu(MQ, EMUL, MD, NO); // multiply MQ by next word that's in AC (from MD),
                        // put least signficant 12 bits in MQ (AC has most significant 12 bits)
                    }
                    // 7407, group 3, DVI, Divide 24 bits (most significant 12 bits in AC
                    // and least significant 12 bits in MQ) by next word,
                    // put the quotient into MQ and the remainder into AC.
                    // Link is 0 for normal divide, 1 for divide overflow
                    else if ((irReg & MIC3_CODE) == MIC3_DIV) {
                        int value = +ac*SCALE + +mq; // get a 24 bit value to use for divide
                        ma = pc; // cpu.alu(MA, NOP, PC, NO); // need to get next word
                        pc = pc + ione; // cpu.alu(PC, ADD, PC, IONE, NO); // and bump PC past it
                        cu(READ); // cpu.cu(READ);
                        ac = value % +md; // AC gets remainder
                        mq = value / +md; // MQ gets quotient
                        // cpu.alu(MQ, EDIV, MD, NO); // DIV by next word that's in MD,
                        // put quotient in MQ (AC has remainder)
                    }
                    if ((irReg & MIC3_NOP) == MIC3_NOP) { // 7401, group 3, NOP, No Operation
                    }
                } else if (!andGroup)
                { // or bit group 2
                    if ((irReg & MIC2O_SMA) == MIC2O_SMA && (+ac & SIGN)){
                        // 7500, group 2 OR, skip on AC < 0 (sign bit of AC is on)
                        pc = pc + ione; // cpu.alu(PC, ADD, PC, IONE, NO);
                    }
                    else if ((irReg & MIC2O_SZA) == MIC2O_SZA && (+ac == 0)){
                        // 7440, group 2 OR, skip on AC == 0 (all bits of AC zero)
                        pc = pc + ione; // cpu.alu(PC, ADD, PC, IONE, NO);
                    }
                    else if ((irReg & MIC2O_SNL) == MIC2O_SNL &&  (+link != 0)){
                        // 7420, group 2 OR, skip on Link is on
                        pc = pc + ione; //   cpu.alu(PC, ADD, PC, IONE, NO);
                    }
                    if (irReg & MIC2O_CLA){ // 7600, group 2 OR, clear AC
                        ac = izero; // cpu.alu(AC, CLR, AC, NO);
                    }
                    if (irReg & MIC2O_OSR){ // 7404, group 2 OR, “OR” front-panel switches with AC
                        ac = ac | switches; // cpu.alu(AC, OR, AC, SWITCHES, NO);
                    }
                    if (irReg & MIC2O_HLT){// 7402, group 2 OR, halt
                        cu(HALT); // cpu.alu(HLT, NOP, IONE, NO);
                    }
                }
                else
                { // and group
                    // following skips needed to be and'd together
                    // try for SPA (7510) and SNA (7450) and SZL (7430)
                    if (irReg & (MIC2A_SPA | MIC2A_SNA | MIC2A_SZL))
                    {
                        bool spa = (irReg & MIC2A_SPA) != MIC2A_SPA; // if bits off then make true
                        bool sna = (irReg & MIC2A_SNA) != MIC2A_SNA;
                        bool szl = (irReg & MIC2A_SZL) != MIC2A_SZL;
                        if (!spa && ((+ac  & SIGN) == 0)) // if spa bits are on and AC is positive
                            spa = true; // set true
                        if (!sna && (+ac  != 0)) // if sna bits are on and AC is not zero
                            sna = true; // set true
                        if (!szl && (+link == 0)) // if szl bits are on and link is off
                            szl = true; // set true
                        if (spa && sna && szl) // if all 3 conditions true
                            pc = pc + ione; // cpu.alu(PC, ADD, PC, IONE, NO); // skip instruction
                    }
                    if ((irReg & MIC2A_CLA)==MIC2A_CLA) { // 7610, group 2 AND, clear AC
                        ac = izero; // cpu.alu(AC, CLR, AC, NO);
                    }
                    if ((irReg & MIC2A_MOVSPA)==MIC2A_MOVSPA) { // 7414, group 2 AND, move SP into the AC
                        ac = stack; // cpu.alu(AC, NOP, AC, NO);
                    }
                    if ((irReg & MIC2A_HLT)==MIC2A_HLT) { // 7412, group 2 AND, halt
                        cu(HALT); // cpu.alu(HLT, NOP, IONE, NO);
                    }
                }
            }
            
            break;
        default:
            break;
    }
}

void stack_8::reset() {
    pc = reg8(PC,0);
    ir = reg8(IR,0);
    ac = reg8(AC,0);
    ma = reg8(MA,0);
    md = reg8(MD,0);
    link = reg8(LINK,0);
    temp = reg8(TEMP,0);
    izero = reg8(IZERO,0);
    device = reg8(DEVICE,0);
    stack = reg8(STACK,0);
    intrpt = reg8(INTRPT,0);
    hlt = reg8(HLT,0);
    mq = reg8(MQ,0);
    opCode = reg8(OPCODE,0);
    iminus1 = reg8(IMINUS1,07777);
    ione = reg8(IONE,1);
    imask7 = reg8(IMASK7,OFFSET7);
    imask6 = reg8(IMASK6,OFFSET6);
}

reg8& stack_8::operator=(reg8 r){
    setReg(r.getReg(), r.getContents());
    if (r.getReg() == AC && r.getCarry())
    {
        if (+link == 1)
            link = LINK_OFF;
        else
            link = LINK_ON;
    }
    reg = r;
    return reg;
}

reg8& stack_8::operator==(REG r){
    switch (r){
        case AC: reg = ac; break;
        case LINK: reg = link; break;
        case PC: reg = pc; break;
        case IR: reg = ir; break;
        case MA: reg = ma; break;
        case MD: reg = md; break;
        case STACK: reg = stack; break;
        case TEMP: reg = temp; break;
        case IMINUS1: reg = iminus1; break;
        case IONE: reg = ione; break;
        case IMASK7: reg = imask7; break;
        case IMASK6: reg = imask6; break;
        case IZERO: reg = izero; break;
        case INTRPT: reg = intrpt; break;
        case DEVICE: reg = device; break;
        case HLT: reg = hlt; break;
        case MQ: reg = mq; break;
        case OPCODE: reg = opCode; break;
        case SWITCHES: reg = switches; break;
        case NOREG: reg = reg8(NOREG, 0); break;
    }
    return reg;
}

void stack_8::setRegs(reg8 r[]){
    for (REG reg = AC; reg < NOREG; reg = nextReg(reg)){
        switch (reg){
            case IMINUS1:
            case IONE:
            case IMASK7:
            case IMASK6:
            case IZERO: ; // these are constant registers, do not change
            default: setReg(r[reg].getReg(), r[reg].getContents());
                break;
        }
    }
}

REG nextReg(REG reg){ // bump to next register
    switch(reg) {
        case AC: return LINK;
        case LINK: return PC;
        case PC: return IR;
        case IR: return MA;
        case MA: return MD;
        case MD: return STACK;
        case STACK: return TEMP;
        case TEMP: return IMINUS1;
        case IMINUS1: return IONE;
        case IONE: return IMASK7;
        case IMASK7: return IMASK6;
        case IMASK6: return IZERO;
        case IZERO: return INTRPT;
        case INTRPT: return DEVICE;
        case DEVICE: return HLT;
        case HLT: return MQ;
        case MQ: return OPCODE;
        case OPCODE: return SWITCHES;
        case SWITCHES: return NOREG;
        default: return NOREG;
    }
}
word convert16(word number) { // to 16 bits for arithmetic
    if (number & SIGN)
        number = 0170000 | number; // make 16 bits
    return number;
}

