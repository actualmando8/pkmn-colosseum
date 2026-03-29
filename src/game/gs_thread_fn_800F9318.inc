    nofralloc
    lwz r0, lbl_8047AC60(r13)
    lwz r5, lbl_8047AC5C(r13)
    mtctr r0
    cmplwi r0, 0x0
    beq @L_800F935C
@L_800F932C:
    lwz r0, 0x4(r5)
    cmplwi r0, 0x0
    beq @L_800F9354
    lwz r0, 0x8(r5)
    cmplw r0, r3
    bne @L_800F9354
    lwz r0, 0xc(r5)
    cmplw r0, r4
    bne @L_800F9354
    b @L_800F9360
@L_800F9354:
    addi r5, r5, 0x14
    bdnz @L_800F932C
@L_800F935C:
    li r5, 0x0
@L_800F9360:
    cmplwi r5, 0x0
    bne @L_800F9370
    li r3, 0x0
    blr
@L_800F9370:
    lwz r3, 0x4(r5)
    blr
