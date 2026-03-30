    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x2e
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212874
    mr r3, r31
    li r4, 0x2e
    bl fn_80202810
@L_80212874:
    mr r3, r31
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212898
    mr r3, r31
    li r4, 0x15
    bl fn_80202810
@L_80212898:
    mr r3, r31
    li r4, 0x28
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802128BC
    mr r3, r31
    li r4, 0x28
    bl fn_80202810
@L_802128BC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
