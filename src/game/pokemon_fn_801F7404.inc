    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_801F7424
    li r3, 0x0
    b @L_801F746C
    @L_801F7424:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801F743C
    li r3, 0x0
    b @L_801F746C
    @L_801F743C:
    cmplwi r31, 0x0
    bne @L_801F744C
    li r0, 0x0
    b @L_801F7458
    @L_801F744C:
    mr r3, r31
    bl fn_801F793C
    clrlwi r0, r3, 16
    @L_801F7458:
    cmpwi r0, 0x0
    bne @L_801F7468
    li r3, 0x0
    b @L_801F746C
    @L_801F7468:
    li r3, 0x1
    @L_801F746C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
