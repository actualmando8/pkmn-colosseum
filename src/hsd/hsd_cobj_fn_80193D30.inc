    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    stw r30, 0x8(r1)
    lwz r3, 0x84(r3)
    bl fn_801C25E4
    cmplwi r31, 0x0
    bne @L_80193D68
    la r3, -23880(r2)
    li r4, 0x2e8
    la r5, -23872(r2)
    bl __assert
@L_80193D68:
    lwz r30, 0x24(r31)
    cmplwi r30, 0x0
    beq @L_80193DE8
    lis r3, 0x1
    lhz r4, 0x4(r30)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_80193D98
    b @L_80193DB0
@L_80193D98:
    lhz r0, 0x4(r30)
    lhz r3, 0x4(r30)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r30)
    srwi r3, r4, 5
@L_80193DB0:
    cmpwi r3, 0x0
    beq @L_80193DE8
    cmplwi r30, 0x0
    beq @L_80193DE8
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
@L_80193DE8:
    cmplwi r31, 0x0
    bne @L_80193E00
    la r3, -23880(r2)
    li r4, 0x2d0
    la r5, -23872(r2)
    bl __assert
@L_80193E00:
    lwz r30, 0x28(r31)
    cmplwi r30, 0x0
    beq @L_80193E80
    lis r3, 0x1
    lhz r4, 0x4(r30)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_80193E30
    b @L_80193E48
@L_80193E30:
    lhz r0, 0x4(r30)
    lhz r3, 0x4(r30)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r30)
    srwi r3, r4, 5
@L_80193E48:
    cmpwi r3, 0x0
    beq @L_80193E80
    cmplwi r30, 0x0
    beq @L_80193E80
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
@L_80193E80:
    lwz r0, 0x88(r31)
    cmplwi r0, 0x0
    beq @L_80193E94
    lwz r3, 0x88(r31)
    bl HSD_MtxFree
@L_80193E94:
    lis r4, lbl_8036C678@ha
    mr r3, r31
    addi r4, r4, lbl_8036C678@l
    lwz r4, 0x14(r4)
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
