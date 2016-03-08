ifdef WIN64

.data
numArgs dq 0
stack   dq 0
funcPtr dq 0
opPtr	dq 0
saveRsp dq 0

.code
callFunc proc funcPtrDontUse:ptr, stackDontUse:ptr, numArgsDontUse:dword, opPtrP:ptr
    push    rdi
    push    rsi

    mov     [funcPtr], rcx    ; simplifies register allocation a lot :)
    mov     [stack], rdx      ; don't use passed variable names above!
    mov     [numArgs], r8     ; MASM does not support this for x64
	mov     [opPtr], r9       ; 

; -------------------- allocate stack space and copy parameters --------------------

	mov		[saveRsp], rsp    ; save rsp

    mov     rcx, [numArgs]    ; number of passed arguments
    sub     rcx, 3            ; 3 of them will be passed in regs
    cmp     rcx, 0            ; some left for the stack?
    jng     noParamsOnStack
    lea     r10, [rcx*8]      ; calculate required stack space
    sub     rsp, r10          ; reserve stack space

    mov     r11, rsp          ; align stack pointer to 16 bytes for later call 
    and     r11, 15           ; mod 16
    jz      dontAlign         ; is already 16 bytes aligned?
    sub     rsp, r11

dontAlign:

    mov     rdi, rsp          ; copy parameters to stack
    mov     rsi, [stack]
    add     rsi, 3*8          ; first 3 parameters are in regs
    rep     movsq

noParamsOnStack:

; -------------------- store first 3 parameters in RDX, R8, R9 --------------------

    mov     rsi, [stack]
    mov     r10, [numArgs]
    cmp     r10, 3
    jge      threeParams
    cmp     r10, 2
    je      twoParams
    cmp     r10, 1
    je      oneParam
    jmp     noParams

threeParams:
    mov     r9, [rsi+16]
    movss   xmm3, dword ptr [rsi+16]
twoParams:
    mov     r8, [rsi+8]
    movss   xmm2, dword ptr [rsi+8]
oneParam:
    mov     rdx, [rsi]
    movss   xmm1, dword ptr [rsi]
noParams:

; -------------------- store opPtr to this-pointer using RCX  --------------------

	mov     rcx, [opPtr]
    movss   xmm0, dword ptr [opPtr]
            
; -------------------- call execution handler for operator --------------------

    sub     rsp, 20h          ; reserve additional space for 4 arguments
    call    [funcPtr]         ; call function pointer	

	mov     rsp, [saveRsp]	  ; restore rsp

; -------------------- restore non-volatile registers --------------------   
	
    pop     rsi
    pop     rdi
    ret
callFunc endp

elseifdef WIN32

.686
.model flat, c                ; calling convention = cdecl
.code

callFunc proc funcPtr:ptr, stack:ptr, numArgs:dword, opPtr:ptr
    push    esi               ; save callee saved regs
    push    edi
    mov     eax, [funcPtr]    ; store stack variables before the
    mov     ecx, [numArgs]    ; new stack frame is installed (they
    mov     esi, [stack]      ; won't be accessible anymore afterwards)
	mov     ebx, [opPtr]      ; 
    push    ebp
    mov     ebp, esp
    sub     esp, ecx          ; space for number of parameters * sizeof(eU32) (= 4)
    sub     esp, ecx
    sub     esp, ecx
    sub     esp, ecx
    mov     edi, esp          ; copy parameters on stack
    rep     movsd
	mov		ecx, ebx
    call    eax               ; call execute handler
    mov     esp, ebp          ; remove stack frame
    pop     ebp
    pop     edi               ; restore callee saved regs
    pop     esi
    ret
callFunc endp

endif

end