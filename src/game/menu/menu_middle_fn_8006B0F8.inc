    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    cmpwi r31, 0x0
    blt @L_8006B11C
    cmplwi r31, 0x4
    blt @L_8006B124
    @L_8006B11C:
    li r3, 0x0
    b @L_8006B140
    @L_8006B124:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    mulli r4, r31, 0x1660
    mr r0, r3
    addi r3, r4, 0x50
    add r3, r0, r3
    @L_8006B140:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
