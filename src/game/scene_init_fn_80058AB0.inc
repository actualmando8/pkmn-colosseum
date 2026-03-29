    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_80105624
    bl fn_800573C0
    cmpwi r3, 0x0
    bne @L_80058ADC
    mr r3, r31
    bl fn_80102ED4
    @L_80058ADC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
