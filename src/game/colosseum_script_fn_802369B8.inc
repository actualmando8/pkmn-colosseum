    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r26, r3
    mr r27, r4
    mr r28, r5
    mr r29, r6
    mr r30, r7
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r26
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    li r31, 0x0
    li r5, 0x0
    li r4, 0x0
    li r3, -0x1
    b @L_80236A50
@L_80236A2C:
    cmplwi r28, 0x0
    beq @L_80236A3C
    clrlslwi r0, r5, 16, 1
    sthx r4, r28, r0
@L_80236A3C:
    cmplwi r29, 0x0
    beq @L_80236A4C
    clrlslwi r0, r5, 16, 1
    sthx r3, r29, r0
@L_80236A4C:
    addi r5, r5, 0x1
@L_80236A50:
    rlwinm r0, r5, 0, 16, 31
    cmplwi r0, 0xa
    blt @L_80236A2C
    li r23, 0x0
    b @L_80236B74
@L_80236A64:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r26
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r27
    bl fn_80205BE8
    mr r3, r27
    bl fn_80205BE8
    rlwinm r24, r23, 0, 24, 31
    li r4, 0x0
    li r5, 0x7f
    mr r6, r24
    bl fn_8012640C
    rlwinm. r25, r3, 0, 16, 31
    beq @L_80236B70
    cmplwi r25, 0x165
    bne @L_80236ADC
    cmplwi r25, 0x163
    beq @L_80236B70
@L_80236ADC:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80236B4C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r26
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r27
    bl fn_80205BE8
    mr r3, r27
    bl fn_80205BE8
    mr r6, r24
    li r4, 0x0
    li r5, 0x80
    bl fn_8012640C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80236B70
@L_80236B4C:
    cmplwi r28, 0x0
    beq @L_80236B5C
    clrlslwi r0, r31, 16, 1
    sthx r25, r28, r0
@L_80236B5C:
    cmplwi r29, 0x0
    beq @L_80236B6C
    clrlslwi r0, r31, 16, 1
    sthx r23, r29, r0
@L_80236B6C:
    addi r31, r31, 0x1
@L_80236B70:
    addi r23, r23, 0x1
@L_80236B74:
    rlwinm r0, r23, 0, 16, 31
    cmplwi r0, 0x4
    blt @L_80236A64
    mr r3, r31
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
