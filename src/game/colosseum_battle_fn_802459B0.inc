    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r29, r6
    mr r27, r4
    mr r28, r5
    mr r26, r3
    mr r5, r29
    li r31, 0x0
    li r4, 0x0
    li r6, 0x0
    bl fn_80235BE4
    mr r30, r3
    mr r4, r29
    li r3, 0x0
    bl fn_801F4354
    li r0, 0x0
    addi r4, r1, 0x8
    sth r0, 0x8(r1)
    bl fn_801F8A18
    cmplwi r3, 0x0
    bne @L_80245A10
    li r30, 0x1
@L_80245A10:
    mr r3, r26
    mr r4, r29
    li r5, 0x3
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80245A74
    mr r4, r26
    li r3, 0x0
    li r5, 0x139
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x139
    bl fn_80239EE8
@L_80245A74:
    mr r3, r26
    mr r4, r29
    li r5, 0x4
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80245AD8
    mr r3, r31
    mr r4, r26
    li r5, 0x13a
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x13a
    bl fn_80239EE8
@L_80245AD8:
    mr r3, r26
    mr r4, r29
    li r5, 0x6
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80245B3C
    mr r3, r31
    mr r4, r26
    li r5, 0x13b
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x13b
    bl fn_80239EE8
@L_80245B3C:
    mr r3, r26
    mr r4, r29
    li r5, 0x1c
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80245BA0
    mr r3, r31
    mr r4, r26
    li r5, 0x13c
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x13c
    bl fn_80239EE8
@L_80245BA0:
    mr r3, r26
    mr r4, r29
    li r5, 0x9
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80245C04
    mr r3, r31
    mr r4, r26
    li r5, 0x13d
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x13d
    bl fn_80239EE8
@L_80245C04:
    mr r3, r26
    mr r4, r29
    li r5, 0xa
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80245C68
    mr r3, r31
    mr r4, r26
    li r5, 0x13e
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x13e
    bl fn_80239EE8
@L_80245C68:
    mr r3, r26
    mr r4, r29
    li r5, 0x18
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80245CCC
    mr r3, r31
    mr r4, r26
    li r5, 0x13f
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x13f
    bl fn_80239EE8
@L_80245CCC:
    mr r3, r26
    mr r4, r29
    li r5, 0x1e
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80245D30
    mr r3, r31
    mr r4, r26
    li r5, 0x140
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x140
    bl fn_80239EE8
@L_80245D30:
    mr r3, r26
    mr r4, r29
    li r5, 0x26
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80245D94
    mr r3, r31
    mr r4, r26
    li r5, 0x141
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x141
    bl fn_80239EE8
@L_80245D94:
    clrlwi. r0, r30, 24
    beq @L_80245DE4
    mr r3, r31
    mr r4, r26
    li r5, 0x142
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x142
    bl fn_80239EE8
@L_80245DE4:
    mr r3, r31
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
