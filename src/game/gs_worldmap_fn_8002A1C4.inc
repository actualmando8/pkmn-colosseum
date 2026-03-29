    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stmw r27, 0x7c(r1)
    bne cr1, @L_8002A1F8
    stfd f1, 0x28(r1)
    stfd f2, 0x30(r1)
    stfd f3, 0x38(r1)
    stfd f4, 0x40(r1)
    stfd f5, 0x48(r1)
    stfd f6, 0x50(r1)
    stfd f7, 0x58(r1)
    stfd f8, 0x60(r1)
    @L_8002A1F8:
    stw r3, 0x8(r1)
    stw r4, 0xc(r1)
    stw r5, 0x10(r1)
    stw r6, 0x14(r1)
    stw r7, 0x18(r1)
    stw r8, 0x1c(r1)
    stw r9, 0x20(r1)
    stw r10, 0x24(r1)
    addi r0, r1, 0x98
    addi r7, r1, 0x8
    lis r8, 0x300
    stw r0, 0x6c(r1)
    slwi r0, r3, 2
    lwz r6, lbl_80478E54(r13)
    stw r8, 0x68(r1)
    slwi r31, r4, 2
    lwz r3, lbl_80478E4C(r13)
    li r30, 0x1
    stw r7, 0x70(r1)
    lbzx r0, r6, r0
    mulli r0, r0, 0x4c
    add r3, r3, r0
    lbz r27, 0x0(r3)
    addi r28, r3, 0x4
    b @L_8002A290
    @L_8002A25C:
    cmpwi r30, 0x0
    beq @L_8002A270
    mr r29, r5
    li r30, 0x0
    b @L_8002A280
    @L_8002A270:
    mr r3, r29
    mr r4, r5
    li r30, 0x1
    bl fn_80132A38
    @L_8002A280:
    addi r3, r1, 0x68
    li r4, 0x1
    bl __va_arg
    lwz r5, 0x0(r3)
    @L_8002A290:
    cmpwi r5, 0x0
    bge @L_8002A25C
    lwzx r4, r28, r31
    mr r7, r27
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    li r3, 0x1
    bl fn_801069FC
    lmw r27, 0x7c(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
