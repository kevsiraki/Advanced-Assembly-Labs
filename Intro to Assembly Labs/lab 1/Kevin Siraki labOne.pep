         BR main
number:  .BLOCK 2
answer:  .BLOCK 2
         LDWA 0, i
         STWA answer, d
main:    STRO menu, d
         LDBA charIn, d
         CPBA 'q', i 
         BREQ quit
         CPBA 'a', i 
         BRNE subtract 
         STRO msg, d
         DECI number, d
         LDWA number, d
         ADDA answer, d 
         STWA answer, d 
         BR result 
subtract:CPBA 's',i
         BRNE negate 
         STRO msg, d
         DECI number, d
         LDWA answer, d 
         SUBA number, d
         STWA answer, d
         BR result 
negate:  CPBA 'n',i
         BRNE quit
         LDWA answer, d 
         nega
         STWA answer, d 
         LDBA charIn, d
result:  LDBA '\n',i 
         STBA charOut, d
         STRO msg2, d
         DECO answer, d
         LDBA '\n', i
         STBA charOut, d
         BR main
quit:    CPBA 'q', i
         BREQ end
         STRO error, d
         BR main
end:     STOP
menu:    .ASCII "Menu\n"
         .ASCII "a. Add \n"
         .ASCII "s. Subtract \n"
         .ASCII "n. Negate \n"
         .ASCII "q. Quit \n"
         .ASCII "Choose: \x00"
msg:     .ASCII "Enter Number = \x00"
msg2:    .ASCII "Result = \x00"
error:   .ASCII "Invalid! \n\x00"
         .END
         
         