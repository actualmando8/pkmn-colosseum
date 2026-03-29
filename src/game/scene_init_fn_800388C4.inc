    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x19
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1a
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1b
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x18
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1e
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1f
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x20
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x21
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x22
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x23
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1d
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
