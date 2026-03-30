    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r31, 0x0
    b @L_801B31C8
@L_801B31BC:
    mr r3, r31
    bl fn_800BBC34
    addi r31, r31, 0x1
@L_801B31C8:
    cmpwi r31, 0x10
    blt @L_801B31BC
    li r3, 0x0
    bl fn_800BBC0C
    li r0, 0x0
    stw r0, -21680(r13)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
