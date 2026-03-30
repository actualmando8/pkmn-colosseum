    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, -21055(r13)
    cmplwi r0, 0x1
    beq @L_801EF748
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801EF748
    bl fn_801EF624
    rlwinm. r0, r3, 0, 16, 31
    beq @L_801EF748
    bl fn_8020DD44
    cmplwi r3, 0x0
    beq @L_801EF740
    li r4, 0x3e8
    li r5, 0xff
    bl fn_801659FC
@L_801EF740:
    li r0, 0x1
    stb r0, -21055(r13)
@L_801EF748:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
