    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stfd f30, 0x50(r1)
    psq_st f30, 0x58(r1), 0, 0
    stfd f29, 0x40(r1)
    psq_st f29, 0x48(r1), 0, 0
    stfd f28, 0x30(r1)
    psq_st f28, 0x38(r1), 0, 0
    stfd f27, 0x20(r1)
    psq_st f27, 0x28(r1), 0, 0
    stfd f26, 0x10(r1)
    psq_st f26, 0x18(r1), 0, 0
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    fmr f26, f1
    lis r3, 0x8027
    fmr f27, f2
    fmr f28, f3
    addi r31, r3, 21184
    fmr f29, f4
    bne @L_801B0554
    addi r3, r31, 0x0
    li r4, 0x31c
    addi r5, r2, -22740
    bl fn_80196E10
@L_801B0554:
    lwz r30, 0x4(r30)
    mr r3, r30
    bl fn_80195794
    fmr f31, f1
    lfs f0, -22752(r2)
    fcmpo cr0, f31, f0
    bgt @L_801B0580
    addi r3, r31, 0x0
    addi r5, r31, 0x34
    li r4, 0x320
    bl fn_80196E10
@L_801B0580:
    mr r3, r30
    bl fn_801943CC
    cmpwi r3, 0x2
    beq @L_801B0638
    bge @L_801B05A0
    cmpwi r3, 0x1
    bge @L_801B05AC
    b @L_801B067C
@L_801B05A0:
    cmpwi r3, 0x4
    bge @L_801B067C
    b @L_801B061C
@L_801B05AC:
    fabs f0, f27
    fabs f1, f26
    fcmpo cr0, f1, f0
    ble @L_801B05C8
    fabs f0, f26
    frsp f2, f0
    b @L_801B05D0
@L_801B05C8:
    fabs f0, f27
    frsp f2, f0
@L_801B05D0:
    fabs f0, f29
    fabs f1, f28
    fcmpo cr0, f1, f0
    ble @L_801B05EC
    fabs f0, f28
    frsp f30, f0
    b @L_801B05F4
@L_801B05EC:
    fabs f0, f29
    frsp f30, f0
@L_801B05F4:
    fdivs f1, f30, f2
    mr r3, r30
    bl fn_80194788
    fmr f1, f30
    fmr f2, f31
    bl fn_800CE2D8
    frsp f1, f1
    mr r3, r30
    bl fn_801947A8
    b @L_801B068C
@L_801B061C:
    fmr f1, f26
    mr r3, r30
    fmr f2, f27
    fmr f3, f28
    fmr f4, f29
    bl fn_80194358
    b @L_801B068C
@L_801B0638:
    mr r3, r30
    bl fn_801944F8
    fdivs f30, f1, f31
    lfs f0, -22752(r2)
    fcmpo cr0, f30, f0
    bgt @L_801B0660
    addi r3, r31, 0x0
    addi r5, r31, 0x44
    li r4, 0x33d
    bl fn_80196E10
@L_801B0660:
    fmuls f1, f30, f26
    mr r3, r30
    fmuls f2, f30, f27
    fmuls f3, f30, f28
    fmuls f4, f30, f29
    bl fn_8019437C
    b @L_801B068C
@L_801B067C:
    addi r3, r31, 0x0
    li r4, 0x345
    addi r5, r2, -22732
    bl fn_80196E10
@L_801B068C:
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    psq_l f30, 0x58(r1), 0, 0
    lfd f30, 0x50(r1)
    psq_l f29, 0x48(r1), 0, 0
    lfd f29, 0x40(r1)
    psq_l f28, 0x38(r1), 0, 0
    lfd f28, 0x30(r1)
    psq_l f27, 0x28(r1), 0, 0
    lfd f27, 0x20(r1)
    psq_l f26, 0x18(r1), 0, 0
    lfd f26, 0x10(r1)
    lwz r31, 0xc(r1)
    lwz r0, 0x74(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
