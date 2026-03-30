    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80211924
    li r3, 0x1
    b @L_80211934
@L_80211924:
    mr r3, r31
    li r4, 0x11
    bl fn_80202810
    li r3, 0x1
@L_80211934:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
