    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr. r29, r3
    bne @L_801FA544
    li r0, 0x0
    b @L_801FA5BC
    @L_801FA544:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801FA55C
    li r0, 0x0
    b @L_801FA5BC
    @L_801FA55C:
    mr r3, r29
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    cmpwi r3, 0x0
    bne @L_801FA580
    li r0, 0x0
    b @L_801FA5BC
    @L_801FA580:
    mr r3, r29
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    bne @L_801FA5A4
    li r0, 0x0
    b @L_801FA5BC
    @L_801FA5A4:
    bl fn_8012A130
    clrlwi. r0, r3, 24
    bne @L_801FA5B8
    li r0, 0x0
    b @L_801FA5BC
    @L_801FA5B8:
    li r0, 0x1
    @L_801FA5BC:
    clrlwi. r0, r0, 24
    bne @L_801FA5CC
    li r3, 0x0
    b @L_801FA620
    @L_801FA5CC:
    li r31, 0x0
    mr r30, r31
    b @L_801FA608
    @L_801FA5D8:
    mr r3, r29
    clrlwi r6, r30, 24
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    bl fn_80206608
    clrlwi. r0, r3, 24
    beq @L_801FA604
    clrlwi r3, r31, 24
    addi r0, r3, 0x1
    clrlwi r31, r0, 24
    @L_801FA604:
    addi r30, r30, 0x1
    @L_801FA608:
    clrlwi r0, r30, 24
    cmplwi r0, 0x6
    blt @L_801FA5D8
    clrlwi r3, r31, 24
    subic r0, r3, 0x1
    subfe r3, r0, r3
    @L_801FA620:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
