    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lbz r0, 0x95(r3)
    li r3, 0x0
    extsb r4, r0
    bl fn_80134A98
    cmplwi r3, 0x0
    bne @L_80055D08
    li r3, 0x0
    b @L_80055D20
    @L_80055D08:
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    li r0, 0xcf
    li r3, 0x0
    stw r0, 0x4c(r31)
    @L_80055D20:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
