    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x14
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802207BC
    mr r3, r31
    bl fn_801FEF74
    cmpwi r3, 0x0
    bgt @L_802207BC
    mr r3, r31
    li r4, 0x14
    bl fn_80202810
@L_802207BC:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
