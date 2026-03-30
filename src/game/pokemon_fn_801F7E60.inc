    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r3
    li r30, 0x0
    b @L_801F7ECC
    @L_801F7E7C:
    mr r3, r29
    mr r6, r30
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr r31, r3
    bl fn_802062FC
    clrlwi. r0, r3, 24
    beq @L_801F7EC8
    mr r3, r31
    li r4, 0x1
    li r5, 0x0
    li r6, 0x0
    bl fn_80204A5C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F7EC8
    li r3, 0x1
    b @L_801F7EDC
    @L_801F7EC8:
    addi r30, r30, 0x1
    @L_801F7ECC:
    clrlwi r0, r30, 16
    cmplwi r0, 0x2
    blt @L_801F7E7C
    li r3, 0x0
    @L_801F7EDC:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
