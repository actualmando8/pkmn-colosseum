    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_800FF56C
    bl fn_80115C48
    mr r31, r3
    li r3, 0xa
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000C5E0
    bl fn_801174C4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000C5CC
    bl fn_80117500
    @L_8000C5CC:
    li r3, 0xa
    bl fn_80102510
    lwz r3, 0x34(r31)
    bl fn_800FEC34
    b @L_8000C60C
    @L_8000C5E0:
    bl fn_801174F4
    li r3, 0xa
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    lwz r3, 0x34(r31)
    bl fn_800FECB8
    @L_8000C60C:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
