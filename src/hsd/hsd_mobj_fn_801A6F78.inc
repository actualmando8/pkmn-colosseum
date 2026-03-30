    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r5
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r0, 0x18(r3)
    cmplwi r0, 0x0
    bne @L_801A6FBC
    lis r4, lbl_80274E5C@ha
    la r3, lbl_8047DC18(r2)
    addi r5, r4, lbl_80274E5C@l
    li r4, 0x31e
    bl fn_80196E10
    @L_801A6FBC:
    lwz r3, 0x18(r29)
    lwz r4, 0x1c(r29)
    bl fn_801B45A4
    mr r3, r30
    mr r4, r31
    bl fn_801BD8D0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
