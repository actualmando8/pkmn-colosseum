    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x60
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_80131428
    mr. r31, r3
    beq @L_80139880
    lis r5, fn_80139898@ha
    lis r6, fn_80139D10@ha
    lis r7, fn_801398E0@ha
    lis r8, fn_80139AC4@ha
    lis r4, fn_80139934@ha
    addi r5, r5, fn_80139898@l
    addi r9, r8, fn_80139AC4@l
    addi r6, r6, fn_80139D10@l
    addi r10, r4, fn_80139934@l
    addi r7, r7, fn_801398E0@l
    li r4, 0x0
    li r8, 0x0
    bl fn_80131200
    mr r3, r31
    li r4, 0x0
    bl fn_8013139C
    @L_80139880:
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
