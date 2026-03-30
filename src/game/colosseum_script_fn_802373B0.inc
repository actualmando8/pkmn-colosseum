    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stmw r28, 0x20(r1)
    fmr f31, f1
    mr r28, r3
    mr r29, r5
    mr r3, r4
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r30, r3
    mr r3, r28
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r28
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r30
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r31, r3, 0, 16, 31
    mr r3, r28
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r30
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    bl fn_8012640C
    lfs f0, -20612(r2)
    rlwinm r0, r3, 0, 16, 31
    fcmpu cr0, f0, f31
    bne @L_802374AC
    lfs f31, -20616(r2)
@L_802374AC:
    lis r3, 0x4330
    stw r0, 0xc(r1)
    lfd f1, -20608(r2)
    extsb. r0, r29
    stw r3, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f1
    fmuls f0, f0, f31
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r3, 0x14(r1)
    bne @L_802374EC
    cmpw r31, r3
    bne @L_8023751C
    li r3, 0x1
    b @L_80237520
@L_802374EC:
    extsb. r0, r29
    ble @L_80237504
    cmpw r31, r3
    blt @L_8023751C
    li r3, 0x1
    b @L_80237520
@L_80237504:
    extsb. r0, r29
    bge @L_8023751C
    cmpw r31, r3
    bgt @L_8023751C
    li r3, 0x1
    b @L_80237520
@L_8023751C:
    li r3, 0x0
@L_80237520:
    psq_l f31, 0x38(r1), 0, 0
    lfd f31, 0x30(r1)
    lmw r28, 0x20(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
