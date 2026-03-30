    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stfd f30, 0x20(r1)
    psq_st f30, 0x28(r1), 0, 0
    stmw r30, 0x18(r1)
    lfs f0, -20424(r2)
    mr r30, r3
    lfd f30, -20416(r2)
    lis r31, 0x4330
    stfs f0, 0x4(r3)
    lfs f31, -20420(r2)
    b @L_80265E10
@L_80265DEC:
    bl fn_800F0308
    bl fn_800D3088
    stw r3, 0xc(r1)
    lfs f0, 0x4(r30)
    stw r31, 0x8(r1)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f30
    fadds f0, f0, f1
    stfs f0, 0x4(r30)
@L_80265E10:
    lfs f0, 0x8(r30)
    lfs f1, 0x4(r30)
    fmuls f0, f31, f0
    fcmpo cr0, f1, f0
    blt @L_80265DEC
    li r0, 0x1
    stb r0, 0x0(r30)
@L_80265E2C:
    bl fn_800F0308
    b @L_80265E2C
