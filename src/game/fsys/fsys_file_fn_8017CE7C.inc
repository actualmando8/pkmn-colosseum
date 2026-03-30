    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r0, 0x4c(r3)
    cmpwi r0, 0x3
    bne @L_8017CEA8
    mr r3, r31
    bl fn_8017D960
    b @L_8017CEB0
    @L_8017CEA8:
    mr r3, r31
    bl fn_8017DAB8
    @L_8017CEB0:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
