    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_80478F40(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_8000876C
    lis r3, 0x1
    subi r3, r3, 0x149d
    b @L_80008778
    @L_8000876C:
    clrlwi r3, r3, 16
    bl fn_8020E488
    bl fn_8020E248
    @L_80008778:
    cmplwi r3, 0x0
    bne @L_80008788
    lis r3, 0x1
    subi r3, r3, 0x149d
    @L_80008788:
    bl fn_800FA280
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
