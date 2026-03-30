    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x3
    stw r31, 0xc(r1)
    mr r31, r3
    bne @L_80101B5C
    bl fn_800BE31C
    stw r3, lbl_8047ACF8(r13)
@L_80101B5C:
    lwz r12, lbl_8047ACF4(r13)
    cmplwi r12, 0x0
    beq @L_80101B74
    mr r3, r31
    mtctr r12
    bctrl
@L_80101B74:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

