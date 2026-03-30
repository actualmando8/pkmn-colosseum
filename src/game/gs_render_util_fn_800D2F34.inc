    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stw r31, 0x5c(r1)
    stw r30, 0x58(r1)
    stw r29, 0x54(r1)
    mr r29, r4
    stw r28, 0x50(r1)
    mr r28, r3
    li r3, 0x0
    bl fn_800D7BF8
    mr r0, r3
    li r3, 0x2
    mr r30, r0
    bl fn_800D7BF8
    cmplwi r30, 0x0
    mr r31, r3
    beq @L_800D2F84
    cmplwi r31, 0x0
    bne @L_800D2F8C
@L_800D2F84:
    li r3, 0x0
    b @L_800D303C
@L_800D2F8C:
    mr r4, r30
    mr r5, r28
    addi r3, r1, 0x8
    bl fn_800DFF98
    lfs f0, 0x10(r1)
    lfs f5, 0x2c(r31)
    fcmpo cr0, f0, f5
    cror eq, gt, eq
    bne @L_800D2FB8
    li r3, 0x1
    b @L_800D303C
@L_800D2FB8:
    lfs f4, -27808(r2)
    mr r3, r30
    lfs f2, -27804(r2)
    mr r6, r29
    stfs f4, 0x2c(r1)
    addi r4, r1, 0x2c
    lfs f1, -27800(r2)
    addi r5, r1, 0x14
    lfs f3, 0x0(r31)
    addi r7, r29, 0x4
    lfs f0, -27796(r2)
    addi r8, r29, 0x8
    stfs f3, 0x30(r1)
    lfs f3, 0x8(r31)
    stfs f3, 0x34(r1)
    lfs f3, 0x14(r31)
    stfs f3, 0x38(r1)
    lfs f3, 0x18(r31)
    stfs f3, 0x3c(r1)
    lfs f3, 0x28(r31)
    stfs f3, 0x40(r1)
    stfs f5, 0x44(r1)
    stfs f4, 0x14(r1)
    stfs f4, 0x18(r1)
    stfs f2, 0x1c(r1)
    stfs f1, 0x20(r1)
    stfs f4, 0x24(r1)
    stfs f0, 0x28(r1)
    lfs f1, 0x0(r28)
    lfs f2, 0x4(r28)
    lfs f3, 0x8(r28)
    bl fn_800BD16C
    li r3, 0x2
@L_800D303C:
    lwz r0, 0x64(r1)
    lwz r31, 0x5c(r1)
    lwz r30, 0x58(r1)
    lwz r29, 0x54(r1)
    lwz r28, 0x50(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
