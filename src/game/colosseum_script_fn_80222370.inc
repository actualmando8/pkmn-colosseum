    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    lwz r3, lbl_8047B610(r13)
    lwz r4, 0x3(r3)
    cmplwi r4, 0x0
    beq @L_802223A4
    lhz r0, 0x0(r4)
    b @L_802223A8
@L_802223A4:
    li r0, 0x0
@L_802223A8:
    lbz r4, 0x2(r3)
    rlwinm r5, r0, 0, 16, 31
    lbz r3, 0x1(r3)
    bl fn_80221104
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
