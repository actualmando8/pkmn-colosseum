    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stfd f31, 0xa0(r1)
    psq_st f31, 0xa8(r1), 0, 0
    stfd f30, 0x90(r1)
    psq_st f30, 0x98(r1), 0, 0
    stfd f29, 0x80(r1)
    psq_st f29, 0x88(r1), 0, 0
    stfd f28, 0x70(r1)
    psq_st f28, 0x78(r1), 0, 0
    stw r31, 0x6c(r1)
    stw r30, 0x68(r1)
    stw r29, 0x64(r1)
    mr r30, r4
    lhz r7, 0x32(r3)
    lwz r0, 0x0(r4)
    mr r31, r6
    mulli r7, r7, 0xd4
    lwz r8, 0x2c(r3)
    cmpwi r0, 0x3
    add r7, r8, r7
    beq @L_801D3A50
    bge @L_801D3514
    cmpwi r0, 0x1
    beq @L_801D3530
    bge @L_801D3820
    cmpwi r0, 0x0
    bge @L_801D3524
    b @L_801D3F40
@L_801D3514:
    cmpwi r0, 0x5
    beq @L_801D3D34
    bge @L_801D3F40
    b @L_801D3C34
@L_801D3524:
    mr r3, r7
    bl fn_801D3F7C
    b @L_801D3F40
@L_801D3530:
    lwz r0, 0x0(r7)
    rlwinm r0, r0, 2, 0, 29
    add r3, r7, r0
    lwz r29, 0x4(r3)
    cmpwi r29, 0x0
    bne @L_801D354C
    lwz r29, 0x8(r3)
@L_801D354C:
    slw r29, r29, r5
    bl fn_800E0BE4
    lfs f2, -21660(r2)
    lfs f0, -21664(r2)
    fmadds f29, f2, f1, f0
    bl fn_800E0BE4
    lfs f2, -21656(r2)
    lfs f0, -21676(r2)
    fmadds f30, f2, f1, f0
    bl fn_800E0BE4
    lfs f2, -21648(r2)
    lfs f0, -21652(r2)
    fmadds f31, f2, f1, f0
    fmr f1, f31
    bl fn_801767E0
    fmr f1, f30
    bl fn_80176868
    lfs f1, -21668(r2)
    bl fn_80176758
    bl fn_80177A64
    addi r3, r1, 0x40
    bl fn_801778B4
    rlwinm. r0, r31, 0, 24, 31
    beq @L_801D35BC
    lfs f1, 0x40(r1)
    lfs f0, -21660(r2)
    fadds f0, f1, f0
    b @L_801D35C8
@L_801D35BC:
    lfs f1, 0x40(r1)
    lfs f0, -21660(r2)
    fsubs f0, f1, f0
@L_801D35C8:
    stfs f0, 0x40(r1)
    addi r3, r1, 0x40
    bl fn_801778DC
    rlwinm. r0, r31, 0, 24, 31
    beq @L_801D35E8
    lfs f0, 0x40(r1)
    fadds f0, f0, f29
    b @L_801D35F0
@L_801D35E8:
    lfs f0, 0x40(r1)
    fsubs f0, f0, f29
@L_801D35F0:
    stfs f0, 0x40(r1)
    bl fn_800D37CC
    lis r5, 0x4330
    xoris r3, r3, 0x8000
    stw r3, 0x54(r1)
    xoris r0, r29, 0x8000
    lfd f2, -21576(r2)
    addi r4, r1, 0x40
    stw r5, 0x50(r1)
    li r3, 0x7
    lfd f0, 0x50(r1)
    stw r0, 0x5c(r1)
    fsubs f1, f0, f2
    stw r5, 0x58(r1)
    lfd f0, 0x58(r1)
    fsubs f0, f0, f2
    fdivs f1, f0, f1
    bl fn_80177574
    fmuls f1, f30, f30
    lfs f0, -21668(r2)
    fmadds f4, f29, f29, f1
    fcmpo cr0, f4, f0
    ble @L_801D3694
    frsqrte f1, f4
    lfd f3, -21640(r2)
    lfd f2, -21632(r2)
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f0, f1, f0
    fmul f1, f4, f0
    frsp f1, f1
    b @L_801D3720
@L_801D3694:
    lfd f0, -21624(r2)
    fcmpo cr0, f4, f0
    bge @L_801D36AC
    lis r3, -32696
    lfs f1, -30016(r3)
    b @L_801D3720
@L_801D36AC:
    stfs f4, 0x18(r1)
    lis r0, 0x7f80
    lwz r4, 0x18(r1)
    rlwinm r3, r4, 0, 1, 8
    cmpw r3, r0
    beq @L_801D36D4
    bge @L_801D3704
    cmpwi r3, 0x0
    beq @L_801D36EC
    b @L_801D3704
@L_801D36D4:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D36E4
    li r0, 0x1
    b @L_801D3708
@L_801D36E4:
    li r0, 0x2
    b @L_801D3708
@L_801D36EC:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D36FC
    li r0, 0x5
    b @L_801D3708
@L_801D36FC:
    li r0, 0x3
    b @L_801D3708
@L_801D3704:
    li r0, 0x4
@L_801D3708:
    cmpwi r0, 0x1
    bne @L_801D371C
    lis r3, -32696
    lfs f1, -30016(r3)
    b @L_801D3720
@L_801D371C:
    fmr f1, f4
@L_801D3720:
    fmadds f4, f31, f31, f4
    lfs f0, -21668(r2)
    stfs f1, 0x28(r30)
    fcmpo cr0, f4, f0
    ble @L_801D377C
    frsqrte f1, f4
    lfd f3, -21640(r2)
    lfd f2, -21632(r2)
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f0, f1, f0
    fmul f4, f4, f0
    frsp f4, f4
    b @L_801D3800
@L_801D377C:
    lfd f0, -21624(r2)
    fcmpo cr0, f4, f0
    bge @L_801D3794
    lis r3, -32696
    lfs f4, -30016(r3)
    b @L_801D3800
@L_801D3794:
    stfs f4, 0x14(r1)
    lis r0, 0x7f80
    lwz r4, 0x14(r1)
    rlwinm r3, r4, 0, 1, 8
    cmpw r3, r0
    beq @L_801D37BC
    bge @L_801D37EC
    cmpwi r3, 0x0
    beq @L_801D37D4
    b @L_801D37EC
@L_801D37BC:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D37CC
    li r0, 0x1
    b @L_801D37F0
@L_801D37CC:
    li r0, 0x2
    b @L_801D37F0
@L_801D37D4:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D37E4
    li r0, 0x5
    b @L_801D37F0
@L_801D37E4:
    li r0, 0x3
    b @L_801D37F0
@L_801D37EC:
    li r0, 0x4
@L_801D37F0:
    cmpwi r0, 0x1
    bne @L_801D3800
    lis r3, -32696
    lfs f4, -30016(r3)
@L_801D3800:
    stfs f4, 0x30(r30)
    lfs f2, -21672(r2)
    lfs f1, 0x28(r30)
    lfs f0, 0x30(r30)
    fadds f0, f1, f0
    fmuls f0, f2, f0
    stfs f0, 0x2c(r30)
    b @L_801D3F40
@L_801D3820:
    lwz r0, 0x0(r7)
    rlwinm r0, r0, 2, 0, 29
    add r3, r7, r0
    lwz r29, 0x4(r3)
    cmpwi r29, 0x0
    bne @L_801D383C
    lwz r29, 0x8(r3)
@L_801D383C:
    lfs f28, 0x20(r30)
    slw r29, r29, r5
    bl fn_800E0BE4
    lfs f0, 0x24(r30)
    lfs f30, 0x18(r30)
    fsubs f0, f0, f28
    fmadds f29, f0, f1, f28
    bl fn_800E0BE4
    lfs f0, 0x1c(r30)
    rlwinm. r0, r31, 0, 24, 31
    fsubs f0, f0, f30
    fmadds f30, f0, f1, f30
    beq @L_801D38AC
    lfs f28, 0xc(r30)
    bl fn_800E0BE4
    lfs f2, 0x10(r30)
    lfs f0, 0x14(r30)
    fsubs f2, f2, f28
    lfs f31, 0xc(r30)
    fsubs f0, f0, f28
    fnmsubs f28, f2, f1, f0
    bl fn_800E0BE4
    lfs f2, 0x10(r30)
    lfs f0, 0x14(r30)
    fsubs f2, f2, f31
    fsubs f0, f0, f31
    fnmsubs f31, f2, f1, f0
    b @L_801D38E4
@L_801D38AC:
    lfs f28, 0xc(r30)
    bl fn_800E0BE4
    lfs f2, 0x10(r30)
    lfs f0, 0x14(r30)
    fsubs f2, f2, f28
    lfs f31, 0xc(r30)
    fadds f0, f0, f28
    fmadds f28, f2, f1, f0
    bl fn_800E0BE4
    lfs f2, 0x10(r30)
    lfs f0, 0x14(r30)
    fsubs f2, f2, f31
    fadds f0, f0, f31
    fmadds f31, f2, f1, f0
@L_801D38E4:
    fcmpo cr0, f28, f31
    ble @L_801D38F8
    fmr f0, f28
    fmr f28, f31
    fmr f31, f0
@L_801D38F8:
    fmr f1, f29
    bl fn_801767E0
    fmr f1, f30
    bl fn_80176868
    fmr f1, f28
    bl fn_80176758
    bl fn_80177A64
    bl fn_800D37CC
    lis r4, 0x4330
    xoris r3, r3, 0x8000
    stw r3, 0x5c(r1)
    xoris r0, r29, 0x8000
    lfs f1, -21668(r2)
    fmr f2, f31
    stw r4, 0x58(r1)
    li r3, 0x7
    lfd f5, -21576(r2)
    fmr f3, f1
    lfd f0, 0x58(r1)
    stw r0, 0x54(r1)
    fsubs f4, f0, f5
    stw r4, 0x50(r1)
    lfd f0, 0x50(r1)
    fsubs f0, f0, f5
    fdivs f4, f0, f4
    bl fn_801773F4
    fmuls f1, f30, f30
    lfs f0, -21668(r2)
    fmadds f4, f29, f29, f1
    fcmpo cr0, f4, f0
    ble @L_801D39BC
    frsqrte f1, f4
    lfd f3, -21640(r2)
    lfd f2, -21632(r2)
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f0, f1, f0
    fmul f4, f4, f0
    frsp f4, f4
    b @L_801D3A40
@L_801D39BC:
    lfd f0, -21624(r2)
    fcmpo cr0, f4, f0
    bge @L_801D39D4
    lis r3, -32696
    lfs f4, -30016(r3)
    b @L_801D3A40
@L_801D39D4:
    stfs f4, 0x10(r1)
    lis r0, 0x7f80
    lwz r4, 0x10(r1)
    rlwinm r3, r4, 0, 1, 8
    cmpw r3, r0
    beq @L_801D39FC
    bge @L_801D3A2C
    cmpwi r3, 0x0
    beq @L_801D3A14
    b @L_801D3A2C
@L_801D39FC:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D3A0C
    li r0, 0x1
    b @L_801D3A30
@L_801D3A0C:
    li r0, 0x2
    b @L_801D3A30
@L_801D3A14:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D3A24
    li r0, 0x5
    b @L_801D3A30
@L_801D3A24:
    li r0, 0x3
    b @L_801D3A30
@L_801D3A2C:
    li r0, 0x4
@L_801D3A30:
    cmpwi r0, 0x1
    bne @L_801D3A40
    lis r3, -32696
    lfs f4, -30016(r3)
@L_801D3A40:
    stfs f4, 0x30(r30)
    stfs f4, 0x2c(r30)
    stfs f4, 0x28(r30)
    b @L_801D3F40
@L_801D3A50:
    lwz r0, 0x0(r7)
    rlwinm r0, r0, 2, 0, 29
    add r3, r7, r0
    lwz r29, 0x4(r3)
    cmpwi r29, 0x0
    bne @L_801D3A6C
    lwz r29, 0x8(r3)
@L_801D3A6C:
    lfs f28, 0x20(r30)
    slw r29, r29, r5
    bl fn_800E0BE4
    lfs f0, 0x24(r30)
    lfs f30, 0x18(r30)
    fsubs f0, f0, f28
    fmadds f29, f0, f1, f28
    bl fn_800E0BE4
    lfs f0, 0x1c(r30)
    rlwinm. r0, r31, 0, 24, 31
    fsubs f0, f0, f30
    fmadds f28, f0, f1, f30
    beq @L_801D3AC0
    lfs f30, 0xc(r30)
    bl fn_800E0BE4
    lfs f2, 0x10(r30)
    lfs f0, 0x14(r30)
    fsubs f2, f2, f30
    fsubs f0, f0, f30
    fnmsubs f30, f2, f1, f0
    b @L_801D3ADC
@L_801D3AC0:
    lfs f30, 0xc(r30)
    bl fn_800E0BE4
    lfs f2, 0x10(r30)
    lfs f0, 0x14(r30)
    fsubs f2, f2, f30
    fadds f0, f0, f30
    fmadds f30, f2, f1, f0
@L_801D3ADC:
    fmr f1, f29
    bl fn_801767E0
    fmr f1, f28
    bl fn_80176868
    fmr f1, f30
    bl fn_80176758
    bl fn_80177A64
    addi r3, r1, 0x34
    bl fn_801778B4
    bl fn_800D37CC
    lis r5, 0x4330
    xoris r3, r3, 0x8000
    stw r3, 0x5c(r1)
    xoris r0, r29, 0x8000
    lfd f2, -21576(r2)
    addi r4, r1, 0x34
    stw r5, 0x58(r1)
    li r3, 0x7
    lfd f0, 0x58(r1)
    stw r0, 0x54(r1)
    fsubs f1, f0, f2
    stw r5, 0x50(r1)
    lfd f0, 0x50(r1)
    fsubs f0, f0, f2
    fdivs f1, f0, f1
    bl fn_80177574
    fmuls f1, f28, f28
    lfs f0, -21668(r2)
    fmadds f4, f29, f29, f1
    fcmpo cr0, f4, f0
    ble @L_801D3BA0
    frsqrte f1, f4
    lfd f3, -21640(r2)
    lfd f2, -21632(r2)
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f0, f1, f0
    fmul f4, f4, f0
    frsp f4, f4
    b @L_801D3C24
@L_801D3BA0:
    lfd f0, -21624(r2)
    fcmpo cr0, f4, f0
    bge @L_801D3BB8
    lis r3, -32696
    lfs f4, -30016(r3)
    b @L_801D3C24
@L_801D3BB8:
    stfs f4, 0xc(r1)
    lis r0, 0x7f80
    lwz r4, 0xc(r1)
    rlwinm r3, r4, 0, 1, 8
    cmpw r3, r0
    beq @L_801D3BE0
    bge @L_801D3C10
    cmpwi r3, 0x0
    beq @L_801D3BF8
    b @L_801D3C10
@L_801D3BE0:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D3BF0
    li r0, 0x1
    b @L_801D3C14
@L_801D3BF0:
    li r0, 0x2
    b @L_801D3C14
@L_801D3BF8:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D3C08
    li r0, 0x5
    b @L_801D3C14
@L_801D3C08:
    li r0, 0x3
    b @L_801D3C14
@L_801D3C10:
    li r0, 0x4
@L_801D3C14:
    cmpwi r0, 0x1
    bne @L_801D3C24
    lis r3, -32696
    lfs f4, -30016(r3)
@L_801D3C24:
    stfs f4, 0x30(r30)
    stfs f4, 0x2c(r30)
    stfs f4, 0x28(r30)
    b @L_801D3F40
@L_801D3C34:
    lwz r0, 0x0(r7)
    rlwinm r0, r0, 2, 0, 29
    add r3, r7, r0
    lwz r29, 0x4(r3)
    cmpwi r29, 0x0
    bne @L_801D3C50
    lwz r29, 0x8(r3)
@L_801D3C50:
    rlwinm. r0, r31, 0, 24, 31
    beq @L_801D3C68
    lfs f1, 0x14(r30)
    lfs f0, -21616(r2)
    fsubs f28, f1, f0
    b @L_801D3C74
@L_801D3C68:
    lfs f1, -21616(r2)
    lfs f0, 0x14(r30)
    fadds f28, f1, f0
@L_801D3C74:
    lfs f1, -21612(r2)
    bl fn_801767E0
    lfs f1, -21664(r2)
    bl fn_80176868
    fmr f1, f28
    bl fn_80176758
    bl fn_80177A64
    addi r3, r1, 0x28
    bl fn_801778B4
    bl fn_800D37CC
    lis r5, 0x4330
    xoris r3, r3, 0x8000
    stw r3, 0x5c(r1)
    xoris r0, r29, 0x8000
    lfd f2, -21576(r2)
    addi r4, r1, 0x28
    stw r5, 0x58(r1)
    li r3, 0x7
    lfd f0, 0x58(r1)
    stw r0, 0x54(r1)
    fsubs f1, f0, f2
    stw r5, 0x50(r1)
    lfd f0, 0x50(r1)
    fsubs f0, f0, f2
    fdivs f1, f0, f1
    bl fn_80177574
    lfd f4, -21608(r2)
    lfd f3, -21640(r2)
    frsqrte f1, f4
    lfd f2, -21632(r2)
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f0, f1, f0
    fmul f0, f4, f0
    frsp f0, f0
    stfs f0, 0x30(r30)
    stfs f0, 0x2c(r30)
    stfs f0, 0x28(r30)
    b @L_801D3F40
@L_801D3D34:
    lwz r0, 0x0(r7)
    lwz r4, 0x10(r3)
    rlwinm r0, r0, 2, 0, 29
    add r3, r7, r0
    lwz r29, 0x4(r3)
    cmpwi r29, 0x0
    bne @L_801D3D54
    lwz r29, 0x8(r3)
@L_801D3D54:
    cmpwi r4, 0x1
    slw r29, r29, r5
    beq @L_801D3D90
    bge @L_801D3D78
    cmpwi r4, 0x0
    bge @L_801D3DA8
    cmpwi r4, -0x2
    bge @L_801D3D88
    b @L_801D3DA8
@L_801D3D78:
    cmpwi r4, 0x3
    beq @L_801D3DA0
    bge @L_801D3DA8
    b @L_801D3D98
@L_801D3D88:
    lfs f1, -21600(r2)
    b @L_801D3DAC
@L_801D3D90:
    lfs f1, -21596(r2)
    b @L_801D3DAC
@L_801D3D98:
    lfs f1, -21592(r2)
    b @L_801D3DAC
@L_801D3DA0:
    lfs f1, -21588(r2)
    b @L_801D3DAC
@L_801D3DA8:
    lfs f1, -21676(r2)
@L_801D3DAC:
    lfs f0, -21584(r2)
    lfs f28, 0x18(r30)
    fmuls f29, f0, f1
    bl fn_800E0BE4
    lfs f0, 0x1c(r30)
    rlwinm. r0, r31, 0, 24, 31
    fsubs f0, f0, f28
    fmadds f30, f0, f1, f28
    beq @L_801D3DE0
    lfs f1, 0x14(r30)
    lfs f0, -21580(r2)
    fsubs f28, f1, f0
    b @L_801D3DEC
@L_801D3DE0:
    lfs f1, -21580(r2)
    lfs f0, 0x14(r30)
    fadds f28, f1, f0
@L_801D3DEC:
    fmr f1, f29
    bl fn_801767E0
    fmr f1, f30
    bl fn_80176868
    fmr f1, f28
    bl fn_80176758
    bl fn_80177A64
    addi r3, r1, 0x1c
    bl fn_801778B4
    bl fn_800D37CC
    lis r5, 0x4330
    xoris r3, r3, 0x8000
    stw r3, 0x5c(r1)
    xoris r0, r29, 0x8000
    lfd f2, -21576(r2)
    addi r4, r1, 0x1c
    stw r5, 0x58(r1)
    li r3, 0x7
    lfd f0, 0x58(r1)
    stw r0, 0x54(r1)
    fsubs f1, f0, f2
    stw r5, 0x50(r1)
    lfd f0, 0x50(r1)
    fsubs f0, f0, f2
    fdivs f1, f0, f1
    bl fn_80177574
    fmuls f1, f30, f30
    lfs f0, -21668(r2)
    fmadds f4, f29, f29, f1
    fcmpo cr0, f4, f0
    ble @L_801D3EB0
    frsqrte f1, f4
    lfd f3, -21640(r2)
    lfd f2, -21632(r2)
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f0, f1, f0
    fmul f4, f4, f0
    frsp f4, f4
    b @L_801D3F34
@L_801D3EB0:
    lfd f0, -21624(r2)
    fcmpo cr0, f4, f0
    bge @L_801D3EC8
    lis r3, -32696
    lfs f4, -30016(r3)
    b @L_801D3F34
@L_801D3EC8:
    stfs f4, 0x8(r1)
    lis r0, 0x7f80
    lwz r4, 0x8(r1)
    rlwinm r3, r4, 0, 1, 8
    cmpw r3, r0
    beq @L_801D3EF0
    bge @L_801D3F20
    cmpwi r3, 0x0
    beq @L_801D3F08
    b @L_801D3F20
@L_801D3EF0:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D3F00
    li r0, 0x1
    b @L_801D3F24
@L_801D3F00:
    li r0, 0x2
    b @L_801D3F24
@L_801D3F08:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D3F18
    li r0, 0x5
    b @L_801D3F24
@L_801D3F18:
    li r0, 0x3
    b @L_801D3F24
@L_801D3F20:
    li r0, 0x4
@L_801D3F24:
    cmpwi r0, 0x1
    bne @L_801D3F34
    lis r3, -32696
    lfs f4, -30016(r3)
@L_801D3F34:
    stfs f4, 0x30(r30)
    stfs f4, 0x2c(r30)
    stfs f4, 0x28(r30)
@L_801D3F40:
    psq_l f31, 0xa8(r1), 0, 0
    lfd f31, 0xa0(r1)
    psq_l f30, 0x98(r1), 0, 0
    lfd f30, 0x90(r1)
    psq_l f29, 0x88(r1), 0, 0
    lfd f29, 0x80(r1)
    psq_l f28, 0x78(r1), 0, 0
    lfd f28, 0x70(r1)
    lwz r31, 0x6c(r1)
    lwz r30, 0x68(r1)
    lwz r0, 0xb4(r1)
    lwz r29, 0x64(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
