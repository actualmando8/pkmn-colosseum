    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    mr r31, r6
    mr r4, r28
    li r3, 0x0
    li r5, 0xfb
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r27, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xfb
    bl fn_80239EE8
    mr r3, r27
    mr r4, r28
    li r5, 0xfc
    bl fn_80239984
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xfc
    bl fn_80239EE8
    mr r4, r28
    mr r8, r29
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80248C34
    mr r3, r27
    mr r4, r28
    li r5, 0xfd
    bl fn_80239984
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xfd
    bl fn_80239EE8
@L_80248C34:
    mr r3, r28
    mr r4, r31
    li r5, 0x7
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80248C94
    mr r3, r27
    mr r4, r28
    li r5, 0xfe
    bl fn_80239984
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xfe
    bl fn_80239EE8
@L_80248C94:
    mr r3, r28
    mr r4, r31
    bl fn_80237310
    clrlwi. r0, r3, 24
    beq @L_80248CE0
    mr r3, r28
    mr r4, r31
    li r5, 0x29
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80248CE0
    mr r3, r28
    mr r4, r31
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80248D24
@L_80248CE0:
    mr r3, r27
    mr r4, r28
    li r5, 0xff
    bl fn_80239984
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xff
    bl fn_80239EE8
@L_80248D24:
    mr r3, r27
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
