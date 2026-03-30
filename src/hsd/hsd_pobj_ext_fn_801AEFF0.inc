    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr. r31, r4
    stw r30, 0x18(r1)
    li r30, 0x0
    stw r29, 0x14(r1)
    mr r29, r3
    bne @L_801AF028
    addi r3, r2, -22864
    li r4, 0x29e
    addi r5, r2, -22840
    bl fn_80196E10
@L_801AF028:
    mr r4, r29
    b @L_801AF048
@L_801AF030:
    lwz r0, 0x4(r4)
    rlwinm r3, r0, 0, 1, 3
    subis r0, r3, 0x2000
    cmplwi r0, 0x0
    beq @L_801AF050
    lwz r4, 0x0(r4)
@L_801AF048:
    cmplwi r4, 0x0
    bne @L_801AF030
@L_801AF050:
    cmplwi r4, 0x0
    beq @L_801AF208
    mr r4, r29
    b @L_801AF1F0
@L_801AF060:
    lwz r0, 0x4(r4)
    rlwinm r3, r0, 0, 1, 3
    subis r0, r3, 0x2000
    cmplwi r0, 0x0
    bne @L_801AF1EC
    lwz r0, 0x4(r4)
    rlwinm r0, r0, 0, 4, 31
    cmplwi r0, 0xc
    bgt @L_801AF1EC
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -12908
    lwzx r3, r3, r0
    mtctr r3
    bctr
    lfs f1, 0x1c(r31)
    lfs f0, 0x8(r4)
    fcmpo cr0, f1, f0
    bge @L_801AF1E8
    lfs f0, 0x8(r4)
    stfs f0, 0x1c(r31)
    b @L_801AF1E8
    lfs f1, 0x1c(r31)
    lfs f0, 0x8(r4)
    fcmpo cr0, f1, f0
    ble @L_801AF1E8
    lfs f0, 0x8(r4)
    stfs f0, 0x1c(r31)
    b @L_801AF1E8
    lfs f1, 0x20(r31)
    lfs f0, 0x8(r4)
    fcmpo cr0, f1, f0
    bge @L_801AF1E8
    lfs f0, 0x8(r4)
    stfs f0, 0x20(r31)
    b @L_801AF1E8
    lfs f1, 0x20(r31)
    lfs f0, 0x8(r4)
    fcmpo cr0, f1, f0
    ble @L_801AF1E8
    lfs f0, 0x8(r4)
    stfs f0, 0x20(r31)
    b @L_801AF1E8
    lfs f1, 0x24(r31)
    lfs f0, 0x8(r4)
    fcmpo cr0, f1, f0
    bge @L_801AF1E8
    lfs f0, 0x8(r4)
    stfs f0, 0x24(r31)
    b @L_801AF1E8
    lfs f1, 0x24(r31)
    lfs f0, 0x8(r4)
    fcmpo cr0, f1, f0
    ble @L_801AF1E8
    lfs f0, 0x8(r4)
    stfs f0, 0x24(r31)
    b @L_801AF1E8
    lfs f1, 0x38(r31)
    lfs f0, 0x8(r4)
    fcmpo cr0, f1, f0
    bge @L_801AF1E8
    lfs f0, 0x8(r4)
    stfs f0, 0x38(r31)
    b @L_801AF1E8
    lfs f1, 0x38(r31)
    lfs f0, 0x8(r4)
    fcmpo cr0, f1, f0
    ble @L_801AF1E8
    lfs f0, 0x8(r4)
    stfs f0, 0x38(r31)
    b @L_801AF1E8
    lfs f1, 0x3c(r31)
    lfs f0, 0x8(r4)
    fcmpo cr0, f1, f0
    bge @L_801AF1E8
    lfs f0, 0x8(r4)
    stfs f0, 0x3c(r31)
    b @L_801AF1E8
    lfs f1, 0x3c(r31)
    lfs f0, 0x8(r4)
    fcmpo cr0, f1, f0
    ble @L_801AF1E8
    lfs f0, 0x8(r4)
    stfs f0, 0x3c(r31)
    b @L_801AF1E8
    lfs f1, 0x3c(r31)
    lfs f0, 0x8(r4)
    fcmpo cr0, f1, f0
    bge @L_801AF1E8
    lfs f0, 0x8(r4)
    stfs f0, 0x3c(r31)
    b @L_801AF1E8
    lfs f1, 0x3c(r31)
    lfs f0, 0x8(r4)
    fcmpo cr0, f1, f0
    ble @L_801AF1E8
    lfs f0, 0x8(r4)
    stfs f0, 0x3c(r31)
@L_801AF1E8:
    li r30, 0x1
@L_801AF1EC:
    lwz r4, 0x0(r4)
@L_801AF1F0:
    cmplwi r4, 0x0
    bne @L_801AF060
    cmpwi r30, 0x0
    beq @L_801AF208
    mr r3, r31
    bl fn_801A3600
@L_801AF208:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
