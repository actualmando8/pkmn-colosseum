    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    b @L_80031B24
    @L_80031B20:
    bl fn_800F0308
    @L_80031B24:
    bl fn_8010B560
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80031B20
    lwz r3, lbl_8047A418(r13)
    bl fn_801CB9D8
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
