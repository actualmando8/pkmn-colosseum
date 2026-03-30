    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    clrlwi r9, r4, 16
    lis r4, fn_801F34EC@ha
    stw r0, 0x24(r1)
    clrlwi r0, r6, 24
    li r8, 0x0
    mr r6, r5
    stw r9, 0x8(r1)
    addi r4, r4, fn_801F34EC@l
    addi r5, r1, 0x8
    stw r8, 0xc(r1)
    stw r0, 0x10(r1)
    stw r7, 0x14(r1)
    bl fn_801F37B0
    lwz r0, 0x24(r1)
    lwz r3, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
