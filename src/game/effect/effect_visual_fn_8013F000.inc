    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0xb4
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_80131428
    mr. r31, r3
    beq @L_8013F060
    lis r4, fn_8013F114@ha
    lis r5, fn_8013F078@ha
    lis r6, fn_8013F344@ha
    lis r8, fn_8013F80C@ha
    lis r7, fn_8013F410@ha
    addi r4, r4, fn_8013F114@l
    addi r9, r8, fn_8013F80C@l
    addi r5, r5, fn_8013F078@l
    addi r10, r7, fn_8013F410@l
    addi r6, r6, fn_8013F344@l
    li r7, 0x0
    li r8, 0x0
    bl fn_80131200
    mr r3, r31
    li r4, 0x0
    bl fn_8013139C
    @L_8013F060:
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
