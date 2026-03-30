    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x20
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_80131428
    mr. r31, r3
    beq @L_8013DC7C
    lis r4, fn_8013DD10@ha
    lis r5, fn_8013DC94@ha
    lis r6, fn_8013DDCC@ha
    lis r7, fn_8013DD7C@ha
    lis r9, fn_8013E470@ha
    lis r8, fn_8013DE6C@ha
    addi r10, r8, fn_8013DE6C@l
    addi r4, r4, fn_8013DD10@l
    addi r5, r5, fn_8013DC94@l
    addi r6, r6, fn_8013DDCC@l
    addi r7, r7, fn_8013DD7C@l
    addi r9, r9, fn_8013E470@l
    li r8, 0x0
    bl fn_80131200
    mr r3, r31
    li r4, 0x0
    bl fn_8013139C
    @L_8013DC7C:
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
