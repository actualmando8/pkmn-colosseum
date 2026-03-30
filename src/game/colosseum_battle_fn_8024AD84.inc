    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r4
    mr r27, r3
    mr r29, r5
    mr r30, r6
    mr r3, r28
    li r31, 0x0
    li r4, 0x0
    li r5, 0xed
    li r6, 0x0
    bl fn_8012640C
    clrlwi. r0, r3, 16
    beq @L_8024AE0C
    mr r4, r27
    li r3, 0x0
    li r5, 0xbd
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
    li r10, 0xbd
    bl fn_80239EE8
@L_8024AE0C:
    mr r3, r27
    mr r4, r30
    li r5, 0x27
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024AE74
    mr r3, r31
    mr r4, r27
    li r5, 0xbe
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
    li r10, 0xbe
    bl fn_80239EE8
    b @L_8024AED8
@L_8024AE74:
    mr r3, r27
    mr r4, r30
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024AED8
    mr r3, r31
    mr r4, r27
    li r5, 0xbe
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
    li r10, 0xbe
    bl fn_80239EE8
@L_8024AED8:
    mr r3, r31
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
