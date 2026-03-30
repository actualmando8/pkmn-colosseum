    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    rlwinm r31, r3, 0, 24, 31
    b @L_80109740
@L_80109730:
    lbz r0, lbl_8047AD20(r13)
    cmplwi r0, 0x0
    bne @L_80109748
    bl fn_800F0308
@L_80109740:
    cmplwi r31, 0x0
    bne @L_80109730
@L_80109748:
    lbz r0, lbl_8047AD20(r13)
    rlwinm r3, r0, 0, 24, 31
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

