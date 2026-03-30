    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    clrlwi r6, r5, 16
    lis r5, fn_801F1F7C@ha
    stw r0, 0x24(r1)
    li r0, 0x0
    stw r4, 0x8(r1)
    addi r4, r5, fn_801F1F7C@l
    addi r5, r1, 0x8
    stw r6, 0xc(r1)
    li r6, 0x0
    stw r0, 0x10(r1)
    bl fn_801F37B0
    lwz r0, 0x10(r1)
    clrlwi r3, r0, 24
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
