    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    cmpwi r31, 0x20
    blt @L_801AA5D8
    addi r3, r2, -23032
    li r4, 0xa4
    addi r5, r2, -23024
    bl fn_80196E10
@L_801AA5D8:
    lis r3, lbl_8036CC40@ha
    rlwinm r0, r31, 2, 0, 29
    addi r3, r3, lbl_8036CC40@l
    add r4, r3, r0
    lwz r3, 0x10(r4)
    addi r0, r3, 0x1
    stw r0, 0x10(r4)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
