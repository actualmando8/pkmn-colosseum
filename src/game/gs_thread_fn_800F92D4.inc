    nofralloc
    lwz r0, lbl_8047AC60(r13)
    lwz r5, lbl_8047AC5C(r13)
    mtctr r0
    cmplwi r0, 0x0
    beq @L_800F9310
@L_800F92E8:
    lwz r4, 0x4(r5)
    cmplwi r4, 0x0
    beq @L_800F9308
    lwz r0, 0xc(r5)
    cmplw r0, r3
    bne @L_800F9308
    mr r3, r4
    blr
@L_800F9308:
    addi r5, r5, 0x14
    bdnz @L_800F92E8
@L_800F9310:
    li r3, 0x0
    blr
