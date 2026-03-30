    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x59
    li r4, 0x20c
    bl fn_80102138
    cmpwi r3, 0x0
    bge @L_801C864C
    li r3, 0x0
@L_801C864C:
    extsb r0, r3
    li r3, 0x0
    stb r3, 0xc(r1)
    addi r4, r1, 0x8
    li r3, 0x2
    stb r0, 0xd(r1)
    lhz r0, 0xc(r1)
    sth r0, 0x8(r1)
    bl fn_80103EAC
    li r3, 0x1
    bl fn_8000D710
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
