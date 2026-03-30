    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr. r23, r3
    mr r26, r4
    mr r27, r5
    mr r28, r6
    bne @L_80136168
    li r3, 0x0
    b @L_80136354
    @L_80136168:
    bl fn_80136050
    mr r0, r3
    mr r3, r23
    mr r29, r0
    bl fn_80136024
    mr r0, r3
    mr r3, r23
    mr r30, r0
    li r4, 0x0
    bl fn_80135FBC
    mr r24, r3
    mr r3, r23
    li r4, 0x1
    bl fn_80135FBC
    mr r25, r3
    mr r3, r23
    bl fn_80135FF8
    clrlwi r23, r3, 24
    mr r3, r29
    mr r4, r26
    mr r6, r30
    clrlwi r7, r24, 16
    li r5, 0x0
    bl fn_80135E44
    cmplwi r23, 0x0
    mr r31, r3
    beq @L_801361E4
    cmplwi r23, 0x2
    beq @L_801361E4
    cmplwi r23, 0x3
    bne @L_8013625C
    @L_801361E4:
    extsh r0, r24
    cmpwi r0, -0x1
    bne @L_8013621C
    mr r3, r29
    mr r4, r26
    clrlwi r6, r25, 16
    li r5, 0x0
    li r7, 0x0
    bl fn_80135E44
    srwi r0, r3, 31
    add r0, r0, r3
    srawi r0, r0, 1
    extsh r24, r0
    b @L_8013625C
    @L_8013621C:
    cmpwi r0, -0x2
    bne @L_80136244
    mr r3, r29
    mr r4, r26
    clrlwi r6, r25, 16
    li r5, 0x0
    li r7, 0x0
    bl fn_80135E44
    extsh r24, r3
    b @L_8013625C
    @L_80136244:
    extsh. r0, r24
    blt @L_80136254
    extsh. r0, r25
    bge @L_8013625C
    @L_80136254:
    li r3, 0x0
    b @L_80136354
    @L_8013625C:
    cmpwi r23, 0x3
    beq @L_801362B8
    bge @L_80136280
    cmpwi r23, 0x1
    beq @L_80136298
    bge @L_801362AC
    cmpwi r23, 0x0
    bge @L_80136290
    b @L_801362F4
    @L_80136280:
    cmpwi r23, 0x5
    beq @L_801362DC
    bge @L_801362F4
    b @L_801362C4
    @L_80136290:
    extsh r23, r24
    b @L_801362FC
    @L_80136298:
    extsh r3, r24
    extsh r0, r25
    mullw r3, r31, r3
    divw r23, r3, r0
    b @L_801362FC
    @L_801362AC:
    extsh r0, r24
    add r23, r31, r0
    b @L_801362FC
    @L_801362B8:
    extsh r0, r24
    subf r23, r0, r31
    b @L_801362FC
    @L_801362C4:
    extsh r3, r24
    extsh r0, r25
    mullw r3, r31, r3
    divw r0, r3, r0
    add r23, r31, r0
    b @L_801362FC
    @L_801362DC:
    extsh r3, r24
    extsh r0, r25
    mullw r3, r31, r3
    divw r0, r3, r0
    subf r23, r0, r31
    b @L_801362FC
    @L_801362F4:
    li r3, 0x0
    b @L_80136354
    @L_801362FC:
    cmplwi r27, 0x0
    beq @L_8013632C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r4, r3, 16
    mr r3, r27
    bl fn_801F0134
    mr r7, r3
    b @L_80136330
    @L_8013632C:
    li r7, 0x0
    @L_80136330:
    mr r3, r29
    mr r4, r26
    mr r6, r30
    mr r8, r23
    li r5, 0x0
    bl fn_80135D10
    cmplwi r28, 0x0
    beq @L_80136354
    stw r23, 0x0(r28)
    @L_80136354:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
