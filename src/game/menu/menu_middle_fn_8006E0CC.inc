    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lis r3, lbl_803B6D68@ha
    addi r31, r3, lbl_803B6D68@l
    b @L_8006E108
    @L_8006E0E8:
    bl fn_8010BBB8
    bl fn_8010BCE4
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_8006E104
    bl fn_800F0308
    b @L_8006E108
    @L_8006E104:
    addi r31, r31, 0x4
    @L_8006E108:
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    bne @L_8006E0E8
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
