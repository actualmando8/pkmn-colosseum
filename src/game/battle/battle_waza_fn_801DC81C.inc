    nofralloc
    stwu r1, -0x110(r1)
    mflr r0
    stw r0, 0x114(r1)
    li r0, 0x0
    stw r31, 0x10c(r1)
    li r31, 0x0
    stw r30, 0x108(r1)
    mr r30, r4
    stw r29, 0x104(r1)
    mr r29, r3
    stw r28, 0x100(r1)
    stw r0, 0x78(r3)
    bl fn_800D37CC
    lwz r0, 0x4(r30)
    xoris r3, r3, 0x8000
    lis r6, 0x4330
    stw r3, 0xe4(r1)
    xoris r5, r0, 0x8000
    lfd f3, -21240(r2)
    stw r6, 0xe0(r1)
    li r0, 0x0
    lfs f0, -21248(r2)
    mr r4, r30
    lfd f1, 0xe0(r1)
    addi r3, r1, 0x8
    stw r5, 0xec(r1)
    fsubs f2, f1, f3
    stw r6, 0xe8(r1)
    lfd f1, 0xe8(r1)
    fsubs f1, f1, f3
    fmuls f1, f1, f2
    fdivs f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0xf0(r1)
    lwz r5, 0xf4(r1)
    stw r5, 0x7c(r29)
    lwz r5, 0x0(r30)
    stw r5, 0x88(r29)
    stw r0, 0x80(r29)
    stw r0, 0x84(r29)
    bl fn_801364A8
    lwz r0, 0x8(r1)
    mr r30, r3
    cmplwi r0, 0xc
    bgt @L_801DCA60
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, 10108
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lwz r28, 0x60(r1)
    addi r3, r1, 0x10
    bl fn_8013AABC
    stw r3, 0x78(r29)
    b @L_801DC908
@L_801DC8FC:
    lwz r0, 0x8(r28)
    lwz r28, 0x10(r28)
    add r31, r31, r0
@L_801DC908:
    cmplwi r28, 0x0
    bne @L_801DC8FC
    b @L_801DCA60
    lwz r0, 0x60(r1)
    addi r3, r1, 0x10
    stw r0, 0x80(r29)
    bl fn_80139820
    stw r3, 0x78(r29)
    lhz r31, 0x5a(r1)
    b @L_801DCA60
    addi r3, r1, 0x10
    bl fn_8013A42C
    stw r3, 0x78(r29)
    lhz r31, 0x1a(r1)
    b @L_801DCA60
    lhz r0, 0x1e(r1)
    addi r3, r1, 0x10
    stw r0, 0x80(r29)
    bl fn_801380D4
    stw r3, 0x78(r29)
    lhz r31, 0x22(r1)
    b @L_801DCA60
    lhz r0, 0x38(r1)
    addi r3, r1, 0x10
    stw r0, 0x80(r29)
    lhz r0, 0x3a(r1)
    stw r0, 0x84(r29)
    bl fn_8013796C
    stw r3, 0x78(r29)
    lhz r31, 0x30(r1)
    b @L_801DCA60
    lha r0, 0x5a(r1)
    addi r3, r1, 0x10
    stw r0, 0x80(r29)
    bl fn_80138B00
    stw r3, 0x78(r29)
    lhz r31, 0x4e(r1)
    b @L_801DCA60
    addi r3, r1, 0x10
    bl fn_8013C5A0
    stw r3, 0x78(r29)
    lhz r31, 0xb6(r1)
    b @L_801DCA60
    addi r3, r1, 0x10
    bl fn_8013B490
    stw r3, 0x78(r29)
    lhz r31, 0xde(r1)
    b @L_801DCA60
    addi r3, r1, 0x10
    bl fn_8013D6B8
    stw r3, 0x78(r29)
    li r31, 0x0
    lwz r3, 0x1c(r1)
    b @L_801DC9EC
@L_801DC9E0:
    lwz r0, 0x8(r3)
    lwz r3, 0x10(r3)
    add r31, r31, r0
@L_801DC9EC:
    cmplwi r3, 0x0
    bne @L_801DC9E0
    b @L_801DCA60
    addi r3, r1, 0x10
    bl fn_8013DC18
    stw r3, 0x78(r29)
    li r31, 0x0
    lwz r3, 0x24(r1)
    b @L_801DCA1C
@L_801DCA10:
    lwz r0, 0x8(r3)
    lwz r3, 0x10(r3)
    add r31, r31, r0
@L_801DCA1C:
    cmplwi r3, 0x0
    bne @L_801DCA10
    b @L_801DCA60
    addi r3, r1, 0x10
    bl fn_8013E4D4
    stw r3, 0x78(r29)
    lhz r31, 0x42(r1)
    b @L_801DCA60
    addi r3, r1, 0x10
    bl fn_8013FBE0
    stw r3, 0x78(r29)
    li r31, 0x0
    b @L_801DCA60
    addi r3, r1, 0x10
    bl fn_8013F000
    stw r3, 0x78(r29)
    lhz r31, 0xc2(r1)
@L_801DCA60:
    lwz r0, 0x14(r29)
    rlwinm r0, r0, 2, 0, 29
    add r3, r29, r0
    lwz r0, 0x28(r3)
    cmpwi r0, 0x0
    bne @L_801DCA7C
    stw r31, 0x28(r3)
@L_801DCA7C:
    lwz r0, 0x114(r1)
    mr r3, r30
    lwz r31, 0x10c(r1)
    lwz r30, 0x108(r1)
    lwz r29, 0x104(r1)
    lwz r28, 0x100(r1)
    mtlr r0
    addi r1, r1, 0x110
    blr
