    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    stw r28, 0x10(r1)
    lhz r0, 0x2(r3)
    lwz r30, 0x8(r3)
    and r31, r4, r0
    clrlwi. r0, r31, 31
    beq @L_800DF290
    li r0, 0x7f
    stb r0, 0xf(r29)
    stb r0, 0xe(r29)
    stb r0, 0xd(r29)
    stb r0, 0xc(r29)
    @L_800DF290:
    rlwinm. r0, r31, 0, 30, 30
    beq @L_800DF2B8
    li r0, 0x0
    li r4, 0x1
    stw r0, 0x10(r29)
    li r3, 0x2
    li r0, 0x3
    stw r4, 0x14(r29)
    stw r3, 0x18(r29)
    stw r0, 0x1c(r29)
    @L_800DF2B8:
    rlwinm. r0, r31, 0, 29, 29
    beq @L_800DF33C
    lwz r5, 0x8(r29)
    lwz r28, 0x20(r29)
    lwz r4, 0x8(r5)
    lwz r3, 0x24(r29)
    cmplwi r4, 0x0
    beq @L_800DF334
    cmplwi r28, 0x0
    beq @L_800DF334
    cmplw r4, r28
    bne @L_800DF310
    lwz r0, 0x8(r28)
    stw r0, 0x8(r5)
    b @L_800DF318
    b @L_800DF310
    @L_800DF2F8:
    lwz r0, 0x8(r4)
    cmplw r0, r28
    bne @L_800DF30C
    lwz r0, 0x8(r28)
    stw r0, 0x8(r4)
    @L_800DF30C:
    lwz r4, 0x8(r4)
    @L_800DF310:
    cmplwi r4, 0x0
    bne @L_800DF2F8
    @L_800DF318:
    cmplwi r3, 0x0
    beq @L_800DF324
    bl fn_801BBD3C
    @L_800DF324:
    cmplwi r28, 0x0
    beq @L_800DF334
    mr r3, r28
    bl fn_801BBED4
    @L_800DF334:
    li r0, 0x0
    stw r0, 0x28(r29)
    @L_800DF33C:
    lhz r0, 0x2(r29)
    andc r0, r0, r31
    sth r0, 0x2(r29)
    lhz r0, 0x2(r29)
    cmplwi r0, 0x0
    bne @L_800DF35C
    li r0, 0x0
    stw r0, 0x20(r30)
    @L_800DF35C:
    mr r3, r30
    bl fn_801A6FF0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
