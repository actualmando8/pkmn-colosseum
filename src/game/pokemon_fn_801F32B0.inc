    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, fn_801F32EC@ha
    li r6, 0x0
    stw r0, 0x14(r1)
    li r0, 0x0
    addi r4, r4, fn_801F32EC@l
    addi r5, r1, 0x8
    stb r0, 0x8(r1)
    bl fn_801F2B5C
    lwz r0, 0x14(r1)
    lbz r3, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
