    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r30, r3
    mr r28, r4
    mr r31, r5
    li r29, 0x0
    bl fn_8023831C
    clrlwi r0, r3, 16
    cmplwi r0, 0x3
    beq @L_802455B0
    cmplwi r0, 0x9
    bne @L_802455F8
@L_802455B0:
    mr r4, r30
    li r3, 0x0
    li r5, 0x148
    bl fn_80239984
    mr r0, r3
    mr r3, r28
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
    li r10, 0x148
    bl fn_80239EE8
@L_802455F8:
    lfs f1, -20592(r2)
    mr r3, r30
    mr r4, r28
    li r5, -0x1
    bl fn_802373B0
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80245660
    mr r3, r29
    mr r4, r30
    li r5, 0x149
    bl fn_80239984
    mr r0, r3
    mr r3, r28
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
    li r10, 0x149
    bl fn_80239EE8
@L_80245660:
    mr r3, r30
    mr r4, r28
    bl fn_80237310
    clrlwi. r0, r3, 24
    bne @L_802456BC
    mr r3, r29
    mr r4, r30
    li r5, 0x14a
    bl fn_80239984
    mr r0, r3
    mr r3, r28
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
    li r10, 0x14a
    bl fn_80239EE8
@L_802456BC:
    mr r3, r29
    mr r4, r30
    li r5, 0x14b
    bl fn_80239984
    mr r29, r3
    mr r3, r28
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x14b
    bl fn_80239EE8
    mr r3, r29
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
