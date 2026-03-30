    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r30, r4
    mr r28, r5
    mr r27, r3
    mr r29, r6
    mr r5, r30
    mr r4, r28
    bl fn_802395C8
    mr r31, r3
    mr r3, r27
    mr r4, r30
    mr r5, r28
    mr r6, r29
    li r7, 0x0
    bl fn_8025C264
    mr r30, r3
    mr r3, r27
    mr r4, r28
    bl fn_80239500
    mr r6, r3
    mr r3, r27
    mr r4, r29
    mr r5, r31
    bl fn_8023793C
    clrlwi r0, r3, 16
    cmplwi r0, 0x43
    bne @L_802554B4
    li r30, 0x0
@L_802554B4:
    cmpwi r30, 0x0
    bne @L_802554C4
    li r3, 0x0
    b @L_802554D8
@L_802554C4:
    cmpwi r30, -0x1
    bne @L_802554D4
    li r3, 0x1
    b @L_802554D8
@L_802554D4:
    li r3, 0x1
@L_802554D8:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
