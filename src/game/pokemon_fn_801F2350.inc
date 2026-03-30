    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r4, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    bne @L_801F2370
    li r3, -0x1
    b @L_801F2420
    @L_801F2370:
    mr r3, r4
    li r4, 0x0
    li r5, 0x122
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
    li r30, -0x1
    li r28, 0x0
    b @L_801F2410
    @L_801F2394:
    clrlwi r0, r28, 16
    mulli r0, r0, 0xc
    add r29, r31, r0
    mr r3, r29
    bl fn_8020E614
    clrlwi. r0, r3, 24
    beq @L_801F240C
    mr r3, r29
    bl fn_801FD104
    cmplwi r3, 0x0
    beq @L_801F240C
    bl fn_80206780
    clrlwi. r0, r3, 24
    beq @L_801F240C
    mr r3, r29
    bl fn_801FD0D4
    mr r0, r3
    mr r3, r29
    mr r29, r0
    bl fn_801FD0BC
    clrlwi. r0, r29, 16
    bne @L_801F23F0
    li r29, 0x1
    @L_801F23F0:
    clrlwi r3, r3, 16
    clrlwi r0, r29, 16
    mulli r3, r3, 0x64
    divw r0, r3, r0
    cmpw r0, r30
    ble @L_801F240C
    mr r30, r0
    @L_801F240C:
    addi r28, r28, 0x1
    @L_801F2410:
    clrlwi r0, r28, 16
    cmplwi r0, 0x4
    blt @L_801F2394
    mr r3, r30
    @L_801F2420:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
