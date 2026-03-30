    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_80135550
    li r3, 0x0
    b @L_801356F4
    @L_80135550:
    bne @L_80135588
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_80135570
    li r0, 0x0
    b @L_801355A4
    @L_80135570:
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_80135588
    li r0, 0x0
    b @L_801355A4
    @L_80135588:
    bl fn_80135B0C
    cmplwi r3, 0x0
    bne @L_8013559C
    li r0, 0x0
    b @L_801355A4
    @L_8013559C:
    bl fn_80135AB8
    clrlwi r0, r3, 24
    @L_801355A4:
    cmpwi r0, 0x0
    bne @L_801355B4
    li r3, 0x0
    b @L_801356F4
    @L_801355B4:
    cmplwi r31, 0x0
    mr r3, r31
    bne @L_801355F4
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_801355DC
    li r0, 0x0
    b @L_80135610
    @L_801355DC:
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_801355F4
    li r0, 0x0
    b @L_80135610
    @L_801355F4:
    bl fn_80135B0C
    cmplwi r3, 0x0
    bne @L_80135608
    li r0, 0x0
    b @L_80135610
    @L_80135608:
    bl fn_80135AA0
    clrlwi r0, r3, 24
    @L_80135610:
    cmpwi r0, 0x0
    bne @L_80135620
    li r3, 0x0
    b @L_801356F4
    @L_80135620:
    cmplwi r31, 0x0
    mr r3, r31
    bne @L_80135660
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_80135648
    li r0, 0x0
    b @L_8013567C
    @L_80135648:
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_80135660
    li r0, 0x0
    b @L_8013567C
    @L_80135660:
    bl fn_80135B0C
    cmplwi r3, 0x0
    bne @L_80135674
    li r0, 0x0
    b @L_8013567C
    @L_80135674:
    bl fn_80135A88
    clrlwi r0, r3, 24
    @L_8013567C:
    cmpwi r0, 0x0
    bne @L_8013568C
    li r3, 0x0
    b @L_801356F4
    @L_8013568C:
    cmplwi r31, 0x0
    mr r3, r31
    bne @L_801356CC
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_801356B4
    li r3, 0x0
    b @L_801356E8
    @L_801356B4:
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_801356CC
    li r3, 0x0
    b @L_801356E8
    @L_801356CC:
    bl fn_80135B0C
    cmplwi r3, 0x0
    bne @L_801356E0
    li r3, 0x0
    b @L_801356E8
    @L_801356E0:
    bl fn_80135A70
    clrlwi r3, r3, 24
    @L_801356E8:
    neg r0, r3
    or r0, r0, r3
    srwi r3, r0, 31
    @L_801356F4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
