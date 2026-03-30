    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r4, 0x18(r3)
    rlwinm r0, r4, 0, 28, 28
    cmpwi r0, 0x8
    bne @L_801DCFC4
    xori r0, r4, 0x8
    li r4, 0x1
    stb r0, 0x18(r31)
    li r5, 0x1
    bl fn_801DEF0C
    mr r3, r31
    bl fn_801DA014
@L_801DCFC4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
