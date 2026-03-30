    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r28, 0x30(r1)
    mr r28, r3
    mr r29, r4
    addi r31, r1, 0x8
    li r30, 0x0
    b @L_801F3B68
    @L_801F3B48:
    mr r3, r28
    mr r6, r30
    li r4, 0x0
    li r5, 0x59
    bl fn_801F54A4
    clrlslwi r0, r30, 16, 2
    addi r30, r30, 0x1
    stwx r3, r31, r0
    @L_801F3B68:
    clrlwi r0, r30, 16
    cmplwi r0, 0x8
    blt @L_801F3B48
    mr r3, r28
    mr r6, r29
    addi r4, r1, 0x8
    li r5, 0x8
    bl fn_801F3BB4
    mr r3, r28
    addi r7, r1, 0x8
    li r4, 0x0
    li r5, 0x5a
    li r6, 0x0
    bl fn_801F4C14
    lmw r28, 0x30(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
