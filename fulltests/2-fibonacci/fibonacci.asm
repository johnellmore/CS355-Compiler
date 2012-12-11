global qFibonacci

section .text

qFibonacci:
push ebp
mov ebp, esp
sub esp, 4

mov al, 2
cbw
cwde

push eax
mov eax, [ebp+8]
pop ecx
cmp eax, ecx
jle label0
mov eax, 0
jmp label1
label0: mov eax, 1
label1: 
cmp eax, 1
jne label2
mov al, 1
cbw
cwde

mov [ebp-4], eax

jmp label3
label2: mov al, 2
cbw
cwde

push eax
mov eax, [ebp+8]
pop ecx
sub eax, ecx
push eax
call qFibonacci
add esp, 4
push eax
mov al, 1
cbw
cwde

push eax
mov eax, [ebp+8]
pop ecx
sub eax, ecx
push eax
call qFibonacci
add esp, 4
pop ecx
add eax, ecx
mov [ebp-4], eax

label3: 

mov eax, [ebp-4]

add esp, 4
pop ebp
ret

section .data

