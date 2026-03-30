    nofralloc
    stwu r1, -0x100(r1)
    mflr r0
    stw r0, 0x104(r1)
    stmw r24, 0xe0(r1)
    mr. r31, r3
    bne @L_801DC034
    li r3, 0x0
    b @L_801DC2FC
@L_801DC034:
    lwz r0, 0x3c(r31)
    cmplwi r0, 0x0
    bne @L_801DC048
    li r3, 0x0
    b @L_801DC2FC
@L_801DC048:
    addi r3, r1, 0x8
    bl fn_801DC46C
    mr r0, r3
    mr r3, r31
    mr r4, r0
    lwz r5, 0x4(r4)
    subi r27, r5, 0x1
    bl fn_801DC5F0
    cmpwi r27, 0x0
    mr r29, r3
    beq @L_801DC2F8
    mulli r3, r27, 0xb4
    bl fn_800E3534
    rlwinm. r0, r3, 0, 16, 31
    mr r25, r3
    beq @L_801DC2F8
    li r0, 0x0
    stw r0, 0x24(r31)
    bl fn_800E27B0
    stw r27, 0x4(r31)
    li r0, 0x0
    lis r4, -32713
    mr r24, r3
    stw r0, 0x0(r31)
    addi r30, r4, 10080
    li r26, 0x0
    sth r25, 0x28(r31)
    b @L_801DC284
@L_801DC0B8:
    mr r3, r24
    mr r4, r29
    bl fn_801DC46C
    lwz r0, 0x4(r24)
    mr r28, r3
    cmplwi r0, 0x6
    bgt @L_801DC248
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r30, r0
    mtctr r0
    bctr
    lwz r0, 0x0(r28)
    addi r29, r28, 0xc
    stw r0, 0x78(r24)
    lwz r0, 0x78(r24)
    cmpwi r0, 0x2
    beq @L_801DC180
    bge @L_801DC110
    cmpwi r0, 0x0
    beq @L_801DC11C
    bge @L_801DC174
    b @L_801DC270
@L_801DC110:
    cmpwi r0, 0x4
    bge @L_801DC270
    b @L_801DC18C
@L_801DC11C:
    bl fn_800D37CC
    lwz r0, 0x4(r28)
    lis r4, 0x4330
    xoris r3, r3, 0x8000
    stw r4, 0xc0(r1)
    xoris r0, r0, 0x8000
    lfd f3, -21240(r2)
    stw r3, 0xc4(r1)
    lfs f0, -21248(r2)
    lfd f1, 0xc0(r1)
    stw r0, 0xcc(r1)
    fsubs f2, f1, f3
    stw r4, 0xc8(r1)
    lfd f1, 0xc8(r1)
    fsubs f1, f1, f3
    fmuls f1, f1, f2
    fdivs f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0xd0(r1)
    lwz r0, 0xd4(r1)
    stw r0, 0x7c(r24)
    b @L_801DC270
@L_801DC174:
    lwz r0, 0x4(r28)
    stw r0, 0x7c(r24)
    b @L_801DC270
@L_801DC180:
    lfs f0, 0x4(r28)
    stfs f0, 0x7c(r24)
    b @L_801DC270
@L_801DC18C:
    lwz r3, 0x4(r28)
    li r0, 0x0
    stw r0, 0x78(r24)
    rlwinm r3, r3, 3, 0, 28
    add r29, r29, r3
    stw r0, 0x7c(r24)
    b @L_801DC270
    mr r3, r31
    mr r4, r24
    mr r5, r28
    bl fn_801DCBC8
    mr r29, r3
    b @L_801DC270
    mr r3, r31
    mr r4, r24
    mr r5, r28
    bl fn_801DCAA0
    mr r29, r3
    b @L_801DC270
    mr r3, r24
    mr r4, r28
    bl fn_801DC81C
    mr r29, r3
    b @L_801DC270
    lwz r0, 0x4(r28)
    cmpwi r0, 0x3
    beq @L_801DC218
    bge @L_801DC218
    cmpwi r0, 0x1
    bge @L_801DC208
    b @L_801DC218
@L_801DC208:
    li r0, 0x0
    li r3, -0x4
    stw r0, 0x7c(r24)
    b @L_801DC224
@L_801DC218:
    lwz r0, 0x8(r28)
    li r3, 0x0
    stw r0, 0x7c(r24)
@L_801DC224:
    lwz r0, 0x0(r28)
    add r29, r3, r28
    addi r29, r29, 0xc
    stw r0, 0x78(r24)
    b @L_801DC270
    lwz r0, 0x0(r28)
    addi r29, r28, 0x8
    stw r0, 0x78(r24)
    b @L_801DC270
@L_801DC248:
    lis r3, -32728
    addi r3, r3, -26244
    crxor 6, 6, 6
    bl fn_800DD970
    mr r3, r25
    bl fn_800E24B0
    mr r3, r25
    bl fn_800E209C
    li r3, 0x0
    b @L_801DC2FC
@L_801DC270:
    mr r3, r31
    mr r4, r24
    bl fn_801DC310
    addi r26, r26, 0x1
    addi r24, r24, 0xb4
@L_801DC284:
    cmpw r26, r27
    blt @L_801DC0B8
    lwz r3, 0x24(r31)
    li r0, 0x0
    b @L_801DC2AC
@L_801DC298:
    lwz r4, 0x70(r3)
    cmpw r4, r0
    bge @L_801DC2A8
    mr r0, r4
@L_801DC2A8:
    lwz r3, 0xa8(r3)
@L_801DC2AC:
    cmplwi r3, 0x0
    bne @L_801DC298
    cmpwi r0, 0x0
    beq @L_801DC2F0
    neg r4, r0
    stw r4, 0x10(r31)
    lwz r3, 0x24(r31)
    b @L_801DC2E8
@L_801DC2CC:
    lwz r0, 0x70(r3)
    add r0, r0, r4
    stw r0, 0x70(r3)
    lwz r0, 0x74(r3)
    add r0, r0, r4
    stw r0, 0x74(r3)
    lwz r3, 0xa8(r3)
@L_801DC2E8:
    cmplwi r3, 0x0
    bne @L_801DC2CC
@L_801DC2F0:
    li r3, 0x1
    b @L_801DC2FC
@L_801DC2F8:
    li r3, 0x1
@L_801DC2FC:
    lmw r24, 0xe0(r1)
    lwz r0, 0x104(r1)
    mtlr r0
    addi r1, r1, 0x100
    blr
