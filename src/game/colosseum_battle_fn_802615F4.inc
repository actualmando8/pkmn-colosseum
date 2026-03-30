    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r3
    li r3, 0xa
    addi r27, r28, 0x4
    bl fn_801DAEF8
    lhz r29, 0x0(r28)
    addi r30, r27, 0x44
    lhz r28, 0x2(r28)
    b @L_8026165C
@L_80261624:
    lhz r3, 0x4(r30)
    bl fn_801DE418
    lwz r4, 0x0(r30)
    mr r31, r3
    stw r31, 0x27c0(r4)
    lwz r4, 0x8(r30)
    stw r31, 0x0(r4)
    bl fn_801DAC3C
    addi r4, r30, 0xc
    bl fn_800E9B2C
    lbz r4, 0x6(r30)
    mr r3, r31
    bl fn_801DA224
    addi r30, r30, 0x78
@L_8026165C:
    clrlwi. r0, r29, 16
    subi r29, r29, 0x1
    bne @L_80261624
    mr r29, r30
    b @L_802616C8
@L_80261670:
    lhz r3, 0x4(r29)
    lwz r4, 0x8(r29)
    lbz r5, 0x7(r29)
    bl fn_801DE190
    lwz r4, 0x0(r29)
    mr r31, r3
    stw r31, 0x600(r4)
    lwz r4, 0xc(r29)
    stw r31, 0x0(r4)
    bl fn_801DAC3C
    addi r4, r29, 0x10
    mr r30, r3
    bl fn_800E9B2C
    lbz r4, 0x6(r29)
    mr r3, r31
    bl fn_801DA224
    mr r3, r30
    bl fn_800E3D08
    mr r4, r3
    mr r3, r31
    bl fn_801DA4E8
    addi r29, r29, 0x7c
@L_802616C8:
    clrlwi. r0, r28, 16
    subi r28, r28, 0x1
    bne @L_80261670
    bl fn_801C3108
    mr r4, r27
    li r5, 0x44
    bl memcpy
    li r3, 0x1
    bl fn_801EF8F4
    bl fn_801C3430
    bl fn_801F198C
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
