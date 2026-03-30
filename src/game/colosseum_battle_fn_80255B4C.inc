    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r27, r4
    mr r28, r5
    mr r26, r3
    mr r29, r6
    mr r5, r27
    mr r4, r28
    bl fn_802395C8
    mr r0, r3
    mr r3, r26
    mr r30, r0
    mr r4, r27
    mr r5, r28
    mr r6, r29
    li r7, 0x0
    bl fn_8025C264
    mr r0, r3
    mr r3, r26
    mr r31, r0
    mr r4, r27
    li r5, 0x2d
    bl fn_80236BFC
    clrlwi. r0, r3, 24
    bne @L_80255BBC
    li r31, 0x0
@L_80255BBC:
    mr r3, r26
    mr r4, r28
    bl fn_80239500
    mr r6, r3
    mr r3, r26
    mr r4, r29
    mr r5, r30
    bl fn_8023793C
    clrlwi r0, r3, 16
    cmplwi r0, 0x43
    bne @L_80255BEC
    li r31, 0x0
@L_80255BEC:
    cmpwi r31, 0x0
    bne @L_80255BFC
    li r3, 0x0
    b @L_80255C04
@L_80255BFC:
    cmpwi r31, -0x1
    li r3, 0x1
@L_80255C04:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
