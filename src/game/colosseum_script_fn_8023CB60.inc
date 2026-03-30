    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    mr r28, r6
    bl fn_802376EC
    mr r27, r3
    mr r3, r29
    mr r4, r30
    mr r5, r31
    mr r6, r28
    li r7, 0x1
    bl fn_8023C370
    mr r28, r3
    li r3, 0x0
    li r4, 0x223
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    srawi r4, r28, 1
    rlwinm r0, r27, 0, 16, 31
    addze r4, r4
    mulli r4, r4, 0x64
    divw r0, r4, r0
    divw r28, r0, r3
    li r3, 0x0
    mr r4, r28
    bl fn_802399FC
    mr r0, r3
    mr r3, r30
    mr r27, r0
    bl fn_80205B8C
    lis r6, 0x1
    stw r28, 0x8(r1)
    mr r5, r3
    mr r4, r29
    subi r3, r6, 0x139c
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x223
    bl fn_80239CCC
    mr r4, r29
    mr r8, r30
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023CC88
    mr r3, r27
    mr r4, r29
    li r5, 0x224
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r27, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x224
    bl fn_80239EE8
@L_8023CC88:
    mr r3, r27
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
