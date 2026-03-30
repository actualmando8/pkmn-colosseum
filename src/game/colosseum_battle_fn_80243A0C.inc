    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r29, r5
    mr r27, r3
    mr r28, r4
    li r30, 0x0
    li r5, 0x7
    bl fn_80237DBC
    mr r31, r3
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80243AB0
    lfs f1, -20592(r2)
    mr r3, r27
    mr r4, r28
    li r5, 0x1
    bl fn_802373B0
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80243AB0
    mr r4, r27
    li r3, 0x0
    li r5, 0x174
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
    li r10, 0x174
    bl fn_80239EE8
    b @L_80243B24
@L_80243AB0:
    clrlwi r0, r31, 24
    cmplwi r0, 0x1
    bne @L_80243B24
    lfs f1, -20576(r2)
    mr r3, r27
    mr r4, r28
    li r5, -0x1
    bl fn_802373B0
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80243B24
    mr r4, r27
    li r3, 0x0
    li r5, 0x175
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
    li r10, 0x175
    bl fn_80239EE8
@L_80243B24:
    clrlwi. r0, r31, 24
    bne @L_80243B88
    mr r3, r27
    mr r4, r28
    bl fn_80235714
    clrlwi. r0, r3, 24
    bne @L_80243B88
    mr r3, r30
    mr r4, r27
    li r5, 0x176
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
    li r10, 0x176
    bl fn_80239EE8
@L_80243B88:
    clrlwi r0, r31, 24
    cmplwi r0, 0x1
    bne @L_80243BDC
    mr r3, r30
    mr r4, r27
    li r5, 0x177
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
    li r10, 0x177
    bl fn_80239EE8
@L_80243BDC:
    clrlwi. r0, r31, 24
    bne @L_80243C44
    mr r3, r27
    mr r4, r28
    bl fn_80235714
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80243C44
    mr r3, r30
    mr r4, r27
    li r5, 0x178
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
    li r10, 0x178
    bl fn_80239EE8
@L_80243C44:
    mr r3, r30
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
