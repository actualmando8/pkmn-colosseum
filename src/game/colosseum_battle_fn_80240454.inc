    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r30, r3
    mr r27, r4
    mr r31, r5
    mr r4, r6
    li r29, 0x0
    li r3, 0x2
    bl fn_801F025C
    li r4, 0x4a
    mr r28, r3
    bl fn_801F6E98
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802404A8
    mr r3, r28
    li r4, 0x4a
    bl fn_801F6D9C
    b @L_802404AC
@L_802404A8:
    li r3, 0x0
@L_802404AC:
    extsh. r0, r3
    bne @L_80240500
    mr r4, r30
    li r3, 0x0
    li r5, 0x1c4
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1c4
    bl fn_80239EE8
    b @L_802405A8
@L_80240500:
    extsh r0, r3
    cmpwi r0, 0x1
    bne @L_80240558
    mr r4, r30
    li r3, 0x0
    li r5, 0x1c5
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1c5
    bl fn_80239EE8
    b @L_802405A8
@L_80240558:
    cmpwi r0, 0x2
    bne @L_802405A8
    mr r4, r30
    li r3, 0x0
    li r5, 0x1c6
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1c6
    bl fn_80239EE8
@L_802405A8:
    mr r3, r29
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
