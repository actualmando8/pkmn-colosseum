    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x0
    bl fn_80083CFC
    cmplwi r3, 0x0
    beq @L_80089C40
    lbz r0, 0x4000(r3)
    cmplwi r0, 0x0
    beq @L_80089C40
    li r3, 0x1
    b @L_80089C44
    @L_80089C40:
    li r3, 0x0
    @L_80089C44:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
