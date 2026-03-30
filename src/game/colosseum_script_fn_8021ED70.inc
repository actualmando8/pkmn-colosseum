    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x14(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r30, 0x8(r1)
    bl fn_801F54A4
    lwz r5, lbl_8047B610(r13)
    rlwinm r30, r3, 0, 16, 31
    li r4, 0x0
    lbz r3, 0x1(r5)
    bl fn_801F025C
    mr r31, r3
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021EE18
    mr r3, r31
    li r4, 0x0
    bl fn_80208ED0
    mr r3, r31
    mr r4, r30
    li r5, 0x1
    bl fn_80265598
    mr r3, r31
    li r4, 0x1
    bl fn_80208ED0
    mr r3, r31
    li r4, 0x2
    bl fn_80208ED0
    mr r3, r31
    li r4, 0x3
    bl fn_80208ED0
    mr r3, r31
    li r4, 0x4
    bl fn_80208ED0
    mr r3, r31
    mr r4, r30
    li r5, 0x0
    bl fn_8026532C
@L_8021EE18:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
