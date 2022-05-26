section .data
    two dd 2.0
    four dd 4.0
    three dd 3.0
    six dd 6.0
    eight dd 8.0
    twfour dd 24.00

section .text
global f1
global f2
global f3

global df1
global df2
global df3

global d2f1
global d2f2
global d2f3

f1:   ;  1 + (4 /(x^2 + 1))
    push ebp 
    mov ebp, esp 
    
    FINIT
    
    fld dword[four]
    fld1
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    
    fmulp
    faddp
    
    fdivp
    fld1
    faddp


    mov esp, ebp
    pop ebp
    ret
    
    
f2:   ;  x^3
    push ebp      
    mov ebp, esp 
    
    FINIT
    
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    
    fmulp
    fmulp
    
    mov esp, ebp
    pop ebp
    ret
  
f3:   ;  2^(-x) F2XM1
    push ebp      
    mov ebp, esp
    
    FINIT
    
    fld dword[ebp+8]  ; x
    fld1    ; 1...x
    fld dword[ebp+8] ; x ... 1 ... x
    fprem    ; дробная часть ... 1 ... x
    
    fsub st2, st0
    
    F2XM1
    faddp
    
    fscale
    
    fld1
    
    fdivrp
    
    mov esp, ebp
    pop ebp
    ret
    
df1: 

    push ebp     
    mov ebp, esp 
    
    fld dword[ebp + 8]
    fld dword[eight]
          
    fmulp
    
    fld1
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    
    fmulp
    faddp
    
    fld1
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    
    fmulp
    faddp
    
    fmulp
    
    fdivp
    
    FCHS
    
    mov esp, ebp
    pop ebp
    ret
    
df2: 

    push ebp      
    mov ebp, esp 
    
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    fld dword[three]
    
    fmulp
    fmulp
    
    mov esp, ebp
    pop ebp
    ret
    
df3: 
    push ebp       
    mov ebp, esp 
    
    FINIT
    
    fld dword[ebp+8]  ; x
    fld1    ; 1...x
    fld dword[ebp+8] ; x ... 1 ... x
    fprem    ; дробная часть ... 1 ... x
    
    fsub st2, st0
    
    F2XM1
    faddp
    
    fscale
    
    fldln2
    
    fdivrp
    
    FCHS
    
    mov esp, ebp
    pop ebp
    ret
    
d2f1: 

    push ebp     
    mov ebp, esp
    
      

    fld dword[twfour]
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    fmulp
    fmulp
    
    fld dword[eight]
    fsubp
    
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    
    fmulp
    fmulp
    fmulp
    fmulp
    fmulp
    
    fld dword[three]
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    
    fmulp
    fmulp
    fmulp
    fmulp
    
    fld dword[three]
    fld dword[ebp + 8]
    fld dword[ebp + 8]
    
    fmulp
    fmulp
    
    fld1
    faddp
    faddp
    faddp
    
    fdivp
    
    mov esp, ebp
    pop ebp
    ret
    
d2f2: 

    push ebp      
    mov ebp, esp 
    
    fld dword[six]
    fld dword[ebp + 8]

    fmulp
    
    mov esp, ebp
    pop ebp
    ret
    
d2f3: 
    push ebp       
    mov ebp, esp 
    
    FINIT
    
    fld dword[ebp+8]  ; x
    fld1    ; 1...x
    fld dword[ebp+8] ; x ... 1 ... x
    fprem    ; дробная часть ... 1 ... x
    
    fsub st2, st0
    
    F2XM1
    faddp
    
    fscale
    
    fldln2
    fldln2
    
    fmulp
    
    fdivrp
    
    mov esp, ebp
    pop ebp
    ret
