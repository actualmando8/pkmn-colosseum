    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r3, 0x38(r3)
    addis r0, r3, 0x102
    cmplwi r0, 0xfefe
    beq @L_800DF014
    bl fn_800B8DF4
    lwz r3, 0x8(r31)
    lwz r0, 0x38(r31)
    lwz r4, 0x8(r3)
    lwz r3, 0x58(r4)
    stw r0, 0x58(r4)
    bl fn_801BBD3C
    lis r3, 0xfeff
    subi r0, r3, 0x102
    stw r0, 0x38(r31)
    @L_800DF014:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
