    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r3, 0x2
    li r4, 0x1
    bl fn_8010264C
    mr r31, r3
    li r3, 0x2
    bl fn_80102510
    cmpwi r31, 0x0
    blt @L_80088FE0
    lwz r0, -25024(r13)
    add r0, r0, r31
    stw r0, -25024(r13)
@L_80088FE0:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
