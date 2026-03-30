    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801F025C
    li r4, 0x2d
    mr r30, r3
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021C4C8
    li r31, 0x0
    b @L_8021C4D8
@L_8021C4C8:
    mr r3, r30
    li r4, 0x2d
    bl fn_80202360
    mr r31, r3
@L_8021C4D8:
    li r3, 0x2d
    bl fn_80119DD0
    rlwinm r3, r3, 0, 24, 31
    extsh r0, r31
    cmpw r0, r3
    bne @L_8021C518
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021C568
@L_8021C518:
    mr r3, r30
    li r4, 0x2d
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8021C540
    mr r3, r30
    li r4, 0x2d
    li r5, 0x0
    bl fn_8020248C
@L_8021C540:
    mr r3, r30
    li r4, 0x2d
    bl fn_80202360
    mr r0, r3
    li r3, 0x2f
    extsh r4, r0
    bl fn_80132A38
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_8021C568:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
