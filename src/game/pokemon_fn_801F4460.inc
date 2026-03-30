    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r3
    mr r26, r4
    li r28, 0x0
    b @L_801F4518
    @L_801F4480:
    mr r3, r25
    mr r6, r28
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr. r31, r3
    beq @L_801F4514
    li r30, 0x0
    b @L_801F4508
    @L_801F44A4:
    mr r3, r31
    mr r6, r30
    li r4, 0x0
    li r5, 0x7
    bl fn_801F76B8
    mr. r27, r3
    beq @L_801F4504
    li r29, 0x0
    b @L_801F44F8
    @L_801F44C8:
    mr r3, r27
    mr r6, r29
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    cmplwi r3, 0x0
    beq @L_801F44F4
    cmplw r26, r3
    bne @L_801F44F4
    mr r3, r27
    b @L_801F4528
    @L_801F44F4:
    addi r29, r29, 0x1
    @L_801F44F8:
    clrlwi r0, r29, 16
    cmplwi r0, 0x6
    blt @L_801F44C8
    @L_801F4504:
    addi r30, r30, 0x1
    @L_801F4508:
    clrlwi r0, r30, 16
    cmplwi r0, 0x2
    blt @L_801F44A4
    @L_801F4514:
    addi r28, r28, 0x1
    @L_801F4518:
    clrlwi r0, r28, 16
    cmplwi r0, 0x2
    blt @L_801F4480
    li r3, 0x0
    @L_801F4528:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
