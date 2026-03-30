    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r0, lbl_8047B2E8(r13)
    cmplw r31, r0
    bne @L_801AA6F8
    li r0, 0x0
    stw r0, lbl_8047B2E8(r13)
@L_801AA6F8:
    lis r3, lbl_8036CCD0@ha
    addi r0, r3, lbl_8036CCD0@l
    cmplw r31, r0
    bne @L_801AA758
    lwz r3, lbl_8047B2EC(r13)
    bl fn_801A6990
    cmpwi r3, 0x0
    beq @L_801AA728
    li r3, 0x0
    li r0, 0x0
    stw r3, lbl_8047B2EC(r13)
    stw r0, lbl_8047B2F4(r13)
@L_801AA728:
    lwz r3, lbl_8047B2F0(r13)
    bl fn_801A6990
    cmpwi r3, 0x0
    beq @L_801AA748
    li r3, 0x0
    li r0, 0x0
    stw r3, lbl_8047B2F0(r13)
    stw r0, lbl_8047B2F8(r13)
@L_801AA748:
    li r3, 0x0
    li r0, 0x0
    stw r3, lbl_8047B2FC(r13)
    stw r0, lbl_8047B300(r13)
@L_801AA758:
    lis r4, lbl_8036CCD0@ha
    mr r3, r31
    addi r4, r4, lbl_8036CCD0@l
    lwz r4, 0x14(r4)
    lwz r12, 0x38(r4)
    mtctr r12
    bctrl
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
