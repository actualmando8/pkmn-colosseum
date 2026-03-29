    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r24, 0x20(r1)
    mr r26, r3
    bl fn_80105624
    mr r30, r3
    mr r3, r26
    bl fn_80103FE4
    mr r31, r3
    lwz r3, 0x4(r26)
    li r29, -0x1
    li r28, 0x0
    li r27, 0x0
    li r4, 0x1
    bl fn_80102254
    li r3, 0xf9
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000F9C0
    li r28, 0xf9
    b @L_8000F9D8
    @L_8000F9C0:
    li r3, 0xfa
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000F9D8
    li r28, 0xfa
    @L_8000F9D8:
    li r24, 0x0
    li r3, 0x1
    li r4, 0x0
    bl fn_800F7920
    mr r25, r3
    li r3, 0x1
    li r4, 0x0
    bl fn_800F7994
    extsb r0, r3
    cmpwi r0, 0x0
    bge @L_8000FA08
    neg r0, r0
    @L_8000FA08:
    cmpwi r0, 0x20
    bgt @L_8000FA28
    extsb r0, r25
    cmpwi r0, 0x0
    bge @L_8000FA20
    neg r0, r0
    @L_8000FA20:
    cmpwi r0, 0x20
    ble @L_8000FB2C
    @L_8000FA28:
    extsb r4, r3
    extsb r0, r25
    lis r3, 0x4330
    lfd f2, lbl_8047B710(r2)
    xoris r4, r4, 0x8000
    xoris r0, r0, 0x8000
    stw r4, 0xc(r1)
    stw r3, 0x8(r1)
    lfd f0, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f1, f0, f2
    stw r3, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f2, f0, f2
    bl fn_800CE2D8
    frsp f2, f1
    lfs f0, lbl_8047B700(r2)
    fcmpo cr0, f2, f0
    ble @L_8000FA7C
    fmr f1, f2
    b @L_8000FA80
    @L_8000FA7C:
    fneg f1, f2
    @L_8000FA80:
    lfs f0, lbl_8047B704(r2)
    fcmpo cr0, f1, f0
    bge @L_8000FA98
    ori r0, r24, 0x2
    clrlwi r24, r0, 16
    b @L_8000FAC4
    @L_8000FA98:
    lfs f0, lbl_8047B700(r2)
    fcmpo cr0, f2, f0
    ble @L_8000FAAC
    fmr f1, f2
    b @L_8000FAB0
    @L_8000FAAC:
    fneg f1, f2
    @L_8000FAB0:
    lfs f0, lbl_8047B708(r2)
    fcmpo cr0, f1, f0
    ble @L_8000FAC4
    ori r0, r24, 0x1
    clrlwi r24, r0, 16
    @L_8000FAC4:
    lfs f0, lbl_8047B700(r2)
    lfs f1, lbl_8047B704(r2)
    fcmpo cr0, f2, f0
    ble @L_8000FADC
    fmr f0, f2
    b @L_8000FAE0
    @L_8000FADC:
    fneg f0, f2
    @L_8000FAE0:
    fcmpo cr0, f1, f0
    bge @L_8000FB2C
    lfs f0, lbl_8047B700(r2)
    fcmpo cr0, f2, f0
    ble @L_8000FAFC
    fmr f1, f2
    b @L_8000FB00
    @L_8000FAFC:
    fneg f1, f2
    @L_8000FB00:
    lfs f0, lbl_8047B708(r2)
    fcmpo cr0, f1, f0
    bge @L_8000FB2C
    lfs f0, lbl_8047B700(r2)
    fcmpo cr0, f2, f0
    bge @L_8000FB24
    ori r0, r24, 0x4
    clrlwi r24, r0, 16
    b @L_8000FB2C
    @L_8000FB24:
    ori r0, r24, 0x8
    clrlwi r24, r0, 16
    @L_8000FB2C:
    clrlwi r3, r24, 16
    cmplwi r3, 0x0
    beq @L_8000FBB4
    clrlwi r0, r3, 31
    cmpwi r0, 0x0
    beq @L_8000FB4C
    li r29, 0x0
    b @L_8000FB84
    @L_8000FB4C:
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8000FB60
    li r29, 0x1
    b @L_8000FB84
    @L_8000FB60:
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8000FB74
    li r29, 0x2
    b @L_8000FB84
    @L_8000FB74:
    rlwinm r0, r3, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8000FB84
    li r29, 0x3
    @L_8000FB84:
    mulli r0, r29, 0xc
    add r3, r31, r0
    lwz r0, 0x4(r3)
    cmplwi r0, 0x0
    bne @L_8000FB9C
    li r29, -0x1
    @L_8000FB9C:
    cmpwi r29, 0x0
    blt @L_8000FC9C
    li r0, 0x1
    stb r0, 0x98(r26)
    stw r29, 0x80(r26)
    b @L_8000FC9C
    @L_8000FBB4:
    lhz r0, 0x4(r30)
    rlwinm r0, r0, 0, 22, 22
    cmpwi r0, 0x0
    beq @L_8000FBDC
    li r3, 0x1
    li r0, -0x1
    stb r3, 0x98(r26)
    stb r3, 0x99(r26)
    stw r0, 0x80(r26)
    b @L_8000FC9C
    @L_8000FBDC:
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 21, 21
    cmpwi r0, 0x0
    beq @L_8000FC9C
    lwz r3, 0x4(r26)
    li r4, 0x0
    bl fn_80102254
    lhz r3, 0x0(r30)
    clrlwi r0, r3, 31
    cmpwi r0, 0x0
    beq @L_8000FC14
    li r0, 0x0
    stw r0, lbl_80478858(r13)
    b @L_8000FC64
    @L_8000FC14:
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8000FC2C
    li r0, 0x1
    stw r0, lbl_80478858(r13)
    b @L_8000FC64
    @L_8000FC2C:
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8000FC44
    li r0, 0x2
    stw r0, lbl_80478858(r13)
    b @L_8000FC64
    @L_8000FC44:
    rlwinm r0, r3, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8000FC5C
    li r0, 0x3
    stw r0, lbl_80478858(r13)
    b @L_8000FC64
    @L_8000FC5C:
    li r0, -0x1
    stw r0, lbl_80478858(r13)
    @L_8000FC64:
    lwz r0, lbl_80478858(r13)
    mulli r0, r0, 0xc
    add r3, r31, r0
    lwz r0, 0x4(r3)
    cmplwi r0, 0x0
    bne @L_8000FC84
    li r0, -0x1
    stw r0, lbl_80478858(r13)
    @L_8000FC84:
    lwz r0, lbl_80478858(r13)
    cmpwi r0, 0x0
    bge @L_8000FC98
    li r27, 0xf9
    b @L_8000FC9C
    @L_8000FC98:
    li r27, 0xfa
    @L_8000FC9C:
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000FCF4
    li r3, 0x0
    bl fn_801F1700
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000FCDC
    bl fn_80265924
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000FCDC
    li r0, 0x1
    b @L_8000FCF8
    @L_8000FCDC:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_8000FCF4
    li r0, 0x1
    b @L_8000FCF8
    @L_8000FCF4:
    li r0, 0x0
    @L_8000FCF8:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_8000FD10
    li r0, 0x1
    stb r0, 0x98(r26)
    stb r0, 0x99(r26)
    @L_8000FD10:
    cmpw r28, r27
    beq @L_8000FDC4
    cmpwi r28, 0x0
    beq @L_8000FD28
    mr r3, r28
    bl fn_80102510
    @L_8000FD28:
    lbz r0, 0x98(r26)
    cmplwi r0, 0x0
    bne @L_8000FDC4
    cmpwi r27, 0xf9
    bne @L_8000FD7C
    lwz r4, 0x4(r26)
    li r3, 0xf9
    li r5, 0x0
    li r6, 0x0
    mr r9, r4
    li r7, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0xf9
    li r4, 0x1
    bl fn_80102254
    li r3, 0xfa
    li r4, 0x0
    bl fn_80102254
    b @L_8000FDC4
    @L_8000FD7C:
    cmpwi r27, 0xfa
    bne @L_8000FDC4
    lwz r4, 0x4(r26)
    li r3, 0xfa
    li r5, 0x0
    li r6, 0x0
    mr r9, r4
    li r7, 0x0
    li r8, 0x2
    la r10, lbl_80478858(r13)
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0xfa
    li r4, 0x1
    bl fn_80102254
    li r3, 0xf9
    li r4, 0x0
    bl fn_80102254
    @L_8000FDC4:
    lmw r24, 0x20(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
