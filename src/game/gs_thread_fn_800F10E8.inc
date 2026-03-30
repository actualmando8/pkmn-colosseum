    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    li r30, 0x0
    stw r29, 0x24(r1)
    mr r29, r3
    stw r28, 0x20(r1)
    lwz r3, 0x14(r3)
    addi r0, r3, 0x2
    stw r0, 0x14(r29)
    lwz r3, 0x14(r29)
    lhz r31, 0x0(r3)
    addi r0, r3, 0x2
    stw r0, 0x14(r29)
    lwz r4, 0x28(r29)
    lwz r5, 0x1c(r29)
    cmpwi r4, 0x40
    ble @L_800F114C
    lis r3, lbl_80271068@ha
    addi r3, r3, lbl_80271068@l
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F1160
    @L_800F114C:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r29)
    add r3, r29, r0
    stw r5, 0x6c(r3)
    @L_800F1160:
    lwz r4, 0x28(r29)
    cmpwi r4, 0x40
    ble @L_800F1180
    lis r3, lbl_80271068@ha
    addi r3, r3, lbl_80271068@l
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F1194
    @L_800F1180:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r29)
    add r3, r29, r0
    stw r31, 0x6c(r3)
    @L_800F1194:
    lwz r0, 0x28(r29)
    addi r3, r31, 0x2
    subf r0, r3, r0
    stw r0, 0x1c(r29)
    lwz r0, 0x1c(r29)
    lwz r4, lbl_80478B00(r13)
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    lwz r3, 0x10(r4)
    lwzx r0, r29, r0
    mulli r0, r0, 0xc
    add r31, r3, r0
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    stw r0, lbl_8047AC38(r13)
    beq @L_800F12C4
    lis r3, lbl_80401BD8@ha
    li r30, 0x0
    addi r3, r3, lbl_80401BD8@l
    li r28, 0x0
    li r4, 0x0
    li r5, 0x20
    bl memset
    lis r3, lbl_80401BB8@ha
    li r4, 0x0
    addi r3, r3, lbl_80401BB8@l
    li r5, 0x20
    bl memset
    lwz r3, 0x1c(r29)
    lwz r0, 0x28(r29)
    subf r3, r3, r0
    subi r0, r3, 0x3
    cmpwi r0, 0x8
    ble @L_800F1220
    li r0, 0x8
    @L_800F1220:
    li r5, 0x0
    mtctr r0
    cmpwi r0, 0x0
    ble @L_800F12A4
    @L_800F1230:
    addi r0, r5, 0x4
    lbzx r0, r31, r0
    cmplwi r0, 0x0
    beq @L_800F12A4
    cmplwi r0, 0x2
    bne @L_800F1274
    lwz r0, 0x1c(r29)
    lis r3, lbl_80401BB8@ha
    addi r3, r3, lbl_80401BB8@l
    add r4, r5, r0
    addi r0, r4, 0x1
    slwi r4, r0, 2
    addi r0, r4, 0x6c
    lfsx f0, r29, r0
    stfsx f0, r3, r30
    addi r30, r30, 0x4
    b @L_800F129C
    @L_800F1274:
    lwz r0, 0x1c(r29)
    lis r3, lbl_80401BD8@ha
    addi r3, r3, lbl_80401BD8@l
    add r4, r5, r0
    addi r0, r4, 0x1
    slwi r4, r0, 2
    addi r0, r4, 0x6c
    lwzx r0, r29, r0
    stwx r0, r3, r28
    addi r28, r28, 0x4
    @L_800F129C:
    addi r5, r5, 0x1
    bdnz @L_800F1230
    @L_800F12A4:
    lis r4, lbl_80401BB8@ha
    lis r3, lbl_80401BD8@ha
    addi r4, r4, lbl_80401BB8@l
    addi r0, r3, lbl_80401BD8@l
    stw r4, lbl_8047AC3C(r13)
    stw r0, lbl_8047AC40(r13)
    bl fn_800F106C
    mr r30, r3
    @L_800F12C4:
    lwz r0, 0x1c(r29)
    slwi r0, r0, 2
    add r3, r29, r0
    stw r30, 0x68(r3)
    lwz r3, 0x28(r29)
    cmpwi r3, 0x0
    bgt @L_800F12FC
    lis r3, lbl_8027107C@ha
    addi r3, r3, lbl_8027107C@l
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r29)
    stw r0, 0xc(r1)
    b @L_800F1314
    @L_800F12FC:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r29)
    add r3, r29, r0
    lwz r0, 0x6c(r3)
    stw r0, 0xc(r1)
    @L_800F1314:
    lwz r3, 0x28(r29)
    lwz r0, 0xc(r1)
    cmpwi r3, 0x0
    stw r0, 0x10(r1)
    bgt @L_800F1344
    lis r3, lbl_8027107C@ha
    addi r3, r3, lbl_8027107C@l
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r29)
    stw r0, 0x8(r1)
    b @L_800F135C
    @L_800F1344:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r29)
    add r3, r29, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x8(r1)
    @L_800F135C:
    lwz r0, 0x8(r1)
    lis r3, lbl_8027107C@ha
    lwz r28, 0x10(r1)
    addi r30, r3, lbl_8027107C@l
    stw r0, 0x14(r1)
    li r31, 0x0
    stw r0, 0x1c(r29)
    b @L_800F13A4
    @L_800F137C:
    lwz r3, 0x28(r29)
    cmpwi r3, 0x0
    bgt @L_800F1398
    mr r3, r30
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F13A0
    @L_800F1398:
    subi r0, r3, 0x1
    stw r0, 0x28(r29)
    @L_800F13A0:
    addi r31, r31, 0x1
    @L_800F13A4:
    cmpw r31, r28
    blt @L_800F137C
    lwz r0, 0x34(r1)
    li r3, 0x1
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
