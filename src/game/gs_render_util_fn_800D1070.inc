    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    stw r0, 0xc4(r1)
    stw r31, 0xbc(r1)
    li r31, 0x0
    stw r30, 0xb8(r1)
    li r30, 0x0
    stw r29, 0xb4(r1)
    stw r28, 0xb0(r1)
    mr r28, r3
    b @L_800D1398
@L_800D109C:
    lwz r0, -23988(r13)
    add r29, r0, r31
    lbz r0, 0x0(r29)
    cmplwi r0, 0x1
    bne @L_800D1390
    lbz r0, 0x4(r29)
    cmplwi r0, 0x1
    bne @L_800D1210
    addi r3, r29, 0x7c
    addi r4, r29, 0x70
    bl fn_800E01D0
    lwz r3, 0xc(r29)
    lfs f1, 0x11c(r29)
    bl fn_80196698
    lwz r3, 0xc(r29)
    bl fn_801966FC
    lwz r3, 0xc(r29)
    addi r4, r29, 0x70
    bl fn_80195904
    lwz r3, 0xc(r29)
    addi r4, r29, 0xf4
    bl fn_801950D0
    lwz r3, 0xc(r29)
    addi r4, r29, 0x100
    bl fn_801959DC
    lis r3, 0x4330
    lbz r0, 0x125(r29)
    stw r28, 0xac(r1)
    extsb r0, r0
    lfd f2, -27896(r2)
    stw r3, 0xa8(r1)
    cmpwi r0, -0x1
    lfs f3, 0x118(r29)
    lfd f0, 0xa8(r1)
    lfs f1, 0x120(r29)
    fsubs f2, f0, f2
    lfs f0, -27920(r2)
    lwz r3, 0x10c(r29)
    fsubs f4, f1, f0
    fmuls f1, f3, f2
    bne @L_800D1150
    lfs f0, 0x11c(r29)
    fsubs f0, f0, f1
    stfs f0, 0x11c(r29)
    b @L_800D1164
@L_800D1150:
    cmpwi r0, 0x1
    bne @L_800D1164
    lfs f0, 0x11c(r29)
    fadds f0, f0, f1
    stfs f0, 0x11c(r29)
@L_800D1164:
    cmpwi r3, 0x1
    beq @L_800D11B8
    bge @L_800D117C
    cmpwi r3, 0x0
    bge @L_800D1188
    b @L_800D1208
@L_800D117C:
    cmpwi r3, 0x3
    bge @L_800D1208
    b @L_800D11D4
@L_800D1188:
    lfs f0, -27916(r2)
    lfs f1, 0x11c(r29)
    fsubs f0, f4, f0
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_800D1208
    li r3, 0x1
    li r0, 0x0
    stb r3, 0x124(r29)
    stb r0, 0x125(r29)
    stfs f0, 0x11c(r29)
    b @L_800D1208
@L_800D11B8:
    lfs f0, 0x11c(r29)
    fcmpo cr0, f0, f4
    cror eq, gt, eq
    bne @L_800D1208
    fsubs f0, f0, f4
    stfs f0, 0x11c(r29)
    b @L_800D1208
@L_800D11D4:
    lfs f1, 0x11c(r29)
    fcmpo cr0, f1, f4
    cror eq, gt, eq
    bne @L_800D11F0
    li r0, -0x1
    stb r0, 0x125(r29)
    b @L_800D1208
@L_800D11F0:
    lfs f0, -27912(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_800D1208
    li r0, 0x1
    stb r0, 0x125(r29)
@L_800D1208:
    li r0, 0x1
    stb r0, 0x2(r29)
@L_800D1210:
    lbz r0, 0x4(r29)
    cmplwi r0, 0x0
    beq @L_800D1258
    lwz r3, 0xc(r29)
    bl fn_80194CC4
    mr r4, r3
    addi r3, r29, 0x94
    bl fn_800E0628
    lwz r3, 0xc(r29)
    addi r4, r29, 0x70
    bl fn_80195904
    lwz r3, 0xc(r29)
    addi r4, r29, 0xf4
    bl fn_801950D0
    lwz r3, 0xc(r29)
    addi r4, r29, 0x100
    bl fn_801959DC
    b @L_800D1388
@L_800D1258:
    lbz r0, 0x1(r29)
    cmplwi r0, 0x1
    bne @L_800D1308
    addi r3, r1, 0x8
    addi r4, r29, 0x70
    addi r5, r29, 0x100
    bl fn_800E0168
    lfs f1, 0x8(r1)
    lfs f0, -27912(r2)
    fcmpo cr0, f1, f0
    ble @L_800D1288
    b @L_800D128C
@L_800D1288:
    fneg f1, f1
@L_800D128C:
    lis r3, -32696
    lfs f2, -30004(r3)
    fcmpo cr0, f1, f2
    bge @L_800D12F0
    lfs f1, 0xc(r1)
    lfs f0, -27912(r2)
    fcmpo cr0, f1, f0
    ble @L_800D12B0
    b @L_800D12B4
@L_800D12B0:
    fneg f1, f1
@L_800D12B4:
    fcmpo cr0, f1, f2
    bge @L_800D12F0
    lfs f1, 0x10(r1)
    lfs f0, -27912(r2)
    fcmpo cr0, f1, f0
    ble @L_800D12D0
    b @L_800D12D4
@L_800D12D0:
    fneg f1, f1
@L_800D12D4:
    fcmpo cr0, f1, f2
    bge @L_800D12F0
    lfs f1, 0x100(r29)
    lfd f0, -27904(r2)
    fadd f0, f1, f0
    frsp f0, f0
    stfs f0, 0x100(r29)
@L_800D12F0:
    addi r3, r29, 0x94
    addi r4, r29, 0x70
    addi r5, r29, 0xf4
    addi r6, r29, 0x100
    bl fn_800E0218
    b @L_800D1388
@L_800D1308:
    lfs f0, 0x88(r29)
    addi r3, r29, 0x94
    fneg f1, f0
    bl fn_800E053C
    lfs f0, 0x8c(r29)
    addi r3, r1, 0x44
    fneg f1, f0
    bl fn_800E0518
    lfs f0, 0x90(r29)
    addi r3, r1, 0x74
    fneg f1, f0
    bl fn_800E04F4
    lfs f1, 0x70(r29)
    addi r3, r1, 0x14
    lfs f2, 0x74(r29)
    lfs f0, 0x78(r29)
    fneg f1, f1
    fneg f2, f2
    fneg f3, f0
    bl fn_800E05C0
    addi r3, r29, 0x94
    addi r5, r1, 0x44
    mr r4, r3
    bl fn_800E0290
    addi r3, r29, 0x94
    addi r5, r1, 0x74
    mr r4, r3
    bl fn_800E0290
    addi r3, r29, 0x94
    addi r5, r1, 0x14
    mr r4, r3
    bl fn_800E0290
@L_800D1388:
    li r0, 0x0
    stb r0, 0x2(r29)
@L_800D1390:
    addi r31, r31, 0x128
    addi r30, r30, 0x1
@L_800D1398:
    lwz r0, -23984(r13)
    cmplw r30, r0
    blt @L_800D109C
    lwz r0, 0xc4(r1)
    lwz r31, 0xbc(r1)
    lwz r30, 0xb8(r1)
    lwz r29, 0xb4(r1)
    lwz r28, 0xb0(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
