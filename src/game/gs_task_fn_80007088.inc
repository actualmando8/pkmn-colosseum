    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_801EF63C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800070AC
    li r3, -0x1
    b @L_800070BC
    @L_800070AC:
    li r3, 0x0
    bl fn_801F1588
    bl fn_801337E4
    li r3, 0x0
    @L_800070BC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
