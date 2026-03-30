    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stfd f30, 0x50(r1)
    psq_st f30, 0x58(r1), 0, 0
    stmw r24, 0x30(r1)
    mr. r26, r7
    lwz r7, 0x0(r6)
    lwz r5, 0x4(r6)
    mr r24, r3
    lwz r0, 0x8(r6)
    mr r25, r4
    stw r7, 0xc(r1)
    li r30, 0x0
    stw r5, 0x10(r1)
    stw r0, 0x14(r1)
    beq @L_8010FB44
    addi r30, r1, 0x18
@L_8010FB44:
    bl fn_8010CBD0
    mr r31, r3
    li r28, 0x0
@L_8010FB50:
    lfs f31, 0x10(r1)
    addi r27, r31, 0xa00
    lfs f30, 0x14(r1)
    li r29, 0x0
@L_8010FB60:
    cmpw r29, r25
    beq @L_8010FD5C
    lhz r3, 0x10(r27)
    rlwinm. r0, r3, 0, 31, 31
    beq @L_8010FD5C
    rlwinm. r0, r3, 0, 30, 30
    bne @L_8010FD5C
    lwz r3, 0x0(r27)
    lwz r4, 0x4(r27)
    bl fn_8018D998
    bl fn_8018D928
    cmplwi r3, 0x0
    bne @L_8010FB9C
    li r0, 0x0
    b @L_8010FD54
@L_8010FB9C:
    bl fn_8018FCBC
    cmplwi r3, 0x0
    bne @L_8010FBB0
    li r0, 0x0
    b @L_8010FD54
@L_8010FBB0:
    lfs f0, 0xc(r24)
    lfs f1, 0x4(r3)
    fadds f0, f31, f0
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    beq @L_8010FBDC
    lfs f0, 0xc(r27)
    fadds f0, f1, f0
    fcmpo cr0, f0, f31
    cror eq, lt, eq
    bne @L_8010FBE4
@L_8010FBDC:
    li r0, 0x0
    b @L_8010FD54
@L_8010FBE4:
    lfs f0, 0x8(r3)
    lfs f3, 0xc(r1)
    fsubs f0, f30, f0
    lfs f2, 0x0(r3)
    lfs f1, -26496(r2)
    fsubs f6, f3, f2
    fmuls f2, f0, f0
    fmadds f5, f6, f6, f2
    fcmpo cr0, f5, f1
    ble @L_8010FC54
    frsqrte f2, f5
    lfd f4, -26488(r2)
    lfd f3, -26480(r2)
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f5, f1, f3
    fmul f2, f2, f1
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f5, f1, f3
    fmul f2, f2, f1
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f5, f1, f3
    fmul f1, f2, f1
    fmul f5, f5, f1
    frsp f5, f5
    b @L_8010FCD8
@L_8010FC54:
    lfd f1, -26472(r2)
    fcmpo cr0, f5, f1
    bge @L_8010FC6C
    lis r4, lbl_80478AC0@ha
    lfs f5, lbl_80478AC0@l(r4)
    b @L_8010FCD8
@L_8010FC6C:
    stfs f5, 0x8(r1)
    lis r0, 0x7f80
    lwz r5, 0x8(r1)
    rlwinm r4, r5, 0, 1, 8
    cmpw r4, r0
    beq @L_8010FC94
    bge @L_8010FCC4
    cmpwi r4, 0x0
    beq @L_8010FCAC
    b @L_8010FCC4
@L_8010FC94:
    rlwinm. r0, r5, 0, 9, 31
    beq @L_8010FCA4
    li r0, 0x1
    b @L_8010FCC8
@L_8010FCA4:
    li r0, 0x2
    b @L_8010FCC8
@L_8010FCAC:
    rlwinm. r0, r5, 0, 9, 31
    beq @L_8010FCBC
    li r0, 0x5
    b @L_8010FCC8
@L_8010FCBC:
    li r0, 0x3
    b @L_8010FCC8
@L_8010FCC4:
    li r0, 0x4
@L_8010FCC8:
    cmpwi r0, 0x1
    bne @L_8010FCD8
    lis r4, lbl_80478AC0@ha
    lfs f5, lbl_80478AC0@l(r4)
@L_8010FCD8:
    lfs f2, 0x8(r27)
    lfs f1, 0x8(r24)
    fadds f3, f2, f1
    fcmpo cr0, f5, f3
    cror eq, gt, eq
    bne @L_8010FCF8
    li r0, 0x0
    b @L_8010FD54
@L_8010FCF8:
    cmplwi r30, 0x0
    bne @L_8010FD08
    li r0, 0x1
    b @L_8010FD54
@L_8010FD08:
    lfs f1, -26496(r2)
    fcmpo cr0, f5, f1
    cror eq, lt, eq
    bne @L_8010FD20
    lfs f6, -26464(r2)
    fmr f5, f6
@L_8010FD20:
    lfs f2, -26460(r2)
    li r0, 0x1
    lfs f1, 0x0(r3)
    fadds f2, f2, f3
    fdivs f2, f2, f5
    fmuls f6, f6, f2
    fmuls f0, f0, f2
    fadds f1, f6, f1
    stfs f1, 0x0(r30)
    stfs f31, 0x4(r30)
    lfs f1, 0x8(r3)
    fadds f0, f0, f1
    stfs f0, 0x8(r30)
@L_8010FD54:
    cmpwi r0, 0x0
    bne @L_8010FD6C
@L_8010FD5C:
    addi r29, r29, 0x1
    addi r27, r27, 0x14
    cmpwi r29, 0x30
    blt @L_8010FB60
@L_8010FD6C:
    cmpwi r29, 0x30
    bge @L_8010FDA8
    cmplwi r30, 0x0
    bne @L_8010FD84
    li r3, 0x1
    b @L_8010FDD4
@L_8010FD84:
    lwz r4, 0x0(r30)
    addi r28, r28, 0x1
    lwz r3, 0x4(r30)
    cmpwi r28, 0xa
    lwz r0, 0x8(r30)
    stw r4, 0xc(r1)
    stw r3, 0x10(r1)
    stw r0, 0x14(r1)
    blt @L_8010FB50
@L_8010FDA8:
    cmpwi r28, 0x0
    bgt @L_8010FDB8
    li r3, 0x0
    b @L_8010FDD4
@L_8010FDB8:
    lwz r0, 0xc(r1)
    li r3, 0x1
    lwz r4, 0x10(r1)
    stw r0, 0x0(r26)
    lwz r0, 0x14(r1)
    stw r4, 0x4(r26)
    stw r0, 0x8(r26)
@L_8010FDD4:
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    psq_l f30, 0x58(r1), 0, 0
    lfd f30, 0x50(r1)
    lmw r24, 0x30(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr

