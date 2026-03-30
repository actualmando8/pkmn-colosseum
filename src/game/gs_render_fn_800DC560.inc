    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lbz r0, lbl_8047AAE0(r13)
    cmplwi r0, 0x0
    beq @L_800DC6BC
    lis r3, lbl_80400EE0@ha
    li r30, 0x0
    addi r0, r3, lbl_80400EE0@l
    mr r31, r0
    @L_800DC594:
    lbz r0, 0x0(r31)
    cmplwi r0, 0x1
    bne @L_800DC6AC
    lwz r3, 0x4(r31)
    li r4, 0x0
    bl fn_800EF1E8
    lwz r12, 0x8(r31)
    cmplwi r12, 0x0
    beq @L_800DC6A0
    lwz r3, 0x4(r31)
    lwz r4, 0x10(r31)
    lwz r5, 0xc(r31)
    mtctr r12
    bctrl
    clrlwi. r0, r3, 24
    bne @L_800DC6A0
    lis r3, lbl_80400EE0@ha
    lwz r4, 0x4(r31)
    addi r29, r3, lbl_80400EE0@l
    lbz r0, 0x0(r29)
    cmplwi r0, 0x1
    bne @L_800DC5FC
    lwz r0, 0x4(r29)
    cmplw r0, r4
    bne @L_800DC5FC
    b @L_800DC654
    @L_800DC5FC:
    lbzu r0, 0x14(r29)
    cmplwi r0, 0x1
    bne @L_800DC618
    lwz r0, 0x4(r29)
    cmplw r0, r4
    bne @L_800DC618
    b @L_800DC654
    @L_800DC618:
    lbzu r0, 0x14(r29)
    cmplwi r0, 0x1
    bne @L_800DC634
    lwz r0, 0x4(r29)
    cmplw r0, r4
    bne @L_800DC634
    b @L_800DC654
    @L_800DC634:
    lbzu r0, 0x14(r29)
    cmplwi r0, 0x1
    bne @L_800DC650
    lwz r0, 0x4(r29)
    cmplw r0, r4
    bne @L_800DC650
    b @L_800DC654
    @L_800DC650:
    li r29, 0x0
    @L_800DC654:
    cmplwi r29, 0x0
    beq @L_800DC6A0
    lwz r3, 0x4(r29)
    bl fn_800EF504
    li r4, 0x0
    lis r3, lbl_80400EE0@ha
    stb r4, 0x0(r29)
    li r0, 0x4
    addi r3, r3, lbl_80400EE0@l
    stb r4, lbl_8047AAE0(r13)
    mtctr r0
    @L_800DC680:
    lbz r0, 0x0(r3)
    cmplwi r0, 0x1
    bne @L_800DC698
    li r0, 0x1
    stb r0, lbl_8047AAE0(r13)
    b @L_800DC6A0
    @L_800DC698:
    addi r3, r3, 0x14
    bdnz @L_800DC680
    @L_800DC6A0:
    lwz r3, 0x10(r31)
    addi r0, r3, 0x1
    stw r0, 0x10(r31)
    @L_800DC6AC:
    addi r30, r30, 0x1
    addi r31, r31, 0x14
    cmplwi r30, 0x4
    blt @L_800DC594
    @L_800DC6BC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
