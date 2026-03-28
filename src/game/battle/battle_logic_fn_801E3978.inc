    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r8, -32697
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    addi r31, r8, -21408
    mr r27, r4
    mr r28, r5
    mr r29, r6
    mr r30, r7
    lwz r0, 0xa0(r31)
    cmpwi r0, 0x0
    beq @L_801E3A38
    lbz r0, 0xa4(r31)
    cmplwi r0, 0x0
    beq @L_801E3A38
    lwz r0, 0xe8(r31)
    cmplwi r0, 0x0
    beq @L_801E3A38
    bl fn_801E1FF8
    lwz r5, 0xe8(r31)
    extsh r0, r30
    lis r3, -32697
    extsh r6, r27
    stw r0, 0x8(r1)
    addi r3, r3, -21408
    extsh r7, r28
    extsh r10, r29
    lwz r4, 0x80(r3)
    lwz r0, 0x84(r3)
    lwz r3, 0x0(r5)
    extsh r8, r4
    lwz r4, 0x4(r5)
    extsh r9, r0
    lwz r5, 0x8(r5)
    bl fn_801E1E1C
    bl fn_801E24B0
    lwz r4, 0xe8(r31)
    lis r3, -32697
    addi r3, r3, -21408
    lwz r4, 0xc(r4)
    lwz r0, 0xc0(r3)
    lwz r3, 0x50(r3)
    add r4, r4, r0
    divwu r0, r4, r3
    mullw r0, r0, r3
    subf r3, r0, r4
    b @L_801E3A3C
@L_801E3A38:
    li r3, -0x1
@L_801E3A3C:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
