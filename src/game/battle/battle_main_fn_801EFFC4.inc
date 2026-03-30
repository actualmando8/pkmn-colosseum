    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr. r30, r3
    beq @L_801EFFF8
    li r31, 0x0
    b @L_801EFFF0
@L_801EFFE4:
    bl fn_800F0308
    bl fn_800D3088
    add r31, r31, r3
@L_801EFFF0:
    cmplw r31, r30
    blt @L_801EFFE4
@L_801EFFF8:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
