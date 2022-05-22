//
//  vonneumann.cpp
//  Twiddle
//
//  Created by Tony Biehl on 12/29/15.
//  Copyright © 2015 Tony Biehl. All rights reserved.
//

#include "stack_8.h"
#include "vonneumann.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

void menu()
{
    cout << "b x - set breakpoint at x\n"
    << "c - continue\n"
    << "d x - display memory starting at x\n"
    << "e x - enter values staring at x\n"
    << "l file - load object from file\n"
    << "p - print all registers\n"
    << "r x - run at location x\n"
    << "s - step\n"
    << "x - set switches\n"
    << "q - quit\n"
    << "? - print menu\n";
}

void printRegs(stack_8 &stk8, bool all) {
    cout << "PC=" << setw(4) << stk8.getReg(PC) << " IR =" << setw(4) << stk8.getReg(IR)
    << " MA=" << setw(4) << stk8.getReg(MA) << " MD=" << setw(4) << stk8.getReg(MD)
    << " LINK=" << setw(1) << stk8.getReg(LINK) << " AC=" << setw(4) << stk8.getReg(AC)
    << " MQ=" << setw(4) << stk8.getReg(MQ) << " SP=" << setw(4) << stk8.getReg(STACK);
    if (all)
        cout << " TEMP=" << setw(4) << stk8.getReg(TEMP) << " DEVICE=" << setw(4) << stk8.getReg(DEVICE)
            << " SWITCHES=" << setw(4) << stk8.getReg(SWITCHES) << " HALT=" << setw(4) << stk8.getReg(HLT)
            << " IMINUS1=" << setw(4) << stk8.getReg(IMINUS1) << " IONE=" << setw(4) << stk8.getReg(IONE)
            << " IMASK7=" << setw(4) << stk8.getReg(IMASK7) << " IMASK6=" << setw(4) << stk8.getReg(IMASK6)
            << " IZERO=" << setw(4) << stk8.getReg(IZERO) << " INTRPT=" << setw(4) << stk8.getReg(INTRPT)
            << " OPCODE=" << setw(4) << stk8.getReg(OPCODE);
    cout << endl;
}

word getValue(string number)
{
    word value = 0;
    int base = 10, digit;

    if (number[0] == '0')
        base = 8;
    for (int x = 0; x < number.length(); x++)
    {
        digit = number[x] - '0';
        if (digit >= base)
            cout << "Invalid value of " << number << endl;
        else
        {
                value = value * base + digit;
        }
    }
    return value;
}

void fetchExecuteCycle(stack_8 &stk8){
    stk8.fetch();
    stk8.decode();
    stk8.execute();
}

void readFile(stack_8 &stk8, string filename)
{
    string inWord;
    word address = 0;
    ifstream inFile(filename.c_str());

    if (inFile.fail())
        cout << "Could not open '" << filename << "'" << endl;
    else
    {
        while (inFile >> inWord)
        {
            if (inWord[0] == '*')
                address = getValue("0"+inWord.substr(1, inWord.length()-1));
            else if (inWord[0] != '$') { // if not at the end of the object file
                stk8.setMem(address, getValue("0"+inWord));
                address++;
            }
        }
    }
    inFile.close();
}
