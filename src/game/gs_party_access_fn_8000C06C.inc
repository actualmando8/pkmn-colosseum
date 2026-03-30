    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x8
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000C098
    li r3, 0x8
    bl fn_80102510
    b @L_8000C0C8
    @L_8000C098:
    li r3, 0x8
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0x8
    li r4, 0x17c
    li r5, 0x20
    bl fn_80102868
    @L_8000C0C8:
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
