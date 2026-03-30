    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r4
    bne @L_8020126C
    li r3, 0x0
    b @L_8020132C
    @L_8020126C:
    li r4, 0x0
    li r5, 0x122
    li r6, 0x0
    bl fn_8012640C
    cmplwi r27, 0x0
    mr r31, r3
    li r4, 0x0
    li r3, 0x0
    beq @L_802012B8
    b @L_802012A0
    @L_80201294:
    clrlslwi r0, r4, 24, 1
    addi r4, r4, 0x1
    sthx r3, r27, r0
    @L_802012A0:
    clrlwi r0, r4, 24
    cmplwi r0, 0x4
    blt @L_80201294
    b @L_802012C4
    b @L_802012B8
    @L_802012B4:
    addi r4, r4, 0x1
    @L_802012B8:
    clrlwi r0, r4, 24
    cmplwi r0, 0x4
    blt @L_802012B4
    @L_802012C4:
    li r28, 0x0
    li r30, 0x0
    b @L_8020131C
    @L_802012D0:
    clrlwi r0, r30, 24
    mulli r0, r0, 0xc
    add r29, r31, r0
    mr r3, r29
    bl fn_8020E614
    clrlwi. r0, r3, 24
    beq @L_80201318
    mr r3, r29
    bl fn_801FD0EC
    clrlwi. r0, r3, 16
    beq @L_80201318
    cmplwi r0, 0x165
    beq @L_80201318
    cmplwi r27, 0x0
    beq @L_80201314
    clrlslwi r0, r28, 24, 1
    sthx r3, r27, r0
    @L_80201314:
    addi r28, r28, 0x1
    @L_80201318:
    addi r30, r30, 0x1
    @L_8020131C:
    clrlwi r0, r30, 24
    cmplwi r0, 0x4
    blt @L_802012D0
    mr r3, r28
    @L_8020132C:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
