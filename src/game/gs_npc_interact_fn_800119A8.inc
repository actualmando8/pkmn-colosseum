    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x4b
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800119E0
    mr r5, r31
    li r3, 0x4b
    li r4, 0x0
    bl fn_80102568
    @L_800119E0:
    li r3, 0xf6
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80011A04
    mr r5, r31
    li r3, 0xf6
    li r4, 0x0
    bl fn_80102568
    @L_80011A04:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
