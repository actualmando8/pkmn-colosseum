    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, lbl_8047B610(r13)
    lwz r3, 0x1(r3)
    bl fn_802624CC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802262B4
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x7(r3)
@L_802262B4:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x3
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
