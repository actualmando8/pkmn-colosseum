    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r4, lbl_8047B610(r13)
    lwz r5, 0x2(r4)
    lbz r3, 0x1(r4)
    cmplwi r5, 0x0
    lhz r6, 0xa(r4)
    lwz r7, 0x6(r4)
    lwz r8, 0xc(r4)
    lwz r31, 0x10(r4)
    bne @L_802139BC
    li r4, 0x0
    b @L_802139C0
@L_802139BC:
    lwz r4, 0x0(r5)
@L_802139C0:
    cmplwi r7, 0x0
    bne @L_802139D0
    li r5, 0x0
    b @L_802139D4
@L_802139D0:
    lbz r5, 0x0(r7)
@L_802139D4:
    cmplwi r8, 0x0
    bne @L_802139E4
    li r7, 0x0
    b @L_802139E8
@L_802139E4:
    lbz r7, 0x0(r8)
@L_802139E8:
    bl fn_80135E44
    stb r3, 0x0(r31)
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x14
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
