    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    beq @L_801DA93C
    bl fn_801DD0C8
    cmplwi r3, 0x0
    beq @L_801DA93C
    li r0, 0x0
    stb r0, 0x16(r3)
@L_801DA93C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
