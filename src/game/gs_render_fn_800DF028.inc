    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r3, 0x38(r3)
    addis r0, r3, 0x102
    cmplwi r0, 0xfefe
    bne @L_800DF074
    lwz r3, 0x8(r29)
    lwz r3, 0x8(r3)
    lwz r0, 0x58(r3)
    stw r0, 0x38(r29)
    bl fn_801BBD84
    mr r31, r3
    b @L_800DF080
    @L_800DF074:
    lwz r3, 0x8(r29)
    lwz r3, 0x8(r3)
    lwz r31, 0x58(r3)
    @L_800DF080:
    cmplwi r31, 0x0
    beq @L_800DF100
    mr r3, r30
    li r4, 0x0
    bl fn_800EF548
    stw r3, 0x0(r31)
    mr r3, r30
    bl fn_800EF4FC
    sth r3, 0x4(r31)
    mr r3, r30
    bl fn_800EF4F4
    sth r3, 0x6(r31)
    mr r3, r30
    li r4, 0x1
    bl fn_800EF3E0
    stw r3, 0x8(r31)
    mr r3, r30
    bl fn_800EF4E4
    clrlwi r4, r3, 24
    lfs f0, lbl_8047CAC8(r2)
    neg r0, r4
    mr r3, r30
    or r0, r0, r4
    srwi r0, r0, 31
    stw r0, 0xc(r31)
    stfs f0, 0x10(r31)
    stfs f0, 0x14(r31)
    bl fn_800EF504
    bl fn_800B8DF4
    lwz r3, 0x8(r29)
    lwz r3, 0x8(r3)
    stw r31, 0x58(r3)
    @L_800DF100:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
