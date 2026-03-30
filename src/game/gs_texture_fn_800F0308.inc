    nofralloc
    stw r3, lbl_8047AC18(r13)
    stw r5, lbl_8047AC14(r13)
    mflr r5
    lwz r3, lbl_8047AC20(r13)
    stw r3, lbl_8047AC1C(r13)
    bl fn_800F015C
    stw r5, 0x80(r3)
    lwz r5, lbl_8047AC18(r13)
    stw r5, 0xc(r3)
    lwz r5, lbl_8047AC14(r13)
    stw r5, 0x14(r3)
    stw r1, 0x4(r3)
    lwz r5, lbl_8047AC10(r13)
    cmplwi r5, 0x0
    beq @L_800F0348
    bl fn_800F01F0
    @L_800F0348:
    lwz r3, lbl_8047AC24(r13)
    stw r3, lbl_8047AC1C(r13)
    bl fn_800F0030
    lwz r5, 0x84(r3)
    mtlr r5
    lwz r1, 0x4(r3)
    lwz r5, 0x14(r3)
    lwz r3, 0xc(r3)
    blr
