    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r4
    mr r29, r5
    mr r27, r3
    li r30, 0x0
    li r4, 0x0
    li r5, 0x1
    bl fn_80235B04
    mr r31, r3
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80245834
    mr r4, r27
    li r3, 0x0
    li r5, 0x144
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x144
    bl fn_80239EE8
@L_80245834:
    lfs f1, -20592(r2)
    mr r3, r27
    mr r4, r28
    li r5, -0x1
    bl fn_802373B0
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024589C
    mr r3, r30
    mr r4, r27
    li r5, 0x145
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x145
    bl fn_80239EE8
@L_8024589C:
    clrlwi r0, r31, 24
    cmplwi r0, 0x2
    beq @L_802458B8
    cmplwi r0, 0x4
    beq @L_802458B8
    cmplwi r0, 0x3
    bne @L_80245900
@L_802458B8:
    mr r3, r30
    mr r4, r27
    li r5, 0x146
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x146
    bl fn_80239EE8
@L_80245900:
    mr r3, r30
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
