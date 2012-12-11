global qFibonacci

section .text
qFibonacci:
push ebp
mov ebp, esp
sub esp, 4


add esp, 4
pop ebp
ret

section .data
