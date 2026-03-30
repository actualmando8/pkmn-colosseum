    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, 0x144(r3)
    cmplwi r3, 0x0
    beq @L_800E3BE0
    li r4, 0x1
    bl fn_80118874
    @L_800E3BE0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
