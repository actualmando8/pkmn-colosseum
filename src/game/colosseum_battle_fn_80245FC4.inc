    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stmw r24, 0x70(r1)
    mr r31, r3
    mr r29, r4
    mr r30, r5
    mr r28, r6
    li r24, 0x0
    li r3, 0x0
    li r4, 0x0
    li r5, 0xf
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r3, r3, 16
    bl fn_80136468
    clrlwi r0, r3, 24
    cmplwi r0, 0x1b
    bgt @L_80247030
    lis r3, jumptable_8039A5D8@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_8039A5D8@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r25, r3, 24
    li r3, 0x0
    li r4, 0x113
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r25, r25, r3
    li r3, 0x0
    mr r4, r25
    bl fn_802399FC
    mr r26, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r25, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x113
    bl fn_80239CCC
    mr r4, r31
    mr r8, r29
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80246100
    mr r3, r26
    mr r4, r31
    li r5, 0x114
    bl fn_80239984
    mr r26, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x114
    bl fn_80239EE8
@L_80246100:
    mr r3, r31
    mr r4, r28
    bl fn_80237310
    clrlwi. r0, r3, 24
    beq @L_8024614C
    mr r3, r31
    mr r4, r28
    li r5, 0x11
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8024614C
    mr r3, r31
    mr r4, r28
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802461B8
@L_8024614C:
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0x115
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    mr r3, r26
    mr r4, r24
    bl fn_802399FC
    mr r26, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x115
    bl fn_80239CCC
@L_802461B8:
    mr r24, r26
    b @L_80247030
    mr r4, r31
    addi r5, r1, 0x10
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1A6C
    mr r26, r3
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0x10d
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    li r3, 0x0
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x10d
    bl fn_80239CCC
    addi r25, r1, 0x10
    clrlwi r26, r26, 16
    li r24, 0x0
    b @L_80246308
@L_80246258:
    mr r3, r28
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    clrlslwi r0, r24, 16, 2
    lwzx r4, r25, r0
    cmplw r3, r4
    beq @L_80246304
    mr r3, r31
    li r5, 0x8
    bl fn_802384B4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80246304
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0x10e
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    mr r3, r27
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x10e
    bl fn_80239CCC
    b @L_80246314
@L_80246304:
    addi r24, r24, 0x1
@L_80246308:
    clrlwi r0, r24, 16
    cmplw r0, r26
    blt @L_80246258
@L_80246314:
    mr r3, r31
    mr r4, r28
    bl fn_80237310
    clrlwi. r0, r3, 24
    beq @L_80246360
    mr r3, r31
    mr r4, r28
    li r5, 0xf
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80246360
    mr r3, r31
    mr r4, r28
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802463CC
@L_80246360:
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0x10f
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    mr r3, r27
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x10f
    bl fn_80239CCC
@L_802463CC:
    mr r24, r27
    b @L_80247030
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0xdb
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    li r3, 0x0
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xdb
    bl fn_80239CCC
    mr r4, r31
    mr r8, r29
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802464AC
    mr r3, r27
    mr r4, r31
    li r5, 0xdc
    bl fn_80239984
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xdc
    bl fn_80239EE8
@L_802464AC:
    mr r3, r31
    mr r4, r28
    bl fn_802358AC
    clrlwi. r0, r3, 24
    bne @L_8024652C
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0xdd
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    mr r3, r27
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xdd
    bl fn_80239CCC
@L_8024652C:
    mr r3, r31
    mr r4, r28
    li r5, 0x1d
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8024659C
    mr r3, r31
    mr r4, r28
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8024659C
    mr r3, r31
    mr r4, r28
    li r5, 0x49
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8024659C
    mr r3, r31
    mr r4, r28
    li r5, 0x33
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80246608
@L_8024659C:
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0xde
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    mr r3, r27
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xde
    bl fn_80239CCC
@L_80246608:
    mr r24, r27
    b @L_80247030
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0xc3
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    li r3, 0x0
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xc3
    bl fn_80239CCC
    mr r4, r31
    mr r8, r29
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802466E8
    mr r3, r27
    mr r4, r31
    li r5, 0xc4
    bl fn_80239984
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xc4
    bl fn_80239EE8
@L_802466E8:
    mr r3, r31
    mr r4, r28
    bl fn_80235A3C
    clrlwi. r0, r3, 24
    bne @L_80246768
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0xc5
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    mr r3, r27
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xc5
    bl fn_80239CCC
@L_80246768:
    mr r3, r31
    mr r4, r28
    li r5, 0x1d
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802467BC
    mr r3, r31
    mr r4, r28
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802467BC
    mr r3, r31
    mr r4, r28
    li r5, 0x49
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80246828
@L_802467BC:
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0xc6
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    mr r3, r27
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xc6
    bl fn_80239CCC
@L_80246828:
    mr r24, r27
    b @L_80247030
    mr r4, r31
    li r3, 0x0
    li r5, 0xbf
    bl fn_80239984
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xbf
    bl fn_80239EE8
    mr r4, r31
    mr r8, r29
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802468E0
    mr r3, r27
    mr r4, r31
    li r5, 0xc0
    bl fn_80239984
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xc0
    bl fn_80239EE8
@L_802468E0:
    mr r3, r31
    mr r4, r28
    bl fn_80235AA0
    clrlwi. r0, r3, 24
    bne @L_80246938
    mr r3, r27
    mr r4, r31
    li r5, 0xc1
    bl fn_80239984
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xc1
    bl fn_80239EE8
@L_80246938:
    mr r3, r31
    mr r4, r28
    li r5, 0x1d
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802469A8
    mr r3, r31
    mr r4, r28
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802469A8
    mr r3, r31
    mr r4, r28
    li r5, 0x49
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_802469A8
    mr r3, r31
    mr r4, r28
    li r5, 0x34
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802469EC
@L_802469A8:
    mr r3, r27
    mr r4, r31
    li r5, 0xc2
    bl fn_80239984
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xc2
    bl fn_80239EE8
@L_802469EC:
    mr r24, r27
    b @L_80247030
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0xc7
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    li r3, 0x0
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xc7
    bl fn_80239CCC
    mr r4, r31
    mr r8, r29
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80246ACC
    mr r3, r27
    mr r4, r31
    li r5, 0xc8
    bl fn_80239984
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xc8
    bl fn_80239EE8
@L_80246ACC:
    mr r3, r31
    mr r4, r28
    li r5, 0x5
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80246B2C
    mr r3, r27
    mr r4, r31
    li r5, 0xc9
    bl fn_80239984
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xc9
    bl fn_80239EE8
@L_80246B2C:
    mr r3, r31
    mr r4, r28
    bl fn_80235910
    clrlwi. r0, r3, 24
    bne @L_80246BAC
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0xca
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    mr r3, r27
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xca
    bl fn_80239CCC
@L_80246BAC:
    mr r3, r31
    mr r4, r28
    li r5, 0x1d
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80246C00
    mr r3, r31
    mr r4, r28
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80246C00
    mr r3, r31
    mr r4, r28
    li r5, 0x49
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80246C6C
@L_80246C00:
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0xcb
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    mr r3, r27
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xcb
    bl fn_80239CCC
@L_80246C6C:
    mr r24, r27
    b @L_80247030
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0xe3
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    li r3, 0x0
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xe3
    bl fn_80239CCC
    mr r4, r31
    mr r8, r29
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80246D4C
    mr r3, r27
    mr r4, r31
    li r5, 0xe4
    bl fn_80239984
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xe4
    bl fn_80239EE8
@L_80246D4C:
    mr r3, r31
    mr r4, r28
    li r5, 0x9
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80246DD4
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0xe5
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    mr r3, r27
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xe5
    bl fn_80239CCC
@L_80246DD4:
    mr r3, r31
    mr r4, r28
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80246E0C
    mr r3, r31
    mr r4, r28
    li r5, 0x14
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80246E78
@L_80246E0C:
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0xe6
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    mr r3, r27
    mr r4, r24
    bl fn_802399FC
    mr r27, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xe6
    bl fn_80239CCC
@L_80246E78:
    mr r24, r27
    b @L_80247030
    mr r3, r31
    mr r4, r29
    mr r5, r30
    mr r6, r28
    bl fn_8024AFC4
    mr r24, r3
    b @L_80247030
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0x104
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    li r3, 0x0
    mr r4, r24
    bl fn_802399FC
    mr r25, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x104
    bl fn_80239CCC
    mr r4, r31
    mr r8, r29
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80246F74
    mr r3, r25
    mr r4, r31
    li r5, 0x105
    bl fn_80239984
    mr r25, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x105
    bl fn_80239EE8
@L_80246F74:
    mr r3, r31
    mr r4, r28
    bl fn_80237310
    clrlwi. r0, r3, 24
    beq @L_80246FC0
    mr r3, r31
    mr r4, r28
    li r5, 0x7
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80246FC0
    mr r3, r31
    mr r4, r28
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024702C
@L_80246FC0:
    mr r3, r31
    mr r4, r30
    bl fn_80239564
    clrlwi r24, r3, 24
    li r3, 0x0
    li r4, 0x106
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r24, r24, r3
    mr r3, r25
    mr r4, r24
    bl fn_802399FC
    mr r25, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r24, 0x8(r1)
    mr r5, r3
    mr r4, r31
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x106
    bl fn_80239CCC
@L_8024702C:
    mr r24, r25
@L_80247030:
    mr r3, r24
    lmw r24, 0x70(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
