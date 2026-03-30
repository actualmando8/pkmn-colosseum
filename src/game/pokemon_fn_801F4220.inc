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
    bne @L_801F4258
    li r30, 0x0
    b @L_801F4310
    @L_801F4258:
    li r29, 0x0
    b @L_801F42F4
    @L_801F4260:
    mr r3, r25
    mr r6, r29
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr. r26, r3
    beq @L_801F42F0
    li r27, 0x0
    b @L_801F42E4
    @L_801F4284:
    mr r3, r26
    mr r6, r27
    li r4, 0x0
    li r5, 0x7
    bl fn_801F76B8
    mr. r30, r3
    beq @L_801F42E0
    li r28, 0x0
    b @L_801F42D4
    @L_801F42A8:
    mr r3, r30
    mr r6, r28
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    cmplwi r3, 0x0
    beq @L_801F42D0
    cmplw r31, r3
    bne @L_801F42D0
    b @L_801F4304
    @L_801F42D0:
    addi r28, r28, 0x1
    @L_801F42D4:
    clrlwi r0, r28, 16
    cmplwi r0, 0x6
    blt @L_801F42A8
    @L_801F42E0:
    addi r27, r27, 0x1
    @L_801F42E4:
    clrlwi r0, r27, 16
    cmplwi r0, 0x2
    blt @L_801F4284
    @L_801F42F0:
    addi r29, r29, 0x1
    @L_801F42F4:
    clrlwi r0, r29, 16
    cmplwi r0, 0x2
    blt @L_801F4260
    li r30, 0x0
    @L_801F4304:
    cmplwi r30, 0x0
    bne @L_801F4310
    li r30, 0x0
    @L_801F4310:
    cmplwi r30, 0x0
    bne @L_801F4320
    li r3, 0x0
    b @L_801F4340
    @L_801F4320:
    mr r3, r30
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    bne @L_801F4340
    li r3, 0x0
    @L_801F4340:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
