    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stfd f30, 0x70(r1)
    psq_st f30, 0x78(r1), 0, 0
    stmw r27, 0x5c(r1)
    lis r6, lbl_80273FB4@ha
    mr r31, r4
    lwzu r4, lbl_80273FB4@l(r6)
    mr r27, r3
    mr r30, r5
    li r28, 0x0
    lwz r3, 0x4(r6)
    lwz r0, 0x8(r6)
    stw r4, 0x34(r1)
    stw r3, 0x38(r1)
    stw r0, 0x3c(r1)
    b @L_80186BE4
    @L_80186BAC:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186BE0
    lwz r0, 0x28(r3)
    cmplw r0, r31
    bne @L_80186BE0
    lwz r0, 0x2c(r3)
    cmplw r0, r30
    bne @L_80186BE0
    lwz r28, 0x4(r3)
    b @L_80186C50
    @L_80186BE0:
    addi r28, r28, 0x1
    @L_80186BE4:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_80186BAC
    li r29, 0x0
    b @L_80186C40
    @L_80186BF8:
    mr r3, r29
    bl fn_8018FD88
    mr r28, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186C3C
    lwz r0, 0x2c(r28)
    cmplw r0, r30
    bne @L_80186C3C
    lis r3, lbl_80273FD8@ha
    mr r4, r31
    addi r3, r3, lbl_80273FD8@l
    mr r5, r30
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r28)
    b @L_80186C50
    @L_80186C3C:
    addi r29, r29, 0x1
    @L_80186C40:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80186BF8
    li r28, 0x0
    @L_80186C50:
    li r29, 0x0
    b @L_80186C84
    @L_80186C58:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186C80
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_80186C80
    mr r31, r3
    b @L_80186C94
    @L_80186C80:
    addi r29, r29, 0x1
    @L_80186C84:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80186C58
    li r31, 0x0
    @L_80186C94:
    cmplwi r31, 0x0
    bne @L_80186CB8
    lwz r3, 0x34(r1)
    lwz r0, 0x38(r1)
    stw r3, 0x0(r27)
    stw r0, 0x4(r27)
    lwz r0, 0x3c(r1)
    stw r0, 0x8(r27)
    b @L_801870C4
    @L_80186CB8:
    li r3, 0x1
    li r4, 0x1
    bl fn_800F7A7C
    mr r0, r3
    li r3, 0x1
    mr r30, r0
    li r4, 0x1
    bl fn_800F7A08
    mr r0, r3
    li r3, 0x1
    mr r29, r0
    li r4, 0x0
    bl fn_800F7A7C
    mr r0, r3
    li r3, 0x1
    mr r28, r0
    li r4, 0x0
    bl fn_800F7A08
    extsb. r0, r30
    bne @L_80186D68
    extsb. r0, r29
    bne @L_80186D68
    li r3, 0x1
    bl fn_800F7BC4
    rlwinm. r0, r3, 0, 28, 28
    beq @L_80186D24
    li r29, -0x38
    @L_80186D24:
    li r3, 0x1
    bl fn_800F7BC4
    rlwinm. r0, r3, 0, 29, 29
    beq @L_80186D38
    li r29, 0x38
    @L_80186D38:
    li r3, 0x1
    bl fn_800F7BC4
    clrlwi. r0, r3, 31
    beq @L_80186D4C
    li r30, -0x38
    @L_80186D4C:
    li r3, 0x1
    bl fn_800F7BC4
    rlwinm. r0, r3, 0, 30, 30
    beq @L_80186D60
    li r30, 0x38
    @L_80186D60:
    mr r28, r30
    mr r3, r29
    @L_80186D68:
    extsb. r0, r30
    bne @L_80186D78
    extsb. r0, r29
    beq @L_801870AC
    @L_80186D78:
    extsb r0, r30
    cmpwi r0, 0x38
    ble @L_80186D8C
    li r30, 0x38
    b @L_80186D98
    @L_80186D8C:
    cmpwi r0, -0x38
    bge @L_80186D98
    li r30, -0x38
    @L_80186D98:
    extsb r0, r29
    cmpwi r0, 0x38
    ble @L_80186DAC
    li r29, 0x38
    b @L_80186DB8
    @L_80186DAC:
    cmpwi r0, -0x38
    bge @L_80186DB8
    li r29, -0x38
    @L_80186DB8:
    extsb. r0, r30
    extsb r0, r30
    neg r4, r0
    ble @L_80186DCC
    mr r4, r0
    @L_80186DCC:
    xoris r0, r4, 0x8000
    lis r5, 0x4330
    stw r0, 0x44(r1)
    extsb r0, r29
    lfd f2, lbl_8047D7C8(r2)
    extsb. r4, r29
    stw r5, 0x40(r1)
    neg r4, r0
    lfs f0, lbl_8047D84C(r2)
    lfd f1, 0x40(r1)
    fsubs f1, f1, f2
    fdivs f5, f1, f0
    ble @L_80186E04
    mr r4, r0
    @L_80186E04:
    xoris r4, r4, 0x8000
    lis r0, 0x4330
    stw r4, 0x4c(r1)
    lfd f3, lbl_8047D7C8(r2)
    stw r0, 0x48(r1)
    lfs f1, lbl_8047D84C(r2)
    lfd f2, 0x48(r1)
    lfs f0, lbl_8047D7A0(r2)
    fsubs f2, f2, f3
    fdivs f6, f2, f1
    fmuls f1, f6, f6
    fmadds f4, f5, f5, f1
    fcmpo cr0, f4, f0
    ble @L_80186E84
    frsqrte f1, f4
    lfd f3, lbl_8047D850(r2)
    lfd f2, lbl_8047D858(r2)
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
    b @L_80186F08
    @L_80186E84:
    lfd f0, lbl_8047D860(r2)
    fcmpo cr0, f4, f0
    bge @L_80186E9C
    lis r4, lbl_80478AC0@ha
    lfs f4, lbl_80478AC0@l(r4)
    b @L_80186F08
    @L_80186E9C:
    stfs f4, 0x8(r1)
    lis r0, 0x7f80
    lwz r5, 0x8(r1)
    rlwinm r4, r5, 0, 1, 8
    cmpw r4, r0
    beq @L_80186EC4
    bge @L_80186EF4
    cmpwi r4, 0x0
    beq @L_80186EDC
    b @L_80186EF4
    @L_80186EC4:
    clrlwi. r0, r5, 9
    beq @L_80186ED4
    li r0, 0x1
    b @L_80186EF8
    @L_80186ED4:
    li r0, 0x2
    b @L_80186EF8
    @L_80186EDC:
    clrlwi. r0, r5, 9
    beq @L_80186EEC
    li r0, 0x5
    b @L_80186EF8
    @L_80186EEC:
    li r0, 0x3
    b @L_80186EF8
    @L_80186EF4:
    li r0, 0x4
    @L_80186EF8:
    cmpwi r0, 0x1
    bne @L_80186F08
    lis r4, lbl_80478AC0@ha
    lfs f4, lbl_80478AC0@l(r4)
    @L_80186F08:
    lfs f0, lbl_8047D7C4(r2)
    fmr f31, f4
    fcmpo cr0, f4, f0
    ble @L_80186F1C
    fmr f31, f0
    @L_80186F1C:
    extsb r0, r28
    cmpwi r0, -0x2
    ble @L_80186F44
    cmpwi r0, 0x2
    bge @L_80186F44
    extsb r0, r3
    cmpwi r0, -0x2
    ble @L_80186F44
    cmpwi r0, 0x2
    blt @L_80186FD0
    @L_80186F44:
    lfs f0, lbl_8047D868(r2)
    fcmpo cr0, f6, f0
    bge @L_80186F58
    lfs f2, lbl_8047D86C(r2)
    b @L_80186F84
    @L_80186F58:
    fdivs f1, f5, f6
    lfs f0, lbl_8047D7E8(r2)
    fcmpo cr0, f1, f0
    ble @L_80186F6C
    fmr f1, f0
    @L_80186F6C:
    lfs f0, lbl_8047D870(r2)
    fdivs f1, f1, f0
    bl fn_800CE148
    frsp f1, f1
    lfs f0, lbl_8047D86C(r2)
    fmuls f2, f0, f1
    @L_80186F84:
    extsb. r0, r29
    blt @L_80186F94
    fmr f30, f2
    b @L_80186F9C
    @L_80186F94:
    lfs f0, lbl_8047D818(r2)
    fsubs f30, f0, f2
    @L_80186F9C:
    extsb. r0, r30
    bge @L_80186FC4
    extsb. r0, r29
    blt @L_80186FBC
    lfs f1, lbl_8047D818(r2)
    fsubs f0, f1, f2
    fadds f30, f1, f0
    b @L_80186FC4
    @L_80186FBC:
    lfs f0, lbl_8047D818(r2)
    fadds f30, f0, f2
    @L_80186FC4:
    bl fn_80176684
    fadds f1, f30, f1
    b @L_80186FD4
    @L_80186FD0:
    lfs f1, 0x40(r31)
    @L_80186FD4:
    lfs f2, lbl_8047D830(r2)
    fcmpo cr0, f31, f2
    cror eq, lt, eq
    bne @L_80186FFC
    fdivs f3, f31, f2
    lfs f2, 0x34(r31)
    lfs f0, lbl_8047D7A4(r2)
    fmuls f0, f2, f0
    fmuls f30, f3, f0
    b @L_80187050
    @L_80186FFC:
    fcmpo cr0, f31, f2
    ble @L_80187038
    lfs f0, lbl_8047D79C(r2)
    fcmpo cr0, f31, f0
    cror eq, lt, eq
    bne @L_80187038
    fsubs f2, f31, f2
    lfs f0, lbl_8047D874(r2)
    lfs f4, 0x34(r31)
    lfs f3, lbl_8047D7A4(r2)
    fdivs f2, f2, f0
    fmuls f3, f4, f3
    fsubs f0, f4, f3
    fmadds f30, f2, f0, f3
    b @L_80187050
    @L_80187038:
    lfs f2, lbl_8047D79C(r2)
    lfs f3, 0x34(r31)
    lfs f0, 0x38(r31)
    fsubs f2, f31, f2
    fsubs f0, f0, f3
    fmadds f30, f2, f0, f3
    @L_80187050:
    lis r3, lbl_8031554C@ha
    addi r4, r3, lbl_8031554C@l
    addi r3, r1, 0xc
    bl fn_800E0718
    lfs f1, lbl_8047D7A0(r2)
    fmr f3, f30
    addi r3, r1, 0x28
    fmr f2, f1
    bl fn_800E01F4
    addi r3, r1, 0x1c
    addi r4, r1, 0xc
    addi r5, r1, 0x28
    bl fn_800DFEEC
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x4c(r1)
    lfd f1, lbl_8047D7D0(r2)
    addi r3, r1, 0x34
    stw r0, 0x48(r1)
    addi r4, r1, 0x1c
    lfd f0, 0x48(r1)
    fsubs f1, f0, f1
    bl fn_800E013C
    @L_801870AC:
    lwz r3, 0x34(r1)
    lwz r0, 0x38(r1)
    stw r3, 0x0(r27)
    stw r0, 0x4(r27)
    lwz r0, 0x3c(r1)
    stw r0, 0x8(r27)
    @L_801870C4:
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    psq_l f30, 0x78(r1), 0, 0
    lfd f30, 0x70(r1)
    lmw r27, 0x5c(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
