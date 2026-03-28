    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r6, -32697
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    addi r29, r6, -23488
    addi r30, r29, 0x820
    mr r28, r3
    mr r27, r4
    mr r31, r5
    lwz r0, 0xa0(r30)
    cmpwi r0, 0x0
    beq @L_801E4454
    lbz r0, 0xa4(r30)
    cmplwi r0, 0x0
    bne @L_801E4454
    cmpwi r28, 0x0
    ble @L_801E41C0
    lwz r5, 0x60(r30)
    cmplwi r5, 0x0
    bne @L_801E4154
    li r3, 0x0
    b @L_801E4458
@L_801E4154:
    lwz r0, 0x50(r30)
    cmplw r0, r28
    ble @L_801E41B8
    subi r0, r28, 0x1
    mr r3, r30
    slwi r0, r0, 2
    addi r4, r29, 0xa0
    add r6, r5, r0
    li r5, 0x20
    li r7, 0x2
    bl fn_800A541C
    cmpwi r3, 0x0
    bge @L_801E4190
    li r3, 0x0
    b @L_801E4458
@L_801E4190:
    addi r3, r29, 0xa0
    lwz r4, 0x64(r30)
    lha r6, 0xa0(r29)
    lha r0, 0x2(r3)
    add r3, r4, r6
    stw r28, 0xc0(r30)
    subf r0, r6, r0
    stw r3, 0xb8(r30)
    stw r0, 0xbc(r30)
    b @L_801E41D4
@L_801E41B8:
    li r3, 0x0
    b @L_801E4458
@L_801E41C0:
    lwz r3, 0x64(r30)
    lwz r0, 0x54(r30)
    stw r3, 0xb8(r30)
    stw r0, 0xbc(r30)
    stw r28, 0xc0(r30)
@L_801E41D4:
    addi r28, r29, 0x820
    lbz r0, 0xa7(r28)
    cmplwi r0, 0x0
    beq @L_801E4204
    cmpwi r31, 0x0
    blt @L_801E41F8
    lwz r0, 0x98(r28)
    cmplw r31, r0
    blt @L_801E4200
@L_801E41F8:
    li r3, 0x0
    b @L_801E4458
@L_801E4200:
    stw r31, 0xdc(r28)
@L_801E4204:
    addi r31, r29, 0x820
    clrlwi r4, r27, 31
    lwz r0, 0xb0(r31)
    li r3, 0x0
    stb r4, 0xa6(r31)
    cmpwi r0, 0x0
    stw r3, 0xd8(r31)
    beq @L_801E4288
    lwz r4, 0xb4(r31)
    mr r3, r31
    lwz r5, 0x58(r31)
    li r7, 0x2
    lwz r6, 0x64(r31)
    bl fn_800A541C
    cmpwi r3, 0x0
    bge @L_801E424C
    li r3, 0x0
    b @L_801E4458
@L_801E424C:
    lwz r4, 0xb4(r31)
    li r3, 0x14
    lwz r0, 0xb8(r31)
    lwz r5, 0x64(r31)
    add r0, r4, r0
    subf r25, r5, r0
    mr r4, r25
    bl fn_801E5470
    lbz r0, 0xa7(r28)
    cmplwi r0, 0x0
    beq @L_801E42B4
    mr r4, r25
    li r3, 0xc
    bl fn_801E4E1C
    b @L_801E42B4
@L_801E4288:
    li r3, 0x14
    li r4, 0x0
    bl fn_801E5470
    lbz r0, 0xa7(r28)
    cmplwi r0, 0x0
    beq @L_801E42AC
    li r3, 0xc
    li r4, 0x0
    bl fn_801E4E1C
@L_801E42AC:
    li r3, 0x8
    bl fn_801E1D7C
@L_801E42B4:
    lwz r0, 0xb0(r31)
    cmpwi r0, 0x0
    bne @L_801E42E8
    li r25, 0x0
    addi r26, r29, 0x820
    mr r27, r25
@L_801E42CC:
    addi r3, r27, 0xf0
    add r3, r26, r3
    bl fn_801E1BB8
    addi r25, r25, 0x1
    addi r27, r27, 0xc
    cmpwi r25, 0xa
    blt @L_801E42CC
@L_801E42E8:
    addi r26, r29, 0x820
    li r25, 0x0
    li r27, 0x0
@L_801E42F4:
    addi r3, r27, 0x168
    add r3, r26, r3
    bl fn_801E4F34
    addi r25, r25, 0x1
    addi r27, r27, 0x10
    cmpwi r25, 0x3
    blt @L_801E42F4
    lbz r0, 0xa7(r28)
    cmplwi r0, 0x0
    beq @L_801E4344
    addi r26, r29, 0x820
    li r25, 0x0
    li r27, 0x0
@L_801E4328:
    addi r3, r27, 0x198
    add r3, r26, r3
    bl fn_801E4B08
    addi r25, r25, 0x1
    addi r27, r27, 0xc
    cmpwi r25, 0x3
    blt @L_801E4328
@L_801E4344:
    addi r3, r29, 0x74
    addi r4, r13, -21416
    li r5, 0x2
    bl fn_8009F1D0
    bl fn_801E543C
    lbz r0, 0xa7(r28)
    cmplwi r0, 0x0
    beq @L_801E4368
    bl fn_801E4DE8
@L_801E4368:
    lwz r0, 0xb0(r31)
    cmpwi r0, 0x0
    bne @L_801E4378
    bl fn_801E1D48
@L_801E4378:
    lbz r0, 0xa7(r28)
    cmplwi r0, 0x0
    beq @L_801E43CC
    addi r3, r29, 0x74
    addi r4, r1, 0x8
    li r5, 0x1
    bl fn_8009F2F8
    addi r3, r29, 0x74
    addi r4, r1, 0xc
    li r5, 0x1
    bl fn_8009F2F8
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    beq @L_801E43C4
    lwz r0, 0xc(r1)
    cmpwi r0, 0x0
    beq @L_801E43C4
    li r0, 0x1
    b @L_801E43F4
@L_801E43C4:
    li r0, 0x0
    b @L_801E43F4
@L_801E43CC:
    addi r3, r29, 0x74
    addi r4, r1, 0x8
    li r5, 0x1
    bl fn_8009F2F8
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    beq @L_801E43F0
    li r0, 0x1
    b @L_801E43F4
@L_801E43F0:
    li r0, 0x0
@L_801E43F4:
    cmpwi r0, 0x0
    bne @L_801E4404
    li r3, 0x0
    b @L_801E4458
@L_801E4404:
    lbz r0, 0xa7(r28)
    li r6, 0x1
    addi r4, r29, 0x820
    li r5, 0x0
    li r3, -0x1
    cmplwi r0, 0x0
    stb r6, 0xa4(r30)
    stb r5, 0xa5(r4)
    stw r5, 0xe8(r4)
    stw r5, 0xec(r4)
    stw r3, 0xe0(r4)
    stw r5, 0xe4(r4)
    beq @L_801E443C
    bl fn_801E34F0
@L_801E443C:
    lis r3, -32738
    addi r3, r3, 14928
    bl fn_800A8850
    stw r3, -21428(r13)
    li r3, 0x1
    b @L_801E4458
@L_801E4454:
    li r3, 0x0
@L_801E4458:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
