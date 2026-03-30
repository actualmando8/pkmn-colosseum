    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    li r28, 0x0
    mr r31, r3
    li r27, 0x0
    b @L_801B8E88
@L_801B8D7C:
    rlwinm r0, r27, 3, 0, 28
    rlwinm r5, r27, 3, 0, 28
    add r3, r31, r0
    rlwinm r4, r27, 3, 0, 28
    lbz r0, 0x34(r3)
    add r5, r31, r5
    add r3, r31, r4
    lwz r30, 0x38(r5)
    cmplwi r0, 0x1
    lbz r29, 0x35(r3)
    bne @L_801B8E84
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801B8E84
    lbz r0, 0xd(r30)
    li r3, 0x0
    cmplwi r0, 0x0
    bne @L_801B8DF8
    lbz r0, 0x35(r30)
    cmplwi r0, 0x7
    bne @L_801B8DF8
    lbz r0, 0x3d(r30)
    cmplwi r0, 0x7
    bne @L_801B8DF8
    lbz r0, 0xf(r30)
    cmplwi r0, 0x0
    bne @L_801B8DF8
    lbz r0, 0x10(r30)
    cmplwi r0, 0x0
    bne @L_801B8DF8
    li r3, 0x1
@L_801B8DF8:
    cmpwi r3, 0x0
    beq @L_801B8E84
    lbz r0, 0x4c(r30)
    cmpwi r0, 0x6
    beq @L_801B8E1C
    bge @L_801B8E84
    cmpwi r0, 0x5
    bge @L_801B8E44
    b @L_801B8E84
@L_801B8E1C:
    lbz r0, 0x20(r31)
    cmplwi r0, 0xff
    bne @L_801B8E34
    lbz r0, 0x20(r30)
    stb r0, 0x20(r31)
    b @L_801B8E44
@L_801B8E34:
    lbz r3, 0x20(r31)
    lbz r0, 0x20(r30)
    cmplw r3, r0
    bne @L_801B8E84
@L_801B8E44:
    rlwinm r0, r27, 3, 0, 28
    lwz r4, 0x4c(r30)
    lwz r5, 0x50(r30)
    add r6, r31, r0
    rlwinm r3, r27, 3, 0, 28
    rlwinm r0, r27, 3, 0, 28
    stw r4, 0x34(r6)
    add r3, r31, r3
    add r4, r31, r0
    stw r5, 0x38(r6)
    lwz r3, 0x38(r3)
    lbz r4, 0x35(r4)
    bl fn_801B7BD4
    mr r3, r30
    mr r4, r29
    bl fn_801B750C
@L_801B8E84:
    addi r27, r27, 0x1
@L_801B8E88:
    cmpwi r27, 0x4
    blt @L_801B8D7C
    li r27, 0x0
    b @L_801B8F98
@L_801B8E98:
    rlwinm r0, r27, 3, 0, 28
    rlwinm r5, r27, 3, 0, 28
    add r3, r31, r0
    rlwinm r4, r27, 3, 0, 28
    lbz r0, 0x54(r3)
    add r5, r31, r5
    add r3, r31, r4
    lwz r30, 0x58(r5)
    cmplwi r0, 0x1
    lbz r29, 0x55(r3)
    bne @L_801B8F94
    lbz r0, 0x19(r30)
    li r3, 0x0
    cmplwi r0, 0x0
    bne @L_801B8F08
    lbz r0, 0x55(r30)
    cmplwi r0, 0x7
    bne @L_801B8F08
    lbz r0, 0x5d(r30)
    cmplwi r0, 0x7
    bne @L_801B8F08
    lbz r0, 0x1b(r30)
    cmplwi r0, 0x0
    bne @L_801B8F08
    lbz r0, 0x1c(r30)
    cmplwi r0, 0x0
    bne @L_801B8F08
    li r3, 0x1
@L_801B8F08:
    cmpwi r3, 0x0
    beq @L_801B8F94
    lbz r0, 0x6c(r30)
    cmpwi r0, 0x6
    beq @L_801B8F2C
    bge @L_801B8F94
    cmpwi r0, 0x5
    bge @L_801B8F54
    b @L_801B8F94
@L_801B8F2C:
    lbz r0, 0x21(r31)
    cmplwi r0, 0xff
    bne @L_801B8F44
    lbz r0, 0x21(r30)
    stb r0, 0x21(r31)
    b @L_801B8F54
@L_801B8F44:
    lbz r3, 0x21(r31)
    lbz r0, 0x21(r30)
    cmplw r3, r0
    bne @L_801B8F94
@L_801B8F54:
    rlwinm r0, r27, 3, 0, 28
    lwz r4, 0x6c(r30)
    lwz r5, 0x70(r30)
    add r6, r31, r0
    rlwinm r3, r27, 3, 0, 28
    rlwinm r0, r27, 3, 0, 28
    stw r4, 0x54(r6)
    add r3, r31, r3
    add r4, r31, r0
    stw r5, 0x58(r6)
    lwz r3, 0x58(r3)
    lbz r4, 0x55(r4)
    bl fn_801B7BD4
    mr r3, r30
    mr r4, r29
    bl fn_801B750C
@L_801B8F94:
    addi r27, r27, 0x1
@L_801B8F98:
    cmpwi r27, 0x4
    blt @L_801B8E98
    mr r3, r28
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
