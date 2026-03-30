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
    b @L_801C1EE4
    @L_801C1838:
    rlwinm. r0, r28, 0, 25, 25
    beq @L_801C1980
    lwz r0, 0x48(r27)
    cmplwi r0, 0x0
    beq @L_801C1980
    cmplwi r30, 0xb
    lwz r3, 0x48(r27)
    bgt @L_801C1980
    lis r4, jumptable_8036DB84@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036DB84@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C1980
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1980
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1980
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1980
    mr r12, r29
    mr r4, r27
    mtctr r12
    bctrl
    b @L_801C1980
    mr r12, r29
    mr r4, r27
    li r5, 0x7
    mtctr r12
    bctrl
    b @L_801C1980
    mr r12, r29
    mr r4, r27
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1980
    mr r12, r29
    mr r4, r27
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1980
    mr r12, r29
    mr r4, r27
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1980
    mr r12, r29
    mr r4, r27
    lfs f1, 0x0(r31)
    li r5, 0x7
    mtctr r12
    bctrl
    b @L_801C1980
    mr r12, r29
    mr r4, r27
    lwz r6, 0x0(r31)
    li r5, 0x7
    mtctr r12
    bctrl
    b @L_801C1980
    mr r12, r29
    mr r4, r27
    lwz r6, 0x0(r31)
    li r5, 0x7
    mtctr r12
    bctrl
    @L_801C1980:
    lwz r26, 0x18(r27)
    cmplwi r26, 0x0
    beq @L_801C1C30
    rlwinm. r0, r28, 0, 20, 20
    beq @L_801C1AD4
    lwz r0, 0x18(r26)
    cmplwi r0, 0x0
    beq @L_801C1AD4
    cmplwi r30, 0xb
    lwz r3, 0x18(r26)
    bgt @L_801C1AD4
    lis r4, jumptable_8036DB54@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036DB54@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C1AD4
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1AD4
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1AD4
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1AD4
    mr r12, r29
    mr r4, r26
    mtctr r12
    bctrl
    b @L_801C1AD4
    mr r12, r29
    mr r4, r26
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C1AD4
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1AD4
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1AD4
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1AD4
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C1AD4
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C1AD4
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xc
    mtctr r12
    bctrl
    @L_801C1AD4:
    lwz r26, 0x1c(r26)
    b @L_801C1C28
    @L_801C1ADC:
    rlwinm. r0, r28, 0, 22, 22
    beq @L_801C1C24
    lwz r0, 0x18(r26)
    cmplwi r0, 0x0
    beq @L_801C1C24
    cmplwi r30, 0xb
    lwz r3, 0x18(r26)
    bgt @L_801C1C24
    lis r4, jumptable_8036DB24@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036DB24@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C1C24
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1C24
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1C24
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1C24
    mr r12, r29
    mr r4, r26
    mtctr r12
    bctrl
    b @L_801C1C24
    mr r12, r29
    mr r4, r26
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C1C24
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1C24
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1C24
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1C24
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C1C24
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C1C24
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    @L_801C1C24:
    lwz r26, 0x0(r26)
    @L_801C1C28:
    cmplwi r26, 0x0
    bne @L_801C1ADC
    @L_801C1C30:
    lwz r26, 0x1c(r27)
    cmplwi r26, 0x0
    beq @L_801C1EE0
    rlwinm. r0, r28, 0, 20, 20
    beq @L_801C1D84
    lwz r0, 0x18(r26)
    cmplwi r0, 0x0
    beq @L_801C1D84
    cmplwi r30, 0xb
    lwz r3, 0x18(r26)
    bgt @L_801C1D84
    lis r4, jumptable_8036DAF4@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036DAF4@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C1D84
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1D84
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1D84
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1D84
    mr r12, r29
    mr r4, r26
    mtctr r12
    bctrl
    b @L_801C1D84
    mr r12, r29
    mr r4, r26
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C1D84
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1D84
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1D84
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1D84
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C1D84
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C1D84
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xc
    mtctr r12
    bctrl
    @L_801C1D84:
    lwz r26, 0x1c(r26)
    b @L_801C1ED8
    @L_801C1D8C:
    rlwinm. r0, r28, 0, 22, 22
    beq @L_801C1ED4
    lwz r0, 0x18(r26)
    cmplwi r0, 0x0
    beq @L_801C1ED4
    cmplwi r30, 0xb
    lwz r3, 0x18(r26)
    bgt @L_801C1ED4
    lis r4, jumptable_8036DAC4@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036DAC4@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C1ED4
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1ED4
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1ED4
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1ED4
    mr r12, r29
    mr r4, r26
    mtctr r12
    bctrl
    b @L_801C1ED4
    mr r12, r29
    mr r4, r26
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C1ED4
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1ED4
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1ED4
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C1ED4
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C1ED4
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C1ED4
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    @L_801C1ED4:
    lwz r26, 0x0(r26)
    @L_801C1ED8:
    cmplwi r26, 0x0
    bne @L_801C1D8C
    @L_801C1EE0:
    lwz r27, 0xc(r27)
    @L_801C1EE4:
    cmplwi r27, 0x0
    bne @L_801C1838
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
