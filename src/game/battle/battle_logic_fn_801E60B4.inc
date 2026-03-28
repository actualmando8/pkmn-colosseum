    nofralloc
    lwz r6, -21208(r13)
    li r8, 0x0
    lwz r5, -21204(r13)
    li r9, 0x0
    lbz r7, 0x0(r6)
    li r3, 0x1
    b @L_801E6108
@L_801E60D0:
    clrlwi r4, r7, 24
    b @L_801E60E8
@L_801E60D8:
    clrlslwi r0, r8, 16, 1
    sthx r9, r5, r0
    addi r8, r8, 0x1
    addi r9, r9, 0x1
@L_801E60E8:
    clrlwi r0, r8, 16
    lbzx r0, r6, r0
    cmplw r4, r0
    beq @L_801E60D8
    clrlwi r0, r9, 16
    slw r0, r0, r3
    clrlwi r9, r0, 16
    addi r7, r7, 0x1
@L_801E6108:
    clrlwi r0, r8, 16
    lbzx r0, r6, r0
    cmplwi r0, 0x0
    bne @L_801E60D0
    blr
