;Kevin Siraki Lab 2b CSIS 165, ONLY LOCALS
         BR      main        
ary:     .EQUATE 8           ;#2h defined in hex so the address is clearer in the stack frame (stores an address of an array)
len:     .EQUATE 6           ;#2d length of the array
sum:     .EQUATE 4           ;#2h defined in hex so the address is clearer in the stack frame (stores an address of a sum)
;retAddr
i:       .EQUATE 0           ;#2d local variable i
sumPos:  SUBSP   2,i         ;#i
         LDWA    0,i         ;put 0 in sum
         STWA    sum,sf      
         LDWX    0,i         ;put 0 in i
         STWX    i,s         
for:     CPWX    len,s       ;see if i is at length
         BRGT    endif       ;yes? break to endif
         ASLX                ;no, shift the index register to the left (each int is 2 bytes)
         LDWA    ary,sfx     ;load ary[i]
         CPWA    0,i         ;is it 0?
         BRLE    endfor      ;yes, break to endfor
         LDWA    sum,sf      ;no, add it to the sum
         ADDA    ary,sfx     
         STWA    sum,sf      
         LDWX    i,s         ;increment i by 1
         ADDX    1,i         
         STWX    i,s         
         BR      for         ;loop back
endfor:  LDWX    i,s         ;add one to i to skip the negative element
         ADDX    1,i         
         STWX    i,s         
         BR      for         
endif:   ADDSP   2,i         ;#i
         RET                 
theArray:.EQUATE 4           ;#2d
k:       .EQUATE 2           ;#2d
total:   .EQUATE 0           ;#2d
;no retAddr
main:    SUBSP   6,i         ;#theArray #k #total
         LDWA    5,i         ;here we basically make the array
         STWA    4,s         
         LDWA    11,i        
         STWA    6,s         
         LDWA    -29,i       
         STWA    8,s         
         LDWA    45,i        
         STWA    10,s        
         LDWA    9,i         
         STWA    12,s        
         LDWA    -1,i        
         STWA    14,s        
         LDWA    6,i         ;pass length by value to sumPos
         STWA    -4,s        
         MOVSPA              ;pass the total by address (see addressing mode)
         ADDA    total,i     
         STWA    -6,s        
         MOVSPA              ;pass the array by address (notice the addressing mode)
         ADDA    theArray,i  
         STWA    -2,s        
         SUBSP   6,i         ;#sum #len #ary no retval since sumPos is void and takes the total as ref
         CALL    sumPos      ;pretty obvious whats going on here
         ADDSP   6,i         ;#ary #len #sum
         LDWX    0,i         ;k=0
         STWX    k,s         
         STRO    arrMess,d   
for2:    CPWX    6,i         ;are we at the last element?
         BRGE    endif2      ;yes. break to endif
         ASLX                ;no, shift left (each int is 2 bytes)
         DECO    theArray,sx ;print theArray[i]
         LDBA    ' ',i       
         STBA    charOut,d   ;space between elements
         LDWX    k,s         ;increment k
         ADDX    1,i         
         STWX    k,s         
         BR      for2        
endif2:  STRO    mess,d      
         DECO    total,s     ;print the total
         ADDSP   6,i         ;#total #k #theArray
         STOP                
arrMess: .ASCII  "The Array: \n\x00"
mess:    .ASCII  "\nPositive sum is: \x00"
         .END                  
