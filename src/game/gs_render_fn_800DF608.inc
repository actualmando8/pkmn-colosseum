    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    lwz r5, 0x3c(r3)
    addis r0, r5, 0x102
    cmplwi r0, 0xfefe
    beq @L_800DF64C
    lwz r4, 0x8(r30)
    lis r3, 0xfeff
    subi r0, r3, 0x102
    stw r5, 0x10(r4)
    stw r0, 0x3c(r30)
    @L_800DF64C:
    lwz r3, 0x38(r30)
    addis r0, r3, 0x102
    cmplwi r0, 0xfefe
    beq @L_800DF684
    bl fn_800B8DF4
    lwz r3, 0x8(r30)
    lwz r0, 0x38(r30)
    lwz r4, 0x8(r3)
    lwz r3, 0x58(r4)
    stw r0, 0x58(r4)
    bl fn_801BBD3C
    lis r3, 0xfeff
    subi r0, r3, 0x102
    stw r0, 0x38(r30)
    @L_800DF684:
    lhz r29, 0x2(r30)
    lwz r31, 0x8(r30)
    clrlwi. r0, r29, 31
    beq @L_800DF6A8
    li r0, 0x7f
    stb r0, 0xf(r30)
    stb r0, 0xe(r30)
    stb r0, 0xd(r30)
    stb r0, 0xc(r30)
    @L_800DF6A8:
    rlwinm. r0, r29, 0, 30, 30
    beq @L_800DF6D0
    li r0, 0x0
    li r4, 0x1
    stw r0, 0x10(r30)
    li r3, 0x2
    li r0, 0x3
    stw r4, 0x14(r30)
    stw r3, 0x18(r30)
    stw r0, 0x1c(r30)
    @L_800DF6D0:
    rlwinm. r0, r29, 0, 29, 29
    beq @L_800DF754
    lwz r5, 0x8(r30)
    lwz r28, 0x20(r30)
    lwz r4, 0x8(r5)
    lwz r3, 0x24(r30)
    cmplwi r4, 0x0
    beq @L_800DF74C
    cmplwi r28, 0x0
    beq @L_800DF74C
    cmplw r4, r28
    bne @L_800DF728
    lwz r0, 0x8(r28)
    stw r0, 0x8(r5)
    b @L_800DF730
    b @L_800DF728
    @L_800DF710:
    lwz r0, 0x8(r4)
    cmplw r0, r28
    bne @L_800DF724
    lwz r0, 0x8(r28)
    stw r0, 0x8(r4)
    @L_800DF724:
    lwz r4, 0x8(r4)
    @L_800DF728:
    cmplwi r4, 0x0
    bne @L_800DF710
    @L_800DF730:
    cmplwi r3, 0x0
    beq @L_800DF73C
    bl fn_801BBD3C
    @L_800DF73C:
    cmplwi r28, 0x0
    beq @L_800DF74C
    mr r3, r28
    bl fn_801BBED4
    @L_800DF74C:
    li r0, 0x0
    stw r0, 0x28(r30)
    @L_800DF754:
    lhz r0, 0x2(r30)
    andc r0, r0, r29
    sth r0, 0x2(r30)
    lhz r0, 0x2(r30)
    cmplwi r0, 0x0
    bne @L_800DF774
    li r0, 0x0
    stw r0, 0x20(r31)
    @L_800DF774:
    mr r3, r31
    bl fn_801A6FF0
    li r0, 0x0
    stb r0, 0x0(r30)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
