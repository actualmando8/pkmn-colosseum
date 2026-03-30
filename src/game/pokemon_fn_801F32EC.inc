    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r28, r5
    mr r31, r3
    bl fn_801FA634
    clrlwi. r0, r3, 24
    bne @L_801F3318
    li r3, 0x1
    b @L_801F33D4
    @L_801F3318:
    li r0, 0x0
    mr r3, r31
    sth r0, 0x8(r1)
    addi r4, r1, 0x8
    bl fn_801F8A18
    cmplwi r3, 0x0
    bne @L_801F333C
    li r3, 0x1
    b @L_801F33D4
    @L_801F333C:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x18
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r29, r3, 16
    li r30, 0x0
    b @L_801F33C4
    @L_801F335C:
    mr r3, r31
    mr r6, r30
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    cmplwi r3, 0x0
    beq @L_801F33C0
    bl fn_802062FC
    clrlwi. r0, r3, 24
    bne @L_801F33C0
    cmplwi r28, 0x0
    beq @L_801F33C0
    lbz r0, 0x0(r28)
    cmplwi r0, 0x2
    beq @L_801F33C0
    mr r3, r31
    bl fn_801FB8F8
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F33B8
    li r0, 0x2
    stb r0, 0x0(r28)
    b @L_801F33C0
    @L_801F33B8:
    li r0, 0x1
    stb r0, 0x0(r28)
    @L_801F33C0:
    addi r30, r30, 0x1
    @L_801F33C4:
    clrlwi r0, r30, 16
    cmplw r0, r29
    blt @L_801F335C
    li r3, 0x1
    @L_801F33D4:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
