;Kevin Siraki Lab 3        
         BR      main        
letter:  .EQUATE 6 ;#1c          
countA:  .EQUATE 4 ;#2d          
countB:  .EQUATE 2 ;#2d           
countC:  .EQUATE 0 ;#2d           
main:    SUBSP   7,i         ;#countC #countB #countA #letter                  
         LDWA    0,i         ;put 0 in all counters         
         STWA    countA,s    
         STWA    countB,s    
         STWA    countC,s    
         LDBA    charIn,d    ;input chars
         STBA    letter,s    
         LDBA    letter,s    ;is the input X?    
         CPBA    'X',i       
         BREQ    print       ;yes, go to print
do:      LDBX    letter,s    ;subtract 65 to make the cases begin at 0
         SUBX    65,i        
         STBX    letter,s    
         ASLX                ;shift left    
         BR      letters,x   ;go to switch
letters: .ADDRSS case0       ;case 'A'       
         .ADDRSS case1       ;case 'B'
         .ADDRSS case2       ;case 'C'
case0:   LDWA    countA,s    ;increment 'A' counter    
         ADDA    1,i         
         STWA    countA,s    
         BR      endsw       ;break 
case1:   LDWA    countB,s    ;same as a but for b
         ADDA    1,i         
         STWA    countB,s    
         BR      endsw       
case2:   LDWA    countC,s    ;same as a but for c
         ADDA    1,i         
         STWA    countC,s    
         BR      endsw       
default: BR      endsw       ;defualt case       
endsw:   LDBA    charIn,d    ;enter another character    
         STBA    letter,s    
while:   CPBA    'X',i       ;while (letter!='X'), loop through do       
         BRNE    do          
print:   STRO    acount,d    ;printing the coutms
         DECO    countA,s    
         LDBA    '\n',i      
         STBA    charOut,d  
         STRO    bcount,d 
         DECO    countB,s    
         LDBA    '\n',i      
         STBA    charOut,d  
         STRO    ccount,d 
         DECO    countC,s    
         ADDSP   7,i         ;#letter #countA #countB #countC
         STOP
acount:  .ASCII  "Number of A's: \x00"    
bcount:  .ASCII  "Number of B's: \x00"
ccount:  .ASCII  "Number of C's: \x00"     
                         
         .END                  
