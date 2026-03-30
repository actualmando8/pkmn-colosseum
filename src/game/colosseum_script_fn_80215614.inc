    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    bl fn_80207BF4
    mr r0, r3
    mr r3, r31
    mr r28, r0
    bl fn_80205184
    mr r0, r3
    li r3, 0x12
    mr r26, r0
    li r4, 0x0
    bl fn_801F025C
    mr r29, r3
    bl fn_80207BF4
    rlwinm. r0, r28, 0, 16, 31
    mr r27, r3
    bne @L_80215694
    rlwinm. r0, r27, 0, 16, 31
    beq @L_802156D8
@L_80215694:
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x19
    beq @L_802156D8
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x19
    beq @L_802156D8
    mr r3, r30
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802156D8
    mr r3, r26
    mr r4, r31
    mr r5, r29
    bl fn_80229934
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802156E8
@L_802156D8:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021570C
@L_802156E8:
    mr r3, r31
    mr r4, r27
    bl fn_80207BC0
    mr r3, r29
    mr r4, r28
    bl fn_80207BC0
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_8021570C:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
