    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bne @L_801F7278
    li r31, 0x0
    b @L_801F7280
    @L_801F7278:
    bl fn_801F78D4
    mr r31, r3
    @L_801F7280:
    mr r3, r31
    bl fn_801FA634
    clrlwi. r0, r3, 24
    bne @L_801F7298
    li r3, 0x0
    b @L_801F729C
    @L_801F7298:
    mr r3, r31
    @L_801F729C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
