    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    beq @L_8013D7F0
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_8013D7F0
    bl fn_800E5FFC
    @L_8013D7F0:
    lwz r0, 0x14(r1)
    li r3, 0x1
    mtlr r0
    addi r1, r1, 0x10
    blr
