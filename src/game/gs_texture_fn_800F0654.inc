    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    stmw r25, 0x84(r1)
    mr r29, r3
    mr r30, r4
    bne cr1, @L_800F0690
    stfd f1, 0x28(r1)
    stfd f2, 0x30(r1)
    stfd f3, 0x38(r1)
    stfd f4, 0x40(r1)
    stfd f5, 0x48(r1)
    stfd f6, 0x50(r1)
    stfd f7, 0x58(r1)
    stfd f8, 0x60(r1)
    @L_800F0690:
    stw r3, 0x8(r1)
    stw r4, 0xc(r1)
    stw r5, 0x10(r1)
    stw r6, 0x14(r1)
    stw r7, 0x18(r1)
    stw r8, 0x1c(r1)
    stw r9, 0x20(r1)
    stw r10, 0x24(r1)
    lbz r0, 0x14(r29)
    cmplwi r0, 0x1
    beq @L_800F0794
    lhz r3, 0x20(r29)
    bl fn_800E27B0
    mr r31, r3
    lhz r3, 0x22(r29)
    bl fn_800E27B0
    addi r4, r1, 0xa8
    addi r0, r1, 0x8
    lis r5, 0x200
    cmpwi r30, 0x8
    stw r5, 0x68(r1)
    mr r27, r3
    li r28, 0xc
    li r25, 0x8
    stw r4, 0x6c(r1)
    stw r0, 0x70(r1)
    bge @L_800F0700
    mr r25, r30
    @L_800F0700:
    li r26, 0x0
    b @L_800F0724
    @L_800F0708:
    addi r3, r1, 0x68
    li r4, 0x1
    bl __va_arg
    lwz r0, 0x0(r3)
    addi r26, r26, 0x1
    stwx r0, r27, r28
    addi r28, r28, 0x4
    @L_800F0724:
    cmpw r26, r25
    blt @L_800F0708
    cmpwi r30, 0x8
    ble @L_800F0784
    subf r3, r26, r30
    lwz r0, 0x4(r27)
    slwi r3, r3, 2
    li r26, 0x8
    subf r0, r3, r0
    stw r0, 0x4(r27)
    lwz r0, 0x4(r27)
    srwi r3, r0, 2
    addi r0, r3, 0x2
    slwi r28, r0, 2
    b @L_800F077C
    @L_800F0760:
    addi r3, r1, 0x68
    li r4, 0x1
    bl __va_arg
    lwz r0, 0x0(r3)
    addi r26, r26, 0x1
    stwx r0, r31, r28
    addi r28, r28, 0x4
    @L_800F077C:
    cmpw r26, r30
    blt @L_800F0760
    @L_800F0784:
    lhz r3, 0x20(r29)
    bl fn_800E24B0
    lhz r3, 0x22(r29)
    bl fn_800E24B0
    @L_800F0794:
    lmw r25, 0x84(r1)
    lwz r0, 0xa4(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
