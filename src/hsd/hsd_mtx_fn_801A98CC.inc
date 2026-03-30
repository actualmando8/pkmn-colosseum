    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stfd f30, 0x20(r1)
    psq_st f30, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    lfs f0, -23108(r2)
    lfs f2, 0x10(r3)
    mr r31, r4
    lfs f1, 0x10(r3)
    lfs f3, 0x0(r3)
    fmuls f1, f2, f1
    lfs f2, 0x0(r3)
    lfs f5, 0x20(r3)
    lfs f4, 0x20(r3)
    fmadds f1, f3, f2, f1
    fmadds f8, f5, f4, f1
    fcmpo cr0, f8, f0
    ble @L_801A9980
    frsqrte f7, f8
    lfd f6, -23080(r2)
    lfd f3, -23072(r2)
    lfd f4, -23080(r2)
    fmul f5, f7, f7
    lfd f1, -23072(r2)
    lfd f2, -23080(r2)
    fmul f6, f6, f7
    lfd f0, -23072(r2)
    fnmsub f3, f8, f5, f3
    fmul f7, f6, f3
    fmul f3, f7, f7
    fmul f4, f4, f7
    fnmsub f1, f8, f3, f1
    fmul f7, f4, f1
    fmul f1, f7, f7
    fmul f2, f2, f7
    fnmsub f0, f8, f1, f0
    fmul f7, f2, f0
    fmul f8, f8, f7
    frsp f8, f8
    b @L_801A9A0C
@L_801A9980:
    lfd f0, -23104(r2)
    fcmpo cr0, f8, f0
    bge @L_801A9998
    lis r3, -32696
    lfs f8, -30016(r3)
    b @L_801A9A0C
@L_801A9998:
    stfs f8, 0x10(r1)
    lis r0, 0x7f80
    lwz r3, 0x10(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_801A99C0
    bge @L_801A99F8
    cmpwi r3, 0x0
    beq @L_801A99DC
    b @L_801A99F8
@L_801A99C0:
    lwz r0, 0x10(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A99D4
    li r0, 0x1
    b @L_801A99FC
@L_801A99D4:
    li r0, 0x2
    b @L_801A99FC
@L_801A99DC:
    lwz r0, 0x10(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A99F0
    li r0, 0x5
    b @L_801A99FC
@L_801A99F0:
    li r0, 0x3
    b @L_801A99FC
@L_801A99F8:
    li r0, 0x4
@L_801A99FC:
    cmpwi r0, 0x1
    bne @L_801A9A0C
    lis r3, -32696
    lfs f8, -30016(r3)
@L_801A9A0C:
    lis r3, -32696
    fmr f0, f8
    lfs f1, -30008(r3)
    fcmpo cr0, f0, f1
    blt @L_801A9DB0
    lfs f2, 0x14(r30)
    lfs f1, 0x14(r30)
    lfs f3, 0x4(r30)
    fmuls f1, f2, f1
    lfs f2, 0x4(r30)
    lfs f5, 0x24(r30)
    lfs f4, 0x24(r30)
    fmadds f2, f3, f2, f1
    lfs f1, -23108(r2)
    fmadds f9, f5, f4, f2
    fcmpo cr0, f9, f1
    ble @L_801A9AA8
    frsqrte f8, f9
    lfd f7, -23080(r2)
    lfd f4, -23072(r2)
    lfd f5, -23080(r2)
    fmul f6, f8, f8
    lfd f2, -23072(r2)
    lfd f3, -23080(r2)
    fmul f7, f7, f8
    lfd f1, -23072(r2)
    fnmsub f4, f9, f6, f4
    fmul f8, f7, f4
    fmul f4, f8, f8
    fmul f5, f5, f8
    fnmsub f2, f9, f4, f2
    fmul f8, f5, f2
    fmul f2, f8, f8
    fmul f3, f3, f8
    fnmsub f1, f9, f2, f1
    fmul f8, f3, f1
    fmul f9, f9, f8
    frsp f9, f9
    b @L_801A9B34
@L_801A9AA8:
    lfd f1, -23104(r2)
    fcmpo cr0, f9, f1
    bge @L_801A9AC0
    lis r3, -32696
    lfs f9, -30016(r3)
    b @L_801A9B34
@L_801A9AC0:
    stfs f9, 0xc(r1)
    lis r0, 0x7f80
    lwz r3, 0xc(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_801A9AE8
    bge @L_801A9B20
    cmpwi r3, 0x0
    beq @L_801A9B04
    b @L_801A9B20
@L_801A9AE8:
    lwz r0, 0xc(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A9AFC
    li r0, 0x1
    b @L_801A9B24
@L_801A9AFC:
    li r0, 0x2
    b @L_801A9B24
@L_801A9B04:
    lwz r0, 0xc(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A9B18
    li r0, 0x5
    b @L_801A9B24
@L_801A9B18:
    li r0, 0x3
    b @L_801A9B24
@L_801A9B20:
    li r0, 0x4
@L_801A9B24:
    cmpwi r0, 0x1
    bne @L_801A9B34
    lis r3, -32696
    lfs f9, -30016(r3)
@L_801A9B34:
    lis r3, -32696
    fmr f31, f9
    lfs f1, -30008(r3)
    fcmpo cr0, f31, f1
    blt @L_801A9DB0
    lfs f2, 0x18(r30)
    lfs f1, 0x18(r30)
    lfs f3, 0x8(r30)
    fmuls f1, f2, f1
    lfs f2, 0x8(r30)
    lfs f5, 0x28(r30)
    lfs f4, 0x28(r30)
    fmadds f2, f3, f2, f1
    lfs f1, -23108(r2)
    fmadds f9, f5, f4, f2
    fcmpo cr0, f9, f1
    ble @L_801A9BD0
    frsqrte f8, f9
    lfd f7, -23080(r2)
    lfd f4, -23072(r2)
    lfd f5, -23080(r2)
    fmul f6, f8, f8
    lfd f2, -23072(r2)
    lfd f3, -23080(r2)
    fmul f7, f7, f8
    lfd f1, -23072(r2)
    fnmsub f4, f9, f6, f4
    fmul f8, f7, f4
    fmul f4, f8, f8
    fmul f5, f5, f8
    fnmsub f2, f9, f4, f2
    fmul f8, f5, f2
    fmul f2, f8, f8
    fmul f3, f3, f8
    fnmsub f1, f9, f2, f1
    fmul f8, f3, f1
    fmul f9, f9, f8
    frsp f9, f9
    b @L_801A9C5C
@L_801A9BD0:
    lfd f1, -23104(r2)
    fcmpo cr0, f9, f1
    bge @L_801A9BE8
    lis r3, -32696
    lfs f9, -30016(r3)
    b @L_801A9C5C
@L_801A9BE8:
    stfs f9, 0x8(r1)
    lis r0, 0x7f80
    lwz r3, 0x8(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_801A9C10
    bge @L_801A9C48
    cmpwi r3, 0x0
    beq @L_801A9C2C
    b @L_801A9C48
@L_801A9C10:
    lwz r0, 0x8(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A9C24
    li r0, 0x1
    b @L_801A9C4C
@L_801A9C24:
    li r0, 0x2
    b @L_801A9C4C
@L_801A9C2C:
    lwz r0, 0x8(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A9C40
    li r0, 0x5
    b @L_801A9C4C
@L_801A9C40:
    li r0, 0x3
    b @L_801A9C4C
@L_801A9C48:
    li r0, 0x4
@L_801A9C4C:
    cmpwi r0, 0x1
    bne @L_801A9C5C
    lis r3, -32696
    lfs f9, -30016(r3)
@L_801A9C5C:
    lis r3, -32696
    fmr f30, f9
    lfs f1, -30008(r3)
    fcmpo cr0, f30, f1
    blt @L_801A9DB0
    lfs f1, 0x20(r30)
    lfs f2, -23112(r2)
    fneg f1, f1
    fdivs f1, f1, f0
    fcmpo cr0, f1, f2
    cror eq, gt, eq
    bne @L_801A9C94
    lfs f0, -23064(r2)
    b @L_801A9CB4
@L_801A9C94:
    lfs f0, -23060(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A9CAC
    lfs f0, -23056(r2)
    b @L_801A9CB4
@L_801A9CAC:
    bl fn_800CE2B8
    frsp f0, f1
@L_801A9CB4:
    stfs f0, 0x4(r31)
    lfs f1, 0x4(r31)
    bl fn_800CDBE0
    lis r3, -32696
    frsp f1, f1
    lfs f0, -30008(r3)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A9D64
    lfs f0, 0x28(r30)
    lfs f1, 0x24(r30)
    fdivs f2, f0, f30
    lfs f0, -23108(r2)
    fdivs f1, f1, f31
    fcmpu cr0, f0, f2
    bne @L_801A9D14
    lfs f0, -23108(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A9D0C
    lfs f0, -23064(r2)
    b @L_801A9D1C
@L_801A9D0C:
    lfs f0, -23056(r2)
    b @L_801A9D1C
@L_801A9D14:
    bl fn_800CE2D8
    frsp f0, f1
@L_801A9D1C:
    stfs f0, 0x0(r31)
    lfs f0, -23108(r2)
    lfs f2, 0x0(r30)
    lfs f1, 0x10(r30)
    fcmpu cr0, f0, f2
    bne @L_801A9D54
    lfs f0, -23108(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A9D4C
    lfs f0, -23064(r2)
    b @L_801A9D5C
@L_801A9D4C:
    lfs f0, -23056(r2)
    b @L_801A9D5C
@L_801A9D54:
    bl fn_800CE2D8
    frsp f0, f1
@L_801A9D5C:
    stfs f0, 0x8(r31)
    b @L_801A9DC8
@L_801A9D64:
    lfs f2, 0x14(r30)
    lfs f0, -23108(r2)
    lfs f1, 0x4(r30)
    fcmpu cr0, f0, f2
    bne @L_801A9D98
    lfs f0, -23108(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A9D90
    lfs f0, -23064(r2)
    b @L_801A9DA0
@L_801A9D90:
    lfs f0, -23056(r2)
    b @L_801A9DA0
@L_801A9D98:
    bl fn_800CE2D8
    frsp f0, f1
@L_801A9DA0:
    stfs f0, 0x0(r31)
    lfs f0, -23108(r2)
    stfs f0, 0x8(r31)
    b @L_801A9DC8
@L_801A9DB0:
    lfs f0, -23108(r2)
    lfs f1, -23108(r2)
    stfs f0, 0x0(r31)
    lfs f0, -23108(r2)
    stfs f1, 0x4(r31)
    stfs f0, 0x8(r31)
@L_801A9DC8:
    psq_l f31, 0x38(r1), 0, 0
    lfd f31, 0x30(r1)
    psq_l f30, 0x28(r1), 0, 0
    lfd f30, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r0, 0x44(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
