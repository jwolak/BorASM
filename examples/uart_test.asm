    MOV R0, #0  ;LOAD immediate value 0 to R0
loop:
    ADD R0, #1
    CMP R0, #10
    JZ reset
    JMP loop
reset:
    MOV R0, #0
    JMP loop