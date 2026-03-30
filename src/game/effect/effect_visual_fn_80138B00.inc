    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x4c
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_80131428
    mr. r31, r3
    beq @L_80138B5C
    lis r5, fn_80138B74@ha
    lis r6, fn_80138CCC@ha
    lis r7, fn_80138BBC@ha
    lis r4, fn_80138DE4@ha
    addi r9, r4, fn_80138DE4@l
    addi r5, r5, fn_80138B74@l
    addi r6, r6, fn_80138CCC@l
    addi r7, r7, fn_80138BBC@l
    li r4, 0x0
    li r8, 0x0
    li r10, 0x0
    bl fn_80131200
    mr r3, r31
    li r4, 0x0
    bl fn_8013139C
    @L_80138B5C:
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
