    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, -21056(r13)
    cmplwi r0, 0x1
    beq @L_801EF7A4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801EF7A4
    bl fn_801EF624
    rlwinm. r0, r3, 0, 16, 31
    beq @L_801EF7A4
    bl fn_8020DD80
    cmplwi r3, 0x0
    beq @L_801EF79C
    li r4, 0x0
    li r5, 0xff
    bl fn_80165A20
@L_801EF79C:
    li r0, 0x1
    stb r0, -21056(r13)
@L_801EF7A4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
