    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r27, 0x5c(r1)
    mr r27, r3
    mr r28, r4
    li r31, 0x0
    li r30, 0x0
    li r29, 0x0
    lbz r0, -21548(r13)
    cmplwi r0, 0x0
    beq @L_801D3488
    lbz r0, 0x76(r27)
    extsb. r0, r0
    bge @L_801D3114
    cmplwi r28, 0x0
    beq @L_801D3110
    lwz r0, 0x8(r28)
    rlwinm r0, r0, 0, 24, 24
    cmpwi r0, 0x80
    beq @L_801D3114
@L_801D3110:
    li r31, 0x1
@L_801D3114:
    cmplwi r28, 0x0
    stw r27, -21556(r13)
    stw r28, -21552(r13)
    beq @L_801D337C
    bl fn_801C2D74
    lwz r3, 0x18(r28)
    cmplwi r3, 0x0
    beq @L_801D32F4
    lwz r4, 0x20(r28)
    cmplwi r4, 0x0
    beq @L_801D32F4
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801D32F4
    rlwinm. r0, r31, 0, 24, 31
    lwz r30, 0x24(r27)
    li r29, 0x0
    beq @L_801D3160
    li r29, 0x4
@L_801D3160:
    lwz r3, 0x18(r28)
    li r5, 0x0
    lwz r4, 0x20(r28)
    li r6, 0x0
    bl fn_80176C78
    lwz r0, 0x8(r28)
    rlwinm. r0, r0, 0, 29, 29
    beq @L_801D320C
    addi r3, r1, 0x44
    bl fn_800E0204
    addi r3, r1, 0x38
    bl fn_800E0204
    lbz r0, 0x76(r27)
    extsb. r0, r0
    bge @L_801D31B8
    lwz r0, 0x8(r28)
    rlwinm r3, r0, 0, 6, 6
    subis r0, r3, 0x200
    cmplwi r0, 0x0
    bne @L_801D31B8
    lfs f0, -21680(r2)
    stfs f0, 0x3c(r1)
@L_801D31B8:
    lwz r0, 0x8(r28)
    rlwinm r3, r0, 0, 8, 8
    subis r0, r3, 0x80
    cmplwi r0, 0x0
    bne @L_801D31F4
    addi r3, r1, 0x2c
    bl fn_801C3B80
    lwz r0, 0x8(r28)
    rlwinm r3, r0, 0, 7, 7
    subis r0, r3, 0x100
    cmplwi r0, 0x0
    bne @L_801D32C0
    lfs f0, -21676(r2)
    stfs f0, 0x30(r1)
    b @L_801D32C0
@L_801D31F4:
    lfs f1, -21676(r2)
    addi r3, r1, 0x2c
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    b @L_801D32C0
@L_801D320C:
    mr r3, r30
    addi r4, r1, 0x44
    bl fn_800E3D98
    lwz r0, 0x8(r28)
    rlwinm. r0, r0, 0, 17, 17
    beq @L_801D3254
    mr r3, r30
    bl fn_800E3C5C
    mr r5, r3
    lfs f1, -21672(r2)
    addi r3, r1, 0x20
    addi r4, r5, 0x10
    addi r5, r5, 0x1c
    bl fn_800E090C
    addi r3, r1, 0x44
    addi r5, r1, 0x20
    mr r4, r3
    bl fn_800E019C
@L_801D3254:
    lhz r0, 0x2e(r28)
    cmplwi r0, 0x2
    bne @L_801D329C
    lbz r0, 0x18(r27)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x2
    bne @L_801D329C
    mr r3, r30
    bl fn_800E6DC0
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D329C
    mr r3, r30
    addi r4, r1, 0x14
    bl fn_800E7290
    lfs f1, 0x48(r1)
    lfs f0, 0x18(r1)
    fadds f0, f1, f0
    stfs f0, 0x48(r1)
@L_801D329C:
    mr r3, r30
    addi r4, r1, 0x38
    bl fn_800E3D6C
    mr r3, r27
    bl fn_801DABAC
    fmr f2, f1
    addi r3, r1, 0x2c
    fmr f3, f2
    bl fn_800E01F4
@L_801D32C0:
    mr r3, r29
    bl fn_801765F4
    bl fn_80176F68
    addi r3, r1, 0x44
    bl fn_80176658
    addi r3, r1, 0x38
    bl fn_8017662C
    addi r3, r1, 0x2c
    bl fn_80176600
    li r3, 0x0
    bl fn_800D1070
    bl fn_80177A64
    b @L_801D3488
@L_801D32F4:
    lwz r3, 0x8(r28)
    rlwinm. r0, r3, 0, 10, 10
    beq @L_801D3308
    ori r30, r30, 0x2
    b @L_801D3348
@L_801D3308:
    rlwinm. r0, r3, 0, 22, 22
    beq @L_801D3318
    ori r30, r30, 0x4
    b @L_801D3348
@L_801D3318:
    rlwinm. r0, r3, 0, 9, 9
    beq @L_801D3348
    lwz r3, 0x24(r27)
    addi r4, r1, 0x8
    bl fn_800E3D98
    lfs f1, 0x10(r1)
    lfs f0, -21668(r2)
    fcmpo cr0, f1, f0
    bge @L_801D3344
    ori r30, r30, 0x4
    b @L_801D3348
@L_801D3344:
    ori r30, r30, 0x8
@L_801D3348:
    lwz r3, 0x8(r28)
    rlwinm. r0, r3, 0, 21, 21
    beq @L_801D335C
    ori r30, r30, 0x20
    b @L_801D3380
@L_801D335C:
    rlwinm. r0, r3, 0, 20, 20
    beq @L_801D336C
    ori r30, r30, 0x40
    b @L_801D3380
@L_801D336C:
    rlwinm. r0, r3, 0, 19, 19
    beq @L_801D3380
    ori r30, r30, 0x80
    b @L_801D3380
@L_801D337C:
    li r30, 0x4
@L_801D3380:
    lbz r0, 0x75(r27)
    cmplwi r0, 0x0
    bne @L_801D33B8
    mr r3, r27
    bl fn_801C2AE8
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_801D33AC
    rlwinm r30, r30, 0, 0, 26
    ori r30, r30, 0x10
    b @L_801D33B8
@L_801D33AC:
    ble @L_801D33B8
    rlwinm r30, r30, 0, 0, 26
    ori r30, r30, 0x1
@L_801D33B8:
    li r3, 0x7
    bl fn_80177A44
    bl fn_80176F68
    lwz r3, -21556(r13)
    lwz r6, -21552(r13)
    lhz r0, 0x32(r3)
    cmplwi r6, 0x0
    lwz r5, 0x2c(r3)
    mulli r0, r0, 0xd4
    lwz r3, 0x0(r27)
    lwz r4, 0x4(r27)
    add r5, r5, r0
    beq @L_801D3400
    lbz r0, 0x17(r6)
    rlwinm r0, r0, 2, 0, 29
    add r5, r5, r0
    lwz r5, 0x4c(r5)
    b @L_801D3404
@L_801D3400:
    lwz r5, 0x54(r5)
@L_801D3404:
    bl fn_801779B0
    bl fn_8017865C
    lwz r3, 0x0(r27)
    li r5, -0x1
    lwz r4, 0x4(r27)
    bl fn_801779B0
    lbz r0, 0x75(r27)
    cmplwi r0, 0x0
    bne @L_801D342C
    li r29, 0x1
@L_801D342C:
    lis r4, -32698
    mr r3, r27
    addi r5, r4, 29600
    mr r4, r30
    bl fn_801D4FB8
    lis r4, -32698
    mr r3, r27
    addi r5, r4, 29600
    mr r4, r28
    bl fn_801D4DA0
    lis r4, -32698
    mr r3, r27
    addi r4, r4, 29600
    mr r5, r29
    mr r6, r31
    bl fn_801D349C
    lis r4, -32698
    mr r3, r27
    addi r4, r4, 29600
    mr r5, r30
    mr r6, r29
    bl fn_801D44C4
    bl fn_80177A64
@L_801D3488:
    lmw r27, 0x5c(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
