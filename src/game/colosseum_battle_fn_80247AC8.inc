    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r31, r3
    mr r27, r4
    mr r28, r5
    mr r29, r6
    mr r4, r31
    li r3, 0x0
    li r5, 0x11d
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x11d
    bl fn_80239EE8
    mr r3, r31
    mr r4, r29
    bl fn_80237310
    clrlwi. r0, r3, 24
    bne @L_80247B8C
    mr r3, r30
    mr r4, r31
    li r5, 0x11e
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x11e
    bl fn_80239EE8
@L_80247B8C:
    mr r3, r31
    mr r4, r29
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80247BFC
    mr r3, r31
    mr r4, r29
    li r5, 0x7
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80247BFC
    mr r3, r31
    mr r4, r29
    li r5, 0x29
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80247BFC
    mr r3, r31
    mr r4, r29
    li r5, 0x28
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80247C44
@L_80247BFC:
    mr r3, r30
    mr r4, r31
    li r5, 0x11f
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x11f
    bl fn_80239EE8
@L_80247C44:
    mr r3, r30
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
