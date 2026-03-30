    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r14, 0x18(r1)
    mr r15, r3
    lis r4, 0xce6
    addi r4, r4, 0x1602
    bl fn_800F9318
    stw r3, -24976(r13)
    lis r4, 0xce6
    mr r3, r15
    addi r4, r4, 0x1002
    bl fn_800F9318
    stw r3, -24972(r13)
    li r3, 0x280
    li r4, 0x1e0
    bl fn_800E8FA0
    lis r3, 0xce6
    li r4, 0x0
    addi r3, r3, 0x1000
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    li r16, 0x1
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_8008D9BC
    lfs f1, -29904(r2)
    bl fn_800C46B0
    mr r16, r3
    cmplwi r16, 0x1
    bge @L_8008D9BC
    li r16, 0x1
@L_8008D9BC:
    li r14, 0x0
    b @L_8008D9D0
@L_8008D9C4:
    bl fn_800F0308
    bl fn_800D3088
    add r14, r14, r3
@L_8008D9D0:
    cmplw r14, r16
    blt @L_8008D9C4
    lis r3, 0xce6
    addi r3, r3, 0x1000
    bl fn_801CB7C4
    lis r4, 0xce6
    mr r3, r15
    addi r4, r4, 0x1000
    bl fn_800F9318
    mr r14, r3
    li r4, 0x1
    lwz r3, 0x144(r14)
    bl fn_80118874
    li r0, 0x0
    lis r4, 0xce6
    stw r0, 0x144(r14)
    mr r3, r15
    lfs f0, -29900(r2)
    addi r4, r4, 0x1004
    stfs f0, 0x8(r1)
    bl fn_800F9318
    li r4, 0x0
    mr r14, r3
    bl fn_800ECCA8
    mr r3, r14
    addi r4, r1, 0x8
    li r5, 0x0
    bl fn_800EC4D0
    lfs f1, 0x8(r1)
    mr r3, r14
    lfs f0, -29896(r2)
    li r4, 0x0
    fsubs f0, f1, f0
    stfs f0, 0x8(r1)
    bl fn_800ECCA8
    lfs f1, 0x8(r1)
    mr r3, r14
    bl fn_800ECA78
    mr r3, r14
    li r4, 0x0
    bl fn_800ECB74
    mr r3, r14
    bl fn_800EC990
    lis r4, 0x111b
    mr r3, r15
    addi r4, r4, 0x1400
    bl fn_800F9318
    lis r4, 0xce6
    mr r14, r3
    mr r3, r15
    addi r4, r4, 0x1000
    bl fn_800F9318
    mr r4, r14
    bl fn_800E3C08
    lis r4, 0xce6
    mr r3, r15
    addi r4, r4, 0x1000
    bl fn_800F9318
    li r4, 0x4
    bl fn_800E3C00
    lis r3, 0xce6
    li r4, 0x3
    addi r3, r3, 0x1000
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    li r16, 0x32
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_8008DB00
    lfs f1, -29888(r2)
    bl fn_800C46B0
    mr r16, r3
    cmplwi r16, 0x1
    bge @L_8008DB00
    li r16, 0x1
@L_8008DB00:
    li r14, 0x0
    b @L_8008DB14
@L_8008DB08:
    bl fn_800F0308
    bl fn_800D3088
    add r14, r14, r3
@L_8008DB14:
    cmplw r14, r16
    blt @L_8008DB08
    lis r4, 0x111f
    mr r3, r15
    addi r4, r4, 0x1400
    bl fn_800F9318
    lis r4, 0xce6
    mr r14, r3
    mr r3, r15
    addi r4, r4, 0x1000
    bl fn_800F9318
    mr r4, r14
    bl fn_800E3C08
    lis r4, 0xce6
    mr r3, r15
    addi r4, r4, 0x1000
    bl fn_800F9318
    li r4, 0x4
    bl fn_800E3C00
    lis r3, 0x6bc
    addi r3, r3, 0x400
    bl fn_801CBA0C
    lis r4, 0x6be
    mr r31, r3
    addi r3, r4, 0x400
    bl fn_801CBA0C
    lis r4, 0xd29
    mr r30, r3
    addi r3, r4, 0x400
    bl fn_801CBA0C
    lis r4, 0x6be
    mr r29, r3
    addi r3, r4, 0x400
    bl fn_801CBA0C
    lis r4, 0xd24
    mr r28, r3
    addi r3, r4, 0x400
    bl fn_801CBA0C
    lis r4, 0xd29
    mr r27, r3
    addi r3, r4, 0x400
    bl fn_801CBA0C
    lis r4, 0xd24
    mr r26, r3
    addi r3, r4, 0x400
    bl fn_801CBA0C
    lis r4, 0x6be
    mr r25, r3
    addi r3, r4, 0x400
    bl fn_801CBA0C
    lis r4, 0xd24
    mr r24, r3
    addi r3, r4, 0x400
    bl fn_801CBA0C
    mr r0, r3
    mr r3, r15
    mr r23, r0
    mr r4, r31
    bl fn_800F9318
    li r4, 0x2
    mr r14, r3
    bl fn_800E9108
    lwz r4, -24976(r13)
    mr r3, r14
    bl fn_800E8FE8
    mr r3, r14
    li r4, 0x1
    addi r5, r13, -24972
    bl fn_800E900C
    mr r3, r15
    mr r4, r30
    bl fn_800F9318
    li r4, 0x2
    mr r14, r3
    bl fn_800E9108
    lwz r4, -24976(r13)
    mr r3, r14
    bl fn_800E8FE8
    mr r3, r14
    li r4, 0x1
    addi r5, r13, -24972
    bl fn_800E900C
    mr r3, r15
    mr r4, r29
    bl fn_800F9318
    li r4, 0x2
    mr r14, r3
    bl fn_800E9108
    lwz r4, -24976(r13)
    mr r3, r14
    bl fn_800E8FE8
    mr r3, r14
    li r4, 0x1
    addi r5, r13, -24972
    bl fn_800E900C
    mr r3, r15
    mr r4, r28
    bl fn_800F9318
    li r4, 0x2
    mr r14, r3
    bl fn_800E9108
    lwz r4, -24976(r13)
    mr r3, r14
    bl fn_800E8FE8
    mr r3, r14
    li r4, 0x1
    addi r5, r13, -24972
    bl fn_800E900C
    mr r3, r15
    mr r4, r27
    bl fn_800F9318
    li r4, 0x2
    mr r14, r3
    bl fn_800E9108
    lwz r4, -24976(r13)
    mr r3, r14
    bl fn_800E8FE8
    mr r3, r14
    li r4, 0x1
    addi r5, r13, -24972
    bl fn_800E900C
    mr r3, r15
    mr r4, r26
    bl fn_800F9318
    li r4, 0x2
    mr r14, r3
    bl fn_800E9108
    lwz r4, -24976(r13)
    mr r3, r14
    bl fn_800E8FE8
    mr r3, r14
    li r4, 0x1
    addi r5, r13, -24972
    bl fn_800E900C
    mr r3, r15
    mr r4, r25
    bl fn_800F9318
    li r4, 0x2
    mr r14, r3
    bl fn_800E9108
    lwz r4, -24976(r13)
    mr r3, r14
    bl fn_800E8FE8
    mr r3, r14
    li r4, 0x1
    addi r5, r13, -24972
    bl fn_800E900C
    mr r3, r15
    mr r4, r24
    bl fn_800F9318
    li r4, 0x2
    mr r14, r3
    bl fn_800E9108
    lwz r4, -24976(r13)
    mr r3, r14
    bl fn_800E8FE8
    mr r3, r14
    li r4, 0x1
    addi r5, r13, -24972
    bl fn_800E900C
    mr r3, r15
    mr r4, r23
    bl fn_800F9318
    li r4, 0x2
    mr r14, r3
    bl fn_800E9108
    lwz r4, -24976(r13)
    mr r3, r14
    bl fn_800E8FE8
    mr r3, r14
    li r4, 0x1
    addi r5, r13, -24972
    bl fn_800E900C
    lis r4, 0xcfe
    mr r3, r15
    addi r4, r4, 0x1800
    li r5, 0x0
    li r6, 0x0
    bl fn_80176E0C
    li r16, 0x1
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_8008DE08
    lfs f1, -29904(r2)
    bl fn_800C46B0
    mr r16, r3
    cmplwi r16, 0x1
    bge @L_8008DE08
    li r16, 0x1
@L_8008DE08:
    li r14, 0x0
    b @L_8008DE1C
@L_8008DE10:
    bl fn_800F0308
    bl fn_800D3088
    add r14, r14, r3
@L_8008DE1C:
    cmplw r14, r16
    blt @L_8008DE10
    lis r3, 0xd08
    addi r3, r3, 0x1000
    bl fn_801CBA0C
    lis r4, 0xd08
    stw r3, 0xc(r1)
    addi r3, r4, 0x1008
    bl fn_801CBA0C
    lis r4, 0xd08
    mr r14, r3
    addi r3, r4, 0x1001
    bl fn_801CBA0C
    lis r4, 0xd08
    mr r22, r3
    addi r3, r4, 0x1002
    bl fn_801CBA0C
    lis r4, 0xd08
    mr r21, r3
    addi r3, r4, 0x1003
    bl fn_801CBA0C
    lis r4, 0xd08
    mr r20, r3
    addi r3, r4, 0x1004
    bl fn_801CBA0C
    lis r4, 0xd08
    mr r19, r3
    addi r3, r4, 0x1005
    bl fn_801CBA0C
    lis r4, 0xd08
    mr r18, r3
    addi r3, r4, 0x1006
    bl fn_801CBA0C
    lis r4, 0xd08
    mr r17, r3
    addi r3, r4, 0x1007
    bl fn_801CBA0C
    mr r0, r3
    lwz r6, 0xc(r1)
    mr r3, r15
    mr r4, r31
    mr r16, r0
    mr r5, r15
    li r7, 0x0
    bl fn_801845E4
    mr r3, r15
    mr r4, r30
    mr r5, r15
    mr r6, r14
    li r7, 0x0
    bl fn_801845E4
    mr r3, r15
    mr r4, r29
    mr r5, r15
    mr r6, r22
    li r7, 0x0
    bl fn_801845E4
    mr r3, r15
    mr r4, r28
    mr r5, r15
    mr r6, r21
    li r7, 0x0
    bl fn_801845E4
    mr r3, r15
    mr r4, r27
    mr r5, r15
    mr r6, r20
    li r7, 0x0
    bl fn_801845E4
    mr r3, r15
    mr r4, r26
    mr r5, r15
    mr r6, r19
    li r7, 0x0
    bl fn_801845E4
    mr r3, r15
    mr r4, r25
    mr r5, r15
    mr r6, r18
    li r7, 0x0
    bl fn_801845E4
    mr r3, r15
    mr r4, r24
    mr r5, r15
    mr r6, r17
    li r7, 0x0
    bl fn_801845E4
    mr r3, r15
    mr r4, r23
    mr r5, r15
    mr r6, r16
    li r7, 0x0
    bl fn_801845E4
    lis r3, 0xce6
    li r4, 0x0
    addi r3, r3, 0x1000
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r31
    li r4, 0x6
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r30
    li r4, 0x9
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r29
    li r4, 0xb
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r28
    li r4, 0x9
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r27
    li r4, 0xb
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r26
    li r4, 0x9
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r25
    li r4, 0xb
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r24
    li r4, 0x9
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r24
    li r4, 0xb
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r23
    li r4, 0xb
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r31
    li r4, 0x1
    bl fn_801CB708
    mr r3, r30
    li r4, 0x1
    bl fn_801CB708
    mr r3, r29
    li r4, 0x1
    bl fn_801CB708
    mr r3, r28
    li r4, 0x1
    bl fn_801CB708
    mr r3, r27
    li r4, 0x1
    bl fn_801CB708
    mr r3, r26
    li r4, 0x1
    bl fn_801CB708
    mr r3, r25
    li r4, 0x1
    bl fn_801CB708
    mr r3, r24
    li r4, 0x1
    bl fn_801CB708
    mr r3, r24
    li r4, 0x1
    bl fn_801CB708
    mr r3, r23
    li r4, 0x1
    bl fn_801CB708
    mr r3, r31
    li r4, 0x7
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r30
    li r4, 0xa
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r29
    li r4, 0xc
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r28
    li r4, 0xa
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r26
    li r4, 0xa
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r27
    li r4, 0xc
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r25
    li r4, 0xc
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r24
    li r4, 0xa
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r24
    li r4, 0xc
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r23
    li r4, 0xc
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r31
    li r4, 0x1
    bl fn_801CB708
    mr r3, r30
    li r4, 0x1
    bl fn_801CB708
    mr r3, r29
    li r4, 0x1
    bl fn_801CB708
    mr r3, r28
    li r4, 0x1
    bl fn_801CB708
    mr r3, r27
    li r4, 0x1
    bl fn_801CB708
    mr r3, r26
    li r4, 0x1
    bl fn_801CB708
    mr r3, r25
    li r4, 0x1
    bl fn_801CB708
    mr r3, r24
    li r4, 0x1
    bl fn_801CB708
    mr r3, r24
    li r4, 0x1
    bl fn_801CB708
    mr r3, r23
    li r4, 0x1
    bl fn_801CB708
    mr r3, r31
    li r4, 0x6
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r30
    li r4, 0x9
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r29
    li r4, 0xb
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r28
    li r4, 0x9
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r27
    li r4, 0xb
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r26
    li r4, 0x9
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r25
    li r4, 0xb
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r24
    li r4, 0x9
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r24
    li r4, 0xb
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    mr r3, r23
    li r4, 0xb
    li r5, 0x0
    li r6, 0x0
    bl fn_801CB834
    li r3, 0x1
    bl fn_80176B48
    li r3, 0x89
    bl fn_800FF58C
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    lmw r14, 0x18(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
