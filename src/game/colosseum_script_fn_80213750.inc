    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_8047B610(r13)
    lwz r5, 0x2(r4)
    lbz r3, 0x1(r4)
    cmplwi r5, 0x0
    lhz r6, 0xa(r4)
    lwz r7, 0x6(r4)
    lwz r8, 0xc(r4)
    lwz r9, 0x10(r4)
    bne @L_80213788
    li r4, 0x0
    b @L_8021378C
@L_80213788:
    lwz r4, 0x0(r5)
@L_8021378C:
    cmplwi r7, 0x0
    bne @L_8021379C
    li r5, 0x0
    b @L_802137A0
@L_8021379C:
    lbz r5, 0x0(r7)
@L_802137A0:
    cmplwi r8, 0x0
    bne @L_802137B0
    li r7, 0x0
    b @L_802137B4
@L_802137B0:
    lbz r7, 0x0(r8)
@L_802137B4:
    lhz r8, 0x0(r9)
    bl fn_80135D10
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x14
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
