    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    clrlwi r7, r4, 24
    lis r4, fn_801F26A8@ha
    stw r0, 0x24(r1)
    clrlwi r0, r6, 24
    li r6, 0x0
    addi r4, r4, fn_801F26A8@l
    stw r5, 0x8(r1)
    addi r5, r1, 0x8
    stw r6, 0xc(r1)
    li r6, 0x0
    stw r7, 0x10(r1)
    stw r0, 0x14(r1)
    bl fn_801F37B0
    lwz r0, 0xc(r1)
    clrlwi r3, r0, 16
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
