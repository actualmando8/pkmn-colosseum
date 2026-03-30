    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr. r29, r3
    bne @L_80134370
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    @L_80134370:
    extsh. r0, r31
    blt @L_80134384
    extsh r0, r31
    cmpwi r0, 0xeb
    blt @L_8013438C
    @L_80134384:
    li r31, 0x0
    b @L_80134398
    @L_8013438C:
    slwi r4, r0, 2
    addi r31, r4, 0x6dec
    add r31, r3, r31
    @L_80134398:
    cmplwi r31, 0x0
    bne @L_801343A8
    li r3, 0x0
    b @L_80134404
    @L_801343A8:
    cmplwi r29, 0x0
    mr r3, r29
    bne @L_801343C0
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    @L_801343C0:
    extsh. r0, r30
    blt @L_801343D4
    extsh r0, r30
    cmpwi r0, 0xeb
    blt @L_801343DC
    @L_801343D4:
    li r4, 0x0
    b @L_801343E8
    @L_801343DC:
    slwi r4, r0, 2
    addi r4, r4, 0x6dec
    add r4, r3, r4
    @L_801343E8:
    cmplwi r4, 0x0
    bne @L_801343F8
    li r3, 0x0
    b @L_80134404
    @L_801343F8:
    mr r3, r31
    bl fn_80140A9C
    li r3, 0x1
    @L_80134404:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
