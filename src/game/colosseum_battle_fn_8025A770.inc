    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r3
    mr r29, r4
    mr r26, r5
    mr r27, r6
    bl fn_80237774
    mr r30, r3
    mr r3, r25
    mr r4, r27
    bl fn_80237774
    mr r31, r3
    mr r3, r25
    mr r4, r26
    mr r5, r29
    bl fn_802395C8
    lis r7, 0x1
    mr r28, r3
    mr r3, r25
    mr r4, r29
    mr r5, r26
    mr r6, r27
    subi r7, r7, 0x1
    bl fn_8025C264
    mr r0, r3
    mr r3, r25
    mr r29, r0
    mr r4, r26
    bl fn_80239500
    mr r6, r3
    mr r3, r25
    mr r4, r27
    mr r5, r28
    bl fn_8023793C
    clrlwi r0, r3, 16
    cmplwi r0, 0x43
    bne @L_8025A810
    li r29, 0x0
@L_8025A810:
    mr r3, r25
    mr r4, r27
    li r5, 0x5
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025A830
    li r29, 0x0
@L_8025A830:
    clrlwi r3, r30, 24
    clrlwi r0, r31, 24
    cmplw r3, r0
    bge @L_8025A844
    li r29, 0x0
@L_8025A844:
    cmpwi r29, 0x0
    bne @L_8025A854
    li r3, 0x0
    b @L_8025A85C
@L_8025A854:
    cmpwi r29, -0x1
    li r3, 0x1
@L_8025A85C:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
