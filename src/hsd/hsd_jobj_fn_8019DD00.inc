    nofralloc
    stwu r1, -0xd0(r1)
    mflr r0
    stw r0, 0xd4(r1)
    stfd f31, 0xc0(r1)
    psq_st f31, 0xc8(r1), 0, 0
    stmw r27, 0xac(r1)
    lis r4, lbl_80274AC4@ha
    mr r30, r3
    addi r5, r4, lbl_80274AC4@l
    lfs f31, lbl_8047DB30(r2)
    lwz r4, 0x0(r5)
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r4, 0x68(r1)
    lwz r31, 0x10(r30)
    stw r3, 0x6c(r1)
    stw r0, 0x70(r1)
    b @L_8019DD64
    @L_8019DD48:
    lwz r0, 0x14(r31)
    rlwinm r3, r0, 0, 9, 10
    subis r0, r3, 0x60
    cmplwi r0, 0x0
    bne @L_8019DD60
    b @L_8019DD70
    @L_8019DD60:
    lwz r31, 0x8(r31)
    @L_8019DD64:
    cmplwi r31, 0x0
    bne @L_8019DD48
    li r31, 0x0
    @L_8019DD70:
    cmplwi r31, 0x0
    bne @L_8019DD88
    la r3, lbl_8047DB20(r2)
    li r4, 0x82d
    la r5, lbl_8047DB68(r2)
    bl fn_80196E10
    @L_8019DD88:
    lwz r3, 0x80(r31)
    lis r4, 0x1000
    li r5, 0x1
    bl fn_801B00E0
    cmplwi r3, 0x0
    beq @L_8019DDA4
    b @L_8019DDA8
    @L_8019DDA4:
    li r31, 0x0
    @L_8019DDA8:
    cmplwi r31, 0x0
    beq @L_8019E444
    lwz r0, 0xc(r30)
    cmplwi r0, 0x0
    bne @L_8019DDC0
    b @L_8019E444
    @L_8019DDC0:
    lwz r0, 0x74(r30)
    cmplwi r0, 0x0
    beq @L_8019DDE8
    lwz r5, 0x74(r30)
    lwz r4, 0x0(r5)
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r4, 0x68(r1)
    stw r3, 0x6c(r1)
    stw r0, 0x70(r1)
    @L_8019DDE8:
    lwz r5, 0xc(r30)
    addi r3, r1, 0x44
    addi r4, r1, 0x44
    addi r5, r5, 0x44
    lfs f0, 0xc(r5)
    stfs f0, 0x50(r1)
    lfs f0, 0x1c(r5)
    stfs f0, 0x54(r1)
    lfs f0, 0x2c(r5)
    stfs f0, 0x58(r1)
    lwz r5, 0xc(r30)
    lfsu f0, 0x44(r5)
    stfs f0, 0x44(r1)
    lfs f0, 0x10(r5)
    stfs f0, 0x48(r1)
    lfs f0, 0x20(r5)
    stfs f0, 0x4c(r1)
    bl fn_800A3B7C
    lfs f3, lbl_8047DB44(r2)
    lfs f2, lbl_8047DB30(r2)
    fadds f1, f3, f1
    lfs f0, lbl_8047DB48(r2)
    fdivs f8, f2, f1
    fcmpo cr0, f8, f0
    ble @L_8019DEA4
    frsqrte f7, f8
    lfd f6, lbl_8047DB50(r2)
    lfd f3, lbl_8047DB58(r2)
    lfd f4, lbl_8047DB50(r2)
    fmul f5, f7, f7
    lfd f1, lbl_8047DB58(r2)
    lfd f2, lbl_8047DB50(r2)
    fmul f6, f6, f7
    lfd f0, lbl_8047DB58(r2)
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
    b @L_8019DF30
    @L_8019DEA4:
    lfd f0, lbl_8047DB60(r2)
    fcmpo cr0, f8, f0
    bge @L_8019DEBC
    lis r3, lbl_80478AC0@ha
    lfs f8, lbl_80478AC0@l(r3)
    b @L_8019DF30
    @L_8019DEBC:
    stfs f8, 0x10(r1)
    lis r0, 0x7f80
    lwz r3, 0x10(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_8019DEE4
    bge @L_8019DF1C
    cmpwi r3, 0x0
    beq @L_8019DF00
    b @L_8019DF1C
    @L_8019DEE4:
    lwz r0, 0x10(r1)
    clrlwi. r0, r0, 9
    beq @L_8019DEF8
    li r0, 0x1
    b @L_8019DF20
    @L_8019DEF8:
    li r0, 0x2
    b @L_8019DF20
    @L_8019DF00:
    lwz r0, 0x10(r1)
    clrlwi. r0, r0, 9
    beq @L_8019DF14
    li r0, 0x5
    b @L_8019DF20
    @L_8019DF14:
    li r0, 0x3
    b @L_8019DF20
    @L_8019DF1C:
    li r0, 0x4
    @L_8019DF20:
    cmpwi r0, 0x1
    bne @L_8019DF30
    lis r3, lbl_80478AC0@ha
    lfs f8, lbl_80478AC0@l(r3)
    @L_8019DF30:
    fmr f1, f8
    addi r3, r1, 0x44
    addi r4, r1, 0x44
    bl fn_800A3AC0
    lwz r3, 0xc(r30)
    lwz r0, 0x74(r3)
    cmplwi r0, 0x0
    beq @L_8019DF5C
    lwz r3, 0xc(r30)
    lwz r3, 0x74(r3)
    lfs f31, 0x0(r3)
    @L_8019DF5C:
    lwz r3, 0xc(r30)
    lis r4, 0x4000
    li r5, 0x0
    lwz r3, 0x80(r3)
    bl fn_801B00E0
    mr. r29, r3
    bne @L_8019DF88
    la r3, lbl_8047DB20(r2)
    li r4, 0x905
    la r5, lbl_8047DB6C(r2)
    bl fn_80196E10
    @L_8019DF88:
    lfs f0, 0x8(r29)
    addi r3, r1, 0x44
    addi r4, r1, 0x44
    fmuls f1, f0, f31
    bl fn_800A3AC0
    addi r3, r1, 0x50
    addi r4, r1, 0x44
    addi r5, r1, 0x5c
    bl fn_800A3A78
    addi r3, r31, 0x38
    addi r4, r1, 0x5c
    addi r5, r1, 0x44
    bl fn_800A3A9C
    addi r3, r1, 0x44
    addi r4, r1, 0x44
    bl fn_800A3B7C
    lfs f3, lbl_8047DB44(r2)
    lfs f2, lbl_8047DB30(r2)
    fadds f1, f3, f1
    lfs f0, lbl_8047DB48(r2)
    fdivs f1, f2, f1
    fcmpo cr0, f1, f0
    ble @L_8019E03C
    frsqrte f8, f1
    lfd f7, lbl_8047DB50(r2)
    lfd f4, lbl_8047DB58(r2)
    lfd f5, lbl_8047DB50(r2)
    fmul f6, f8, f8
    lfd f2, lbl_8047DB58(r2)
    lfd f3, lbl_8047DB50(r2)
    fmul f7, f7, f8
    lfd f0, lbl_8047DB58(r2)
    fnmsub f4, f1, f6, f4
    fmul f8, f7, f4
    fmul f4, f8, f8
    fmul f5, f5, f8
    fnmsub f2, f1, f4, f2
    fmul f8, f5, f2
    fmul f2, f8, f8
    fmul f3, f3, f8
    fnmsub f0, f1, f2, f0
    fmul f8, f3, f0
    fmul f1, f1, f8
    frsp f1, f1
    b @L_8019E0C8
    @L_8019E03C:
    lfd f0, lbl_8047DB60(r2)
    fcmpo cr0, f1, f0
    bge @L_8019E054
    lis r3, lbl_80478AC0@ha
    lfs f1, lbl_80478AC0@l(r3)
    b @L_8019E0C8
    @L_8019E054:
    stfs f1, 0xc(r1)
    lis r0, 0x7f80
    lwz r3, 0xc(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_8019E07C
    bge @L_8019E0B4
    cmpwi r3, 0x0
    beq @L_8019E098
    b @L_8019E0B4
    @L_8019E07C:
    lwz r0, 0xc(r1)
    clrlwi. r0, r0, 9
    beq @L_8019E090
    li r0, 0x1
    b @L_8019E0B8
    @L_8019E090:
    li r0, 0x2
    b @L_8019E0B8
    @L_8019E098:
    lwz r0, 0xc(r1)
    clrlwi. r0, r0, 9
    beq @L_8019E0AC
    li r0, 0x5
    b @L_8019E0B8
    @L_8019E0AC:
    li r0, 0x3
    b @L_8019E0B8
    @L_8019E0B4:
    li r0, 0x4
    @L_8019E0B8:
    cmpwi r0, 0x1
    bne @L_8019E0C8
    lis r3, lbl_80478AC0@ha
    lfs f1, lbl_80478AC0@l(r3)
    @L_8019E0C8:
    addi r3, r1, 0x44
    addi r4, r1, 0x44
    bl fn_800A3AC0
    lwz r3, 0x80(r30)
    lis r4, 0x2000
    li r5, 0x5
    bl fn_801B00E0
    mr r0, r3
    lwz r3, 0x80(r30)
    mr r31, r0
    lis r4, 0x2000
    li r5, 0x6
    bl fn_801B00E0
    cmplwi r31, 0x0
    mr r29, r3
    bne @L_8019E110
    cmplwi r29, 0x0
    beq @L_8019E240
    @L_8019E110:
    lwz r3, 0x80(r30)
    li r28, 0x0
    lis r4, 0x4000
    li r5, 0x0
    bl fn_801B00E0
    mr. r27, r3
    bne @L_8019E13C
    la r3, lbl_8047DB20(r2)
    li r4, 0x927
    la r5, lbl_8047DB6C(r2)
    bl fn_80196E10
    @L_8019E13C:
    lwz r5, 0xc(r30)
    addi r3, r1, 0x20
    lwz r0, 0x4(r27)
    addi r4, r1, 0x20
    lfsu f0, 0x44(r5)
    extrwi r27, r0, 1, 29
    stfs f0, 0x20(r1)
    lfs f0, 0x10(r5)
    stfs f0, 0x24(r1)
    lfs f0, 0x20(r5)
    stfs f0, 0x28(r1)
    bl fn_800A3ADC
    addi r3, r1, 0x20
    addi r4, r1, 0x44
    bl fn_800A3B7C
    lfs f0, lbl_8047DB30(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_8019E190
    lfs f0, lbl_8047DB48(r2)
    b @L_8019E1B0
    @L_8019E190:
    lfs f0, lbl_8047DB74(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_8019E1A8
    lfs f0, lbl_8047DB78(r2)
    b @L_8019E1B0
    @L_8019E1A8:
    bl fn_800CE298
    frsp f0, f1
    @L_8019E1B0:
    fmr f1, f0
    cmpwi r27, 0x0
    bne @L_8019E1C0
    fneg f1, f1
    @L_8019E1C0:
    cmplwi r31, 0x0
    beq @L_8019E1E0
    lfs f0, 0x8(r31)
    fcmpo cr0, f1, f0
    bge @L_8019E1E0
    lfs f1, 0x8(r31)
    li r28, 0x1
    b @L_8019E1FC
    @L_8019E1E0:
    cmplwi r29, 0x0
    beq @L_8019E1FC
    lfs f0, 0x8(r29)
    fcmpo cr0, f0, f1
    bge @L_8019E1FC
    lfs f1, 0x8(r29)
    li r28, 0x1
    @L_8019E1FC:
    cmpwi r28, 0x0
    beq @L_8019E240
    lwz r5, 0xc(r30)
    addi r3, r1, 0x74
    addi r4, r1, 0x14
    addi r5, r5, 0x44
    lfs f0, 0x8(r5)
    stfs f0, 0x14(r1)
    lfs f0, 0x18(r5)
    stfs f0, 0x18(r1)
    lfs f0, 0x28(r5)
    stfs f0, 0x1c(r1)
    bl fn_800A3244
    addi r3, r1, 0x74
    addi r4, r1, 0x20
    addi r5, r1, 0x44
    bl fn_800A37CC
    @L_8019E240:
    lwz r6, 0xc(r30)
    addi r3, r1, 0x2c
    addi r4, r1, 0x44
    addi r5, r1, 0x38
    addi r6, r6, 0x44
    lfs f0, 0x8(r6)
    stfs f0, 0x2c(r1)
    lfs f0, 0x18(r6)
    stfs f0, 0x30(r1)
    lfs f0, 0x28(r6)
    stfs f0, 0x34(r1)
    bl fn_800A3B9C
    addi r3, r1, 0x38
    addi r4, r1, 0x38
    bl fn_800A3B7C
    lfs f3, lbl_8047DB44(r2)
    lfs f2, lbl_8047DB30(r2)
    fadds f1, f3, f1
    lfs f0, lbl_8047DB48(r2)
    fdivs f8, f2, f1
    fcmpo cr0, f8, f0
    ble @L_8019E2F0
    frsqrte f7, f8
    lfd f6, lbl_8047DB50(r2)
    lfd f3, lbl_8047DB58(r2)
    lfd f4, lbl_8047DB50(r2)
    fmul f5, f7, f7
    lfd f1, lbl_8047DB58(r2)
    lfd f2, lbl_8047DB50(r2)
    fmul f6, f6, f7
    lfd f0, lbl_8047DB58(r2)
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
    b @L_8019E37C
    @L_8019E2F0:
    lfd f0, lbl_8047DB60(r2)
    fcmpo cr0, f8, f0
    bge @L_8019E308
    lis r3, lbl_80478AC0@ha
    lfs f8, lbl_80478AC0@l(r3)
    b @L_8019E37C
    @L_8019E308:
    stfs f8, 0x8(r1)
    lis r0, 0x7f80
    lwz r3, 0x8(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_8019E330
    bge @L_8019E368
    cmpwi r3, 0x0
    beq @L_8019E34C
    b @L_8019E368
    @L_8019E330:
    lwz r0, 0x8(r1)
    clrlwi. r0, r0, 9
    beq @L_8019E344
    li r0, 0x1
    b @L_8019E36C
    @L_8019E344:
    li r0, 0x2
    b @L_8019E36C
    @L_8019E34C:
    lwz r0, 0x8(r1)
    clrlwi. r0, r0, 9
    beq @L_8019E360
    li r0, 0x5
    b @L_8019E36C
    @L_8019E360:
    li r0, 0x3
    b @L_8019E36C
    @L_8019E368:
    li r0, 0x4
    @L_8019E36C:
    cmpwi r0, 0x1
    bne @L_8019E37C
    lis r3, lbl_80478AC0@ha
    lfs f8, lbl_80478AC0@l(r3)
    @L_8019E37C:
    fmr f1, f8
    addi r3, r1, 0x38
    addi r4, r1, 0x38
    bl fn_800A3AC0
    addi r3, r1, 0x44
    addi r4, r1, 0x38
    addi r5, r1, 0x2c
    bl fn_800A3B9C
    lfs f2, 0x44(r1)
    lfs f1, 0x68(r1)
    lfs f0, 0x68(r1)
    fmuls f1, f2, f1
    lfs f6, 0x68(r1)
    lfs f5, 0x6c(r1)
    lfs f4, 0x6c(r1)
    stfs f1, 0x44(r30)
    lfs f3, 0x6c(r1)
    lfs f1, 0x48(r1)
    lfs f2, 0x70(r1)
    fmuls f7, f1, f0
    lfs f1, 0x70(r1)
    lfs f0, 0x70(r1)
    stfs f7, 0x54(r30)
    lfs f7, 0x4c(r1)
    fmuls f6, f7, f6
    stfs f6, 0x64(r30)
    lfs f6, 0x38(r1)
    fmuls f5, f6, f5
    stfs f5, 0x48(r30)
    lfs f5, 0x3c(r1)
    fmuls f4, f5, f4
    stfs f4, 0x58(r30)
    lfs f4, 0x40(r1)
    fmuls f3, f4, f3
    stfs f3, 0x68(r30)
    lfs f3, 0x2c(r1)
    fmuls f2, f3, f2
    stfs f2, 0x4c(r30)
    lfs f2, 0x30(r1)
    fmuls f1, f2, f1
    stfs f1, 0x5c(r30)
    lfs f1, 0x34(r1)
    fmuls f0, f1, f0
    stfs f0, 0x6c(r30)
    lfs f0, 0x5c(r1)
    stfs f0, 0x50(r30)
    lfs f0, 0x60(r1)
    stfs f0, 0x60(r30)
    lfs f0, 0x64(r1)
    stfs f0, 0x70(r30)
    @L_8019E444:
    psq_l f31, 0xc8(r1), 0, 0
    lfd f31, 0xc0(r1)
    lmw r27, 0xac(r1)
    lwz r0, 0xd4(r1)
    mtlr r0
    addi r1, r1, 0xd0
    blr
