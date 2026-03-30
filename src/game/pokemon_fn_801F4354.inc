    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r5, 0xd5
    li r6, 0x0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r3
    mr r3, r4
    li r4, 0x0
    bl fn_8012640C
    mr. r31, r3
    bne @L_801F438C
    li r3, 0x0
    b @L_801F444C
    @L_801F438C:
    li r29, 0x0
    b @L_801F4428
    @L_801F4394:
    mr r3, r25
    mr r6, r29
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr. r26, r3
    beq @L_801F4424
    li r27, 0x0
    b @L_801F4418
    @L_801F43B8:
    mr r3, r26
    mr r6, r27
    li r4, 0x0
    li r5, 0x7
    bl fn_801F76B8
    mr. r30, r3
    beq @L_801F4414
    li r28, 0x0
    b @L_801F4408
    @L_801F43DC:
    mr r3, r30
    mr r6, r28
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    cmplwi r3, 0x0
    beq @L_801F4404
    cmplw r31, r3
    bne @L_801F4404
    b @L_801F4438
    @L_801F4404:
    addi r28, r28, 0x1
    @L_801F4408:
    clrlwi r0, r28, 16
    cmplwi r0, 0x6
    blt @L_801F43DC
    @L_801F4414:
    addi r27, r27, 0x1
    @L_801F4418:
    clrlwi r0, r27, 16
    cmplwi r0, 0x2
    blt @L_801F43B8
    @L_801F4424:
    addi r29, r29, 0x1
    @L_801F4428:
    clrlwi r0, r29, 16
    cmplwi r0, 0x2
    blt @L_801F4394
    li r30, 0x0
    @L_801F4438:
    cmplwi r30, 0x0
    bne @L_801F4448
    li r3, 0x0
    b @L_801F444C
    @L_801F4448:
    mr r3, r30
    @L_801F444C:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
