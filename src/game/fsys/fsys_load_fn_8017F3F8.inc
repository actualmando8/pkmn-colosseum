    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    stw r5, 0x8(r1)
    lwz r5, 0x8(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    addi r0, r5, 0x1f
    clrrwi r30, r0, 5
    bl fn_800F9318
    mr r0, r3
    mr r3, r30
    stw r0, 0xc(r1)
    mr r5, r28
    mr r6, r29
    li r4, 0x20
    li r7, 0x0
    bl fn_800F9418
    mr. r31, r3
    bne @L_8017F460
    li r3, 0x0
    b @L_8017F464
    @L_8017F460:
    mr r3, r31
    @L_8017F464:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
