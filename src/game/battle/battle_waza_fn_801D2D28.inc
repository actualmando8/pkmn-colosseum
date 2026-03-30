    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    lis r3, -32698
    stw r0, 0x44(r1)
    addi r3, r3, 29652
    stw r31, 0x3c(r1)
    mr r31, r3
    stw r30, 0x38(r1)
    stw r29, 0x34(r1)
    lbz r0, -21548(r13)
    cmplwi r0, 0x0
    beq @L_801D2F78
    lwz r0, -21556(r13)
    cmplwi r0, 0x0
    beq @L_801D2F78
    lwz r3, -21552(r13)
    cmplwi r3, 0x0
    beq @L_801D2D88
    lwz r0, 0x18(r3)
    cmplwi r0, 0x0
    beq @L_801D2D88
    lwz r0, 0x20(r3)
    cmplwi r0, 0x0
    bne @L_801D2F78
@L_801D2D88:
    bl fn_800D3088
    lwz r5, -21556(r13)
    mr r30, r3
    lwz r6, -21552(r13)
    lhz r0, 0x32(r5)
    cmplwi r6, 0x0
    lwz r4, 0x2c(r5)
    mulli r0, r0, 0xd4
    lwz r3, 0x24(r5)
    add r4, r4, r0
    beq @L_801D2DC8
    lbz r0, 0x17(r6)
    rlwinm r0, r0, 2, 0, 29
    add r4, r4, r0
    lwz r4, 0x4c(r4)
    b @L_801D2DCC
@L_801D2DC8:
    lwz r4, 0x54(r4)
@L_801D2DCC:
    bl fn_800EE150
    mr. r29, r3
    beq @L_801D2E58
    addi r4, r1, 0x14
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    lwz r3, -21552(r13)
    cmplwi r3, 0x0
    beq @L_801D2E20
    lhz r0, 0x2e(r3)
    cmplwi r0, 0x3
    bne @L_801D2E20
    lhz r0, 0x2c(r3)
    cmplwi r0, 0x154
    beq @L_801D2E14
    cmplwi r0, 0x6e
    bne @L_801D2E20
@L_801D2E14:
    addi r3, r1, 0x8
    bl fn_80177908
    b @L_801D2E30
@L_801D2E20:
    lwz r3, -21556(r13)
    addi r4, r1, 0x8
    lwz r3, 0x24(r3)
    bl fn_800E3D98
@L_801D2E30:
    addi r3, r1, 0x14
    addi r5, r1, 0x8
    mr r4, r3
    bl fn_800E0168
    lfs f1, -21696(r2)
    addi r4, r1, 0x14
    li r3, 0x7
    bl fn_80177670
    mr r3, r29
    bl fn_800EE828
@L_801D2E58:
    lwz r3, -21560(r13)
    li r0, 0x2
    add r4, r3, r30
    stw r4, -21560(r13)
    mtctr r0
@L_801D2E6C:
    lwz r0, 0x8(r31)
    cmplw r4, r0
    bgt @L_801D2E84
    lfs f0, 0x0(r31)
    stfs f0, -31556(r13)
    b @L_801D2EE8
@L_801D2E84:
    lwz r5, 0xc(r31)
    cmplw r4, r5
    bgt @L_801D2ED8
    lis r3, 0x4330
    subf r4, r0, r4
    subf r0, r0, r5
    stw r4, 0x24(r1)
    lfd f4, -21688(r2)
    stw r3, 0x20(r1)
    lfs f1, 0x0(r31)
    lfd f0, 0x20(r1)
    stw r0, 0x2c(r1)
    fsubs f3, f0, f4
    lfs f2, 0x4(r31)
    stw r3, 0x28(r1)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f4
    fdivs f3, f3, f0
    bl fn_800E09B4
    stfs f1, -31556(r13)
    b @L_801D2EE8
@L_801D2ED8:
    lfs f0, 0x4(r31)
    addi r31, r31, 0x10
    stfs f0, -31556(r13)
    bdnz @L_801D2E6C
@L_801D2EE8:
    lfs f1, -31556(r13)
    bl fn_801766A8
    lwz r0, -21552(r13)
    cmplwi r0, 0x0
    bne @L_801D2F78
    li r3, 0x0
    bl fn_80176F98
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801D2F78
    li r0, 0x0
    cmpwi r0, 0x0
    bne @L_801D2F78
    lwz r3, -21552(r13)
    cmplwi r3, 0x0
    bne @L_801D2F30
    li r3, 0x8
    bl fn_80177A44
    b @L_801D2F54
@L_801D2F30:
    lwz r0, 0x18(r3)
    cmplwi r0, 0x0
    beq @L_801D2F4C
    lwz r0, 0x20(r3)
    cmplwi r0, 0x0
    beq @L_801D2F4C
    bl fn_80176AE4
@L_801D2F4C:
    li r0, 0x0
    stw r0, -21552(r13)
@L_801D2F54:
    li r3, 0x0
    bl fn_801765F4
    li r0, 0x0
    stw r0, -21556(r13)
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D2F78
    li r3, 0x2
    bl fn_80177A44
@L_801D2F78:
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
