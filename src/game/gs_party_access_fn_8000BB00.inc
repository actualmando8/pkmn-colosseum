    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x2
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, -0x1
    bne @L_8000BB28
    li r3, 0x0
    b @L_8000BB38
    @L_8000BB28:
    mr r4, r3
    li r3, 0x0
    bl fn_801294C4
    li r3, 0x0
    @L_8000BB38:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
