    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_80478F00(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_80008714
    lis r3, 0x1
    subi r3, r3, 0x149d
    b @L_80008720
    @L_80008714:
    clrlwi r3, r3, 16
    bl fn_8020E204
    bl fn_8020E1EC
    @L_80008720:
    cmplwi r3, 0x0
    bne @L_80008730
    lis r3, 0x1
    subi r3, r3, 0x149d
    @L_80008730:
    bl fn_800FA280
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
