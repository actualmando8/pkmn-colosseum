    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    addi r31, r1, 0x8
    stw r30, 0x18(r1)
    mr r30, r3
    stw r29, 0x14(r1)
    b @L_801A4410
    @L_801A4368:
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801A4384
    lwz r3, 0x0(r30)
    bl fn_80193748
    cmplwi r3, 0x0
    bne @L_801A43C4
    @L_801A4384:
    lwz r0, lbl_8047B2B0(r13)
    cmplwi r0, 0x0
    beq @L_801A4398
    lwz r3, lbl_8047B2B0(r13)
    b @L_801A43A0
    @L_801A4398:
    lis r3, lbl_8036CA20@ha
    addi r3, r3, lbl_8036CA20@l
    @L_801A43A0:
    bl fn_80193828
    mr. r29, r3
    bne @L_801A43BC
    la r3, lbl_8047DBB8(r2)
    li r4, 0x5d5
    la r5, lbl_8047DBC0(r2)
    bl fn_80196E10
    @L_801A43BC:
    stw r29, 0x0(r31)
    b @L_801A43E8
    @L_801A43C4:
    bl fn_80193828
    stw r3, 0x0(r31)
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_801A43E8
    la r3, lbl_8047DBB8(r2)
    li r4, 0x67b
    la r5, lbl_8047DBC4(r2)
    bl fn_80196E10
    @L_801A43E8:
    lwz r5, 0x0(r31)
    mr r4, r30
    lwz r3, 0x0(r31)
    lwz r5, 0x0(r5)
    lwz r12, 0x3c(r5)
    mtctr r12
    bctrl
    lwz r3, 0x0(r31)
    lwz r30, 0x4(r30)
    addi r31, r3, 0xc
    @L_801A4410:
    cmplwi r30, 0x0
    bne @L_801A4368
    li r0, 0x0
    stw r0, 0x0(r31)
    lwz r3, 0x8(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
