mov 1,R0
mov 1,R1
jmp .L2
.L3:
mov R0,R2
mul R1,R2
mov R2,R0
add 1,R1
.L2:
cmp 4,R1
jle .L3
mov R0,R3
