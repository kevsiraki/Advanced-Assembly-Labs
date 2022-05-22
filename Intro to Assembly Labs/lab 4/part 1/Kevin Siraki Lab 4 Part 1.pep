;Kevin Siraki Lab 4 Part 1

         BR      main        

;******* struct node
data:    .EQUATE 0           ;struct field #2d
next:    .EQUATE 2           ;struct field #2h

;******* void insert(node*& ptr, int value) {
temp:    .EQUATE 0           ;#2h
valueS:  .EQUATE 4           ;#2d
ptr:     .EQUATE 6           ;#2h
insert:  SUBSP   2,i         ;#temp
         LDWA    4,i         ;node* temp = new node;
         CALL    malloc      
         STWX    temp,s      
         LDWA    ptr,sf      ;temp->next = ptr
         LDWX    next,i      
         STWA    temp,sfx    
         LDWA    valueS,s    ;temp->data = valueS
         LDWX    data,i      
         STWA    temp,sfx    
         LDWA    temp,s      ;ptr = temp
         STWA    ptr,sf      
         ADDSP   2,i         ;#temp
         RET                 

;******* void main ()
root:    .EQUATE 4           ;local variable #2h
p:       .EQUATE 2           ;local variable #2h
value:   .EQUATE 0           ;local variable #2d
main:    SUBSP   6,i         ;push #root #p #value
         LDWA    0,i         ;first = 0
         STWA    root,s      
         DECI    value,s     ;scanf("%d", &value);
while:   LDWA    value,s     ;while (value <= 0)
         CPWA    0,i         
         BRLT    endWh       
         LDWA    value,s     ;pass local variable value
         STWA    -4,s        ;stack location -4
         MOVSPA              ;pass the local pointer variable
         ADDA    root,i      
         STWA    -2,s        
         SUBSP   4,i         ;#ptr #valueS
         CALL    insert      ;inset(root,value);
         ADDSP   4,i         ;#valueS #ptr
         DECI    value,s     ;scanf("%d", &value)
         BR      while       ;reloop
endWh:   LDWA    root,s      ;for (p = first
         STWA    p,s         
for:     LDWA    p,s         ;p != 0
         CPWA    0,i         
         BREQ    endFor      
         LDWX    data,i      ;printf("%d ", p->data)
         DECO    p,sfx       
         LDBA    ' ',i       
         STBA    charOut,d   
         LDWX    next,i      ;p = p->next)
         LDWA    p,sfx       
         STWA    p,s         
         BR      for         
endFor:  ADDSP   6,i         ;pop #value #p #root
         STOP                

;******* malloc()
malloc:  LDWX    hpPtr,d     ;returned pointer
         ADDA    hpPtr,d     ;allocate from heap
         STWA    hpPtr,d     ;update hpPtr
         RET                 
hpPtr:   .ADDRSS heap        ;address of next free byte
heap:    .BLOCK  1           ;first byte in the heap
         .END                  
