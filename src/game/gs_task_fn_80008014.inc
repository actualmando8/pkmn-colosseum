    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_801EF63C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80008038
    li r3, -0x1
    b @L_8000803C
    @L_80008038:
    li r3, 0x1
    @L_8000803C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
