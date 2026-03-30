    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    bl fn_80113F48
    mr r4, r29
    mr r31, r3
    bl fn_8018D998
    cmplwi r3, 0x0
    beq @L_801CB758
    mr r3, r31
    mr r4, r29
    rlwinm r5, r30, 0, 24, 31
    bl fn_8018B07C
    rlwinm r3, r3, 0, 24, 31
    b @L_801CB7A8
@L_801CB758:
    mr r3, r31
    mr r4, r29
    bl fn_800F9318
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_801CB778
    li r3, 0x0
    b @L_801CB7A8
@L_801CB778:
    mr r3, r31
    bl fn_800EC954
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CB794
    li r3, 0x0
    b @L_801CB7A8
@L_801CB794:
    cmpwi r30, 0x0
    beq @L_801CB7A4
    bl fn_800F0308
    b @L_801CB778
@L_801CB7A4:
    li r3, 0x1
@L_801CB7A8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
