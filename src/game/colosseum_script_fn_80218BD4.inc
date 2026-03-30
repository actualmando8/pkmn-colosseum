    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    bl fn_801F025C
    mr r4, r3
    li r3, 0x2
    bl fn_801F025C
    mr r30, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r29, r3
    li r3, 0x2
    mr r4, r29
    bl fn_801F025C
    mr r31, r3
    mr r3, r29
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80218C54
    cmplw r30, r31
    beq @L_80218C54
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 7, 7
    bne @L_80218C54
    ori r0, r3, 0x40
    stw r0, lbl_8047B618(r13)
@L_80218C54:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
