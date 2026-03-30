    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    rlwinm r31, r3, 0, 24, 31
    b @L_8010968C
@L_8010967C:
    lbz r0, lbl_8047AD24(r13)
    cmplwi r0, 0x0
    beq @L_80109694
    bl fn_800F0308
@L_8010968C:
    cmplwi r31, 0x0
    bne @L_8010967C
@L_80109694:
    lbz r3, lbl_8047AD24(r13)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

