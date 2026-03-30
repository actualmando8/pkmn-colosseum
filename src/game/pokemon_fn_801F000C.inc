    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_80008184
    mr. r31, r3
    beq @L_801F0044
    li r30, 0x0
    b @L_801F003C
    @L_801F0030:
    bl fn_800F0308
    bl fn_800D3088
    add r30, r30, r3
    @L_801F003C:
    cmplw r30, r31
    blt @L_801F0030
    @L_801F0044:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
