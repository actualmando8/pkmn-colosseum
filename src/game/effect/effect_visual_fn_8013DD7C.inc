    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    beq @L_8013DDB8
    lwz r0, 0x10(r3)
    cmpwi r0, 0x0
    beq @L_8013DDA8
    lwz r3, 0xc(r3)
    bl fn_800DC298
    b @L_8013DDB8
    @L_8013DDA8:
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_8013DDB8
    bl fn_800E5790
    @L_8013DDB8:
    lwz r0, 0x14(r1)
    li r3, 0x1
    mtlr r0
    addi r1, r1, 0x10
    blr
