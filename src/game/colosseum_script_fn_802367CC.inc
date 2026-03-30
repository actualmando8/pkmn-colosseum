    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r25, r3
    mr r26, r4
    mr r27, r5
    mr r28, r6
    mr r29, r7
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r25
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    li r30, 0x0
    li r5, 0x0
    li r4, 0x0
    li r3, -0x1
    b @L_80236864
@L_80236840:
    cmplwi r27, 0x0
    beq @L_80236850
    clrlslwi r0, r5, 16, 1
    sthx r4, r27, r0
@L_80236850:
    cmplwi r28, 0x0
    beq @L_80236860
    clrlslwi r0, r5, 16, 1
    sthx r3, r28, r0
@L_80236860:
    addi r5, r5, 0x1
@L_80236864:
    rlwinm r0, r5, 0, 16, 31
    cmplwi r0, 0xa
    blt @L_80236840
    li r31, 0x0
    b @L_80236994
@L_80236878:
    mr r3, r26
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r24, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r25
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r24
    bl fn_80205BE8
    mr r3, r24
    bl fn_80205BE8
    rlwinm r23, r31, 0, 24, 31
    li r4, 0x0
    li r5, 0x7f
    mr r6, r23
    bl fn_8012640C
    rlwinm. r24, r3, 0, 16, 31
    beq @L_80236990
    cmplwi r24, 0x165
    bne @L_80236908
    cmplwi r24, 0x163
    beq @L_80236990
@L_80236908:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023696C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r25
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r26
    mr r4, r23
    li r5, 0x0
    li r6, 0x0
    bl fn_801FFEC8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80236990
@L_8023696C:
    cmplwi r27, 0x0
    beq @L_8023697C
    clrlslwi r0, r30, 16, 1
    sthx r24, r27, r0
@L_8023697C:
    cmplwi r28, 0x0
    beq @L_8023698C
    clrlslwi r0, r30, 16, 1
    sthx r31, r28, r0
@L_8023698C:
    addi r30, r30, 0x1
@L_80236990:
    addi r31, r31, 0x1
@L_80236994:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x4
    blt @L_80236878
    mr r3, r30
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
