section .data
    zero dd 0.0
    half dd 0.5
    two dd 2.0
    three dd 3.0
    four dd 4.0
    five dd 5.0
    six dd 6.0
    nine dd 9.0
    ten dd 10.0
    tfour dd 24.0
    


section .text
global f1
f1:  ; f1 = 3 (0.5 / (x + 1) + 1)
    push ebp
    mov ebp, esp
    
    fld dword[half] ; 0.5
    fld dword[ebp+8]  ; x ... 0.5
    fld1   ; 1 ... x ... 0.5
    faddp
    fdivp    ; 0.5 / (x + 1)
    fld1     ;1 ... 0.5 / (x + 1)
    faddp    ; (0.5 / (x + 1) + 1)
    fld dword[three] ; 3 ... (0.5 / (x + 1) + 1)
    fmulp  ; 3 (0.5 / (x + 1) + 1)
    
    mov ebp, esp
    pop ebp
    ret

global f2
f2:  ; f2 = 2.5x − 9.5
    push ebp
    mov ebp, esp
    
    fld dword[half] ; 0.5
    fld dword[two] ; 2 ... 0.5
    faddp     ; 2.5
    fld dword[ebp+8]  ; x ... 2.5
    fmulp   ; 2.5x
    fld dword[nine] ; 9 ... 2.5x
    fld dword[half] ; 0.5 ... 9 ... 2.5x
    faddp    ; 9.5 ... 2.5x
    fsubp   ; 2.5x − 9.5
    
    mov ebp, esp
    pop ebp
    ret

global f3
f3:  ; f3 = 5 / x (x > 0)
    push ebp
    mov ebp, esp
    
    fld dword[five] ; 5 ... x
    fld dword[ebp+8]  ; x 
    fdivp    ; 5 / x
    
    mov ebp, esp
    pop ebp
    ret

global df1
df1:  ; df1 = -6 / (2x + 2)^2
    push ebp
    mov ebp, esp
    
    fld dword[ebp+8]  ; x
    fld dword[two]  ;  2 ... x
    fmulp  
    fld dword[two]  ; 2 ... 2x
    faddp
    fmul st0, st0
    fld dword[six]
    fdivrp
    fchs
    
    mov ebp, esp
    pop ebp
    ret

global df2
df2:  ; df2 = 2.5
    push ebp
    mov ebp, esp
    
    fld dword[half] ; 0.5
    fld dword[two] ; 2 ... 0.5
    faddp     ; 2.5
    
    mov ebp, esp
    pop ebp
    ret

global df3
df3:  ; df3 = -5/x^2
    push ebp
    mov ebp, esp
    
    fld dword[ebp+8]  ; x
    fld dword[ebp+8]  ; x ... x
    fmulp   ; x^2
    fld dword[five]  ; 5 ... x^2
    fdivrp   ; 5/x^2
    fchs
    
    mov ebp, esp
    pop ebp
    ret

global d2f1
d2f1:  ; d2f1 = 24/(2x+2)^3
    push ebp
    mov ebp, esp
    
    fld dword[ebp+8]  ; x
    fld dword[two]  ;  2 ... x
    fmulp  
    fld dword[two]  ; 2 ... 2x
    faddp   ; 2x+2
    fmul st0, st0  ; (2x+2)^2
    fld dword[ebp+8]  ; x
    fld dword[two]  ;  2 ... x
    fmulp  
    fld dword[two]  ; 2 ... 2x
    faddp   ; 2x+2 ... (2x+2)^2
    fmulp
    fld dword[tfour]
    fdivrp
    
    mov ebp, esp
    pop ebp
    ret

global d2f2
d2f2:  ; d2f2 = 0
    push ebp
    mov ebp, esp
    
    fld dword[zero] ; 0.0
    
    mov ebp, esp
    pop ebp
    ret

global d2f3
d2f3:  ; d2f3 = 10/x^3
    push ebp
    mov ebp, esp
    
    fld dword[ebp+8]  ; x
    fld dword[ebp+8]  ; x ... x
    fmulp   ; x^2
    fld dword[ebp+8]  ; x ... x^2
    fmulp   ; x^3
    fld dword[ten]  ; 10 ... x^3
    fdivrp   ; 10/x^3
    
    mov ebp, esp
    pop ebp
    ret 
  
