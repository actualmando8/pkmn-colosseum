    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    mr r0, r3
    li r3, 0x0
    stmw r25, 0x54(r1)
    mr r25, r5
    mr r28, r0
    mr r27, r4
    bl fn_800D7BF8
    mr r0, r3
    li r3, 0x2
    mr r30, r0
    bl fn_800D7BF8
    cmplwi r30, 0x0
    mr r31, r3
    beq @L_800D2E34
    cmplwi r31, 0x0
    bne @L_800D2E3C
@L_800D2E34:
    li r3, 0x0
    b @L_800D2F20
@L_800D2E3C:
    lfs f4, -27808(r2)
    li r26, 0x2
    lfs f2, -27804(r2)
    li r29, 0x0
    stfs f4, 0x2c(r1)
    lfs f1, -27800(r2)
    lfs f3, 0x0(r31)
    lfs f0, -27796(r2)
    stfs f3, 0x30(r1)
    lfs f3, 0x8(r31)
    stfs f3, 0x34(r1)
    lfs f3, 0x14(r31)
    stfs f3, 0x38(r1)
    lfs f3, 0x18(r31)
    stfs f3, 0x3c(r1)
    lfs f3, 0x28(r31)
    stfs f3, 0x40(r1)
    lfs f3, 0x2c(r31)
    stfs f3, 0x44(r1)
    stfs f4, 0x14(r1)
    stfs f4, 0x18(r1)
    stfs f2, 0x1c(r1)
    stfs f1, 0x20(r1)
    stfs f4, 0x24(r1)
    stfs f0, 0x28(r1)
    b @L_800D2F14
@L_800D2EA4:
    mr r4, r30
    mr r5, r28
    addi r3, r1, 0x8
    bl fn_800DFF98
    lfs f1, 0x10(r1)
    lfs f0, 0x2c(r31)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_800D2EE0
    lfs f0, -27796(r2)
    li r26, 0x3
    stfs f0, 0x8(r27)
    stfs f0, 0x4(r27)
    stfs f0, 0x0(r27)
    b @L_800D2F08
@L_800D2EE0:
    lfs f1, 0x0(r28)
    mr r3, r30
    lfs f2, 0x4(r28)
    mr r6, r27
    lfs f3, 0x8(r28)
    addi r4, r1, 0x2c
    addi r5, r1, 0x14
    addi r7, r27, 0x4
    addi r8, r27, 0x8
    bl fn_800BD16C
@L_800D2F08:
    addi r29, r29, 0x1
    addi r28, r28, 0xc
    addi r27, r27, 0xc
@L_800D2F14:
    cmplw r29, r25
    blt @L_800D2EA4
    mr r3, r26
@L_800D2F20:
    lmw r25, 0x54(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
