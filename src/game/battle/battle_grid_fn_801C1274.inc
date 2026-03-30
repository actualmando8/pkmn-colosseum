    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    b @L_801C17F4
    @L_801C129C:
    rlwinm. r0, r28, 0, 29, 29
    beq @L_801C13E4
    lwz r0, 0x10(r27)
    cmplwi r0, 0x0
    beq @L_801C13E4
    cmplwi r30, 0xb
    lwz r3, 0x10(r27)
    bgt @L_801C13E4
    lis r4, jumptable_8036DA94@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036DA94@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C13E4
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C13E4
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C13E4
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C13E4
    mr r12, r29
    mr r4, r27
    mtctr r12
    bctrl
    b @L_801C13E4
    mr r12, r29
    mr r4, r27
    li r5, 0x3
    mtctr r12
    bctrl
    b @L_801C13E4
    mr r12, r29
    mr r4, r27
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C13E4
    mr r12, r29
    mr r4, r27
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C13E4
    mr r12, r29
    mr r4, r27
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C13E4
    mr r12, r29
    mr r4, r27
    lfs f1, 0x0(r31)
    li r5, 0x3
    mtctr r12
    bctrl
    b @L_801C13E4
    mr r12, r29
    mr r4, r27
    lwz r6, 0x0(r31)
    li r5, 0x3
    mtctr r12
    bctrl
    b @L_801C13E4
    mr r12, r29
    mr r4, r27
    lwz r6, 0x0(r31)
    li r5, 0x3
    mtctr r12
    bctrl
    @L_801C13E4:
    lwz r26, 0x8(r27)
    cmplwi r26, 0x0
    beq @L_801C1694
    rlwinm. r0, r28, 0, 24, 24
    beq @L_801C1538
    lwz r0, 0x14(r26)
    cmplwi r0, 0x0
    beq @L_801C1538
    cmplwi r30, 0xb
    lwz r3, 0x14(r26)
    bgt @L_801C1538
    lis r4, jumptable_8036DA64@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036DA64@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C1538
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1538
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1538
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1538
    mr r12, r29
    mr r4, r26
    mtctr r12
    bctrl
    b @L_801C1538
    mr r12, r29
    mr r4, r26
    li r5, 0x8
    mtctr r12
    bctrl
    b @L_801C1538
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1538
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1538
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1538
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    li r5, 0x8
    mtctr r12
    bctrl
    b @L_801C1538
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0x8
    mtctr r12
    bctrl
    b @L_801C1538
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0x8
    mtctr r12
    bctrl
    @L_801C1538:
    lwz r26, 0x8(r26)
    b @L_801C168C
    @L_801C1540:
    rlwinm. r0, r28, 0, 21, 21
    beq @L_801C1688
    lwz r0, 0x64(r26)
    cmplwi r0, 0x0
    beq @L_801C1688
    cmplwi r30, 0xb
    lwz r3, 0x64(r26)
    bgt @L_801C1688
    lis r4, jumptable_8036DA34@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036DA34@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C1688
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1688
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1688
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1688
    mr r12, r29
    mr r4, r26
    mtctr r12
    bctrl
    b @L_801C1688
    mr r12, r29
    mr r4, r26
    li r5, 0xb
    mtctr r12
    bctrl
    b @L_801C1688
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1688
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1688
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1688
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    li r5, 0xb
    mtctr r12
    bctrl
    b @L_801C1688
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xb
    mtctr r12
    bctrl
    b @L_801C1688
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xb
    mtctr r12
    bctrl
    @L_801C1688:
    lwz r26, 0x8(r26)
    @L_801C168C:
    cmplwi r26, 0x0
    bne @L_801C1540
    @L_801C1694:
    lwz r26, 0xc(r27)
    b @L_801C17E8
    @L_801C169C:
    rlwinm. r0, r28, 0, 23, 23
    beq @L_801C17E4
    lwz r0, 0x18(r26)
    cmplwi r0, 0x0
    beq @L_801C17E4
    cmplwi r30, 0xb
    lwz r3, 0x18(r26)
    bgt @L_801C17E4
    lis r4, jumptable_8036DA04@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036DA04@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C17E4
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C17E4
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C17E4
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C17E4
    mr r12, r29
    mr r4, r26
    mtctr r12
    bctrl
    b @L_801C17E4
    mr r12, r29
    mr r4, r26
    li r5, 0x9
    mtctr r12
    bctrl
    b @L_801C17E4
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C17E4
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C17E4
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C17E4
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    li r5, 0x9
    mtctr r12
    bctrl
    b @L_801C17E4
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0x9
    mtctr r12
    bctrl
    b @L_801C17E4
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0x9
    mtctr r12
    bctrl
    @L_801C17E4:
    lwz r26, 0x4(r26)
    @L_801C17E8:
    cmplwi r26, 0x0
    bne @L_801C169C
    lwz r27, 0x4(r27)
    @L_801C17F4:
    cmplwi r27, 0x0
    bne @L_801C129C
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
