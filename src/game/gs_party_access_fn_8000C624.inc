    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    cmpwi r3, 0xd4
    beq @L_8000C660
    bge @L_8000C674
    cmpwi r3, 0xd3
    bge @L_8000C648
    b @L_8000C674
    @L_8000C648:
    li r3, 0x7f
    li r4, 0x0
    li r5, 0x0
    li r6, 0x1
    bl fn_80166D18
    b @L_8000C674
    @L_8000C660:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x1
    bl fn_80166D18
    @L_8000C674:
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
