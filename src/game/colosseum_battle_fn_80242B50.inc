    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r6
    mr r30, r4
    mr r29, r3
    mr r31, r5
    mr r4, r25
    li r26, 0x0
    bl fn_80236FFC
    mr r28, r3
    mr r3, r29
    mr r4, r25
    bl fn_8023715C
    mr r27, r3
    mr r3, r29
    mr r4, r25
    bl fn_802370AC
    clrlwi r4, r28, 16
    clrlwi r0, r27, 16
    cmplw r4, r0
    mr r28, r3
    ble @L_80242BF8
    mr r4, r29
    li r3, 0x0
    li r5, 0x191
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x191
    bl fn_80239EE8
@L_80242BF8:
    clrlwi r3, r27, 16
    clrlwi r0, r28, 16
    cmplw r3, r0
    ble @L_80242C50
    mr r3, r26
    mr r4, r29
    li r5, 0x192
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x192
    bl fn_80239EE8
@L_80242C50:
    mr r3, r29
    mr r4, r25
    li r5, 0x14
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80242CB4
    mr r3, r26
    mr r4, r29
    li r5, 0x193
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x193
    bl fn_80239EE8
@L_80242CB4:
    mr r3, r26
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
