    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r31, r4
    mr r28, r5
    mr r27, r3
    mr r29, r6
    li r4, 0x0
    li r5, 0x1
    bl fn_80235B04
    clrlwi r0, r3, 24
    cmplwi r0, 0x2
    bne @L_8025401C
    li r30, 0xb
    b @L_80254050
@L_8025401C:
    cmplwi r0, 0x3
    bne @L_8025402C
    li r30, 0x5
    b @L_80254050
@L_8025402C:
    cmplwi r0, 0x1
    bne @L_8025403C
    li r30, 0xa
    b @L_80254050
@L_8025403C:
    cmplwi r0, 0x4
    bne @L_8025404C
    li r30, 0xf
    b @L_80254050
@L_8025404C:
    li r30, 0x0
@L_80254050:
    mr r3, r27
    mr r4, r31
    mr r5, r28
    mr r6, r29
    li r7, 0x0
    bl fn_8025C264
    mr r0, r3
    mr r3, r27
    mr r31, r0
    mr r4, r28
    bl fn_80239500
    mr r6, r3
    mr r3, r27
    mr r4, r29
    mr r5, r30
    bl fn_8023793C
    clrlwi r0, r3, 16
    cmplwi r0, 0x43
    bne @L_802540A0
    li r31, 0x0
@L_802540A0:
    cmpwi r31, 0x0
    bne @L_802540B0
    li r3, 0x0
    b @L_802540B8
@L_802540B0:
    cmpwi r31, -0x1
    li r3, 0x1
@L_802540B8:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
