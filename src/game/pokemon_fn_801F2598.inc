    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    mr r9, r5
    li r8, 0x0
    stw r0, 0x34(r1)
    mr r7, r6
    mr r6, r8
    addi r5, r1, 0x8
    stw r31, 0x2c(r1)
    b @L_801F25CC
    @L_801F25C0:
    clrlslwi r0, r8, 16, 2
    addi r8, r8, 0x1
    stwx r6, r5, r0
    @L_801F25CC:
    clrlwi r0, r8, 16
    cmplwi r0, 0x8
    blt @L_801F25C0
    mr r5, r4
    mr r6, r9
    addi r4, r1, 0x8
    bl fn_801F61EC
    clrlwi. r0, r3, 16
    mr r31, r3
    bne @L_801F25FC
    li r3, 0x0
    b @L_801F2640
    @L_801F25FC:
    bl fn_800E0C54
    clrlwi r4, r3, 16
    clrlwi r3, r31, 16
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r0, r0, r4
    clrlwi r0, r0, 16
    cmplwi r0, 0x8
    blt @L_801F2628
    li r3, 0x0
    b @L_801F2640
    @L_801F2628:
    clrlslwi r0, r0, 16, 2
    addi r3, r1, 0x8
    lwzx r3, r3, r0
    cmplwi r3, 0x0
    bne @L_801F2640
    li r3, 0x0
    @L_801F2640:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
