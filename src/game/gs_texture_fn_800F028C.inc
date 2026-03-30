    nofralloc
    stwu r1, -0xc(r1)
    stw r3, 0xc(r1)
    mflr r3
    stw r3, 0x8(r1)
    lwz r3, lbl_8047AC24(r13)
    stw r3, lbl_8047AC1C(r13)
    bl fn_800F015C
    stw r1, 0x4(r3)
    lwz r3, lbl_8047AC20(r13)
    stw r3, lbl_8047AC1C(r13)
    bl fn_800F0030
    lwz r5, lbl_8047AC10(r13)
    cmplwi r5, 0x0
    beq @L_800F02C8
    bl fn_800F00C0
    @L_800F02C8:
    lwz r5, 0x84(r3)
    mtlr r5
    lwz r1, 0x4(r3)
    li r5, 0x0
    subi r5, r5, 0x1
    stw r5, 0x0(r1)
    lwz r5, 0x80(r3)
    mtctr r5
    lwz r5, 0x14(r3)
    lwz r3, 0xc(r3)
    bctr
