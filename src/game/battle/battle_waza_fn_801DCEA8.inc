    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r3, 0x18(r3)
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x2
    bne @L_801DCEEC
    xori r0, r3, 0x2
    stb r0, 0x18(r31)
    lwz r3, 0x24(r31)
    bl fn_800E6DCC
    mr r3, r31
    li r4, 0x1
    li r5, 0x0
    bl fn_801DEF0C
@L_801DCEEC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
