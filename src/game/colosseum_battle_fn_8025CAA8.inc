    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r4
    mr r31, r5
    li r4, 0x0
    li r5, 0x43
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x24
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025CB24
    mr r3, r30
    mr r4, r31
    bl fn_80229C28
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025CB24
    li r3, 0x1
    b @L_8025CB28
@L_8025CB24:
    li r3, 0x0
@L_8025CB28:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
