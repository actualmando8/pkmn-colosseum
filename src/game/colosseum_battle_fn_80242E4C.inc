    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r30, r6
    mr r28, r4
    mr r27, r3
    mr r29, r5
    mr r4, r30
    li r31, 0x0
    bl fn_8023715C
    mr r24, r3
    mr r3, r27
    mr r4, r30
    bl fn_80236FFC
    mr r26, r3
    mr r3, r27
    mr r4, r30
    bl fn_802370AC
    mr r25, r3
    mr r3, r27
    mr r4, r30
    bl fn_80236F4C
    clrlwi r4, r24, 16
    clrlwi r0, r26, 16
    cmplw r4, r0
    mr r26, r3
    ble @L_80242F04
    mr r4, r27
    li r3, 0x0
    li r5, 0x18c
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x18c
    bl fn_80239EE8
@L_80242F04:
    clrlwi r3, r25, 16
    clrlwi r0, r26, 16
    cmplw r3, r0
    ble @L_80242F5C
    mr r3, r31
    mr r4, r27
    li r5, 0x18d
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x18d
    bl fn_80239EE8
@L_80242F5C:
    mr r3, r27
    mr r4, r30
    bl fn_80235AA0
    clrlwi r0, r3, 24
    cmplwi r0, 0x4
    bgt @L_80242FD4
    mr r3, r27
    mr r4, r30
    bl fn_80235A3C
    clrlwi r0, r3, 24
    cmplwi r0, 0x4
    bgt @L_80242FD4
    mr r3, r31
    mr r4, r27
    li r5, 0x18e
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x18e
    bl fn_80239EE8
@L_80242FD4:
    mr r3, r31
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
