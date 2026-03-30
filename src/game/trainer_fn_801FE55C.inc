    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr. r31, r3
    mr r30, r5
    beq @L_801FE5C0
    cmplwi r4, 0x0
    beq @L_801FE5C0
    cmplw r31, r4
    beq @L_801FE5C0
    mr r3, r4
    li r4, 0x0
    li r5, 0x122
    li r6, 0x0
    bl fn_8012640C
    mr r5, r31
    li r4, 0x4
    bl fn_8020E57C
    mr. r31, r3
    beq @L_801FE5C0
    bl fn_801FD0BC
    add r4, r3, r30
    mr r3, r31
    bl fn_801FD07C
    @L_801FE5C0:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
