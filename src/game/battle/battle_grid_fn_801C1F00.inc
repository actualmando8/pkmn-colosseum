    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr. r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    beq @L_801C25D0
    rlwinm. r0, r28, 0, 30, 30
    beq @L_801C2070
    lwz r0, 0x84(r27)
    cmplwi r0, 0x0
    beq @L_801C2070
    cmplwi r30, 0xb
    lwz r3, 0x84(r27)
    bgt @L_801C2070
    lis r4, jumptable_8036DC74@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036DC74@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C2070
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2070
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2070
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2070
    mr r12, r29
    mr r4, r27
    mtctr r12
    bctrl
    b @L_801C2070
    mr r12, r29
    mr r4, r27
    li r5, 0x2
    mtctr r12
    bctrl
    b @L_801C2070
    mr r12, r29
    mr r4, r27
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2070
    mr r12, r29
    mr r4, r27
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2070
    mr r12, r29
    mr r4, r27
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2070
    mr r12, r29
    mr r4, r27
    lfs f1, 0x0(r31)
    li r5, 0x2
    mtctr r12
    bctrl
    b @L_801C2070
    mr r12, r29
    mr r4, r27
    lwz r6, 0x0(r31)
    li r5, 0x2
    mtctr r12
    bctrl
    b @L_801C2070
    mr r12, r29
    mr r4, r27
    lwz r6, 0x0(r31)
    li r5, 0x2
    mtctr r12
    bctrl
    @L_801C2070:
    lwz r26, 0x24(r27)
    cmplwi r26, 0x0
    beq @L_801C2320
    rlwinm. r0, r28, 0, 20, 20
    beq @L_801C21C4
    lwz r0, 0x18(r26)
    cmplwi r0, 0x0
    beq @L_801C21C4
    cmplwi r30, 0xb
    lwz r3, 0x18(r26)
    bgt @L_801C21C4
    lis r4, jumptable_8036DC44@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036DC44@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C21C4
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C21C4
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C21C4
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C21C4
    mr r12, r29
    mr r4, r26
    mtctr r12
    bctrl
    b @L_801C21C4
    mr r12, r29
    mr r4, r26
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C21C4
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C21C4
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C21C4
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C21C4
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C21C4
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C21C4
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xc
    mtctr r12
    bctrl
    @L_801C21C4:
    lwz r26, 0x1c(r26)
    b @L_801C2318
    @L_801C21CC:
    rlwinm. r0, r28, 0, 22, 22
    beq @L_801C2314
    lwz r0, 0x18(r26)
    cmplwi r0, 0x0
    beq @L_801C2314
    cmplwi r30, 0xb
    lwz r3, 0x18(r26)
    bgt @L_801C2314
    lis r4, jumptable_8036DC14@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036DC14@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C2314
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2314
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2314
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2314
    mr r12, r29
    mr r4, r26
    mtctr r12
    bctrl
    b @L_801C2314
    mr r12, r29
    mr r4, r26
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C2314
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2314
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2314
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2314
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C2314
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C2314
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    @L_801C2314:
    lwz r26, 0x0(r26)
    @L_801C2318:
    cmplwi r26, 0x0
    bne @L_801C21CC
    @L_801C2320:
    lwz r26, 0x28(r27)
    cmplwi r26, 0x0
    beq @L_801C25D0
    rlwinm. r0, r28, 0, 20, 20
    beq @L_801C2474
    lwz r0, 0x18(r26)
    cmplwi r0, 0x0
    beq @L_801C2474
    cmplwi r30, 0xb
    lwz r3, 0x18(r26)
    bgt @L_801C2474
    lis r4, jumptable_8036DBE4@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036DBE4@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C2474
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2474
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2474
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2474
    mr r12, r29
    mr r4, r26
    mtctr r12
    bctrl
    b @L_801C2474
    mr r12, r29
    mr r4, r26
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C2474
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2474
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2474
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C2474
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C2474
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xc
    mtctr r12
    bctrl
    b @L_801C2474
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xc
    mtctr r12
    bctrl
    @L_801C2474:
    lwz r26, 0x1c(r26)
    b @L_801C25C8
    @L_801C247C:
    rlwinm. r0, r28, 0, 22, 22
    beq @L_801C25C4
    lwz r0, 0x18(r26)
    cmplwi r0, 0x0
    beq @L_801C25C4
    cmplwi r30, 0xb
    lwz r3, 0x18(r26)
    bgt @L_801C25C4
    lis r4, jumptable_8036DBB4@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036DBB4@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C25C4
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C25C4
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C25C4
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C25C4
    mr r12, r29
    mr r4, r26
    mtctr r12
    bctrl
    b @L_801C25C4
    mr r12, r29
    mr r4, r26
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C25C4
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C25C4
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C25C4
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C25C4
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C25C4
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C25C4
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    @L_801C25C4:
    lwz r26, 0x0(r26)
    @L_801C25C8:
    cmplwi r26, 0x0
    bne @L_801C247C
    @L_801C25D0:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
