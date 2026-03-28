    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r3, -32697
    lis r4, -32697
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    addi r28, r3, -21408
    li r29, 0x0
    addi r27, r4, -28608
    lwz r31, 0xb8(r28)
    lwz r30, 0xbc(r28)
@L_801E1C48:
    addi r3, r27, 0x13d0
    addi r4, r1, 0x8
    li r5, 0x1
    bl fn_8009F2F8
    lwz r26, 0x8(r1)
    mr r3, r28
    mr r5, r30
    mr r6, r31
    lwz r4, 0x0(r26)
    li r7, 0x2
    bl fn_800A541C
    cmpw r3, r30
    beq @L_801E1CA4
    cmpwi r3, -0x1
    bne @L_801E1C8C
    li r0, -0x1
    stw r0, 0xa8(r28)
@L_801E1C8C:
    cmpwi r29, 0x0
    bne @L_801E1C9C
    li r3, 0x0
    bl fn_801E446C
@L_801E1C9C:
    addi r3, r27, 0x1000
    bl fn_800A221C
@L_801E1CA4:
    stw r29, 0x4(r26)
    mr r4, r26
    addi r3, r27, 0x13b0
    li r5, 0x1
    bl fn_8009F230
    lwz r0, 0xc0(r28)
    add r31, r31, r30
    lwz r6, 0x50(r28)
    add r4, r29, r0
    lwz r5, 0x0(r26)
    divwu r3, r4, r6
    subi r0, r6, 0x1
    lwz r30, 0x0(r5)
    mullw r3, r3, r6
    subf r3, r3, r4
    cmplw r3, r0
    bne @L_801E1D04
    lbz r0, 0xa6(r28)
    clrlwi. r0, r0, 31
    beq @L_801E1CFC
    lwz r31, 0x64(r28)
    b @L_801E1D04
@L_801E1CFC:
    addi r3, r27, 0x1000
    bl fn_800A221C
@L_801E1D04:
    addi r29, r29, 0x1
    b @L_801E1C48
