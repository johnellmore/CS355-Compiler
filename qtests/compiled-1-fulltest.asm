global increment
global sum
global addthree
global thethree
global negate
global position
global andtest

section .text

increment:
push ebp
mov ebp, esp





mov ax, [ebp+8]

pop ebp
ret

sum:
push ebp
mov ebp, esp
sub esp, 8




mov eax, [ebp0]

add esp, 8
pop ebp
ret

addthree:
push ebp
mov ebp, esp
sub esp, 4







add esp, 4
pop ebp
ret

thethree:
push ebp
mov ebp, esp




pop ebp
ret

negate:
push ebp
mov ebp, esp




pop ebp
ret

position:
push ebp
mov ebp, esp
sub esp, 8







add esp, 8
pop ebp
ret

andtest:
push ebp
mov ebp, esp


mov al, [ebp+8]
cbw


pop ebp
ret

section .data



