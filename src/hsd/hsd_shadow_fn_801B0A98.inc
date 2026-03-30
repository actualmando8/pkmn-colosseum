    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stw r31, 0x3c(r1)
    stw r30, 0x38(r1)
    mr. r30, r3
    fmr f31, f1
    mr r31, r4
    bne @L_801B0AD8
    lis r3, 0x8027
    li r4, 0x24d
    addi r3, r3, 21184
    addi r5, r2, -22740
    bl fn_80196E10
@L_801B0AD8:
    cmplwi r31, 0x0
    bne @L_801B0AF4
    lis r3, 0x8027
    li r4, 0x24e
    addi r3, r3, 21184
    addi r5, r2, -22720
    bl fn_80196E10
@L_801B0AF4:
    lhz r0, 0x8(r31)
    clrlwi r0, r0, 30
    cmpwi r0, 0x1
    beq @L_801B0B14
    blt @L_801B0BA4
    cmpwi r0, 0x4
    bge @L_801B0BA4
    b @L_801B0B88
@L_801B0B14:
    lfs f0, -22752(r2)
    fcmpo cr0, f31, f0
    bgt @L_801B0B38
    lis r3, 0x8027
    lis r5, 0x8027
    addi r3, r3, 21184
    li r4, 0x252
    addi r5, r5, 21236
    bl fn_80196E10
@L_801B0B38:
    lwz r3, 0x4(r30)
    addi r4, r1, 0x14
    bl fn_801959DC
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_801A497C
    addi r3, r1, 0x8
    bl fn_800A3B38
    fdivs f1, f31, f1
    addi r3, r1, 0x8
    addi r4, r1, 0x8
    bl fn_800A3AC0
    addi r3, r1, 0x14
    addi r4, r1, 0x8
    addi r5, r1, 0x20
    bl fn_800A3A78
    lwz r3, 0x4(r30)
    addi r4, r1, 0x20
    bl fn_80195898
    b @L_801B0BB8
@L_801B0B88:
    mr r3, r31
    addi r4, r1, 0x20
    bl fn_801A497C
    lwz r3, 0x4(r30)
    addi r4, r1, 0x20
    bl fn_80195898
    b @L_801B0BB8
@L_801B0BA4:
    lis r3, 0x8027
    li r4, 0x262
    addi r3, r3, 21184
    addi r5, r2, -22732
    bl fn_80196E10
@L_801B0BB8:
    psq_l f31, 0x48(r1), 0, 0
    lwz r0, 0x54(r1)
    lfd f31, 0x40(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
