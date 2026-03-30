    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_80478F48(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    blt @L_800087C4
    lis r3, 0x1
    subi r3, r3, 0x149d
    b @L_800087D8
    @L_800087C4:
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_801F54A4
    @L_800087D8:
    cmplwi r3, 0x0
    bne @L_800087E8
    lis r3, 0x1
    subi r3, r3, 0x149d
    @L_800087E8:
    bl fn_800FA280
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
