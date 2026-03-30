    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r30, 0x28(r1)
    mr r30, r3
    mr r31, r7
    li r7, 0x0
    li r3, 0x0
    b @L_801F1A9C
    @L_801F1A90:
    clrlslwi r0, r7, 16, 2
    addi r7, r7, 0x1
    stwx r3, r5, r0
    @L_801F1A9C:
    clrlwi r0, r7, 16
    cmplwi r0, 0x18
    blt @L_801F1A90
    clrlwi r7, r6, 24
    li r0, 0x0
    stw r4, 0x8(r1)
    li r3, 0x0
    li r4, 0x0
    li r6, 0x0
    stw r5, 0x14(r1)
    li r5, 0x17
    stw r7, 0xc(r1)
    stw r0, 0x10(r1)
    bl fn_801F54A4
    clrlwi r0, r31, 24
    lis r4, fn_801F1B14@ha
    stw r3, 0x18(r1)
    addi r4, r4, fn_801F1B14@l
    mr r3, r30
    addi r5, r1, 0x8
    stw r0, 0x1c(r1)
    li r6, 0x0
    bl fn_801F2B5C
    lwz r0, 0x10(r1)
    lmw r30, 0x28(r1)
    clrlwi r3, r0, 16
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
