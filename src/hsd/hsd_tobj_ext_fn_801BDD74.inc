    nofralloc
    stwu r1, -0x160(r1)
    mflr r0
    stw r0, 0x164(r1)
    stw r31, 0x15c(r1)
    stw r30, 0x158(r1)
    mr r30, r3
    stw r29, 0x154(r1)
    lwz r0, 0x4c(r3)
    rlwinm r0, r0, 0, 28, 31
    cmplwi r0, 0x4
    beq @L_801BE298
    lwz r0, 0x4c(r30)
    rlwinm. r0, r0, 0, 0, 0
    beq @L_801BDDCC
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x3c(r4)
    mtctr r12
    bctrl
    lwz r0, 0x4c(r30)
    rlwinm r0, r0, 0, 1, 31
    stw r0, 0x4c(r30)
@L_801BDDCC:
    lwz r0, 0x4c(r30)
    rlwinm r0, r0, 0, 28, 31
    cmpwi r0, 0x4
    beq @L_801BE268
    bge @L_801BDDF8
    cmpwi r0, 0x2
    beq @L_801BE0C8
    bge @L_801BE23C
    cmpwi r0, 0x1
    bge @L_801BDE08
    b @L_801BE268
@L_801BDDF8:
    cmpwi r0, 0x6
    beq @L_801BE05C
    bge @L_801BE268
    b @L_801BDED4
@L_801BDE08:
    li r12, 0x0
    b @L_801BDEB8
@L_801BDE10:
    rlwinm r3, r12, 4, 0, 27
    rlwinm r5, r12, 4, 0, 27
    add r4, r30, r3
    rlwinm r6, r12, 4, 0, 27
    lfs f1, -22472(r2)
    rlwinm r7, r12, 4, 0, 27
    lfs f0, 0x74(r4)
    rlwinm r0, r12, 4, 0, 27
    add r4, r30, r0
    rlwinm r9, r12, 4, 0, 27
    fmuls f0, f1, f0
    addi r8, r1, 0x118
    lfs f3, -22468(r2)
    rlwinm r11, r12, 4, 0, 27
    addi r10, r1, 0x11c
    rlwinm r3, r12, 4, 0, 27
    stfsx f0, r8, r9
    rlwinm r9, r12, 4, 0, 27
    rlwinm r0, r12, 4, 0, 27
    lfs f0, -22464(r2)
    lfs f2, 0x78(r4)
    add r4, r30, r3
    addi r8, r1, 0x120
    lfs f1, -22472(r2)
    fmuls f2, f3, f2
    add r5, r30, r5
    lfs f3, -22472(r2)
    add r6, r30, r6
    add r7, r30, r7
    addi r3, r1, 0x124
    stfsx f2, r10, r11
    addi r12, r12, 0x1
    stfsx f0, r8, r9
    lfs f0, 0x78(r4)
    lfs f2, 0x74(r5)
    fmuls f0, f1, f0
    lfs f1, 0x7c(r6)
    lfs f4, 0x80(r7)
    fmadds f0, f3, f2, f0
    fadds f0, f1, f0
    fadds f0, f4, f0
    stfsx f0, r3, r0
@L_801BDEB8:
    cmpwi r12, 0x3
    blt @L_801BDE10
    lwz r4, 0x14(r30)
    addi r3, r1, 0x118
    li r5, 0x0
    bl fn_800BD58C
    b @L_801BE298
@L_801BDED4:
    bl fn_801942B8
    li r12, 0x0
    mr r31, r3
    b @L_801BDF8C
@L_801BDEE4:
    rlwinm r3, r12, 4, 0, 27
    rlwinm r5, r12, 4, 0, 27
    add r4, r30, r3
    rlwinm r6, r12, 4, 0, 27
    lfs f1, -22472(r2)
    rlwinm r7, r12, 4, 0, 27
    lfs f0, 0x74(r4)
    rlwinm r0, r12, 4, 0, 27
    add r4, r30, r0
    rlwinm r9, r12, 4, 0, 27
    fmuls f0, f1, f0
    addi r8, r1, 0xe8
    lfs f3, -22468(r2)
    rlwinm r11, r12, 4, 0, 27
    addi r10, r1, 0xec
    rlwinm r3, r12, 4, 0, 27
    stfsx f0, r8, r9
    rlwinm r9, r12, 4, 0, 27
    rlwinm r0, r12, 4, 0, 27
    lfs f0, -22464(r2)
    lfs f2, 0x78(r4)
    add r4, r30, r3
    addi r8, r1, 0xf0
    lfs f1, -22472(r2)
    fmuls f2, f3, f2
    add r5, r30, r5
    lfs f3, -22472(r2)
    add r6, r30, r6
    add r7, r30, r7
    addi r3, r1, 0xf4
    stfsx f2, r10, r11
    addi r12, r12, 0x1
    stfsx f0, r8, r9
    lfs f0, 0x78(r4)
    lfs f2, 0x74(r5)
    fmuls f0, f1, f0
    lfs f1, 0x7c(r6)
    lfs f4, 0x80(r7)
    fmadds f0, f3, f2, f0
    fadds f0, f1, f0
    fadds f0, f4, f0
    stfsx f0, r3, r0
@L_801BDF8C:
    cmpwi r12, 0x3
    blt @L_801BDEE4
    mr r3, r31
    bl fn_801943CC
    cmpwi r3, 0x2
    beq @L_801BDFDC
    bge @L_801BE010
    cmpwi r3, 0x1
    bge @L_801BDFB8
    b @L_801BE010
    b @L_801BE010
@L_801BDFB8:
    lfs f1, 0x40(r31)
    addi r3, r1, 0xb8
    lfs f2, 0x44(r31)
    lfs f3, -22472(r2)
    lfs f4, -22468(r2)
    lfs f5, -22472(r2)
    lfs f6, -22472(r2)
    bl fn_800A3678
    b @L_801BE038
@L_801BDFDC:
    lfs f0, -22472(r2)
    addi r3, r1, 0xb8
    lfs f6, -22472(r2)
    stfs f0, 0x8(r1)
    lfs f7, -22468(r2)
    lfs f1, 0x40(r31)
    lfs f2, 0x44(r31)
    lfs f3, 0x48(r31)
    lfs f4, 0x4c(r31)
    lfs f5, 0x38(r31)
    lfs f8, -22472(r2)
    bl fn_800A35E4
    b @L_801BE038
@L_801BE010:
    lfs f1, 0x40(r31)
    addi r3, r1, 0xb8
    lfs f2, 0x44(r31)
    lfs f3, 0x48(r31)
    lfs f4, 0x4c(r31)
    lfs f5, -22472(r2)
    lfs f6, -22468(r2)
    lfs f7, -22472(r2)
    lfs f8, -22472(r2)
    bl fn_800A3744
@L_801BE038:
    addi r3, r1, 0xe8
    addi r4, r1, 0xb8
    addi r5, r1, 0xe8
    bl fn_800A2D98
    lwz r4, 0x14(r30)
    addi r3, r1, 0xe8
    li r5, 0x0
    bl fn_800BD58C
    b @L_801BE298
@L_801BE05C:
    lfs f1, -22464(r2)
    addi r3, r1, 0x88
    lfs f9, -22460(r2)
    li r5, 0x0
    lfs f8, -22464(r2)
    lfs f7, -22464(r2)
    lfs f6, -22464(r2)
    lfs f5, -22464(r2)
    lfs f4, -22460(r2)
    lfs f3, -22464(r2)
    lfs f2, -22464(r2)
    lfs f0, -22460(r2)
    stfs f9, 0x88(r1)
    stfs f8, 0x8c(r1)
    stfs f7, 0x90(r1)
    stfs f6, 0x94(r1)
    stfs f5, 0x98(r1)
    stfs f4, 0x9c(r1)
    stfs f3, 0xa0(r1)
    stfs f2, 0xa4(r1)
    stfs f1, 0xb0(r1)
    stfs f1, 0xac(r1)
    stfs f1, 0xa8(r1)
    stfs f0, 0xb4(r1)
    lwz r4, 0x14(r30)
    bl fn_800BD58C
    b @L_801BE298
@L_801BE0C8:
    li r3, 0x1
    bl fn_801A4AC4
    mr. r31, r3
    beq @L_801BE224
    bl fn_801942B8
    mr. r29, r3
    bne @L_801BE0F4
    addi r3, r2, -22512
    li r4, 0x312
    addi r5, r2, -22456
    bl fn_80196E10
@L_801BE0F4:
    addi r0, r29, 0x54
    mr r3, r31
    mr r29, r0
    addi r4, r1, 0x1c
    bl fn_801A620C
    mr r3, r29
    addi r4, r1, 0x1c
    addi r5, r1, 0x1c
    bl fn_800A3820
    lfs f1, 0x24(r1)
    addi r3, r1, 0x1c
    lfs f0, -22448(r2)
    addi r4, r1, 0x10
    fadds f0, f1, f0
    stfs f0, 0x24(r1)
    bl fn_800A3ADC
    lfs f1, 0x10(r1)
    addi r3, r1, 0x58
    lfd f0, -22440(r2)
    li r5, 0x0
    lfs f3, 0x14(r1)
    lfd f2, -22440(r2)
    fmul f5, f1, f0
    lfs f1, 0x18(r1)
    lfd f0, -22440(r2)
    fmul f3, f3, f2
    lfs f8, -22472(r2)
    fmul f2, f1, f0
    lfs f4, -22472(r2)
    frsp f6, f5
    lfs f1, -22464(r2)
    frsp f5, f3
    lfs f0, -22460(r2)
    stfs f6, 0x10(r1)
    frsp f3, f2
    stfs f5, 0x14(r1)
    lfs f2, 0x10(r1)
    stfs f3, 0x18(r1)
    lfs f7, 0x14(r1)
    lfs f3, 0x74(r30)
    lfs f6, 0x18(r1)
    fmuls f9, f3, f2
    lfs f5, 0x10(r1)
    lfs f3, 0x14(r1)
    lfs f2, 0x18(r1)
    stfs f9, 0x58(r1)
    lfs f9, 0x74(r30)
    fmuls f7, f9, f7
    stfs f7, 0x5c(r1)
    lfs f7, 0x74(r30)
    fmuls f6, f7, f6
    stfs f6, 0x60(r1)
    lfs f7, 0x74(r30)
    lfs f6, 0x80(r30)
    fmadds f6, f8, f7, f6
    stfs f6, 0x64(r1)
    lfs f6, 0x84(r30)
    fmuls f5, f6, f5
    stfs f5, 0x68(r1)
    lfs f5, 0x84(r30)
    fmuls f3, f5, f3
    stfs f3, 0x6c(r1)
    lfs f3, 0x84(r30)
    fmuls f2, f3, f2
    stfs f2, 0x70(r1)
    lfs f3, 0x84(r30)
    lfs f2, 0x90(r30)
    fmadds f2, f4, f3, f2
    stfs f1, 0x80(r1)
    stfs f1, 0x7c(r1)
    stfs f2, 0x74(r1)
    stfs f1, 0x78(r1)
    stfs f0, 0x84(r1)
    lwz r4, 0x14(r30)
    bl fn_800BD58C
    b @L_801BE298
@L_801BE224:
    lis r3, lbl_8036D43C@ha
    lwz r4, 0x14(r30)
    addi r3, r3, lbl_8036D43C@l
    li r5, 0x0
    bl fn_800BD58C
    b @L_801BE298
@L_801BE23C:
    bl fn_801942B8
    bl fn_80194CF4
    mr r4, r3
    addi r3, r30, 0x74
    addi r5, r1, 0x28
    bl fn_800A2D98
    lwz r4, 0x14(r30)
    addi r3, r1, 0x28
    li r5, 0x0
    bl fn_800BD58C
    b @L_801BE298
@L_801BE268:
    lwz r0, 0x4c(r30)
    rlwinm. r0, r0, 0, 7, 7
    beq @L_801BE288
    lwz r4, 0x14(r30)
    addi r3, r30, 0x74
    li r5, 0x1
    bl fn_800BD58C
    b @L_801BE298
@L_801BE288:
    lwz r4, 0x14(r30)
    addi r3, r30, 0x74
    li r5, 0x0
    bl fn_800BD58C
@L_801BE298:
    lwz r0, 0x164(r1)
    lwz r31, 0x15c(r1)
    lwz r30, 0x158(r1)
    lwz r29, 0x154(r1)
    mtlr r0
    addi r1, r1, 0x160
    blr

