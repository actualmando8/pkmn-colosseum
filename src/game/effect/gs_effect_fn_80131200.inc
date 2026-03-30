    nofralloc
    cmplwi r3, 0x0
    beq @L_8013123C
    lis r11, -32714
    addi r11, r11, 13760
    lwz r0, 0x0(r11)
    cmplw r3, r0
    bgt @L_8013123C
    subi r0, r3, 0x1
    lwz r3, 0xc(r11)
    mulli r0, r0, 0x34
    add r3, r3, r0
    lwz r0, 0x4(r3)
    cmpwi r0, -0x1
    beq @L_8013123C
    b @L_80131240
@L_8013123C:
    li r3, 0x0
@L_80131240:
    cmplwi r3, 0x0
    beqlr
    stw r4, 0x8(r3)
    stw r5, 0xc(r3)
    stw r6, 0x10(r3)
    stw r7, 0x14(r3)
    stw r8, 0x20(r3)
    stw r9, 0x18(r3)
    stw r10, 0x1c(r3)
    blr
