    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    lwz r4, lbl_8047B610(r13)
    lwz r0, 0x2(r4)
    lwz r31, 0x7(r4)
    rlwinm r4, r0, 0, 16, 31
    bl fn_802026E4
    lwz r4, lbl_8047B610(r13)
    rlwinm r0, r3, 0, 24, 31
    lbz r3, 0x6(r4)
    cmplw r3, r0
    bne @L_80223AD8
    mr r0, r31
    b @L_80223ADC
@L_80223AD8:
    addi r0, r4, 0xb
@L_80223ADC:
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
