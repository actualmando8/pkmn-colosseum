    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, lbl_8047AB20(r13)
    lwz r3, lbl_8047AB1C(r13)
    mtctr r0
    cmplwi r0, 0x0
    ble @L_800DF7DC
    @L_800DF7C4:
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_800DF7D4
    b @L_800DF7E0
    @L_800DF7D4:
    addi r3, r3, 0x40
    bdnz @L_800DF7C4
    @L_800DF7DC:
    li r3, 0x0
    @L_800DF7E0:
    cmplwi r3, 0x0
    bne @L_800DF800
    lis r3, lbl_8027056C@ha
    addi r3, r3, lbl_8027056C@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_800DF844
    @L_800DF800:
    li r7, 0x1
    lis r4, 0xfeff
    stb r7, 0x0(r3)
    subi r6, r4, 0x102
    li r5, 0x0
    li r4, 0x2
    stw r6, 0x3c(r3)
    li r0, 0x3
    sth r5, 0x2(r3)
    stw r5, 0x10(r3)
    stw r7, 0x14(r3)
    stw r4, 0x18(r3)
    stw r0, 0x1c(r3)
    stw r5, 0x28(r3)
    stw r5, 0x20(r3)
    stw r5, 0x24(r3)
    stw r6, 0x38(r3)
    @L_800DF844:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
