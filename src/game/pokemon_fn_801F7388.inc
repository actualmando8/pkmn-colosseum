    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r3
    li r30, 0x0
    li r31, 0x0
    b @L_801F73E0
    @L_801F73A8:
    cmplwi r29, 0x0
    bne @L_801F73B8
    li r3, 0x0
    b @L_801F73C4
    @L_801F73B8:
    mr r3, r29
    clrlwi r4, r31, 24
    bl fn_801F78D4
    @L_801F73C4:
    bl fn_801FA524
    clrlwi. r0, r3, 24
    beq @L_801F73DC
    clrlwi r3, r30, 24
    addi r0, r3, 0x1
    clrlwi r30, r0, 24
    @L_801F73DC:
    addi r31, r31, 0x1
    @L_801F73E0:
    clrlwi r0, r31, 24
    cmplwi r0, 0x2
    blt @L_801F73A8
    mr r3, r30
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
