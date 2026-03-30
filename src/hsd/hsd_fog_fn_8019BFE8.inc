    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r5
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    stw r28, 0x10(r1)
    mr r28, r3
    lwz r6, 0x0(r3)
    lwz r12, 0x3c(r6)
    mtctr r12
    bctrl
    lwz r0, 0x8(r28)
    mr r30, r3
    cmplw r30, r0
    blt @L_8019C044
    lis r4, lbl_80274800@ha
    la r3, lbl_8047DA98(r2)
    addi r5, r4, lbl_80274800@l
    li r4, 0x71
    bl fn_80196E10
    @L_8019C044:
    lwz r3, 0x4(r28)
    slwi r0, r30, 2
    lwzx r0, r3, r0
    cmplwi r0, 0x0
    bne @L_8019C060
    li r30, 0x0
    b @L_8019C0A8
    @L_8019C060:
    lwz r3, 0x4(r28)
    slwi r0, r30, 2
    lwzx r30, r3, r0
    b @L_8019C09C
    @L_8019C070:
    lwz r6, 0x0(r28)
    mr r3, r28
    mr r5, r29
    lwz r4, 0x4(r30)
    lwz r12, 0x40(r6)
    mtctr r12
    bctrl
    cmpwi r3, 0x0
    bne @L_8019C098
    b @L_8019C0A8
    @L_8019C098:
    lwz r30, 0x0(r30)
    @L_8019C09C:
    cmplwi r30, 0x0
    bne @L_8019C070
    li r30, 0x0
    @L_8019C0A8:
    cmplwi r31, 0x0
    mr r3, r30
    beq @L_8019C0C4
    neg r0, r3
    or r0, r0, r3
    srwi r0, r0, 31
    stw r0, 0x0(r31)
    @L_8019C0C4:
    cmplwi r3, 0x0
    beq @L_8019C0D4
    lwz r3, 0x8(r3)
    b @L_8019C0D8
    @L_8019C0D4:
    li r3, 0x0
    @L_8019C0D8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
