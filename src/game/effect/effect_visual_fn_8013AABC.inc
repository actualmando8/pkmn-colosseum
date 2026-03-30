    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x5c
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_80131428
    mr. r31, r3
    beq @L_8013AB1C
    lis r5, fn_8013B034@ha
    lis r6, fn_8013B158@ha
    lis r7, fn_8013B0A0@ha
    lis r8, fn_8013AD68@ha
    lis r4, fn_8013AD9C@ha
    addi r5, r5, fn_8013B034@l
    addi r9, r4, fn_8013AD9C@l
    addi r6, r6, fn_8013B158@l
    addi r7, r7, fn_8013B0A0@l
    addi r8, r8, fn_8013AD68@l
    li r4, 0x0
    li r10, 0x0
    bl fn_80131200
    mr r3, r31
    li r4, 0x0
    bl fn_8013139C
    @L_8013AB1C:
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
