    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    mr r31, r4
    lis r3, lbl_803A6818@ha
    lfs f2, lbl_8047BCA0(r2)
    addi r3, r3, lbl_803A6818@l
    lfs f0, 0x4c(r3)
    fmuls f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r30)
    lha r0, 0x6(r31)
    cmpwi r0, 0xfbf
    beq @L_80044360
    bge @L_8004401C
    cmpwi r0, 0x6d8
    beq @L_800440EC
    bge @L_80044010
    cmpwi r0, 0x6d7
    bge @L_80044040
    b @L_80044360
    @L_80044010:
    cmpwi r0, 0x6da
    bge @L_80044360
    b @L_80044198
    @L_8004401C:
    cmpwi r0, 0x10c8
    beq @L_800442A4
    bge @L_80044034
    cmpwi r0, 0x10c7
    bge @L_80044244
    b @L_80044360
    @L_80044034:
    cmpwi r0, 0x10ca
    bge @L_80044360
    b @L_80044304
    @L_80044040:
    lfs f0, 0x4c(r3)
    lfs f1, lbl_8047BCA8(r2)
    fmuls f0, f2, f0
    lfs f2, lbl_8047BCA4(r2)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r30)
    lfs f0, 0x40(r3)
    fmuls f0, f1, f0
    fmuls f1, f2, f0
    bl fn_800CE148
    frsp f3, f1
    lfs f2, lbl_8047BD14(r2)
    lfs f1, lbl_8047BD10(r2)
    lfs f0, lbl_8047BCA0(r2)
    fmadds f1, f2, f3, f1
    fcmpo cr0, f1, f0
    ble @L_80044090
    fmr f1, f0
    @L_80044090:
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    bge @L_800440A0
    fmr f1, f0
    @L_800440A0:
    fctiwz f0, f1
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    stb r0, 0x67(r31)
    lwz r0, 0x28(r3)
    cmpwi r0, 0x2
    bne @L_800440D8
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_80044360
    @L_800440D8:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_80044360
    @L_800440EC:
    lfs f0, 0x4c(r3)
    lfs f1, lbl_8047BCA8(r2)
    fmuls f0, f2, f0
    lfs f2, lbl_8047BCA4(r2)
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    stb r0, 0x8b(r30)
    lfs f0, 0x40(r3)
    fmuls f0, f1, f0
    fmuls f1, f2, f0
    bl fn_800CE148
    frsp f3, f1
    lfs f2, lbl_8047BD14(r2)
    lfs f1, lbl_8047BD10(r2)
    lfs f0, lbl_8047BCA0(r2)
    fmadds f1, f2, f3, f1
    fcmpo cr0, f1, f0
    ble @L_8004413C
    fmr f1, f0
    @L_8004413C:
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    bge @L_8004414C
    fmr f1, f0
    @L_8004414C:
    fctiwz f0, f1
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x67(r31)
    lwz r0, 0x28(r3)
    cmpwi r0, 0x1
    bne @L_80044184
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_80044360
    @L_80044184:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_80044360
    @L_80044198:
    lfs f0, 0x4c(r3)
    lfs f1, lbl_8047BCA8(r2)
    fmuls f0, f2, f0
    lfs f2, lbl_8047BCA4(r2)
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    stb r0, 0x8b(r30)
    lfs f0, 0x40(r3)
    fmuls f0, f1, f0
    fmuls f1, f2, f0
    bl fn_800CE148
    frsp f3, f1
    lfs f2, lbl_8047BD14(r2)
    lfs f1, lbl_8047BD10(r2)
    lfs f0, lbl_8047BCA0(r2)
    fmadds f1, f2, f3, f1
    fcmpo cr0, f1, f0
    ble @L_800441E8
    fmr f1, f0
    @L_800441E8:
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    bge @L_800441F8
    fmr f1, f0
    @L_800441F8:
    fctiwz f0, f1
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x67(r31)
    lwz r0, 0x28(r3)
    cmpwi r0, 0x0
    bne @L_80044230
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_80044360
    @L_80044230:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_80044360
    @L_80044244:
    li r3, 0x3717
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r31)
    extsh r4, r3
    lis r3, lbl_803A6818@ha
    subf r6, r4, r0
    li r4, 0x0
    addi r3, r3, lbl_803A6818@l
    lwz r0, 0x28(r3)
    srwi r5, r6, 31
    add r3, r5, r6
    srawi r3, r3, 1
    cmpwi r0, 0x0
    extsh r3, r3
    bne @L_8004428C
    lis r5, 0x14
    b @L_80044290
    @L_8004428C:
    lis r5, 0x4a
    @L_80044290:
    lbz r0, 0x8b(r30)
    li r6, 0x3717
    or r5, r5, r0
    bl fn_800FB680
    b @L_80044360
    @L_800442A4:
    li r3, 0x3718
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r31)
    extsh r4, r3
    lis r3, lbl_803A6818@ha
    subf r6, r4, r0
    li r4, 0x0
    addi r3, r3, lbl_803A6818@l
    lwz r0, 0x28(r3)
    srwi r5, r6, 31
    add r3, r5, r6
    srawi r3, r3, 1
    cmpwi r0, 0x1
    extsh r3, r3
    bne @L_800442EC
    lis r5, 0x14
    b @L_800442F0
    @L_800442EC:
    lis r5, 0x4a
    @L_800442F0:
    lbz r0, 0x8b(r30)
    li r6, 0x3718
    or r5, r5, r0
    bl fn_800FB680
    b @L_80044360
    @L_80044304:
    li r3, 0x3719
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r31)
    extsh r4, r3
    lis r3, lbl_803A6818@ha
    subf r6, r4, r0
    li r4, 0x0
    addi r3, r3, lbl_803A6818@l
    lwz r0, 0x28(r3)
    srwi r5, r6, 31
    add r3, r5, r6
    srawi r3, r3, 1
    cmpwi r0, 0x2
    extsh r3, r3
    bne @L_8004434C
    lis r5, 0x14
    b @L_80044350
    @L_8004434C:
    lis r5, 0x4a
    @L_80044350:
    lbz r0, 0x8b(r30)
    li r6, 0x3719
    or r5, r5, r0
    bl fn_800FB680
    @L_80044360:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
