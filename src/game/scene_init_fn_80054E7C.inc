    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_80105624
    bl fn_800573C0
    cmpwi r3, 0x0
    bne @L_80054EB4
    bl fn_800566E8
    cmpwi r3, 0x0
    bne @L_80054EB4
    mr r3, r31
    bl fn_80102ED4
    @L_80054EB4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
