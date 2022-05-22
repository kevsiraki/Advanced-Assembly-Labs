.data
.balign
cstring:.space  20,0 		
prompt: .asciz  "Enter sentence : "
format: .asciz  "%[^\n]s"		
output: .asciz  "Caps: %s\n"

.text
.global main
.extern printf
.extern scanf

main:   push    {ip, lr}		
        ldr     r0, =prompt		
        bl      printf
        ldr     r0, =format     
        ldr     r1, =cstring	
        bl      scanf
        ldr     r4, =cstring	
while:  ldrsb   r2, [r4]		
        cmp     r2, #0			
        beq     endwh			
        cmp     r2, #'a'       	
		blt     endif
        cmp     r2, #'z'        
		bgt     endif
        and     r2, # 0xDF		 
        strb	r2, [r4]        
endif:  add     r4, #1          
        b       while			
endwh:	ldr     r0, =output 	
        ldr     r1, =cstring
        bl      printf
        pop     {ip, pc}        
