    nofralloc
    mflr r0
    li r5, 0x0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    li r31, 0x0
    stw r30, 0x20(r1)
    li r30, 0x0
    stw r29, 0x1c(r1)
    addi r29, r4, 0x0
    addi r4, r1, 0x10
    @L_800B2710:
    li r8, 0x1ffc
    lwz r6, 0x80(r3)
    addi r0, r5, 0x3
    srawi r8, r8, 1
    slwi r0, r0, 13
    add r7, r6, r0
    addze. r8, r8
    stw r7, 0x0(r4)
    addi r6, r7, 0x4
    li r11, 0x0
    li r10, 0x0
    ble @L_800B27F8
    srwi. r0, r8, 3
    mtctr r0
    beq @L_800B27DC
    @L_800B274C:
    lhz r9, 0x0(r6)
    nor r0, r9, r9
    add r10, r10, r9
    lhz r9, 0x2(r6)
    add r11, r11, r0
    nor r0, r9, r9
    add r10, r10, r9
    lhz r9, 0x4(r6)
    add r11, r11, r0
    nor r0, r9, r9
    add r10, r10, r9
    lhz r9, 0x6(r6)
    add r11, r11, r0
    nor r0, r9, r9
    add r10, r10, r9
    lhz r9, 0x8(r6)
    add r11, r11, r0
    nor r0, r9, r9
    add r10, r10, r9
    lhz r9, 0xa(r6)
    add r11, r11, r0
    nor r0, r9, r9
    add r10, r10, r9
    lhz r9, 0xc(r6)
    add r11, r11, r0
    nor r0, r9, r9
    add r10, r10, r9
    lhz r9, 0xe(r6)
    add r11, r11, r0
    nor r0, r9, r9
    add r10, r10, r9
    add r11, r11, r0
    addi r6, r6, 0x10
    bdnz @L_800B274C
    andi. r8, r8, 0x7
    beq @L_800B27F8
    @L_800B27DC:
    mtctr r8
    @L_800B27E0:
    lhz r9, 0x0(r6)
    addi r6, r6, 0x2
    nor r0, r9, r9
    add r10, r10, r9
    add r11, r11, r0
    bdnz @L_800B27E0
    @L_800B27F8:
    clrlwi r0, r10, 16
    cmplwi r0, 0xffff
    bne @L_800B2808
    li r10, 0x0
    @L_800B2808:
    clrlwi r0, r11, 16
    cmplwi r0, 0xffff
    bne @L_800B2818
    li r11, 0x0
    @L_800B2818:
    lhz r6, 0x0(r7)
    clrlwi r0, r10, 16
    cmplw r6, r0
    bne @L_800B2838
    lhz r6, 0x2(r7)
    clrlwi r0, r11, 16
    cmplw r6, r0
    beq @L_800B284C
    @L_800B2838:
    li r0, 0x0
    stw r0, 0x88(r3)
    addi r30, r5, 0x0
    addi r31, r31, 0x1
    b @L_800B28A4
    @L_800B284C:
    lhz r8, 0x10(r3)
    addi r6, r7, 0xa
    li r10, 0x0
    li r9, 0x5
    b @L_800B2878
    @L_800B2860:
    lhz r0, 0x0(r6)
    cmplwi r0, 0x0
    bne @L_800B2870
    addi r10, r10, 0x1
    @L_800B2870:
    addi r6, r6, 0x2
    addi r9, r9, 0x1
    @L_800B2878:
    clrlwi r0, r9, 16
    cmplw r0, r8
    blt @L_800B2860
    lhz r0, 0x6(r7)
    clrlwi r6, r10, 16
    cmplw r6, r0
    beq @L_800B28A4
    li r0, 0x0
    stw r0, 0x88(r3)
    addi r30, r5, 0x0
    addi r31, r31, 0x1
    @L_800B28A4:
    addi r5, r5, 0x1
    cmpwi r5, 0x2
    addi r4, r4, 0x4
    blt @L_800B2710
    cmpwi r31, 0x0
    bne @L_800B293C
    lwz r4, 0x88(r3)
    cmplwi r4, 0x0
    bne @L_800B2920
    lwz r5, 0x14(r1)
    lwz r4, 0x10(r1)
    lha r5, 0x4(r5)
    lha r0, 0x4(r4)
    subf. r0, r5, r0
    bge @L_800B28E8
    li r30, 0x0
    b @L_800B28EC
    @L_800B28E8:
    li r30, 0x1
    @L_800B28EC:
    slwi r0, r30, 2
    addi r6, r1, 0x10
    add r6, r6, r0
    lwz r4, 0x0(r6)
    xori r0, r30, 0x1
    slwi r0, r0, 2
    stw r4, 0x88(r3)
    addi r4, r1, 0x10
    li r5, 0x2000
    lwz r3, 0x0(r6)
    lwzx r4, r4, r0
    bl memcpy
    b @L_800B293C
    @L_800B2920:
    lwz r0, 0x10(r1)
    cmplw r4, r0
    bne @L_800B2934
    li r0, 0x0
    b @L_800B2938
    @L_800B2934:
    li r0, 0x1
    @L_800B2938:
    mr r30, r0
    @L_800B293C:
    cmplwi r29, 0x0
    beq @L_800B2948
    stw r30, 0x0(r29)
    @L_800B2948:
    mr r3, r31
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
