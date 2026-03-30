    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_801E11E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8000C300
    bl fn_801E1258
    b @L_8000C304
    @L_8000C300:
    bl fn_801E11F0
    @L_8000C304:
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
