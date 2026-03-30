    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    stw r0, 0xc4(r1)
    stw r31, 0xbc(r1)
    stw r30, 0xb8(r1)
    lwz r30, -23980(r13)
    cmplwi r30, 0x0
    beq @L_800D256C
    lbz r4, 0x4(r30)
    cmplwi r4, 0x0
    beq @L_800D2324
    lwz r31, 0xc(r30)
    cmplwi r31, 0x0
    bne @L_800D2290
    addi r3, r2, -27868
    li r4, 0x1ae
    addi r5, r2, -27860
    bl fn_80196E10
@L_800D2290:
    lwz r0, 0x8(r31)
    addi r4, r1, 0x14
    addi r5, r1, 0x10
    rlwinm r0, r0, 0, 31, 29
    stw r0, 0x8(r31)
    lwz r3, 0xc(r30)
    bl fn_8019431C
    lwz r3, 0xc(r30)
    bl fn_801944F8
    stfs f1, 0xc(r1)
    lwz r3, 0xc(r30)
    bl fn_801944D0
    fmr f0, f1
    lfs f1, 0x14(r1)
    lfs f2, 0x10(r1)
    lfs f3, 0xc(r1)
    frsp f4, f0
    stfs f0, 0x8(r1)
    bl fn_800D9BD0
    lwz r3, 0xc(r30)
    bl fn_80194CC4
    mr r4, r3
    addi r3, r30, 0x94
    bl fn_800E0628
    bl fn_800D834C
    addi r3, r30, 0x94
    bl fn_800D7FE4
    lwz r3, 0xc(r30)
    addi r4, r30, 0x70
    bl fn_80195904
    lwz r3, 0xc(r30)
    addi r4, r30, 0xf4
    bl fn_801950D0
    lwz r3, 0xc(r30)
    addi r4, r30, 0x100
    bl fn_801959DC
    b @L_800D256C
@L_800D2324:
    lwz r3, 0xc(r30)
    lbz r0, 0x50(r3)
    cmplwi r0, 0x1
    bne @L_800D2540
    lbz r0, 0x2(r30)
    cmplwi r0, 0x0
    beq @L_800D24B8
    cmplwi r4, 0x0
    beq @L_800D2380
    bl fn_80194CC4
    mr r4, r3
    addi r3, r30, 0x94
    bl fn_800E0628
    lwz r3, 0xc(r30)
    addi r4, r30, 0x70
    bl fn_80195904
    lwz r3, 0xc(r30)
    addi r4, r30, 0xf4
    bl fn_801950D0
    lwz r3, 0xc(r30)
    addi r4, r30, 0x100
    bl fn_801959DC
    b @L_800D24B0
@L_800D2380:
    lbz r0, 0x1(r30)
    cmplwi r0, 0x1
    bne @L_800D2430
    addi r3, r1, 0x18
    addi r4, r30, 0x70
    addi r5, r30, 0x100
    bl fn_800E0168
    lfs f1, 0x18(r1)
    lfs f0, -27912(r2)
    fcmpo cr0, f1, f0
    ble @L_800D23B0
    b @L_800D23B4
@L_800D23B0:
    fneg f1, f1
@L_800D23B4:
    lis r3, -32696
    lfs f2, -30004(r3)
    fcmpo cr0, f1, f2
    bge @L_800D2418
    lfs f1, 0x1c(r1)
    lfs f0, -27912(r2)
    fcmpo cr0, f1, f0
    ble @L_800D23D8
    b @L_800D23DC
@L_800D23D8:
    fneg f1, f1
@L_800D23DC:
    fcmpo cr0, f1, f2
    bge @L_800D2418
    lfs f1, 0x20(r1)
    lfs f0, -27912(r2)
    fcmpo cr0, f1, f0
    ble @L_800D23F8
    b @L_800D23FC
@L_800D23F8:
    fneg f1, f1
@L_800D23FC:
    fcmpo cr0, f1, f2
    bge @L_800D2418
    lfs f1, 0x100(r30)
    lfd f0, -27904(r2)
    fadd f0, f1, f0
    frsp f0, f0
    stfs f0, 0x100(r30)
@L_800D2418:
    addi r3, r30, 0x94
    addi r4, r30, 0x70
    addi r5, r30, 0xf4
    addi r6, r30, 0x100
    bl fn_800E0218
    b @L_800D24B0
@L_800D2430:
    lfs f0, 0x88(r30)
    addi r3, r30, 0x94
    fneg f1, f0
    bl fn_800E053C
    lfs f0, 0x8c(r30)
    addi r3, r1, 0x54
    fneg f1, f0
    bl fn_800E0518
    lfs f0, 0x90(r30)
    addi r3, r1, 0x84
    fneg f1, f0
    bl fn_800E04F4
    lfs f1, 0x70(r30)
    addi r3, r1, 0x24
    lfs f2, 0x74(r30)
    lfs f0, 0x78(r30)
    fneg f1, f1
    fneg f2, f2
    fneg f3, f0
    bl fn_800E05C0
    addi r3, r30, 0x94
    addi r5, r1, 0x54
    mr r4, r3
    bl fn_800E0290
    addi r3, r30, 0x94
    addi r5, r1, 0x84
    mr r4, r3
    bl fn_800E0290
    addi r3, r30, 0x94
    addi r5, r1, 0x24
    mr r4, r3
    bl fn_800E0290
@L_800D24B0:
    li r0, 0x0
    stb r0, 0x2(r30)
@L_800D24B8:
    lwz r31, 0xc(r30)
    cmplwi r31, 0x0
    bne @L_800D24D4
    addi r3, r2, -27868
    li r4, 0x1a2
    addi r5, r2, -27860
    bl fn_80196E10
@L_800D24D4:
    lwz r0, 0x8(r31)
    addi r3, r30, 0x94
    addi r4, r31, 0x54
    oris r0, r0, 0x8000
    ori r0, r0, 0x2
    stw r0, 0x8(r31)
    bl fn_800A2D64
    lwz r3, 0xc(r30)
    addi r4, r1, 0x14
    addi r5, r1, 0x10
    bl fn_8019431C
    lwz r3, 0xc(r30)
    bl fn_801944F8
    stfs f1, 0xc(r1)
    lwz r3, 0xc(r30)
    bl fn_801944D0
    fmr f0, f1
    lfs f1, 0x14(r1)
    lfs f2, 0x10(r1)
    lfs f3, 0xc(r1)
    frsp f4, f0
    stfs f0, 0x8(r1)
    bl fn_800D9BD0
    bl fn_800D834C
    addi r3, r30, 0x94
    bl fn_800D7FE4
    b @L_800D256C
@L_800D2540:
    addi r4, r1, 0x10
    addi r5, r1, 0x8
    addi r6, r1, 0x14
    addi r7, r1, 0xc
    bl fn_801942C0
    lfs f1, 0x14(r1)
    lfs f2, 0x10(r1)
    lfs f3, 0xc(r1)
    lfs f4, 0x8(r1)
    bl fn_800D9B58
    bl fn_800D834C
@L_800D256C:
    lwz r0, 0xc4(r1)
    lwz r31, 0xbc(r1)
    lwz r30, 0xb8(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
