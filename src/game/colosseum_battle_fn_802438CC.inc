    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    mr r27, r4
    mr r28, r5
    mr r29, r6
    li r30, 0x0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r4, r3, 16
    mr r3, r27
    bl fn_801F0134
    mr r31, r3
    mr r3, r26
    mr r4, r29
    li r5, 0x1d
    bl fn_80236BFC
    clrlwi. r0, r3, 24
    bne @L_80243974
    mr r4, r26
    li r3, 0x0
    li r5, 0x179
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x179
    bl fn_80239EE8
@L_80243974:
    mr r3, r26
    mr r4, r29
    li r5, 0x1d
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802439F4
    mr r3, r29
    li r4, 0x1d
    bl fn_80201D84
    clrlwi r3, r3, 16
    clrlwi r0, r31, 16
    cmplw r0, r3
    bne @L_802439F4
    mr r3, r30
    mr r4, r26
    li r5, 0x17a
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x17a
    bl fn_80239EE8
@L_802439F4:
    mr r3, r30
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
