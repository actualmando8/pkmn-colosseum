    nofralloc
    cmplwi r3, 0x0
    beq @L_801310E4
    lis r4, -32714
    addi r4, r4, 13760
    lwz r0, 0x0(r4)
    cmplw r3, r0
    bgt @L_801310E4
    subi r0, r3, 0x1
    lwz r3, 0xc(r4)
    mulli r0, r0, 0x34
    add r3, r3, r0
    lwz r0, 0x4(r3)
    cmpwi r0, -0x1
    beq @L_801310E4
    b @L_801310E8
@L_801310E4:
    li r3, 0x0
@L_801310E8:
    cmplwi r3, 0x0
    beq @L_80131114
    lwz r0, 0x4(r3)
    cmpwi r0, -0x1
    beq @L_80131114
    cmpwi r0, 0x0
    beq @L_80131114
    cmpwi r0, 0x1
    beq @L_80131114
    li r3, 0x1
    blr
@L_80131114:
    li r3, 0x0
    blr
