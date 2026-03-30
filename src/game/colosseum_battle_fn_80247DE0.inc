    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    mr r27, r6
    mr r4, r29
    li r3, 0x0
    li r5, 0x116
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x116
    bl fn_80239EE8
    mr r3, r28
    mr r4, r29
    li r5, 0x117
    bl fn_80239984
    mr r28, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x117
    bl fn_80239EE8
    mr r4, r29
    mr r8, r30
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80247EF8
    mr r3, r28
    mr r4, r29
    li r5, 0x118
    bl fn_80239984
    mr r28, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x118
    bl fn_80239EE8
@L_80247EF8:
    mr r3, r29
    mr r4, r27
    bl fn_80237310
    clrlwi. r0, r3, 24
    beq @L_80247F44
    mr r3, r29
    mr r4, r27
    li r5, 0x11
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80247F44
    mr r3, r29
    mr r4, r27
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80247F88
@L_80247F44:
    mr r3, r28
    mr r4, r29
    li r5, 0x119
    bl fn_80239984
    mr r28, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x119
    bl fn_80239EE8
@L_80247F88:
    mr r3, r28
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
