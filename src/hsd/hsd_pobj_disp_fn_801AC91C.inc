    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    rlwinm r4, r4, 2, 0, 29
    stw r0, 0x54(r1)
    stw r31, 0x4c(r1)
    mr r31, r6
    stw r30, 0x48(r1)
    mr r30, r3
    stw r29, 0x44(r1)
    lwz r3, 0x8(r3)
    lwz r7, 0xc(r30)
    lwz r0, 0x4(r3)
    lwzx r6, r7, r4
    cmpwi r0, 0x3
    bne @L_801AC978
    rlwinm r4, r5, 1, 0, 30
    rlwinm r0, r5, 1, 0, 30
    add r3, r6, r0
    lbzx r29, r6, r4
    lbz r0, 0x1(r3)
    rlwinm r3, r29, 8, 0, 23
    add r29, r3, r0
    b @L_801AC97C
@L_801AC978:
    lbzx r29, r6, r5
@L_801AC97C:
    lwz r3, 0x8(r30)
    lwz r0, 0x8(r3)
    cmpwi r0, 0x1
    beq @L_801AC9A0
    lis r4, lbl_80275084@ha
    addi r3, r2, -23016
    addi r5, r4, lbl_80275084@l
    li r4, 0x43a
    bl fn_80196E10
@L_801AC9A0:
    lwz r4, 0x8(r30)
    lwz r3, 0x8(r30)
    lhz r0, 0x12(r4)
    lwz r5, 0x8(r30)
    mullw r4, r29, r0
    lwz r0, 0xc(r3)
    lwz r3, 0x14(r5)
    cmpwi r0, 0x4
    add r4, r3, r4
    bne @L_801AC9D8
    mr r3, r31
    li r5, 0xc
    bl memcpy
    b @L_801ACD60
@L_801AC9D8:
    lwz r3, 0x8(r30)
    li r6, 0x1
    lwz r5, 0x8(r30)
    lwz r3, 0xc(r3)
    lbz r0, 0x10(r5)
    cmpwi r3, 0x2
    slw r0, r6, r0
    beq @L_801ACBB8
    bge @L_801ACA0C
    cmpwi r3, 0x0
    beq @L_801ACA18
    bge @L_801ACADC
    b @L_801ACD4C
@L_801ACA0C:
    cmpwi r3, 0x4
    bge @L_801ACD4C
    b @L_801ACC7C
@L_801ACA18:
    lbz r3, 0x0(r4)
    lis r5, 0x4330
    stw r5, 0x8(r1)
    xoris r10, r0, 0x8000
    lis r9, 0x4330
    xoris r7, r0, 0x8000
    stw r3, 0xc(r1)
    xoris r3, r0, 0x8000
    lfd f2, -22936(r2)
    lis r6, 0x4330
    lfd f0, 0x8(r1)
    lis r8, 0x4330
    stw r10, 0x14(r1)
    lis r0, 0x4330
    fsubs f4, f0, f2
    lfd f1, -22944(r2)
    stw r9, 0x10(r1)
    lis r5, 0x4330
    lfd f3, -22944(r2)
    lfd f0, 0x10(r1)
    stw r7, 0x24(r1)
    fsubs f0, f0, f1
    lfd f5, -22936(r2)
    stw r6, 0x20(r1)
    lfd f1, -22944(r2)
    fdivs f4, f4, f0
    lfd f0, 0x20(r1)
    stw r3, 0x34(r1)
    lfd f2, -22936(r2)
    stw r8, 0x18(r1)
    stw r0, 0x30(r1)
    stfs f4, 0x0(r31)
    fsubs f3, f0, f3
    lfd f0, 0x30(r1)
    lbz r3, 0x1(r4)
    fsubs f0, f0, f1
    stw r5, 0x28(r1)
    stw r3, 0x1c(r1)
    lfd f4, 0x18(r1)
    fsubs f1, f4, f5
    fdivs f1, f1, f3
    stfs f1, 0x4(r31)
    lbz r0, 0x2(r4)
    stw r0, 0x2c(r1)
    lfd f1, 0x28(r1)
    fsubs f1, f1, f2
    fdivs f0, f1, f0
    stfs f0, 0x8(r31)
    b @L_801ACD60
@L_801ACADC:
    lbz r3, 0x0(r4)
    lis r11, 0x4330
    xoris r10, r0, 0x8000
    lis r9, 0x4330
    extsb r3, r3
    xoris r7, r0, 0x8000
    xoris r3, r3, 0x8000
    lis r6, 0x4330
    stw r3, 0x34(r1)
    xoris r3, r0, 0x8000
    lis r0, 0x4330
    lis r8, 0x4330
    stw r11, 0x30(r1)
    lis r5, 0x4330
    lfd f2, -22944(r2)
    lfd f0, 0x30(r1)
    stw r10, 0x2c(r1)
    fsubs f5, f0, f2
    lfd f1, -22944(r2)
    stw r9, 0x28(r1)
    lfd f3, -22944(r2)
    lfd f0, 0x28(r1)
    stw r7, 0x1c(r1)
    fsubs f0, f0, f1
    lfd f1, -22944(r2)
    stw r6, 0x18(r1)
    lfd f4, -22944(r2)
    fdivs f5, f5, f0
    lfd f0, 0x18(r1)
    stw r3, 0xc(r1)
    lfd f2, -22944(r2)
    stw r0, 0x8(r1)
    stw r8, 0x20(r1)
    stfs f5, 0x0(r31)
    fsubs f3, f0, f3
    lfd f0, 0x8(r1)
    lbz r3, 0x1(r4)
    fsubs f0, f0, f1
    stw r5, 0x10(r1)
    extsb r0, r3
    xoris r0, r0, 0x8000
    stw r0, 0x24(r1)
    lfd f1, 0x20(r1)
    fsubs f1, f1, f4
    fdivs f1, f1, f3
    stfs f1, 0x4(r31)
    lbz r0, 0x2(r4)
    extsb r0, r0
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f1, 0x10(r1)
    fsubs f1, f1, f2
    fdivs f0, f1, f0
    stfs f0, 0x8(r31)
    b @L_801ACD60
@L_801ACBB8:
    lhz r3, 0x0(r4)
    lis r5, 0x4330
    stw r5, 0x30(r1)
    xoris r10, r0, 0x8000
    lis r9, 0x4330
    xoris r7, r0, 0x8000
    stw r3, 0x34(r1)
    xoris r3, r0, 0x8000
    lfd f2, -22936(r2)
    lis r6, 0x4330
    lfd f0, 0x30(r1)
    lis r8, 0x4330
    stw r10, 0x2c(r1)
    lis r0, 0x4330
    fsubs f4, f0, f2
    lfd f1, -22944(r2)
    stw r9, 0x28(r1)
    lis r5, 0x4330
    lfd f3, -22944(r2)
    lfd f0, 0x28(r1)
    stw r7, 0x1c(r1)
    fsubs f0, f0, f1
    lfd f5, -22936(r2)
    stw r6, 0x18(r1)
    lfd f1, -22944(r2)
    fdivs f4, f4, f0
    lfd f0, 0x18(r1)
    stw r3, 0xc(r1)
    lfd f2, -22936(r2)
    stw r8, 0x20(r1)
    stw r0, 0x8(r1)
    stfs f4, 0x0(r31)
    fsubs f3, f0, f3
    lfd f0, 0x8(r1)
    lhz r3, 0x2(r4)
    fsubs f0, f0, f1
    stw r5, 0x10(r1)
    stw r3, 0x24(r1)
    lfd f4, 0x20(r1)
    fsubs f1, f4, f5
    fdivs f1, f1, f3
    stfs f1, 0x4(r31)
    lhz r0, 0x4(r4)
    stw r0, 0x14(r1)
    lfd f1, 0x10(r1)
    fsubs f1, f1, f2
    fdivs f0, f1, f0
    stfs f0, 0x8(r31)
    b @L_801ACD60
@L_801ACC7C:
    lha r3, 0x0(r4)
    lis r11, 0x4330
    xoris r10, r0, 0x8000
    lis r9, 0x4330
    xoris r3, r3, 0x8000
    xoris r7, r0, 0x8000
    stw r3, 0x34(r1)
    xoris r3, r0, 0x8000
    lis r6, 0x4330
    lis r0, 0x4330
    stw r11, 0x30(r1)
    lis r8, 0x4330
    lfd f2, -22944(r2)
    lis r5, 0x4330
    lfd f0, 0x30(r1)
    stw r10, 0x2c(r1)
    fsubs f4, f0, f2
    lfd f1, -22944(r2)
    stw r9, 0x28(r1)
    lfd f3, -22944(r2)
    lfd f0, 0x28(r1)
    stw r3, 0xc(r1)
    fsubs f0, f0, f1
    lfd f5, -22944(r2)
    stw r7, 0x1c(r1)
    lfd f1, -22944(r2)
    fdivs f4, f4, f0
    stw r6, 0x18(r1)
    lfd f2, -22944(r2)
    lfd f0, 0x18(r1)
    stw r0, 0x8(r1)
    stw r8, 0x20(r1)
    stfs f4, 0x0(r31)
    fsubs f3, f0, f3
    lfd f0, 0x8(r1)
    lha r3, 0x2(r4)
    fsubs f0, f0, f1
    stw r5, 0x10(r1)
    xoris r0, r3, 0x8000
    stw r0, 0x24(r1)
    lfd f4, 0x20(r1)
    fsubs f1, f4, f5
    fdivs f1, f1, f3
    stfs f1, 0x4(r31)
    lha r0, 0x4(r4)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f1, 0x10(r1)
    fsubs f1, f1, f2
    fdivs f0, f1, f0
    stfs f0, 0x8(r31)
    b @L_801ACD60
@L_801ACD4C:
    lis r4, lbl_802750B4@ha
    addi r3, r2, -23016
    addi r5, r4, lbl_802750B4@l
    li r4, 0x465
    bl fn_80196D78
@L_801ACD60:
    lwz r0, 0x54(r1)
    lwz r31, 0x4c(r1)
    lwz r30, 0x48(r1)
    lwz r29, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
