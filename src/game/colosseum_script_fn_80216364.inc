    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801F025C
    mr r31, r3
    bl fn_80205B8C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    bl fn_80203C5C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802163F0
    mr r3, r30
    bl fn_801252E0
    mr r3, r31
    li r4, 0x0
    bl fn_80202998
    lwz r4, lbl_8047B610(r13)
    mr r3, r31
    addi r0, r4, 0x5
    stw r0, lbl_8047B610(r13)
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802163FC
    mr r3, r31
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
    b @L_802163FC
@L_802163F0:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_802163FC:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
