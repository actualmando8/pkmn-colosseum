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
    bne @L_80213700
    li r4, 0x0
    b @L_80213704
@L_80213700:
    lwz r4, 0x0(r5)
@L_80213704:
    cmplwi r7, 0x0
    bne @L_80213714
    li r5, 0x0
    b @L_80213718
@L_80213714:
    lbz r5, 0x0(r7)
@L_80213718:
    cmplwi r8, 0x0
    bne @L_80213728
    li r7, 0x0
    b @L_8021372C
@L_80213728:
    lbz r7, 0x0(r8)
@L_8021372C:
    lwz r8, 0x0(r9)
    bl fn_80135D10
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x14
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
