    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x4
    stw r0, 0x14(r1)
    bl fn_80102620
    clrlwi. r0, r3, 24
    beq @L_801335FC
    li r3, 0x4
    bl fn_80102510
    b @L_8013361C
    @L_801335FC:
    li r3, 0x4
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    @L_8013361C:
    lwz r0, 0x14(r1)
    li r3, 0x0
    mtlr r0
    addi r1, r1, 0x10
    blr
