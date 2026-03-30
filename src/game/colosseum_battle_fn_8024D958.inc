    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    mr r31, r6
    bl fn_802376EC
    mr r26, r3
    mr r3, r28
    mr r4, r29
    mr r5, r30
    mr r6, r31
    li r7, 0x1
    bl fn_8023C370
    mr r27, r3
    li r3, 0x0
    li r4, 0x63
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    srawi r4, r27, 1
    clrlwi r0, r26, 16
    addze r4, r4
    mulli r4, r4, 0x64
    divw r0, r4, r0
    divw r27, r0, r3
    li r3, 0x0
    mr r4, r27
    bl fn_802399FC
    mr r0, r3
    mr r3, r29
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    stw r27, 0x8(r1)
    mr r5, r3
    mr r4, r28
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x63
    bl fn_80239CCC
    mr r4, r28
    mr r8, r29
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024DA80
    mr r3, r26
    mr r4, r28
    li r5, 0x64
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x64
    bl fn_80239EE8
@L_8024DA80:
    mr r3, r28
    mr r4, r31
    li r5, 0x40
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024DAE4
    mr r3, r26
    mr r4, r28
    li r5, 0x65
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x65
    bl fn_80239EE8
@L_8024DAE4:
    mr r3, r26
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
