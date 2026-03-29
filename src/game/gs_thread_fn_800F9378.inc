    nofralloc
    lwz r8, lbl_8047AC5C(r13)
    lwz r9, lbl_8047AC60(r13)
    mr r7, r8
    mtctr r9
    cmplwi r9, 0x0
    beq @L_800F93C0
@L_800F9390:
    lwz r0, 0x4(r7)
    cmplwi r0, 0x0
    beq @L_800F93B8
    lwz r0, 0x8(r7)
    cmplw r0, r4
    bne @L_800F93B8
    lwz r0, 0xc(r7)
    cmplw r0, r5
    bne @L_800F93B8
    b @L_800F93C4
@L_800F93B8:
    addi r7, r7, 0x14
    bdnz @L_800F9390
@L_800F93C0:
    li r7, 0x0
@L_800F93C4:
    cmplwi r7, 0x0
    bnelr
    mtctr r9
    cmplwi r9, 0x0
    ble @L_800F93F0
@L_800F93D8:
    lwz r0, 0x4(r8)
    cmplwi r0, 0x0
    bne @L_800F93E8
    b @L_800F93F4
@L_800F93E8:
    addi r8, r8, 0x14
    bdnz @L_800F93D8
@L_800F93F0:
    li r8, 0x0
@L_800F93F4:
    cmplwi r8, 0x0
    beqlr
    li r0, 0x0
    sth r0, 0x0(r8)
    stw r3, 0x4(r8)
    stw r4, 0x8(r8)
    stw r5, 0xc(r8)
    stw r6, 0x10(r8)
    blr
