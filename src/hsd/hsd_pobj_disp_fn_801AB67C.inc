    nofralloc
    stwu r1, -0x130(r1)
    mflr r0
    stw r0, 0x134(r1)
    stfd f31, 0x120(r1)
    psq_st f31, 0x128(r1), 0, 0
    stmw r26, 0x108(r1)
    lwz r0, lbl_8047B2F4(r13)
    mr r26, r3
    lis r3, lbl_80274EE0@ha
    lwz r30, 0x14(r26)
    cmplwi r0, 0x0
    addi r31, r3, lbl_80274EE0@l
    bne @L_801AB6C8
    li r0, 0x7d0
    stw r0, lbl_8047B2F4(r13)
    lwz r0, lbl_8047B2F4(r13)
    mulli r3, r0, 0xc
    bl fn_801A6928
    stw r3, lbl_8047B2EC(r13)
@L_801AB6C8:
    lwz r3, lbl_8047B2F4(r13)
    lwz r0, 0x4(r30)
    cmplw r3, r0
    bge @L_801AB6E8
    addi r5, r31, 0x64
    addi r3, r2, -23016
    li r4, 0x56b
    bl fn_80196E10
@L_801AB6E8:
    lwz r0, 0x14(r30)
    cmplwi r0, 0x0
    beq @L_801AB718
    lwz r0, lbl_8047B2F8(r13)
    cmplwi r0, 0x0
    bne @L_801AB718
    li r0, 0x7d0
    stw r0, lbl_8047B2F8(r13)
    lwz r0, lbl_8047B2F8(r13)
    mulli r3, r0, 0xc
    bl fn_801A6928
    stw r3, lbl_8047B2F0(r13)
@L_801AB718:
    lwz r0, 0x14(r30)
    cmplwi r0, 0x0
    beq @L_801AB784
    lwz r3, 0x14(r30)
    lwz r0, 0x0(r3)
    cmpwi r0, 0xa
    bne @L_801AB75C
    lwz r3, lbl_8047B2F8(r13)
    lwz r0, 0x10(r30)
    cmplw r3, r0
    bge @L_801AB754
    addi r5, r31, 0x98
    addi r3, r2, -23016
    li r4, 0x574
    bl fn_80196E10
@L_801AB754:
    li r28, 0x0
    b @L_801AB784
@L_801AB75C:
    lwz r0, 0x10(r30)
    lwz r3, lbl_8047B2F8(r13)
    mulli r0, r0, 0x3
    cmplw r3, r0
    bge @L_801AB780
    addi r5, r31, 0xcc
    addi r3, r2, -23016
    li r4, 0x577
    bl fn_80196E10
@L_801AB780:
    li r28, 0x1
@L_801AB784:
    lhz r0, 0x0(r30)
    rlwinm. r0, r0, 0, 31, 31
    beq @L_801ABB14
    lfs f31, 0x1c(r30)
    lhz r3, 0x2(r30)
    fctiwz f1, f31
    fctiwz f0, f31
    subi r0, r3, 0x1
    stfd f1, 0xc0(r1)
    stfd f0, 0xc8(r1)
    lwz r4, 0xc4(r1)
    lwz r3, 0xcc(r1)
    srawi r4, r4, 31
    andc r3, r3, r4
    cmpw r3, r0
    bge @L_801AB7E8
    fctiwz f1, f31
    fctiwz f0, f31
    stfd f1, 0xd0(r1)
    stfd f0, 0xd8(r1)
    lwz r3, 0xd4(r1)
    lwz r0, 0xdc(r1)
    srawi r3, r3, 31
    andc r31, r0, r3
    b @L_801AB7F0
@L_801AB7E8:
    lhz r3, 0x2(r30)
    subi r31, r3, 0x1
@L_801AB7F0:
    xoris r3, r31, 0x8000
    lis r0, 0x4330
    stw r3, 0xe4(r1)
    lfd f1, -22944(r2)
    stw r0, 0xe0(r1)
    lfd f2, -22960(r2)
    lfd f0, 0xe0(r1)
    fsubs f0, f0, f1
    fsubs f0, f31, f0
    fcmpo cr0, f2, f0
    ble @L_801AB824
    lfd f1, -22960(r2)
    b @L_801AB844
@L_801AB824:
    xoris r3, r31, 0x8000
    lis r0, 0x4330
    stw r3, 0xec(r1)
    lfd f1, -22944(r2)
    stw r0, 0xe8(r1)
    lfd f0, 0xe8(r1)
    fsubs f0, f0, f1
    fsubs f1, f31, f0
@L_801AB844:
    lfd f0, -22952(r2)
    fcmpo cr0, f1, f0
    bge @L_801AB8A8
    xoris r3, r31, 0x8000
    lis r0, 0x4330
    stw r3, 0xf4(r1)
    lfd f1, -22944(r2)
    stw r0, 0xf0(r1)
    lfd f2, -22960(r2)
    lfd f0, 0xf0(r1)
    fsubs f0, f0, f1
    fsubs f0, f31, f0
    fcmpo cr0, f2, f0
    ble @L_801AB884
    lfd f31, -22960(r2)
    b @L_801AB8AC
@L_801AB884:
    xoris r3, r31, 0x8000
    lis r0, 0x4330
    stw r3, 0xfc(r1)
    lfd f1, -22944(r2)
    stw r0, 0xf8(r1)
    lfd f0, 0xf8(r1)
    fsubs f0, f0, f1
    fsubs f31, f31, f0
    b @L_801AB8AC
@L_801AB8A8:
    lfd f31, -22952(r2)
@L_801AB8AC:
    frsp f31, f31
    li r29, 0x0
    b @L_801AB96C
@L_801AB8B8:
    mr r3, r30
    mr r4, r31
    mr r5, r29
    addi r6, r1, 0x44
    bl fn_801AC91C
    lhz r4, 0x2(r30)
    addi r5, r31, 0x1
    mr r3, r30
    subi r0, r4, 0x1
    cmpw r5, r0
    bge @L_801AB8EC
    addi r4, r31, 0x1
    b @L_801AB8F4
@L_801AB8EC:
    lhz r4, 0x2(r30)
    subi r4, r4, 0x1
@L_801AB8F4:
    mr r5, r29
    addi r6, r1, 0x38
    bl fn_801AC91C
    lfs f2, 0x38(r1)
    mulli r4, r29, 0xc
    lfs f1, 0x44(r1)
    lfs f0, 0x44(r1)
    fsubs f1, f2, f1
    lwz r5, lbl_8047B2EC(r13)
    mulli r3, r29, 0xc
    fmadds f0, f31, f1, f0
    mulli r0, r29, 0xc
    addi r29, r29, 0x1
    stfsx f0, r5, r4
    lfs f2, 0x3c(r1)
    lfs f1, 0x48(r1)
    lfs f0, 0x48(r1)
    fsubs f1, f2, f1
    lwz r4, lbl_8047B2EC(r13)
    add r3, r4, r3
    fmadds f0, f31, f1, f0
    stfs f0, 0x4(r3)
    lfs f2, 0x40(r1)
    lfs f1, 0x4c(r1)
    lfs f0, 0x4c(r1)
    fsubs f1, f2, f1
    lwz r3, lbl_8047B2EC(r13)
    add r3, r3, r0
    fmadds f0, f31, f1, f0
    stfs f0, 0x8(r3)
@L_801AB96C:
    lwz r0, 0x4(r30)
    cmpw r29, r0
    blt @L_801AB8B8
    lwz r0, 0x10(r30)
    cmpwi r0, 0x0
    beq @L_801ABDA8
    cmpwi r28, 0x0
    beq @L_801ABA48
    li r29, 0x0
    b @L_801ABA38
@L_801AB994:
    mulli r28, r29, 0x3
    mr r3, r30
    mr r4, r31
    mr r5, r29
    addi r6, r1, 0x98
    bl fn_801AC1F8
    lhz r4, 0x2(r30)
    addi r5, r31, 0x1
    mr r3, r30
    subi r0, r4, 0x1
    cmpw r5, r0
    bge @L_801AB9CC
    addi r4, r31, 0x1
    b @L_801AB9D4
@L_801AB9CC:
    lhz r4, 0x2(r30)
    subi r4, r4, 0x1
@L_801AB9D4:
    mr r5, r29
    addi r6, r1, 0x74
    bl fn_801AC1F8
    li r6, 0x0
    b @L_801ABA2C
@L_801AB9E8:
    rlwinm r0, r6, 2, 0, 29
    addi r5, r1, 0x74
    lfsx f2, r5, r0
    rlwinm r4, r6, 2, 0, 29
    addi r3, r1, 0x98
    rlwinm r0, r6, 2, 0, 29
    lfsx f1, r3, r4
    addi r3, r1, 0x98
    lfsx f0, r3, r0
    rlwinm r0, r6, 2, 0, 29
    fsubs f1, f2, f1
    mulli r3, r28, 0xc
    lwz r4, lbl_8047B2F0(r13)
    addi r6, r6, 0x1
    fmadds f0, f31, f1, f0
    add r0, r3, r0
    stfsx f0, r4, r0
@L_801ABA2C:
    cmpwi r6, 0x9
    blt @L_801AB9E8
    addi r29, r29, 0x1
@L_801ABA38:
    lwz r0, 0x10(r30)
    cmpw r29, r0
    blt @L_801AB994
    b @L_801ABDA8
@L_801ABA48:
    li r29, 0x0
    b @L_801ABB04
@L_801ABA50:
    mr r3, r30
    mr r4, r31
    mr r5, r29
    addi r6, r1, 0x2c
    bl fn_801AC4BC
    lhz r4, 0x2(r30)
    addi r5, r31, 0x1
    mr r3, r30
    subi r0, r4, 0x1
    cmpw r5, r0
    bge @L_801ABA84
    addi r4, r31, 0x1
    b @L_801ABA8C
@L_801ABA84:
    lhz r4, 0x2(r30)
    subi r4, r4, 0x1
@L_801ABA8C:
    mr r5, r29
    addi r6, r1, 0x20
    bl fn_801AC4BC
    lfs f2, 0x20(r1)
    mulli r4, r29, 0xc
    lfs f1, 0x2c(r1)
    lfs f0, 0x2c(r1)
    fsubs f1, f2, f1
    lwz r5, lbl_8047B2F0(r13)
    mulli r3, r29, 0xc
    fmadds f0, f31, f1, f0
    mulli r0, r29, 0xc
    addi r29, r29, 0x1
    stfsx f0, r5, r4
    lfs f2, 0x24(r1)
    lfs f1, 0x30(r1)
    lfs f0, 0x30(r1)
    fsubs f1, f2, f1
    lwz r4, lbl_8047B2F0(r13)
    add r3, r4, r3
    fmadds f0, f31, f1, f0
    stfs f0, 0x4(r3)
    lfs f2, 0x28(r1)
    lfs f1, 0x34(r1)
    lfs f0, 0x34(r1)
    fsubs f1, f2, f1
    lwz r3, lbl_8047B2F0(r13)
    add r3, r3, r0
    fmadds f0, f31, f1, f0
    stfs f0, 0x8(r3)
@L_801ABB04:
    lwz r0, 0x10(r30)
    cmpw r29, r0
    blt @L_801ABA50
    b @L_801ABDA8
@L_801ABB14:
    lwz r27, 0x1c(r30)
    li r29, 0x0
    b @L_801ABBE4
@L_801ABB20:
    mulli r0, r29, 0xc
    lwz r4, lbl_8047B2EC(r13)
    mr r3, r30
    mr r5, r29
    add r6, r4, r0
    li r4, 0x0
    bl fn_801AC91C
    li r31, 0x0
    b @L_801ABBD4
@L_801ABB44:
    rlwinm r0, r31, 2, 0, 29
    lfd f1, -22960(r2)
    lfsx f0, r27, r0
    fcmpo cr0, f1, f0
    ble @L_801ABB60
    lfd f31, -22960(r2)
    b @L_801ABB68
@L_801ABB60:
    rlwinm r0, r31, 2, 0, 29
    lfsx f31, r27, r0
@L_801ABB68:
    frsp f31, f31
    mr r3, r30
    mr r5, r29
    addi r4, r31, 0x1
    addi r6, r1, 0x14
    bl fn_801AC91C
    mulli r4, r29, 0xc
    lwz r5, lbl_8047B2EC(r13)
    lfs f1, 0x14(r1)
    addi r31, r31, 0x1
    lfsx f0, r5, r4
    mulli r3, r29, 0xc
    fmadds f0, f1, f31, f0
    mulli r0, r29, 0xc
    stfsx f0, r5, r4
    lwz r4, lbl_8047B2EC(r13)
    lfs f1, 0x18(r1)
    add r3, r4, r3
    lfs f0, 0x4(r3)
    fmadds f0, f1, f31, f0
    stfs f0, 0x4(r3)
    lwz r3, lbl_8047B2EC(r13)
    lfs f1, 0x1c(r1)
    add r3, r3, r0
    lfs f0, 0x8(r3)
    fmadds f0, f1, f31, f0
    stfs f0, 0x8(r3)
@L_801ABBD4:
    lhz r0, 0x2(r30)
    cmpw r31, r0
    blt @L_801ABB44
    addi r29, r29, 0x1
@L_801ABBE4:
    lwz r0, 0x4(r30)
    cmpw r29, r0
    blt @L_801ABB20
    lwz r0, 0x10(r30)
    cmpwi r0, 0x0
    beq @L_801ABDA8
    cmpwi r28, 0x0
    beq @L_801ABCD0
    li r29, 0x0
    b @L_801ABCC0
@L_801ABC0C:
    mulli r28, r29, 0x3
    lwz r6, lbl_8047B2F0(r13)
    mr r3, r30
    mr r5, r29
    mulli r0, r28, 0xc
    li r4, 0x0
    add r6, r6, r0
    bl fn_801AC1F8
    li r31, 0x0
    b @L_801ABCB0
@L_801ABC34:
    rlwinm r0, r31, 2, 0, 29
    lfd f1, -22960(r2)
    lfsx f0, r27, r0
    fcmpo cr0, f1, f0
    ble @L_801ABC50
    lfd f31, -22960(r2)
    b @L_801ABC58
@L_801ABC50:
    rlwinm r0, r31, 2, 0, 29
    lfsx f31, r27, r0
@L_801ABC58:
    frsp f31, f31
    mr r3, r30
    mr r5, r29
    addi r4, r31, 0x1
    addi r6, r1, 0x50
    bl fn_801AC1F8
    li r7, 0x0
    b @L_801ABCA4
@L_801ABC78:
    mulli r3, r28, 0xc
    rlwinm r0, r7, 2, 0, 29
    rlwinm r6, r7, 2, 0, 29
    lwz r4, lbl_8047B2F0(r13)
    addi r5, r1, 0x50
    add r0, r3, r0
    lfsx f1, r5, r6
    addi r7, r7, 0x1
    lfsx f0, r4, r0
    fmadds f0, f31, f1, f0
    stfsx f0, r4, r0
@L_801ABCA4:
    cmpwi r7, 0x9
    blt @L_801ABC78
    addi r31, r31, 0x1
@L_801ABCB0:
    lhz r0, 0x2(r30)
    cmpw r31, r0
    blt @L_801ABC34
    addi r29, r29, 0x1
@L_801ABCC0:
    lwz r0, 0x10(r30)
    cmpw r29, r0
    blt @L_801ABC0C
    b @L_801ABDA8
@L_801ABCD0:
    li r29, 0x0
    b @L_801ABD9C
@L_801ABCD8:
    mulli r0, r29, 0xc
    lwz r4, lbl_8047B2F0(r13)
    mr r3, r30
    mr r5, r29
    add r6, r4, r0
    li r4, 0x0
    bl fn_801AC4BC
    li r31, 0x0
    b @L_801ABD8C
@L_801ABCFC:
    rlwinm r0, r31, 2, 0, 29
    lfd f1, -22960(r2)
    lfsx f0, r27, r0
    fcmpo cr0, f1, f0
    ble @L_801ABD18
    lfd f31, -22960(r2)
    b @L_801ABD20
@L_801ABD18:
    rlwinm r0, r31, 2, 0, 29
    lfsx f31, r27, r0
@L_801ABD20:
    frsp f31, f31
    mr r3, r30
    mr r5, r29
    addi r4, r31, 0x1
    addi r6, r1, 0x8
    bl fn_801AC4BC
    mulli r4, r29, 0xc
    lwz r5, lbl_8047B2F0(r13)
    lfs f1, 0x8(r1)
    addi r31, r31, 0x1
    lfsx f0, r5, r4
    mulli r3, r29, 0xc
    fmadds f0, f1, f31, f0
    mulli r0, r29, 0xc
    stfsx f0, r5, r4
    lwz r4, lbl_8047B2F0(r13)
    lfs f1, 0xc(r1)
    add r3, r4, r3
    lfs f0, 0x4(r3)
    fmadds f0, f1, f31, f0
    stfs f0, 0x4(r3)
    lwz r3, lbl_8047B2F0(r13)
    lfs f1, 0x10(r1)
    add r3, r3, r0
    lfs f0, 0x8(r3)
    fmadds f0, f1, f31, f0
    stfs f0, 0x8(r3)
@L_801ABD8C:
    lhz r0, 0x2(r30)
    cmpw r31, r0
    blt @L_801ABCFC
    addi r29, r29, 0x1
@L_801ABD9C:
    lwz r0, 0x10(r30)
    cmpw r29, r0
    blt @L_801ABCD8
@L_801ABDA8:
    lwz r4, lbl_8047B2EC(r13)
    mr r3, r26
    lwz r5, lbl_8047B2F0(r13)
    bl fn_801ABDD4
    psq_l f31, 0x128(r1), 0, 0
    lfd f31, 0x120(r1)
    lmw r26, 0x108(r1)
    lwz r0, 0x134(r1)
    mtlr r0
    addi r1, r1, 0x130
    blr
