    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_801EF63C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80007134
    li r3, -0x1
    b @L_80007144
    @L_80007134:
    li r3, 0x2
    bl fn_801EF62C
    bl fn_801337E4
    li r3, 0x0
    @L_80007144:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
