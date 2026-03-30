    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    clrlwi r8, r4, 16
    lis r4, fn_801F3678@ha
    stw r0, 0x24(r1)
    clrlwi r0, r5, 24
    li r7, 0x0
    addi r4, r4, fn_801F3678@l
    stw r6, 0x14(r1)
    addi r5, r1, 0x8
    li r6, 0x0
    stw r8, 0x8(r1)
    stw r7, 0xc(r1)
    stw r0, 0x10(r1)
    bl fn_801F37B0
    lwz r0, 0xc(r1)
    clrlwi r3, r0, 16
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
