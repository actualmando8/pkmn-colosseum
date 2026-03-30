    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r23, 0x3c(r1)
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
    li r31, 0x0
    addi r5, r1, 0x1c
    mr r6, r31
    addi r3, r1, 0x8
    li r4, -0x1
    b @L_80236620
@L_802365FC:
    addic. r0, r1, 0x1c
    beq @L_8023660C
    clrlslwi r0, r6, 16, 1
    sthx r31, r5, r0
@L_8023660C:
    addic. r0, r1, 0x8
    beq @L_8023661C
    clrlslwi r0, r6, 16, 1
    sthx r4, r3, r0
@L_8023661C:
    addi r6, r6, 0x1
@L_80236620:
    rlwinm r0, r6, 0, 16, 31
    cmplwi r0, 0xa
    blt @L_802365FC
    li r30, 0x0
    b @L_80236758
@L_80236634:
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
    rlwinm r23, r30, 0, 24, 31
    li r4, 0x0
    li r5, 0x7f
    mr r6, r23
    bl fn_8012640C
    rlwinm. r24, r3, 0, 16, 31
    beq @L_80236754
    cmplwi r24, 0x165
    bne @L_802366C4
    cmplwi r24, 0x163
    beq @L_80236754
@L_802366C4:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80236728
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
    bne @L_80236754
@L_80236728:
    addic. r0, r1, 0x1c
    beq @L_8023673C
    clrlslwi r0, r31, 16, 1
    addi r3, r1, 0x1c
    sthx r24, r3, r0
@L_8023673C:
    addic. r0, r1, 0x8
    beq @L_80236750
    clrlslwi r0, r31, 16, 1
    addi r3, r1, 0x8
    sthx r30, r3, r0
@L_80236750:
    addi r31, r31, 0x1
@L_80236754:
    addi r30, r30, 0x1
@L_80236758:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x4
    blt @L_80236634
    rlwinm r5, r27, 0, 16, 31
    addi r4, r1, 0x1c
    rlwinm r0, r31, 0, 16, 31
    li r7, 0x0
    b @L_802367A8
@L_80236778:
    clrlslwi r6, r7, 16, 1
    lhzx r3, r4, r6
    cmplw r5, r3
    bne @L_802367A4
    cmplwi r28, 0x0
    beq @L_8023679C
    addi r3, r1, 0x8
    lhax r0, r3, r6
    sth r0, 0x0(r28)
@L_8023679C:
    li r3, 0x1
    b @L_802367B8
@L_802367A4:
    addi r7, r7, 0x1
@L_802367A8:
    rlwinm r3, r7, 0, 16, 31
    cmplw r3, r0
    blt @L_80236778
    li r3, 0x0
@L_802367B8:
    lmw r23, 0x3c(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
