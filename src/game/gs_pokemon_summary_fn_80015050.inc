    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    lbz r0, 0x95(r29)
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    extsb r0, r0
    mulli r0, r0, 0x4c
    add r3, r3, r0
    lwz r31, 0x18(r3)
    cmplwi r31, 0x0
    beq @L_800150C4
    lwz r0, 0x1c(r3)
    clrlwi r3, r0, 16
    bl fn_80103E68
    srwi r0, r3, 16
    mr r12, r31
    mr r3, r29
    mr r4, r30
    sth r0, 0x8(r1)
    addi r5, r1, 0x8
    mtctr r12
    bctrl
    b @L_800150C8
    @L_800150C4:
    li r3, 0x0
    @L_800150C8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
