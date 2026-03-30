    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r3
    li r30, 0x0
    li r31, 0x0
    b @L_801F990C
    @L_801F98DC:
    mr r3, r29
    clrlwi r6, r31, 24
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    bl fn_802062FC
    clrlwi. r0, r3, 24
    beq @L_801F9908
    clrlwi r3, r30, 24
    addi r0, r3, 0x1
    clrlwi r30, r0, 24
    @L_801F9908:
    addi r31, r31, 0x1
    @L_801F990C:
    clrlwi r0, r31, 24
    cmplwi r0, 0x2
    blt @L_801F98DC
    mr r3, r30
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
