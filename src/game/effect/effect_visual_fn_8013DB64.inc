    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x28(r1)
    fmr f31, f2
    stfd f30, 0x20(r1)
    fmr f30, f1
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    li r3, 0x14
    bl fn_800E3534
    clrlwi. r0, r3, 16
    mr r31, r3
    beq @L_8013DBF0
    bl fn_800E27B0
    sth r31, 0xc(r3)
    stfs f30, 0x0(r3)
    stfs f31, 0x4(r3)
    stw r30, 0x8(r3)
    lwz r4, 0x14(r29)
    cmplwi r4, 0x0
    beq @L_8013DBE4
    b @L_8013DBD0
    @L_8013DBCC:
    mr r4, r0
    @L_8013DBD0:
    lwz r0, 0x10(r4)
    cmplwi r0, 0x0
    bne @L_8013DBCC
    stw r3, 0x10(r4)
    b @L_8013DBE8
    @L_8013DBE4:
    stw r3, 0x14(r29)
    @L_8013DBE8:
    li r0, 0x0
    stw r0, 0x10(r3)
    @L_8013DBF0:
    lwz r0, 0x34(r1)
    li r3, 0x0
    lfd f31, 0x28(r1)
    lfd f30, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
