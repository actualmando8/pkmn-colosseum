    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r9, 0x0
    stw r0, 0x24(r1)
    mr r8, r9
    b @L_801F1C3C
    @L_801F1C30:
    clrlslwi r0, r9, 16, 2
    addi r9, r9, 0x1
    stwx r8, r5, r0
    @L_801F1C3C:
    clrlwi r0, r9, 16
    cmplwi r0, 0x8
    blt @L_801F1C30
    clrlwi r8, r6, 24
    clrlwi r7, r7, 24
    li r9, 0x0
    lis r6, fn_801F1C98@ha
    stw r5, 0xc(r1)
    addi r0, r6, fn_801F1C98@l
    addi r5, r1, 0x8
    li r6, 0x0
    stw r4, 0x8(r1)
    mr r4, r0
    stw r9, 0x10(r1)
    stw r8, 0x14(r1)
    stw r7, 0x18(r1)
    bl fn_801F37B0
    lwz r0, 0x10(r1)
    clrlwi r3, r0, 16
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
