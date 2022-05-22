//
//  main.cpp
//  Stack8
//
//  Created by Tony Biehl on 1/24/16.
//  Copyright © 2016 Tony Biehl. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <string>
#include "stack_8.h"
#include "vonneumann.h"

using namespace std;

int main() {
    stack_8 stk8;
    char selection;
    string value;
    string macPrefix = ""; // "/Users/tonybiehl/Documents/Stack_8/";
    word address, mem;
    word breakpoint = NO_BREAK;
    cout << setbase(8) << setfill('0');
    cout << "Stack 8 computer (all numbers in octal):\n";
    menu();
    do
    {
        cout << ": ";
        cin >> selection;
        getline(cin, value); // get rest of line
        while (value.length() > 0 && value[0] == ' ') // remove leading spaces
            value = value.substr(1,value.length()-1);
        switch (selection)
        {
            case 'b' :
            case 'B' :
                breakpoint = getValue("0"+value);
                break;
            case 'c':
            case 'C':
                stk8.cu(EXEC);
                break;
            case 'd':
            case 'D':
                address = getValue("0"+value);
                cout << setw(4) << address << ": ";
                for (word count = 0; count < 8; count++)
                    if (address + count < MAX_MEM)
                      cout <<  setw(4) << stk8.getMem(address + count) << " ";
                cout << endl;
                break;
            case 'e':
            case 'E':
                address = getValue("0"+value);
                mem = 0;
                cout << "Enter values starting at " << setw(4) << address << " (negative to end)\n";
                do
                {
                    cout << setw(4) << address << ": ";
                    cin >> value;
                    if (value[0] != '-') {
                        stk8.setMem(address, getValue(value));
                        address++;
                    }
                } while (value[0] != '-');
                break;
            case 'l':
            case 'L':
                readFile(stk8, macPrefix+value);
                break;
            case 'p':
            case 'P':
                printRegs(stk8, true);
                break;
            case 'r':
            case 'R':
                stk8.reset(); // reset FIRST then set PC
                stk8.setPC(getValue("0"+value));
                stk8.cu(EXEC);
                break;
            case 's':
            case 'S':
                // set halt to execute just one instruction
                stk8.cu(HALT);
                break;
            case 'x':
            case 'X':
                stk8.cu(SETSW); //stk8.alu(SETSW, getValue("0"+value));
                break;
            case 'q':
                cout << "Quit\n";
                break;
            case '?':
                menu();
                break;
            default:
                cout << "Invalid selection, press '?' for menu\n";
                break;
        }
        if (selection == 'c' || selection == 'r' || selection == 's')
        {
            do
            {
                fetchExecuteCycle(stk8);
                if (stk8.getReg(PC) == breakpoint)
                    stk8.cu(HALT);
            } while (!stk8.getReg(HLT));
            printRegs(stk8, false);
        }
    } while (selection != 'q');
    
    return 0;
}
