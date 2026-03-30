    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r5
    stw r30, 0x18(r1)
    mr. r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    bne @L_801AB570
    addi r3, r2, -23016
    li r4, 0x663
    addi r5, r2, -22976
    bl fn_80196E10
@L_801AB570:
    cmplwi r31, 0x0
    bne @L_801AB588
    addi r3, r2, -23016
    li r4, 0x664
    addi r5, r2, -22972
    bl fn_80196E10
@L_801AB588:
    cmpwi r29, 0x0
    blt @L_801AB598
    cmpwi r29, 0x2
    blt @L_801AB5AC
@L_801AB598:
    li r3, 0x0
    li r0, 0x0
    stw r3, 0x0(r30)
    stw r0, 0x0(r31)
    b @L_801AB5DC
@L_801AB5AC:
    lis r4, lbl_80465678@ha
    lis r3, lbl_80465678@ha
    rlwinm r5, r29, 3, 0, 28
    rlwinm r0, r29, 3, 0, 28
    addi r4, r4, lbl_80465678@l
    addi r3, r3, lbl_80465678@l
    add r4, r4, r5
    lwz r4, 0x0(r4)
    add r3, r3, r0
    stw r4, 0x0(r30)
    lwz r0, 0x4(r3)
    stw r0, 0x0(r31)
@L_801AB5DC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
