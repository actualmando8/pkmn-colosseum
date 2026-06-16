    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    mr r31, r4
    stw r30, 0x28(r1)
    mr. r30, r3
    lhz r0, 0x4(r4)
    stw r0, 0x8(r3)
    lhz r3, 0x4(r4)
    beq @L_80194048
    lwz r0, 0x8(r30)
    rlwimi r0, r3, 0, 2, 31
    stw r0, 0x8(r30)
@L_80194048:
    cmplwi r30, 0x0
    beq @L_801940E0
    lha r0, 0x8(r31)
    lis r5, 0x4330
    lis r4, 0x4330
    lis r3, 0x4330
    xoris r6, r0, 0x8000
    lis r0, 0x4330
    stw r6, 0xc(r1)
    lfd f1, -23856(r2)
    stw r5, 0x8(r1)
    lfd f3, -23856(r2)
    lfd f0, 0x8(r1)
    stw r4, 0x10(r1)
    fsubs f0, f0, f1
    lfd f2, -23856(r2)
    stw r3, 0x18(r1)
    lfd f1, -23856(r2)
    stfs f0, 0xc(r30)
    lha r3, 0xa(r31)
    stw r0, 0x20(r1)
    xoris r0, r3, 0x8000
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f3
    stfs f0, 0x10(r30)
    lha r0, 0xc(r31)
    xoris r0, r0, 0x8000
    stw r0, 0x1c(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f2
    stfs f0, 0x14(r30)
    lha r0, 0xe(r31)
    xoris r0, r0, 0x8000
    stw r0, 0x24(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    stfs f0, 0x18(r30)
@L_801940E0:
    cmplwi r30, 0x0
    beq @L_801940F8
    lwz r3, 0x10(r31)
    lwz r0, 0x14(r31)
    stw r3, 0x1c(r30)
    stw r0, 0x20(r30)
@L_801940F8:
    lwz r3, 0x24(r30)
    lwz r4, 0x18(r31)
    bl HSD_WObjInit
    lwz r3, 0x28(r30)
    lwz r4, 0x1c(r31)
    bl HSD_WObjInit
    cmplwi r30, 0x0
    lfs f0, 0x28(r31)
    beq @L_80194120
    stfs f0, 0x38(r30)
@L_80194120:
    cmplwi r30, 0x0
    lfs f0, 0x2c(r31)
    beq @L_80194130
    stfs f0, 0x3c(r30)
@L_80194130:
    lhz r0, 0x4(r31)
    clrlwi. r0, r0, 31
    beq @L_8019416C
    lwz r0, 0x24(r31)
    cmplwi r0, 0x0
    beq @L_80194158
    lwz r4, 0x24(r31)
    mr r3, r30
    bl fn_80194DA4
    b @L_80194178
@L_80194158:
    lis r4, lbl_8036C6D4@ha
    mr r3, r30
    addi r4, r4, lbl_8036C6D4@l
    bl fn_80194DA4
    b @L_80194178
@L_8019416C:
    lfs f1, 0x20(r31)
    mr r3, r30
    bl fn_801947C8
@L_80194178:
    lhz r0, 0x6(r31)
    cmpwi r0, 0x2
    beq @L_801941F8
    bge @L_80194194
    cmpwi r0, 0x1
    bge @L_801941A0
    b @L_8019422C
@L_80194194:
    cmpwi r0, 0x4
    bge @L_8019422C
    b @L_801941C4
@L_801941A0:
    cmplwi r30, 0x0
    lfs f1, 0x34(r31)
    lfs f0, 0x30(r31)
    beq @L_8019423C
    li r0, 0x1
    stb r0, 0x50(r30)
    stfs f0, 0x40(r30)
    stfs f1, 0x44(r30)
    b @L_8019423C
@L_801941C4:
    cmplwi r30, 0x0
    lfs f3, 0x3c(r31)
    lfs f2, 0x38(r31)
    lfs f1, 0x34(r31)
    lfs f0, 0x30(r31)
    beq @L_8019423C
    li r0, 0x3
    stb r0, 0x50(r30)
    stfs f0, 0x40(r30)
    stfs f1, 0x44(r30)
    stfs f2, 0x48(r30)
    stfs f3, 0x4c(r30)
    b @L_8019423C
@L_801941F8:
    cmplwi r30, 0x0
    lfs f3, 0x3c(r31)
    lfs f2, 0x38(r31)
    lfs f1, 0x34(r31)
    lfs f0, 0x30(r31)
    beq @L_8019423C
    li r0, 0x2
    stb r0, 0x50(r30)
    stfs f0, 0x40(r30)
    stfs f1, 0x44(r30)
    stfs f2, 0x48(r30)
    stfs f3, 0x4c(r30)
    b @L_8019423C
@L_8019422C:
    la r3, -23880(r2)
    li r4, 0x7d2
    la r5, -23864(r2)
    bl __assert
@L_8019423C:
    lwz r0, 0x34(r1)
    li r3, 0x0
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
