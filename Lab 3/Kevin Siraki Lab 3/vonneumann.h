//
//  vonneumann.hpp
//  Twiddle
//
//  Created by Tony Biehl on 12/29/15.
//  Copyright © 2015 Tony Biehl. All rights reserved.
//

#ifndef vonneumann_h
#define vonneumann_h

#include <stdio.h>
#include <string>
#include "stack_8.h"

using std::string;

const word NO_BREAK = -1;

word getValue(string);
void readFile(stack_8 &, string);
void menu();
void printRegs(stack_8 &, bool);
void fetchExecuteCycle(stack_8 &stk8);

#endif /* vonneumann_hpp */
