;Kevin Siraki Lab 2A
;The fib sequence 0-10 just as a reference :)... 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89,.......
         BR      main        
temp:    .EQUATE 0           ;#2d
;retAdd  .EQUATE 2           ;address
n:       .EQUATE 4           ;#2d
retval:  .EQUATE 6           ;#2d
fib:     SUBSP   2,i         ;alloc #temp
         LDWA    n,s         ;load n
         CPWA    0,i         ;compare n to 0
         BRLE    endif0      ;break if n is less than or equal to 0
         LDWA    n,s         ;load n
         CPWA    2,i         ;compare n to 2
         BRLT    endif2      ;break if n is less than 2   ;NOTE: I did not use BRLE since a 2 passed into the algorithm will yield 1 either way.
else:    LDWA    n,s         ;load n
         SUBA    1,i         ;n-1
         STWA    -4,s        ;store on stack
         SUBSP   4,i         ;allocate #n #retval
         CALL    fib         ;recursive call 1
         ADDSP   4,i         ;deallocate #n #retval
         STWA    temp,s      ;store sum of recursive call 1 (fib(n-1)) in temp
         LDWA    n,s         ;load n again
         SUBA    2,i         ;n-2
         STWA    -4,s        ;store on stack
         SUBSP   4,i         ;alloc #n #retval
         CALL    fib         ;sum of recursive call 2 (fib(n-2))
         ADDSP   4,i         ;deallocate #n #retval
         ADDA    temp,s      ;add fib(n-1)+fib(n-2)            
endif2:  STWA    retval,s    ;store this result in the return value
         ADDSP   2,i         ;dealloc #temp
         RET                 ;return retval                 
endif0:  LDWA    0,i         ;load a 0 into the accumulator
         STWA    retval,s    ;store this result in the return value
         ADDSP   2,i         ;dealloc #temp
         RET                 ;return retval
nm:      .EQUATE 0           ;#2d local variable in main
main:    SUBSP   2,i         ;alloc #nm
         STRO    msg,d       ;promp user input of range
         DECI    nm,s        ;get the range
         LDWA    nm,s        ;load range into accum
         STWA    -4,s        ;store in parameter n
         SUBSP   4,i         ;#n #retval
         CALL    fib         ;call the subroutine
         ADDSP   4,i         ;#n #retval
         STRO    msg2,d      ;output msg2
         DECO    -2,s        ;output the retval of the function call on stack
         STRO    lf,d        ;newline
         ADDSP   2,i         ;dealloc #nm
         STOP                ;halt execution               
msg:     .ASCII  "Which Fibonacci number? \n\x00" ;prompt user input
msg2:    .ASCII  "Result: \x00"                   ;display the result
lf:      .ASCII  "\n\x00"                         ;print a newline   
         .END                  
