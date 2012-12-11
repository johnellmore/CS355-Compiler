global increment
global multiply
global divide
global subtract
global mod
global lessb
global greaterb
global lesseq
global greatereq
global equal
global notequal

section .text

increment:
push ebp
mov ebp, esp


mov al, 1
cbw
cwde

push eax
mov eax, [ebp+8]
pop ecx
add eax, ecx

pop ebp
ret

multiply:
push ebp
mov ebp, esp


mov eax, [ebp+12]
push eax
mov eax, [ebp+8]
pop ecx
imul eax, ecx

pop ebp
ret

divide:
push ebp
mov ebp, esp


mov eax, [ebp+12]
push eax
mov eax, [ebp+8]
pop ecx
xor edx, edx
idiv ecx

pop ebp
ret

subtract:
push ebp
mov ebp, esp


mov eax, [ebp+12]
push eax
mov eax, [ebp+8]
pop ecx
sub eax, ecx

pop ebp
ret

mod:
push ebp
mov ebp, esp


mov eax, [ebp+12]
push eax
mov eax, [ebp+8]
pop ecx
xor edx, edx
idiv ecx
mov eax, edx

pop ebp
ret

lessb:
push ebp
mov ebp, esp


mov eax, [ebp+12]
push eax
mov eax, [ebp+8]
pop ecx
cmp eax, ecx
jl lab_0
mov eax, 0
jmp lab_1
lab_0: mov eax, 1
lab_1: 

pop ebp
ret

greaterb:
push ebp
mov ebp, esp


mov eax, [ebp+12]
push eax
mov eax, [ebp+8]
pop ecx
cmp eax, ecx
jg lab_2
mov eax, 0
jmp lab_3
lab_2: mov eax, 1
lab_3: 

pop ebp
ret

lesseq:
push ebp
mov ebp, esp


mov eax, [ebp+12]
push eax
mov eax, [ebp+8]
pop ecx
cmp eax, ecx
jle lab_4
mov eax, 0
jmp lab_5
lab_4: mov eax, 1
lab_5: 

pop ebp
ret

greatereq:
push ebp
mov ebp, esp


mov eax, [ebp+12]
push eax
mov eax, [ebp+8]
pop ecx
cmp eax, ecx
jge lab_6
mov eax, 0
jmp lab_7
lab_6: mov eax, 1
lab_7: 

pop ebp
ret

equal:
push ebp
mov ebp, esp


mov eax, [ebp+12]
push eax
mov eax, [ebp+8]
pop ecx
cmp eax, ecx
je lab_8
mov eax, 0
jmp lab_9
lab_8: mov eax, 1
lab_9: 

pop ebp
ret

notequal:
push ebp
mov ebp, esp


mov eax, [ebp+12]
push eax
mov eax, [ebp+8]
pop ecx
cmp eax, ecx
jne lab_10
mov eax, 0
jmp lab_11
lab_10: mov eax, 1
lab_11: 

pop ebp
ret

section .data

