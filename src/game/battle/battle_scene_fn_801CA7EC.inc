    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x0
    li r4, 0xd
    li r5, 0x0
    bl fn_8012A5B0
    cmpwi r31, 0x0
    blt @L_801CA828
    mr r4, r31
    li r3, 0x0
    bl fn_801293FC
    b @L_801CA834
@L_801CA828:
    neg r4, r31
    li r3, 0x0
    bl fn_80129384
@L_801CA834:
    li r3, 0x0
    li r4, 0xd
    li r5, 0x0
    bl fn_8012A5B0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
