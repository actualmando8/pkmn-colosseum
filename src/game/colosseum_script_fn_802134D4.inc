    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802134FC
    li r3, 0x1
    b @L_80213544
@L_802134FC:
    mr r3, r31
    li r4, 0x11
    bl fn_80202810
    mr r3, r31
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213540
    mr r3, r31
    li r4, 0x22
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213540
    mr r3, r31
    bl fn_80200B10
@L_80213540:
    li r3, 0x1
@L_80213544:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
