    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    la r31, lbl_80478D78(r13)
    lis r4, lbl_80279EF4@ha
    lbz r0, 0x3(r31)
    addi r4, r4, lbl_80279EF4@l
    mr r30, r3
    rlwinm r0, r0, 1, 0, 30
    lhzx r4, r4, r0
    cmplwi r4, 0x0
    mr r29, r4
    beq @L_802247A4
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802247A4
    mr r3, r30
    mr r4, r29
    bl fn_80202810
@L_802247A4:
    lwz r3, lbl_8047B610(r13)
    li r4, 0x0
    stb r4, 0x3(r31)
    addi r0, r3, 0x2
    stb r4, lbl_8047B625(r13)
    stw r0, lbl_8047B610(r13)
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
