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
    mr r0, r3
    li r3, 0x0
    mr r31, r0
    mr r4, r31
    bl fn_801F4354
    mr r3, r31
    li r4, 0x2
    bl fn_8020912C
    mr r3, r31
    mr r4, r30
    li r5, 0x0
    bl fn_8026532C
    mr r3, r31
    li r4, 0x3
    bl fn_8020912C
    lis r3, -32733
    mr r5, r31
    addi r4, r3, 12260
    li r6, 0x0
    li r3, 0x0
    bl fn_801F37B0
    mr r3, r31
    bl fn_80205B8C
    bl fn_801252E0
    mr r3, r31
    li r4, 0x0
    bl fn_80202998
    mr r3, r31
    li r4, 0x17
    bl fn_80202810
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80223FF4
    mr r3, r31
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80223FF4:
    mr r3, r31
    li r4, 0x3e
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80224040
    mr r3, r31
    li r4, 0x3e
    bl fn_80202810
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80224040
    mr r3, r31
    li r4, 0xc8
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80224040:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
