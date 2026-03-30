    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_8013FDB8
    lwz r3, 0x14(r31)
    cmplwi r3, 0x0
    beq @L_8013FD94
    li r4, 0x0
    bl fn_80118874
    @L_8013FD94:
    lwz r3, 0x10(r31)
    cmplwi r3, 0x0
    beq @L_8013FDB8
    bl fn_800EC96C
    lwz r3, 0x10(r31)
    bl fn_800EC1D4
    lwz r3, 0x10(r31)
    li r4, 0x0
    bl fn_800E4014
    @L_8013FDB8:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
