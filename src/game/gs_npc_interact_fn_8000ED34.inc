    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r23, 0x1c(r1)
    mr r31, r3
    bl fn_80105624
    mr r30, r3
    mr r3, r31
    li r28, -0x1
    li r27, 0x0
    li r26, 0x0
    li r4, 0x0
    bl fn_801040D0
    mr r0, r3
    mr r3, r31
    mr r25, r0
    li r4, 0x1
    bl fn_801040D0
    mr r3, r31
    li r4, 0x2
    bl fn_801040D0
    clrlwi r24, r3, 24
    lwz r3, 0x4(r31)
    li r4, 0x1
    bl fn_80102254
    li r3, 0xfc
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000EDB4
    li r27, 0xfc
    b @L_8000EDCC
    @L_8000EDB4:
    li r3, 0xfd
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000EDCC
    li r27, 0xfd
    @L_8000EDCC:
    li r29, 0x0
    li r3, 0x1
    li r4, 0x0
    bl fn_800F7920
    mr r23, r3
    li r3, 0x1
    li r4, 0x0
    bl fn_800F7994
    extsb r0, r3
    cmpwi r0, 0x0
    bge @L_8000EDFC
    neg r0, r0
    @L_8000EDFC:
    cmpwi r0, 0x20
    bgt @L_8000EE1C
    extsb r0, r23
    cmpwi r0, 0x0
    bge @L_8000EE14
    neg r0, r0
    @L_8000EE14:
    cmpwi r0, 0x20
    ble @L_8000EF20
    @L_8000EE1C:
    extsb r4, r3
    extsb r0, r23
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
    ble @L_8000EE70
    fmr f1, f2
    b @L_8000EE74
    @L_8000EE70:
    fneg f1, f2
    @L_8000EE74:
    lfs f0, lbl_8047B704(r2)
    fcmpo cr0, f1, f0
    bge @L_8000EE8C
    ori r0, r29, 0x2
    clrlwi r29, r0, 16
    b @L_8000EEB8
    @L_8000EE8C:
    lfs f0, lbl_8047B700(r2)
    fcmpo cr0, f2, f0
    ble @L_8000EEA0
    fmr f1, f2
    b @L_8000EEA4
    @L_8000EEA0:
    fneg f1, f2
    @L_8000EEA4:
    lfs f0, lbl_8047B708(r2)
    fcmpo cr0, f1, f0
    ble @L_8000EEB8
    ori r0, r29, 0x1
    clrlwi r29, r0, 16
    @L_8000EEB8:
    lfs f0, lbl_8047B700(r2)
    lfs f1, lbl_8047B704(r2)
    fcmpo cr0, f2, f0
    ble @L_8000EED0
    fmr f0, f2
    b @L_8000EED4
    @L_8000EED0:
    fneg f0, f2
    @L_8000EED4:
    fcmpo cr0, f1, f0
    bge @L_8000EF20
    lfs f0, lbl_8047B700(r2)
    fcmpo cr0, f2, f0
    ble @L_8000EEF0
    fmr f1, f2
    b @L_8000EEF4
    @L_8000EEF0:
    fneg f1, f2
    @L_8000EEF4:
    lfs f0, lbl_8047B708(r2)
    fcmpo cr0, f1, f0
    bge @L_8000EF20
    lfs f0, lbl_8047B700(r2)
    fcmpo cr0, f2, f0
    bge @L_8000EF18
    ori r0, r29, 0x4
    clrlwi r29, r0, 16
    b @L_8000EF20
    @L_8000EF18:
    ori r0, r29, 0x8
    clrlwi r29, r0, 16
    @L_8000EF20:
    clrlwi r3, r29, 16
    clrlwi r0, r3, 31
    cmpwi r0, 0x0
    beq @L_8000EF38
    li r28, 0x0
    b @L_8000EF9C
    @L_8000EF38:
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8000EF4C
    li r28, 0x1
    b @L_8000EF9C
    @L_8000EF4C:
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8000EF60
    li r28, 0x2
    b @L_8000EF9C
    @L_8000EF60:
    rlwinm r0, r3, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8000EF74
    li r28, 0x3
    b @L_8000EF9C
    @L_8000EF74:
    lhz r3, 0x4(r30)
    rlwinm r0, r3, 0, 24, 24
    cmpwi r0, 0x0
    beq @L_8000EF8C
    li r28, 0x4
    b @L_8000EF9C
    @L_8000EF8C:
    rlwinm r0, r3, 0, 25, 25
    cmpwi r0, 0x0
    beq @L_8000EF9C
    li r28, 0x5
    @L_8000EF9C:
    cmpwi r28, 0x0
    blt @L_8000F03C
    clrlwi r29, r28, 16
    mr r3, r25
    cmplwi r29, 0x6
    blt @L_8000EFBC
    li r23, 0x0
    b @L_8000F01C
    @L_8000EFBC:
    cmplwi r25, 0x0
    bne @L_8000EFCC
    li r3, 0x0
    bl fn_801F2A7C
    @L_8000EFCC:
    cmplwi r3, 0x0
    bne @L_8000EFDC
    li r23, 0x0
    b @L_8000F01C
    @L_8000EFDC:
    mr r4, r29
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8000EFF4
    li r23, 0x0
    b @L_8000F01C
    @L_8000EFF4:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r23, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8000F01C
    li r23, 0x0
    @L_8000F01C:
    cmplwi r23, 0x0
    bne @L_8000F028
    li r28, -0x1
    @L_8000F028:
    cmpwi r28, 0x0
    blt @L_8000F03C
    li r0, 0x1
    stb r0, 0x98(r31)
    stw r28, 0x80(r31)
    @L_8000F03C:
    cmpwi r28, 0x0
    bge @L_8000F1D4
    lhz r0, 0x4(r30)
    rlwinm r0, r0, 0, 22, 22
    cmpwi r0, 0x0
    beq @L_8000F074
    cmplwi r24, 0x0
    beq @L_8000F1D4
    li r3, 0x1
    li r0, -0x1
    stb r3, 0x98(r31)
    stb r3, 0x99(r31)
    stw r0, 0x80(r31)
    b @L_8000F1D4
    @L_8000F074:
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 21, 21
    cmpwi r0, 0x0
    beq @L_8000F1D4
    lwz r3, 0x4(r31)
    li r4, 0x0
    bl fn_80102254
    li r0, -0x1
    stw r0, lbl_8047885C(r13)
    lhz r3, 0x0(r30)
    clrlwi r0, r3, 31
    cmpwi r0, 0x0
    beq @L_8000F0E4
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8000F0C0
    li r0, 0x5
    stw r0, lbl_8047885C(r13)
    b @L_8000F128
    @L_8000F0C0:
    rlwinm r0, r3, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8000F0D8
    li r0, 0x4
    stw r0, lbl_8047885C(r13)
    b @L_8000F128
    @L_8000F0D8:
    li r0, 0x0
    stw r0, lbl_8047885C(r13)
    b @L_8000F128
    @L_8000F0E4:
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8000F0FC
    li r0, 0x2
    stw r0, lbl_8047885C(r13)
    b @L_8000F128
    @L_8000F0FC:
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8000F114
    li r0, 0x1
    stw r0, lbl_8047885C(r13)
    b @L_8000F128
    @L_8000F114:
    rlwinm r0, r3, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8000F128
    li r0, 0x3
    stw r0, lbl_8047885C(r13)
    @L_8000F128:
    lwz r0, lbl_8047885C(r13)
    cmpwi r0, 0x0
    blt @L_8000F1BC
    clrlwi r24, r0, 16
    mr r3, r25
    cmplwi r24, 0x6
    blt @L_8000F14C
    li r23, 0x0
    b @L_8000F1AC
    @L_8000F14C:
    cmplwi r25, 0x0
    bne @L_8000F15C
    li r3, 0x0
    bl fn_801F2A7C
    @L_8000F15C:
    cmplwi r3, 0x0
    bne @L_8000F16C
    li r23, 0x0
    b @L_8000F1AC
    @L_8000F16C:
    mr r4, r24
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8000F184
    li r23, 0x0
    b @L_8000F1AC
    @L_8000F184:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r23, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8000F1AC
    li r23, 0x0
    @L_8000F1AC:
    cmplwi r23, 0x0
    bne @L_8000F1BC
    li r0, -0x1
    stw r0, lbl_8047885C(r13)
    @L_8000F1BC:
    lwz r0, lbl_8047885C(r13)
    cmpwi r0, 0x0
    bge @L_8000F1D0
    li r26, 0xfc
    b @L_8000F1D4
    @L_8000F1D0:
    li r26, 0xfd
    @L_8000F1D4:
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000F22C
    li r3, 0x0
    bl fn_801F1700
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000F214
    bl fn_80265924
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000F214
    li r0, 0x1
    b @L_8000F230
    @L_8000F214:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_8000F22C
    li r0, 0x1
    b @L_8000F230
    @L_8000F22C:
    li r0, 0x0
    @L_8000F230:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_8000F248
    li r0, 0x1
    stb r0, 0x98(r31)
    stb r0, 0x99(r31)
    @L_8000F248:
    cmpw r27, r26
    beq @L_8000F2FC
    cmpwi r27, 0x0
    beq @L_8000F260
    mr r3, r27
    bl fn_80102510
    @L_8000F260:
    lbz r0, 0x98(r31)
    cmplwi r0, 0x0
    bne @L_8000F2FC
    cmpwi r26, 0xfc
    bne @L_8000F2B4
    lwz r4, 0x4(r31)
    mr r9, r25
    li r3, 0xfc
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0xfc
    li r4, 0x1
    bl fn_80102254
    li r3, 0xfd
    li r4, 0x0
    bl fn_80102254
    b @L_8000F2FC
    @L_8000F2B4:
    cmpwi r26, 0xfd
    bne @L_8000F2FC
    lwz r4, 0x4(r31)
    mr r9, r25
    li r3, 0xfd
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x2
    la r10, lbl_8047885C(r13)
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0xfd
    li r4, 0x1
    bl fn_80102254
    li r3, 0xfc
    li r4, 0x0
    bl fn_80102254
    @L_8000F2FC:
    lmw r23, 0x1c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
