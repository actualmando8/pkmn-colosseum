    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    extsb r0, r5
    cmpwi r0, 0x78
    stw r31, 0x3c(r1)
    mr r31, r6
    stw r30, 0x38(r1)
    mr r30, r4
    stw r29, 0x34(r1)
    mr r29, r3
    beq @L_801A95E4
    bge @L_801A95D8
    cmpwi r0, 0x59
    beq @L_801A96B4
    bge @L_801A9784
    cmpwi r0, 0x58
    bge @L_801A95E4
    b @L_801A9784
@L_801A95D8:
    cmpwi r0, 0x7a
    bge @L_801A9784
    b @L_801A96B4
@L_801A95E4:
    lfs f0, 0x0(r29)
    addi r3, r1, 0x20
    addi r4, r1, 0x20
    stfs f0, 0x20(r1)
    lfs f0, 0x10(r29)
    stfs f0, 0x24(r1)
    lfs f0, 0x20(r29)
    stfs f0, 0x28(r1)
    bl fn_800A3ADC
    extsb r0, r31
    cmpwi r0, 0x7a
    beq @L_801A9624
    bge @L_801A966C
    cmpwi r0, 0x5a
    beq @L_801A9624
    b @L_801A966C
@L_801A9624:
    lfs f0, 0x8(r29)
    addi r3, r1, 0x8
    addi r4, r1, 0x8
    stfs f0, 0x8(r1)
    lfs f0, 0x18(r29)
    stfs f0, 0xc(r1)
    lfs f0, 0x28(r29)
    stfs f0, 0x10(r1)
    bl fn_800A3ADC
    addi r3, r1, 0x8
    addi r4, r1, 0x20
    addi r5, r1, 0x14
    bl fn_800A3B9C
    addi r3, r1, 0x20
    addi r4, r1, 0x14
    addi r5, r1, 0x8
    bl fn_800A3B9C
    b @L_801A9850
@L_801A966C:
    lfs f0, 0x4(r29)
    addi r3, r1, 0x14
    addi r4, r1, 0x14
    stfs f0, 0x14(r1)
    lfs f0, 0x14(r29)
    stfs f0, 0x18(r1)
    lfs f0, 0x24(r29)
    stfs f0, 0x1c(r1)
    bl fn_800A3ADC
    addi r3, r1, 0x20
    addi r4, r1, 0x14
    addi r5, r1, 0x8
    bl fn_800A3B9C
    addi r3, r1, 0x8
    addi r4, r1, 0x20
    addi r5, r1, 0x14
    bl fn_800A3B9C
    b @L_801A9850
@L_801A96B4:
    lfs f0, 0x4(r29)
    addi r3, r1, 0x14
    addi r4, r1, 0x14
    stfs f0, 0x14(r1)
    lfs f0, 0x14(r29)
    stfs f0, 0x18(r1)
    lfs f0, 0x24(r29)
    stfs f0, 0x1c(r1)
    bl fn_800A3ADC
    extsb r0, r31
    cmpwi r0, 0x78
    beq @L_801A96F4
    bge @L_801A973C
    cmpwi r0, 0x58
    beq @L_801A96F4
    b @L_801A973C
@L_801A96F4:
    lfs f0, 0x0(r29)
    addi r3, r1, 0x20
    addi r4, r1, 0x20
    stfs f0, 0x20(r1)
    lfs f0, 0x10(r29)
    stfs f0, 0x24(r1)
    lfs f0, 0x20(r29)
    stfs f0, 0x28(r1)
    bl fn_800A3ADC
    addi r3, r1, 0x20
    addi r4, r1, 0x14
    addi r5, r1, 0x8
    bl fn_800A3B9C
    addi r3, r1, 0x14
    addi r4, r1, 0x8
    addi r5, r1, 0x20
    bl fn_800A3B9C
    b @L_801A9850
@L_801A973C:
    lfs f0, 0x8(r29)
    addi r3, r1, 0x8
    addi r4, r1, 0x8
    stfs f0, 0x8(r1)
    lfs f0, 0x18(r29)
    stfs f0, 0xc(r1)
    lfs f0, 0x28(r29)
    stfs f0, 0x10(r1)
    bl fn_800A3ADC
    addi r3, r1, 0x14
    addi r4, r1, 0x8
    addi r5, r1, 0x20
    bl fn_800A3B9C
    addi r3, r1, 0x20
    addi r4, r1, 0x14
    addi r5, r1, 0x8
    bl fn_800A3B9C
    b @L_801A9850
@L_801A9784:
    lfs f0, 0x8(r29)
    addi r3, r1, 0x8
    addi r4, r1, 0x8
    stfs f0, 0x8(r1)
    lfs f0, 0x18(r29)
    stfs f0, 0xc(r1)
    lfs f0, 0x28(r29)
    stfs f0, 0x10(r1)
    bl fn_800A3ADC
    extsb r0, r31
    cmpwi r0, 0x79
    beq @L_801A97C4
    bge @L_801A980C
    cmpwi r0, 0x59
    beq @L_801A97C4
    b @L_801A980C
@L_801A97C4:
    lfs f0, 0x4(r29)
    addi r3, r1, 0x14
    addi r4, r1, 0x14
    stfs f0, 0x14(r1)
    lfs f0, 0x14(r29)
    stfs f0, 0x18(r1)
    lfs f0, 0x24(r29)
    stfs f0, 0x1c(r1)
    bl fn_800A3ADC
    addi r3, r1, 0x14
    addi r4, r1, 0x8
    addi r5, r1, 0x20
    bl fn_800A3B9C
    addi r3, r1, 0x8
    addi r4, r1, 0x20
    addi r5, r1, 0x14
    bl fn_800A3B9C
    b @L_801A9850
@L_801A980C:
    lfs f0, 0x0(r29)
    addi r3, r1, 0x20
    addi r4, r1, 0x20
    stfs f0, 0x20(r1)
    lfs f0, 0x10(r29)
    stfs f0, 0x24(r1)
    lfs f0, 0x20(r29)
    stfs f0, 0x28(r1)
    bl fn_800A3ADC
    addi r3, r1, 0x8
    addi r4, r1, 0x20
    addi r5, r1, 0x14
    bl fn_800A3B9C
    addi r3, r1, 0x14
    addi r4, r1, 0x8
    addi r5, r1, 0x20
    bl fn_800A3B9C
@L_801A9850:
    lfs f0, 0x20(r1)
    lfs f2, -23108(r2)
    stfs f0, 0x0(r30)
    lfs f1, -23108(r2)
    lfs f3, 0x24(r1)
    lfs f0, -23108(r2)
    stfs f3, 0x10(r30)
    lfs f3, 0x28(r1)
    stfs f3, 0x20(r30)
    lfs f3, 0x14(r1)
    stfs f3, 0x4(r30)
    lfs f3, 0x18(r1)
    stfs f3, 0x14(r30)
    lfs f3, 0x1c(r1)
    stfs f3, 0x24(r30)
    lfs f3, 0x8(r1)
    stfs f3, 0x8(r30)
    lfs f3, 0xc(r1)
    stfs f3, 0x18(r30)
    lfs f3, 0x10(r1)
    stfs f3, 0x28(r30)
    stfs f2, 0xc(r30)
    stfs f1, 0x1c(r30)
    stfs f0, 0x2c(r30)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
