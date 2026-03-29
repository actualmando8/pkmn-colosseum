    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r3, 0x8(r1)
    li r0, 0x0
    stw r0, 0xc(r1)
    bl fn_801046B8
    mr r4, r3
    addi r5, r1, 0x8
    addi r9, r1, 0xc
    li r3, 0x75
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    mr r31, r3
    cmpwi r31, -0x1
    beq @L_8004DC7C
    lwz r0, 0x8(r1)
    cmpw r31, r0
    beq @L_8004DC7C
    li r0, 0x1
    stw r0, 0xc(r1)
    @L_8004DC7C:
    li r3, 0x75
    bl fn_80102510
    li r3, 0x75
    li r4, 0x1
    bl fn_80102428
    cmpwi r31, 0x0
    blt @L_8004DCA0
    cmpwi r31, 0x4
    blt @L_8004DCA8
    @L_8004DCA0:
    li r3, -0x1
    b @L_8004DCAC
    @L_8004DCA8:
    mr r3, r31
    @L_8004DCAC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
