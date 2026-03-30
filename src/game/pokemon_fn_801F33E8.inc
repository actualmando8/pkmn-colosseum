    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    clrlwi r5, r4, 16
    lis r4, fn_801F3430@ha
    stw r0, 0x14(r1)
    li r0, 0x0
    addi r4, r4, fn_801F3430@l
    li r6, 0x0
    stw r5, 0x8(r1)
    addi r5, r1, 0x8
    stw r0, 0xc(r1)
    bl fn_801F37B0
    lwz r0, 0xc(r1)
    clrlwi r3, r0, 16
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
