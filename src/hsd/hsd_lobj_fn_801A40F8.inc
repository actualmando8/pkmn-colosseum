    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    stw r30, 0x8(r1)
    lwz r3, 0x48(r3)
    bl fn_801C25E4
    cmplwi r31, 0x0
    beq @L_801A4128
    lwz r30, 0x18(r31)
    b @L_801A412C
    @L_801A4128:
    li r30, 0x0
    @L_801A412C:
    cmplwi r30, 0x0
    beq @L_801A41A8
    lis r3, 0x1
    lhz r4, 0x4(r30)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_801A4158
    b @L_801A4170
    @L_801A4158:
    lhz r0, 0x4(r30)
    lhz r3, 0x4(r30)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r30)
    srwi r3, r4, 5
    @L_801A4170:
    cmpwi r3, 0x0
    beq @L_801A41A8
    cmplwi r30, 0x0
    beq @L_801A41A8
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
    @L_801A41A8:
    cmplwi r31, 0x0
    beq @L_801A41B8
    lwz r30, 0x1c(r31)
    b @L_801A41BC
    @L_801A41B8:
    li r30, 0x0
    @L_801A41BC:
    cmplwi r30, 0x0
    beq @L_801A4238
    lis r3, 0x1
    lhz r4, 0x4(r30)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_801A41E8
    b @L_801A4200
    @L_801A41E8:
    lhz r0, 0x4(r30)
    lhz r3, 0x4(r30)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r30)
    srwi r3, r4, 5
    @L_801A4200:
    cmpwi r3, 0x0
    beq @L_801A4238
    cmplwi r30, 0x0
    beq @L_801A4238
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
    @L_801A4238:
    lis r4, lbl_8036CA20@ha
    mr r3, r31
    addi r4, r4, lbl_8036CA20@l
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
