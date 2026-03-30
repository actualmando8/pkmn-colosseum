    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stfd f31, 0x50(r1)
    psq_st f31, 0x58(r1), 0, 0
    stw r31, 0x4c(r1)
    stw r30, 0x48(r1)
    stw r29, 0x44(r1)
    mr. r29, r3
    lfs f31, -23652(r2)
    mr r30, r4
    mr r31, r5
    beq @L_80199B48
    cmplwi r29, 0x0
    bne @L_80199B3C
    li r3, 0x0
    b @L_80199B4C
@L_80199B3C:
    lbz r0, 0x10(r29)
    rlwinm r3, r0, 0, 28, 31
    b @L_80199B4C
@L_80199B48:
    li r3, 0x0
@L_80199B4C:
    cmplwi r3, 0x0
    beq @L_8019A228
    lfs f2, 0x1c(r29)
    lfd f0, -23648(r2)
    fadds f1, f2, f1
    stfs f1, 0x1c(r29)
    lfs f1, 0x1c(r29)
    fcmpo cr0, f1, f0
    blt @L_8019A228
@L_80199B70:
    cmpwi r3, 0x4
    beq @L_80199FCC
    bge @L_80199B94
    cmpwi r3, 0x0
    beq @L_8019A228
    blt @L_80199B70
    cmpwi r3, 0x3
    bge @L_80199D80
    b @L_80199D74
@L_80199B94:
    cmpwi r3, 0x6
    beq @L_80199BA4
    bge @L_80199B70
    b @L_8019A208
@L_80199BA4:
    lfs f0, 0x1c(r29)
    fadds f0, f0, f31
    stfs f0, 0x1c(r29)
    lbz r0, 0x10(r29)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_80199BE4
    lbz r0, 0x11(r29)
    stb r0, 0x12(r29)
    lbz r0, 0x10(r29)
    rlwinm r0, r0, 0, 26, 24
    stb r0, 0x10(r29)
    lbz r0, 0x10(r29)
    ori r0, r0, 0x80
    stb r0, 0x10(r29)
    lfs f0, 0x24(r29)
    stfs f0, 0x20(r29)
@L_80199BE4:
    cmplwi r31, 0x0
    beq @L_8019A228
    lbz r0, 0x12(r29)
    cmpwi r0, 0x2
    beq @L_80199C7C
    bge @L_80199C08
    cmpwi r0, 0x1
    bge @L_80199C3C
    b @L_80199D58
@L_80199C08:
    cmpwi r0, 0x6
    beq @L_80199C18
    bge @L_80199D58
    b @L_80199CFC
@L_80199C18:
    lbz r0, 0x10(r29)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_8019A228
    lfs f0, 0x20(r29)
    stfs f0, 0x20(r1)
    lbz r0, 0x10(r29)
    rlwinm r0, r0, 0, 25, 31
    stb r0, 0x10(r29)
    b @L_80199D58
@L_80199C3C:
    lhz r3, 0x1a(r29)
    lis r0, 0x4330
    stw r0, 0x30(r1)
    lfd f1, -23632(r2)
    stw r3, 0x34(r1)
    lfs f2, 0x1c(r29)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f1
    fcmpo cr0, f2, f0
    cror eq, gt, eq
    bne @L_80199C70
    lfs f0, 0x24(r29)
    b @L_80199C74
@L_80199C70:
    lfs f0, 0x20(r29)
@L_80199C74:
    stfs f0, 0x20(r1)
    b @L_80199D58
@L_80199C7C:
    lbz r0, 0x10(r29)
    rlwinm. r0, r0, 0, 26, 26
    beq @L_80199CE4
    lbz r0, 0x10(r29)
    rlwinm r0, r0, 0, 27, 25
    stb r0, 0x10(r29)
    lhz r0, 0x1a(r29)
    cmplwi r0, 0x0
    beq @L_80199CD4
    lhz r3, 0x1a(r29)
    lis r0, 0x4330
    stw r0, 0x30(r1)
    lfs f3, 0x24(r29)
    stw r3, 0x34(r1)
    lfs f2, 0x20(r29)
    lfd f1, -23632(r2)
    lfd f0, 0x30(r1)
    fsubs f2, f3, f2
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    stfs f0, 0x28(r29)
    b @L_80199CE4
@L_80199CD4:
    lfs f0, -23652(r2)
    stfs f0, 0x28(r29)
    lfs f0, 0x24(r29)
    stfs f0, 0x20(r29)
@L_80199CE4:
    lfs f2, 0x28(r29)
    lfs f1, 0x1c(r29)
    lfs f0, 0x20(r29)
    fmadds f0, f2, f1, f0
    stfs f0, 0x20(r1)
    b @L_80199D58
@L_80199CFC:
    lhz r0, 0x1a(r29)
    cmplwi r0, 0x0
    beq @L_80199D50
    lhz r3, 0x1a(r29)
    lis r0, 0x4330
    stw r0, 0x30(r1)
    lfd f1, -23632(r2)
    stw r3, 0x34(r1)
    lfd f6, -23640(r2)
    lfd f0, 0x30(r1)
    lfs f2, 0x1c(r29)
    fsub f0, f0, f1
    lfs f3, 0x20(r29)
    lfs f4, 0x24(r29)
    lfs f5, 0x28(r29)
    fdiv f1, f6, f0
    lfs f6, 0x2c(r29)
    frsp f1, f1
    bl fn_801B2560
    stfs f1, 0x20(r1)
    b @L_80199D58
@L_80199D50:
    lfs f0, 0x24(r29)
    stfs f0, 0x20(r1)
@L_80199D58:
    mr r12, r31
    mr r3, r30
    addi r5, r1, 0x20
    lbz r4, 0x13(r29)
    mtctr r12
    bctrl
    b @L_8019A228
@L_80199D74:
    mr r3, r29
    bl fn_8019A24C
    b @L_80199B70
@L_80199D80:
    lbz r0, 0x10(r29)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_80199F18
    cmplwi r31, 0x0
    beq @L_80199F18
    lbz r0, 0x12(r29)
    cmpwi r0, 0x2
    beq @L_80199E24
    bge @L_80199DB0
    cmpwi r0, 0x1
    bge @L_80199DE4
    b @L_80199F00
@L_80199DB0:
    cmpwi r0, 0x6
    beq @L_80199DC0
    bge @L_80199F00
    b @L_80199EA4
@L_80199DC0:
    lbz r0, 0x10(r29)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_80199F18
    lfs f0, 0x20(r29)
    stfs f0, 0x14(r1)
    lbz r0, 0x10(r29)
    rlwinm r0, r0, 0, 25, 31
    stb r0, 0x10(r29)
    b @L_80199F00
@L_80199DE4:
    lhz r3, 0x1a(r29)
    lis r0, 0x4330
    stw r0, 0x30(r1)
    lfd f1, -23632(r2)
    stw r3, 0x34(r1)
    lfs f2, 0x1c(r29)
    lfd f0, 0x30(r1)
    fsubs f0, f0, f1
    fcmpo cr0, f2, f0
    cror eq, gt, eq
    bne @L_80199E18
    lfs f0, 0x24(r29)
    b @L_80199E1C
@L_80199E18:
    lfs f0, 0x20(r29)
@L_80199E1C:
    stfs f0, 0x14(r1)
    b @L_80199F00
@L_80199E24:
    lbz r0, 0x10(r29)
    rlwinm. r0, r0, 0, 26, 26
    beq @L_80199E8C
    lbz r0, 0x10(r29)
    rlwinm r0, r0, 0, 27, 25
    stb r0, 0x10(r29)
    lhz r0, 0x1a(r29)
    cmplwi r0, 0x0
    beq @L_80199E7C
    lhz r3, 0x1a(r29)
    lis r0, 0x4330
    stw r0, 0x30(r1)
    lfs f3, 0x24(r29)
    stw r3, 0x34(r1)
    lfs f2, 0x20(r29)
    lfd f1, -23632(r2)
    lfd f0, 0x30(r1)
    fsubs f2, f3, f2
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    stfs f0, 0x28(r29)
    b @L_80199E8C
@L_80199E7C:
    lfs f0, -23652(r2)
    stfs f0, 0x28(r29)
    lfs f0, 0x24(r29)
    stfs f0, 0x20(r29)
@L_80199E8C:
    lfs f2, 0x28(r29)
    lfs f1, 0x1c(r29)
    lfs f0, 0x20(r29)
    fmadds f0, f2, f1, f0
    stfs f0, 0x14(r1)
    b @L_80199F00
@L_80199EA4:
    lhz r0, 0x1a(r29)
    cmplwi r0, 0x0
    beq @L_80199EF8
    lhz r3, 0x1a(r29)
    lis r0, 0x4330
    stw r0, 0x30(r1)
    lfd f1, -23632(r2)
    stw r3, 0x34(r1)
    lfd f6, -23640(r2)
    lfd f0, 0x30(r1)
    lfs f2, 0x1c(r29)
    fsub f0, f0, f1
    lfs f3, 0x20(r29)
    lfs f4, 0x24(r29)
    lfs f5, 0x28(r29)
    fdiv f1, f6, f0
    lfs f6, 0x2c(r29)
    frsp f1, f1
    bl fn_801B2560
    stfs f1, 0x14(r1)
    b @L_80199F00
@L_80199EF8:
    lfs f0, 0x24(r29)
    stfs f0, 0x14(r1)
@L_80199F00:
    mr r12, r31
    mr r3, r30
    addi r5, r1, 0x14
    lbz r4, 0x13(r29)
    mtctr r12
    bctrl
@L_80199F18:
    cmplwi r29, 0x0
    bne @L_80199F28
    li r0, 0x0
    b @L_80199F30
@L_80199F28:
    lbz r0, 0x10(r29)
    rlwinm r0, r0, 0, 28, 31
@L_80199F30:
    cmplwi r0, 0x3
    beq @L_80199F4C
    lis r4, -32729
    addi r3, r2, -23664
    addi r5, r4, 18276
    li r4, 0x16c
    bl fn_80196E10
@L_80199F4C:
    lwz r4, 0x8(r29)
    lwz r3, 0x4(r29)
    lwz r0, 0xc(r29)
    subf r3, r4, r3
    cmplw r3, r0
    blt @L_80199F6C
    li r3, 0x6
    b @L_80199B70
@L_80199F6C:
    li r5, 0x0
    li r4, 0x0
@L_80199F74:
    lwz r3, 0x4(r29)
    addi r0, r3, 0x1
    stw r0, 0x4(r29)
    lbz r3, 0x0(r3)
    rlwinm. r0, r3, 0, 24, 24
    rlwinm r3, r3, 0, 25, 31
    slw r3, r3, r4
    addi r4, r4, 0x7
    or r5, r5, r3
    bne @L_80199F74
    sth r5, 0x1a(r29)
    cmplwi r29, 0x0
    lbz r0, 0x10(r29)
    ori r0, r0, 0x20
    stb r0, 0x10(r29)
    beq @L_80199FC4
    lbz r0, 0x10(r29)
    rlwinm r0, r0, 0, 24, 27
    ori r0, r0, 0x2
    stb r0, 0x10(r29)
@L_80199FC4:
    li r3, 0x2
    b @L_80199B70
@L_80199FCC:
    lhz r3, 0x1a(r29)
    lis r0, 0x4330
    stw r0, 0x30(r1)
    lfd f2, -23632(r2)
    stw r3, 0x34(r1)
    lfs f0, 0x1c(r29)
    lfd f1, 0x30(r1)
    fsubs f1, f1, f2
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_8019A05C
    lhz r3, 0x1a(r29)
    lis r0, 0x4330
    lhz r5, 0x1a(r29)
    lis r4, 0x4330
    stw r3, 0x3c(r1)
    cmplwi r29, 0x0
    lfd f1, -23632(r2)
    li r3, 0x3
    stw r0, 0x38(r1)
    lfs f2, 0x1c(r29)
    lfd f0, 0x38(r1)
    stw r5, 0x34(r1)
    fsubs f0, f0, f1
    lfd f3, -23632(r2)
    stw r4, 0x30(r1)
    fsubs f0, f2, f0
    lfd f1, 0x30(r1)
    fsubs f31, f1, f3
    stfs f0, 0x1c(r29)
    beq @L_80199B70
    lbz r0, 0x10(r29)
    rlwinm r0, r0, 0, 24, 27
    rlwimi r0, r3, 0, 28, 31
    stb r0, 0x10(r29)
    b @L_80199B70
@L_8019A05C:
    cmplwi r31, 0x0
    beq @L_8019A1E8
    lbz r0, 0x12(r29)
    cmpwi r0, 0x2
    beq @L_8019A0F4
    bge @L_8019A080
    cmpwi r0, 0x1
    bge @L_8019A0B4
    b @L_8019A1D0
@L_8019A080:
    cmpwi r0, 0x6
    beq @L_8019A090
    bge @L_8019A1D0
    b @L_8019A174
@L_8019A090:
    lbz r0, 0x10(r29)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_8019A1E8
    lfs f0, 0x20(r29)
    stfs f0, 0x8(r1)
    lbz r0, 0x10(r29)
    rlwinm r0, r0, 0, 25, 31
    stb r0, 0x10(r29)
    b @L_8019A1D0
@L_8019A0B4:
    lhz r3, 0x1a(r29)
    lis r0, 0x4330
    stw r0, 0x38(r1)
    lfd f1, -23632(r2)
    stw r3, 0x3c(r1)
    lfs f2, 0x1c(r29)
    lfd f0, 0x38(r1)
    fsubs f0, f0, f1
    fcmpo cr0, f2, f0
    cror eq, gt, eq
    bne @L_8019A0E8
    lfs f0, 0x24(r29)
    b @L_8019A0EC
@L_8019A0E8:
    lfs f0, 0x20(r29)
@L_8019A0EC:
    stfs f0, 0x8(r1)
    b @L_8019A1D0
@L_8019A0F4:
    lbz r0, 0x10(r29)
    rlwinm. r0, r0, 0, 26, 26
    beq @L_8019A15C
    lbz r0, 0x10(r29)
    rlwinm r0, r0, 0, 27, 25
    stb r0, 0x10(r29)
    lhz r0, 0x1a(r29)
    cmplwi r0, 0x0
    beq @L_8019A14C
    lhz r3, 0x1a(r29)
    lis r0, 0x4330
    stw r0, 0x38(r1)
    lfs f3, 0x24(r29)
    stw r3, 0x3c(r1)
    lfs f2, 0x20(r29)
    lfd f1, -23632(r2)
    lfd f0, 0x38(r1)
    fsubs f2, f3, f2
    fsubs f0, f0, f1
    fdivs f0, f2, f0
    stfs f0, 0x28(r29)
    b @L_8019A15C
@L_8019A14C:
    lfs f0, -23652(r2)
    stfs f0, 0x28(r29)
    lfs f0, 0x24(r29)
    stfs f0, 0x20(r29)
@L_8019A15C:
    lfs f2, 0x28(r29)
    lfs f1, 0x1c(r29)
    lfs f0, 0x20(r29)
    fmadds f0, f2, f1, f0
    stfs f0, 0x8(r1)
    b @L_8019A1D0
@L_8019A174:
    lhz r0, 0x1a(r29)
    cmplwi r0, 0x0
    beq @L_8019A1C8
    lhz r3, 0x1a(r29)
    lis r0, 0x4330
    stw r0, 0x38(r1)
    lfd f1, -23632(r2)
    stw r3, 0x3c(r1)
    lfd f6, -23640(r2)
    lfd f0, 0x38(r1)
    lfs f2, 0x1c(r29)
    fsub f0, f0, f1
    lfs f3, 0x20(r29)
    lfs f4, 0x24(r29)
    lfs f5, 0x28(r29)
    fdiv f1, f6, f0
    lfs f6, 0x2c(r29)
    frsp f1, f1
    bl fn_801B2560
    stfs f1, 0x8(r1)
    b @L_8019A1D0
@L_8019A1C8:
    lfs f0, 0x24(r29)
    stfs f0, 0x8(r1)
@L_8019A1D0:
    mr r12, r31
    mr r3, r30
    addi r5, r1, 0x8
    lbz r4, 0x13(r29)
    mtctr r12
    bctrl
@L_8019A1E8:
    cmplwi r29, 0x0
    li r3, 0x5
    beq @L_8019A228
    lbz r0, 0x10(r29)
    rlwinm r0, r0, 0, 24, 27
    rlwimi r0, r3, 0, 28, 31
    stb r0, 0x10(r29)
    b @L_8019A228
@L_8019A208:
    cmplwi r29, 0x0
    li r3, 0x4
    beq @L_80199B70
    lbz r0, 0x10(r29)
    rlwinm r0, r0, 0, 24, 27
    rlwimi r0, r3, 0, 28, 31
    stb r0, 0x10(r29)
    b @L_80199B70
@L_8019A228:
    psq_l f31, 0x58(r1), 0, 0
    lwz r0, 0x64(r1)
    lfd f31, 0x50(r1)
    lwz r31, 0x4c(r1)
    lwz r30, 0x48(r1)
    lwz r29, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
