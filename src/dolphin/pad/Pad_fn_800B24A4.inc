    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x38(r1)
    stw r31, 0x34(r1)
    addi r7, r1, 0x1c
    addi r8, r1, 0x14
    stw r30, 0x30(r1)
    li r31, 0x0
    li r30, 0x0
    stw r29, 0x2c(r1)
    addi r29, r4, 0x0
    li r4, 0x0
    @L_800B24D4:
    addi r0, r4, 0x1
    lwz r5, 0x80(r3)
    slwi r0, r0, 13
    add r0, r5, r0
    stw r0, 0x0(r7)
    li r6, 0x1ffc
    srawi r6, r6, 1
    lwz r5, 0x0(r7)
    addze. r6, r6
    li r11, 0x0
    addi r0, r5, 0x1fc0
    stw r0, 0x0(r8)
    li r10, 0x0
    lwz r5, 0x0(r7)
    ble @L_800B25C8
    srwi. r0, r6, 3
    mtctr r0
    beq @L_800B25AC
    @L_800B251C:
    lhz r9, 0x0(r5)
    nor r0, r9, r9
    add r10, r10, r9
    lhz r9, 0x2(r5)
    add r11, r11, r0
    nor r0, r9, r9
    add r10, r10, r9
    lhz r9, 0x4(r5)
    add r11, r11, r0
    nor r0, r9, r9
    add r10, r10, r9
    lhz r9, 0x6(r5)
    add r11, r11, r0
    nor r0, r9, r9
    add r10, r10, r9
    lhz r9, 0x8(r5)
    add r11, r11, r0
    nor r0, r9, r9
    add r10, r10, r9
    lhz r9, 0xa(r5)
    add r11, r11, r0
    nor r0, r9, r9
    add r10, r10, r9
    lhz r9, 0xc(r5)
    add r11, r11, r0
    nor r0, r9, r9
    add r10, r10, r9
    lhz r9, 0xe(r5)
    add r11, r11, r0
    nor r0, r9, r9
    add r10, r10, r9
    add r11, r11, r0
    addi r5, r5, 0x10
    bdnz @L_800B251C
    andi. r6, r6, 0x7
    beq @L_800B25C8
    @L_800B25AC:
    mtctr r6
    @L_800B25B0:
    lhz r9, 0x0(r5)
    addi r5, r5, 0x2
    nor r0, r9, r9
    add r10, r10, r9
    add r11, r11, r0
    bdnz @L_800B25B0
    @L_800B25C8:
    clrlwi r0, r10, 16
    cmplwi r0, 0xffff
    bne @L_800B25D8
    li r10, 0x0
    @L_800B25D8:
    clrlwi r0, r11, 16
    cmplwi r0, 0xffff
    bne @L_800B25E8
    li r11, 0x0
    @L_800B25E8:
    lwz r6, 0x0(r8)
    clrlwi r5, r10, 16
    lhz r0, 0x3c(r6)
    cmplw r5, r0
    bne @L_800B260C
    lhz r0, 0x3e(r6)
    clrlwi r5, r11, 16
    cmplw r5, r0
    beq @L_800B261C
    @L_800B260C:
    li r0, 0x0
    stw r0, 0x84(r3)
    addi r30, r4, 0x0
    addi r31, r31, 0x1
    @L_800B261C:
    addi r4, r4, 0x1
    cmpwi r4, 0x2
    addi r7, r7, 0x4
    addi r8, r8, 0x4
    blt @L_800B24D4
    cmpwi r31, 0x0
    bne @L_800B26B8
    lwz r4, 0x84(r3)
    cmplwi r4, 0x0
    bne @L_800B269C
    lwz r5, 0x18(r1)
    lwz r4, 0x14(r1)
    lha r5, 0x3a(r5)
    lha r0, 0x3a(r4)
    subf. r0, r5, r0
    bge @L_800B2664
    li r30, 0x0
    b @L_800B2668
    @L_800B2664:
    li r30, 0x1
    @L_800B2668:
    slwi r0, r30, 2
    addi r6, r1, 0x1c
    add r6, r6, r0
    lwz r4, 0x0(r6)
    xori r0, r30, 0x1
    slwi r0, r0, 2
    stw r4, 0x84(r3)
    addi r4, r1, 0x1c
    li r5, 0x2000
    lwz r3, 0x0(r6)
    lwzx r4, r4, r0
    bl memcpy
    b @L_800B26B8
    @L_800B269C:
    lwz r0, 0x1c(r1)
    cmplw r4, r0
    bne @L_800B26B0
    li r0, 0x0
    b @L_800B26B4
    @L_800B26B0:
    li r0, 0x1
    @L_800B26B4:
    mr r30, r0
    @L_800B26B8:
    cmplwi r29, 0x0
    beq @L_800B26C4
    stw r30, 0x0(r29)
    @L_800B26C4:
    mr r3, r31
    lwz r0, 0x3c(r1)
    lwz r31, 0x34(r1)
    lwz r30, 0x30(r1)
    lwz r29, 0x2c(r1)
    addi r1, r1, 0x38
    mtlr r0
    blr
