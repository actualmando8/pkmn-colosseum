    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r3, 0x27
    li r4, 0x1
    bl fn_8010264C
    mr r31, r3
    li r3, 0x27
    bl fn_80102510
    li r3, 0x27
    li r4, 0x1
    bl fn_80102428
    cmpwi r31, -0x1
    bne @L_8003AEC8
    li r3, 0x0
    b @L_8003AEDC
    @L_8003AEC8:
    cmpwi r31, 0x0
    bne @L_8003AED8
    li r3, 0x1
    b @L_8003AEDC
    @L_8003AED8:
    li r3, 0x0
    @L_8003AEDC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
