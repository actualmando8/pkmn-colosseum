    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x1
    li r4, 0x7
    stw r0, 0x14(r1)
    li r5, 0x0
    bl fn_800BCE88
    li r3, 0x0
    li r4, 0x1
    li r5, 0x0
    li r6, 0xf
    bl fn_800BCDDC
    li r3, 0x1
    bl fn_800B884C
    li r3, 0x0
    bl fn_800BA6B0
    li r3, 0x1
    bl fn_800BC8C8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0xff
    bl fn_800BC6F0
    li r3, 0x0
    li r4, 0x3
    bl fn_800BC114
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    bl fn_800BC52C
    li r3, 0x1
    li r4, 0x0
    li r5, 0x0
    bl fn_800BC52C
    li r3, 0x2
    li r4, 0x0
    li r5, 0x0
    bl fn_800BC52C
    li r3, 0x3
    li r4, 0x0
    li r5, 0x0
    bl fn_800BC52C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x1
    li r6, 0x2
    li r7, 0x3
    bl fn_800BC580
    li r3, 0x1
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x3
    bl fn_800BC580
    li r3, 0x2
    li r4, 0x1
    li r5, 0x1
    li r6, 0x1
    li r7, 0x3
    bl fn_800BC580
    li r3, 0x3
    li r4, 0x2
    li r5, 0x2
    li r6, 0x2
    li r7, 0x3
    bl fn_800BC580
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
