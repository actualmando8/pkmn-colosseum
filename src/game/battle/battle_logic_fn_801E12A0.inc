    nofralloc
    lwz r4, -21492(r13)
    addi r0, r4, 0x1
    cmplwi r0, 0x4
    blt @L_801E12B8
    li r3, 0x0
    blr
@L_801E12B8:
    lis r4, -32698
    li r0, 0x4
    addi r4, r4, 31992
    mr r5, r4
    mtctr r0
@L_801E12CC:
    lwz r0, 0x0(r5)
    cmplwi r0, 0x0
    bne @L_801E12F0
    lwz r4, -21492(r13)
    stw r3, 0x0(r5)
    li r3, 0x1
    addi r0, r4, 0x1
    stw r0, -21492(r13)
    blr
@L_801E12F0:
    addi r5, r5, 0x4
    bdnz @L_801E12CC
    li r3, 0x0
    blr
