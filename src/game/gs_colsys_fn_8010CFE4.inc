    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, -32704
    stw r0, 0x14(r1)
    addi r4, r4, 19560
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r0, 0x3704(r4)
    cmpwi r0, 0x0
    bge @L_8010D014
    li r3, 0x0
    b @L_8010D024
@L_8010D014:
    bl fn_8010CE04
    lis r4, -32704
    li r3, 0x1
    stw r31, 19560(r4)
@L_8010D024:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
