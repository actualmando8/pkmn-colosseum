    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    lis r5, lbl_80266BF0@ha
    lwz r31, 0x60(r3)
    addi r3, r5, lbl_80266BF0@l
    li r8, 0x0
    lwz r7, 0x0(r3)
    lwz r6, 0x4(r3)
    lwz r5, 0x8(r3)
    lwz r3, 0xc(r3)
    lha r0, 0x6(r4)
    cmpw r0, r7
    beq @L_80019318
    li r8, 0x1
    cmpw r0, r6
    beq @L_80019318
    li r8, 0x2
    cmpw r0, r5
    beq @L_80019318
    li r8, 0x3
    cmpw r0, r3
    beq @L_80019318
    li r8, 0x4
    @L_80019318:
    lwz r3, 0xc(r31)
    subfic r0, r3, 0x4
    subf r8, r0, r8
    cmpwi r8, 0x0
    blt @L_80019334
    cmpw r8, r3
    blt @L_8001933C
    @L_80019334:
    li r3, 0x0
    b @L_800194B0
    @L_8001933C:
    slwi r3, r8, 1
    addi r28, r3, 0x4
    lhzx r3, r31, r28
    cmplwi r3, 0x0
    bne @L_80019358
    li r3, 0x134
    b @L_80019360
    @L_80019358:
    bl fn_801440A0
    bl fn_80144088
    @L_80019360:
    mr r6, r3
    li r3, 0x0
    li r4, 0x0
    li r5, -0x1
    bl fn_800FB680
    lhzx r3, r31, r28
    cmplwi r3, 0x0
    beq @L_800194AC
    bl fn_801440A0
    bl fn_80144014
    mr r4, r3
    addi r6, r1, 0xa
    li r3, 0x0
    li r5, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    lhz r4, 0xa(r1)
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xca
    bl fn_800FA444
    srwi r0, r3, 16
    li r3, 0x12e
    extsh r29, r0
    bl fn_800FA444
    srwi r0, r3, 16
    li r4, 0x0
    extsh r0, r0
    li r5, -0x1
    add r29, r29, r0
    li r6, 0x12e
    extsh r0, r29
    subfic r3, r0, 0xc3
    bl fn_800FB680
    lhzx r28, r31, r28
    li r31, 0x0
    mr r3, r28
    bl fn_801440A0
    bl fn_80144014
    mr r4, r3
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    li r30, 0x0
    mr r29, r3
    b @L_80019470
    @L_80019428:
    mr r3, r29
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80019468
    mr r3, r29
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    cmplw r28, r3
    bne @L_80019468
    mr r3, r29
    bl fn_80143C50
    clrlwi r0, r3, 16
    add r31, r31, r0
    @L_80019468:
    addi r30, r30, 0x1
    addi r29, r29, 0x4
    @L_80019470:
    lhz r0, 0x8(r1)
    cmpw r30, r0
    blt @L_80019428
    mr r4, r31
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xca
    bl fn_800FA444
    srwi r0, r3, 16
    li r4, 0x0
    extsh r0, r0
    li r5, -0x1
    subfic r3, r0, 0xc3
    li r6, 0xca
    bl fn_800FB680
    @L_800194AC:
    li r3, 0x0
    @L_800194B0:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
