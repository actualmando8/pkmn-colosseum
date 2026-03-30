    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x8(r1)
    lwz r12, lbl_8047A940(r13)
    cmplwi r12, 0x0
    beq @L_800AE528
    lwz r3, lbl_8047A938(r13)
    mtlr r12
    blrl
    li r0, 0x0
    stw r0, lbl_8047A938(r13)
    stw r0, lbl_8047A940(r13)
    b @L_800AE54C
    @L_800AE528:
    lwz r12, lbl_8047A944(r13)
    cmplwi r12, 0x0
    beq @L_800AE54C
    lwz r3, lbl_8047A93C(r13)
    mtlr r12
    blrl
    li r0, 0x0
    stw r0, lbl_8047A93C(r13)
    stw r0, lbl_8047A944(r13)
    @L_800AE54C:
    lwz r6, lbl_8047A928(r13)
    cmplwi r6, 0x0
    beq @L_800AE5A0
    lwz r3, 0x8(r6)
    cmplwi r3, 0x0
    bne @L_800AE578
    lwz r4, 0x10(r6)
    lwz r5, 0x14(r6)
    lwz r6, 0x18(r6)
    bl fn_800AC990
    b @L_800AE588
    @L_800AE578:
    lwz r4, 0x14(r6)
    lwz r5, 0x10(r6)
    lwz r6, 0x18(r6)
    bl fn_800AC990
    @L_800AE588:
    lwz r3, lbl_8047A928(r13)
    lwz r0, 0x1c(r3)
    stw r0, lbl_8047A940(r13)
    stw r3, lbl_8047A938(r13)
    lwz r0, 0x0(r3)
    stw r0, lbl_8047A928(r13)
    @L_800AE5A0:
    lwz r0, lbl_8047A938(r13)
    cmplwi r0, 0x0
    bne @L_800AE5B0
    bl fn_800AE3F0
    @L_800AE5B0:
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
