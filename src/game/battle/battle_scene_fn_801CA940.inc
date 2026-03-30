    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    cmpwi r3, 0x0
    blt @L_801CA964
    mr r4, r3
    li r3, 0x0
    bl fn_801294C4
    b @L_801CA970
@L_801CA964:
    neg r4, r3
    li r3, 0x0
    bl fn_80129474
@L_801CA970:
    li r3, 0x0
    li r4, 0xc
    li r5, 0x0
    bl fn_8012A5B0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
