    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    cmpwi r3, 0xd2
    beq @L_8000C6C4
    bge @L_8000C6D8
    cmpwi r3, 0xd1
    bge @L_8000C6AC
    b @L_8000C6D8
    @L_8000C6AC:
    li r3, 0x7f
    li r4, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80166D18
    b @L_8000C6D8
    @L_8000C6C4:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80166D18
    @L_8000C6D8:
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
