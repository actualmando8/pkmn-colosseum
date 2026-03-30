    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x34
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_80131428
    mr. r31, r3
    beq @L_8013E534
    lis r4, fn_8013E5AC@ha
    lis r5, fn_8013E54C@ha
    lis r6, fn_8013E658@ha
    lis r8, fn_8013E8A4@ha
    lis r7, fn_8013E6C4@ha
    addi r4, r4, fn_8013E5AC@l
    addi r9, r8, fn_8013E8A4@l
    addi r5, r5, fn_8013E54C@l
    addi r10, r7, fn_8013E6C4@l
    addi r6, r6, fn_8013E658@l
    li r7, 0x0
    li r8, 0x0
    bl fn_80131200
    mr r3, r31
    li r4, 0x0
    bl fn_8013139C
    @L_8013E534:
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
