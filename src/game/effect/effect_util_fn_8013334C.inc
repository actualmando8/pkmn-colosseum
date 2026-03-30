    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    b @L_8013337C
    @L_80133360:
    mr r3, r31
    bl fn_801D1CC4
    cmpwi r3, 0x0
    bne @L_8013337C
    mr r3, r31
    bl fn_801D1D58
    bl fn_801D268C
    @L_8013337C:
    li r3, 0x2
    li r4, 0x1
    bl fn_8010264C
    mr r31, r3
    cmpwi r31, -0x1
    bne @L_80133360
    lwz r0, 0x14(r1)
    li r3, 0x0
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
