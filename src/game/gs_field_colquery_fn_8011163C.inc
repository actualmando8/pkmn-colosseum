    nofralloc
    stwu r1, -0x160(r1)
    mflr r0
    stw r0, 0x164(r1)
    stfd f31, 0x150(r1)
    psq_st f31, 0x158(r1), 0, 0
    stfd f30, 0x140(r1)
    psq_st f30, 0x148(r1), 0, 0
    stfd f29, 0x130(r1)
    psq_st f29, 0x138(r1), 0, 0
    stfd f28, 0x120(r1)
    psq_st f28, 0x128(r1), 0, 0
    stfd f27, 0x110(r1)
    psq_st f27, 0x118(r1), 0, 0
    stmw r27, 0xfc(r1)
    fmr f27, f1
    mr r29, r3
    mr r27, r4
    mr r30, r5
    li r31, 0x0
    li r28, 0x0
    bl fn_8010CBC0
    cmplwi r3, 0x0
    bne @L_801116A0
    li r3, 0x0
    b @L_80111828
@L_801116A0:
    mr r3, r27
    mr r4, r29
    addi r5, r1, 0x14
    bl fn_800A3A9C
    addi r3, r1, 0x14
    bl fn_800A3B38
    lfs f0, -26440(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801116D0
    li r3, 0x0
    b @L_80111828
@L_801116D0:
    fdivs f28, f27, f1
    lfs f0, -26436(r2)
    fcmpo cr0, f28, f0
    ble @L_801116E4
    fmr f28, f0
@L_801116E4:
    lfs f29, -26440(r2)
    lfs f30, -26436(r2)
    fmr f31, f29
    b @L_80111814
@L_801116F4:
    fadds f1, f29, f28
    fcmpo cr0, f1, f30
    ble @L_80111704
    fmr f1, f30
@L_80111704:
    addi r3, r1, 0x14
    addi r4, r1, 0x8
    bl fn_800A3AC0
    addi r3, r1, 0x8
    mr r4, r29
    mr r5, r3
    bl fn_800A3A78
    fmr f1, f27
    addi r3, r1, 0x8
    addi r4, r1, 0x14
    addi r5, r1, 0x20
    bl fn_80111470
    addi r6, r1, 0x20
    li r7, 0x0
    b @L_801117F4
@L_80111740:
    mr r5, r30
    li r8, 0x0
    mtctr r31
    cmpwi r31, 0x0
    ble @L_80111770
@L_80111754:
    lhz r4, 0x30(r6)
    lhz r0, 0x30(r5)
    cmplw r4, r0
    beq @L_80111770
    addi r5, r5, 0x34
    addi r8, r8, 0x1
    bdnz @L_80111754
@L_80111770:
    cmpw r8, r31
    blt @L_801117EC
    lwz r4, 0x0(r6)
    add r5, r30, r28
    lwz r0, 0x4(r6)
    addi r31, r31, 0x1
    addi r28, r28, 0x34
    stw r4, 0x0(r5)
    stw r0, 0x4(r5)
    lwz r4, 0x8(r6)
    lwz r0, 0xc(r6)
    stw r4, 0x8(r5)
    stw r0, 0xc(r5)
    lwz r4, 0x10(r6)
    lwz r0, 0x14(r6)
    stw r4, 0x10(r5)
    stw r0, 0x14(r5)
    lwz r4, 0x18(r6)
    lwz r0, 0x1c(r6)
    stw r4, 0x18(r5)
    stw r0, 0x1c(r5)
    lwz r4, 0x20(r6)
    lwz r0, 0x24(r6)
    stw r4, 0x20(r5)
    stw r0, 0x24(r5)
    lwz r4, 0x28(r6)
    lwz r0, 0x2c(r6)
    stw r4, 0x28(r5)
    stw r0, 0x2c(r5)
    lwz r0, 0x30(r6)
    stw r0, 0x30(r5)
@L_801117EC:
    addi r6, r6, 0x34
    addi r7, r7, 0x1
@L_801117F4:
    cmpw r7, r3
    bge @L_80111804
    cmpwi r31, 0x4
    blt @L_80111740
@L_80111804:
    fcmpo cr0, f28, f31
    cror eq, lt, eq
    beq @L_80111824
    fadds f29, f29, f28
@L_80111814:
    fcmpo cr0, f29, f30
    bge @L_80111824
    cmpwi r31, 0x4
    blt @L_801116F4
@L_80111824:
    mr r3, r31
@L_80111828:
    psq_l f31, 0x158(r1), 0, 0
    lfd f31, 0x150(r1)
    psq_l f30, 0x148(r1), 0, 0
    lfd f30, 0x140(r1)
    psq_l f29, 0x138(r1), 0, 0
    lfd f29, 0x130(r1)
    psq_l f28, 0x128(r1), 0, 0
    lfd f28, 0x120(r1)
    psq_l f27, 0x118(r1), 0, 0
    lfd f27, 0x110(r1)
    lmw r27, 0xfc(r1)
    lwz r0, 0x164(r1)
    mtlr r0
    addi r1, r1, 0x160
    blr

