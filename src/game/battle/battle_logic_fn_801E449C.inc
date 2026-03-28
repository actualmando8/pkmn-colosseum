    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r4, -32697
    stw r0, 0x34(r1)
    addi r5, r4, -21408
    stmw r25, 0x14(r1)
    lwz r0, 0xa0(r5)
    cmpwi r0, 0x0
    beq @L_801E4638
    lbz r0, 0xa4(r5)
    cmplwi r0, 0x0
    bne @L_801E4638
    lwz r0, 0xb0(r5)
    mr r31, r3
    cmpwi r0, 0x0
    beq @L_801E44EC
    lwz r0, 0x58(r5)
    stw r3, 0xb4(r5)
    add r31, r31, r0
    b @L_801E4518
@L_801E44EC:
    li r0, 0xa
    li r3, 0x0
    mtctr r0
@L_801E44F8:
    add r4, r5, r3
    addi r3, r3, 0xc
    stw r31, 0xf0(r4)
    lwz r4, 0x44(r5)
    addi r0, r4, 0x1f
    clrrwi r0, r0, 5
    add r31, r31, r0
    bdnz @L_801E44F8
@L_801E4518:
    lis r3, -32697
    li r27, 0x0
    addi r29, r3, -21408
    li r30, 0x0
    lwz r3, 0x80(r29)
    lwz r0, 0x84(r29)
    mullw r4, r3, r0
    srwi r3, r4, 2
    addi r4, r4, 0x1f
    addi r0, r3, 0x1f
    clrrwi r26, r4, 5
    clrrwi r25, r0, 5
@L_801E4548:
    add r28, r29, r30
    mr r3, r31
    stw r31, 0x168(r28)
    mr r4, r26
    bl DCInvalidateRange
    add r31, r31, r26
    mr r4, r25
    stw r31, 0x16c(r28)
    mr r3, r31
    bl DCInvalidateRange
    add r31, r31, r25
    mr r4, r25
    stw r31, 0x170(r28)
    mr r3, r31
    bl DCInvalidateRange
    addi r27, r27, 0x1
    add r31, r31, r25
    cmplwi r27, 0x3
    addi r30, r30, 0x10
    blt @L_801E4548
    lis r3, -32697
    addi r5, r3, -21408
    lbz r0, 0xa7(r5)
    cmplwi r0, 0x0
    beq @L_801E4624
    li r0, 0x3
    li r6, 0x0
    li r3, 0x0
    mtctr r0
@L_801E45BC:
    add r4, r5, r3
    addi r3, r3, 0xc
    stw r31, 0x198(r4)
    stw r31, 0x19c(r4)
    stw r6, 0x1a0(r4)
    lwz r0, 0x48(r5)
    slwi r4, r0, 2
    addi r0, r4, 0x1f
    clrrwi r0, r0, 5
    add r31, r31, r0
    bdnz @L_801E45BC
    lis r4, -32697
    li r3, 0x1f4
    addi r5, r4, -21408
    stw r31, -21424(r13)
    lwz r4, 0x90(r5)
    lwz r0, 0x8c(r5)
    mulli r4, r4, 0x28
    cmplwi r0, 0x2
    divwu r3, r4, r3
    addi r0, r3, 0x1f
    clrrwi r0, r0, 5
    add r31, r31, r0
    bne @L_801E4624
    stw r31, -21420(r13)
    add r31, r31, r0
@L_801E4624:
    lis r4, -32697
    li r3, 0x1
    addi r4, r4, -21408
    stw r31, 0x9c(r4)
    b @L_801E463C
@L_801E4638:
    li r3, 0x0
@L_801E463C:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
