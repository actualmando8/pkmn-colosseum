    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    lwz r4, lbl_8047B610(r13)
    lbz r4, 0x2(r4)
    bl fn_80207AE0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80222478
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x3(r3)
    b @L_80222480
@L_80222478:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
@L_80222480:
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
