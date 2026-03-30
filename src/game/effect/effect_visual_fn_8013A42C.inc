    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x24
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_80131428
    mr. r31, r3
    beq @L_8013A484
    lis r6, fn_8013A49C@ha
    lis r7, fn_8013AA8C@ha
    lis r5, fn_8013A520@ha
    li r4, 0x0
    addi r9, r7, fn_8013AA8C@l
    addi r6, r6, fn_8013A49C@l
    addi r10, r5, fn_8013A520@l
    li r5, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80131200
    mr r3, r31
    li r4, 0x0
    bl fn_8013139C
    @L_8013A484:
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
