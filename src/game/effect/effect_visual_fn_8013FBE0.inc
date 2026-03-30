    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x24
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_80131428
    mr. r31, r3
    beq @L_8013FC40
    lis r4, fn_8013FCC4@ha
    lis r5, fn_8013FC58@ha
    lis r6, fn_8013FDD0@ha
    lis r7, fn_8013FD68@ha
    lis r8, fn_8013FF0C@ha
    addi r4, r4, fn_8013FCC4@l
    addi r9, r8, fn_8013FF0C@l
    addi r5, r5, fn_8013FC58@l
    addi r6, r6, fn_8013FDD0@l
    addi r7, r7, fn_8013FD68@l
    li r8, 0x0
    li r10, 0x0
    bl fn_80131200
    mr r3, r31
    li r4, 0x0
    bl fn_8013139C
    @L_8013FC40:
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
