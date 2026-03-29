    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    mr r29, r3
    cmplwi r29, 0x0
    bne @L_800FE050
    lis r3, lbl_802717D4@ha
    addi r3, r3, lbl_802717D4@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_800FE33C
@L_800FE050:
    lis r3, lbl_80402480@ha
    li r4, 0x0
    addi r3, r3, lbl_80402480@l
    li r5, 0x68
    mr r30, r3
    bl memset
    lis r3, lbl_80402480@ha
    lfs f0, lbl_8047CD08(r2)
    addi r3, r3, lbl_80402480@l
    li r4, 0x1
    li r0, -0x1
    stb r4, 0x0(r3)
    lwz r6, lbl_80478B08(r13)
    li r5, 0x0
    stfs f0, 0x60(r3)
    stfs f0, 0x64(r3)
    stw r0, 0x24(r3)
    stw r29, 0x28(r3)
    stw r29, 0x2c(r3)
    stw r29, 0x30(r3)
    stb r4, 0x1(r3)
    lhz r0, 0x4(r6)
    lhz r4, 0x20(r30)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_800FE14C
@L_800FE0B8:
    lwz r0, 0x24(r6)
    add r3, r0, r5
    lhz r0, 0x0(r3)
    cmplw r0, r4
    bne @L_800FE144
    lbz r0, 0x2(r3)
    cmplwi r4, 0x0
    stb r0, 0x22(r30)
    lbz r3, 0x3(r3)
    stb r3, 0x23(r30)
    bne @L_800FE0F0
    li r0, 0xb
    stb r0, 0x42(r30)
    b @L_800FE14C
@L_800FE0F0:
    cmplwi r4, 0x1
    beq @L_800FE0FC
    bne @L_800FE108
@L_800FE0FC:
    li r0, 0x6
    stb r0, 0x42(r30)
    b @L_800FE14C
@L_800FE108:
    lis r0, 0x4330
    stw r3, 0xc(r1)
    lfd f2, lbl_8047CD28(r2)
    stw r0, 0x8(r1)
    lfd f3, lbl_8047CD20(r2)
    lfd f1, 0x8(r1)
    lfd f0, lbl_8047CD18(r2)
    fsub f1, f1, f2
    fmadd f0, f3, f1, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    extsb r0, r0
    stb r0, 0x42(r30)
    b @L_800FE14C
@L_800FE144:
    addi r5, r5, 0x8
    bdnz @L_800FE0B8
@L_800FE14C:
    lwz r3, 0x30(r30)
    lhz r4, 0x0(r3)
    cmplwi r4, 0x0
    bne @L_800FE190
    lbz r3, 0x40(r30)
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_800FE170
    b @L_800FE198
@L_800FE170:
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x40(r30)
    slwi r3, r0, 2
    addi r0, r3, 0x34
    lwzx r0, r30, r0
    stw r0, 0x30(r30)
    b @L_800FE14C
@L_800FE190:
    addi r0, r3, 0x2
    stw r0, 0x30(r30)
@L_800FE198:
    clrlwi r0, r4, 16
    cmplwi r0, 0x0
    beq @L_800FE330
    cmplwi r0, 0xffff
    bne @L_800FE14C
    lwz r4, 0x30(r30)
    lwz r3, lbl_80478B08(r13)
    addi r0, r4, 0x1
    stw r0, 0x30(r30)
    lwz r3, 0x28(r3)
    lbz r4, 0x0(r4)
    cmplwi r3, 0x0
    beq @L_800FE14C
    lbz r0, 0x1(r30)
    cmplwi r0, 0x0
    bne @L_800FE1E8
    slwi r0, r4, 3
    lbzx r0, r3, r0
    extrwi r0, r0, 1, 27
    b @L_800FE1F4
@L_800FE1E8:
    slwi r0, r4, 3
    lbzx r0, r3, r0
    extrwi r0, r0, 1, 28
@L_800FE1F4:
    cmplwi r0, 0x0
    beq @L_800FE14C
    slwi r0, r4, 3
    add r28, r3, r0
    lwz r12, 0x4(r28)
    cmplwi r12, 0x0
    beq @L_800FE14C
    mr r3, r30
    mtctr r12
    bctrl
    lbz r0, 0x0(r28)
    extrwi r0, r0, 2, 24
    cmplwi r0, 0x0
    beq @L_800FE14C
    cmplwi r3, 0x0
    beq @L_800FE14C
    cmpwi r0, 0x2
    beq @L_800FE254
    bge @L_800FE2E4
    cmpwi r0, 0x1
    bge @L_800FE24C
    b @L_800FE2E4
@L_800FE24C:
    mr r31, r3
    b @L_800FE2E4
@L_800FE254:
    cmplwi r3, 0x0
    bne @L_800FE264
    li r31, 0x0
    b @L_800FE2E4
@L_800FE264:
    lwz r4, lbl_80478B08(r13)
    srwi r5, r3, 20
    clrlwi r3, r3, 12
    lwz r9, 0x8(r4)
    b @L_800FE2D8
@L_800FE278:
    lhz r0, 0x0(r9)
    cmplw r0, r5
    bne @L_800FE2D4
    lhz r6, 0x4(r9)
    addi r8, r9, 0x10
    li r7, 0x0
    b @L_800FE2CC
@L_800FE294:
    add r0, r7, r6
    srwi r4, r0, 1
    slwi r0, r4, 3
    add r10, r8, r0
    lwz r0, 0x0(r10)
    cmplw r0, r3
    bne @L_800FE2BC
    lwz r0, 0x4(r10)
    add r31, r9, r0
    b @L_800FE2E4
@L_800FE2BC:
    bge @L_800FE2C8
    addi r7, r4, 0x1
    b @L_800FE2CC
@L_800FE2C8:
    mr r6, r4
@L_800FE2CC:
    cmplw r7, r6
    blt @L_800FE294
@L_800FE2D4:
    lwz r9, 0x8(r9)
@L_800FE2D8:
    cmplwi r9, 0x0
    bne @L_800FE278
    li r31, 0x0
@L_800FE2E4:
    lbz r4, 0x40(r30)
    extsb r3, r4
    cmpwi r3, 0x3
    blt @L_800FE310
    lis r3, lbl_80271700@ha
    lis r4, lbl_80315678@ha
    addi r3, r3, lbl_80271700@l
    addi r4, r4, lbl_80315678@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_800FE14C
@L_800FE310:
    addi r0, r4, 0x1
    slwi r3, r3, 2
    stb r0, 0x40(r30)
    addi r0, r3, 0x34
    lwz r3, 0x30(r30)
    stwx r3, r30, r0
    stw r31, 0x30(r30)
    b @L_800FE14C
@L_800FE330:
    lwz r0, 0x30(r30)
    subf r3, r29, r0
    addi r3, r3, 0x2
@L_800FE33C:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
