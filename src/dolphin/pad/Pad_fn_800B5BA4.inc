    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x60(r1)
    stw r31, 0x5c(r1)
    addi r31, r5, 0x0
    addi r5, r1, 0x14
    bl fn_800B57D0
    cmpwi r3, 0x0
    bne @L_800B5BD0
    lbz r0, 0x48(r1)
    stb r0, 0x0(r31)
    @L_800B5BD0:
    lwz r0, 0x64(r1)
    lwz r31, 0x5c(r1)
    addi r1, r1, 0x60
    mtlr r0
    blr
