    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r28, 0x10(r1)
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r29, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r29
    mr r28, r0
    li r4, 0x0
    li r5, 0xf7
    li r6, 0x0
    bl fn_8012640C
    rlwinm. r31, r3, 0, 16, 31
    beq @L_8021CDDC
    cmplwi r31, 0x165
    beq @L_8021CDDC
    cmplwi r31, 0xffff
    beq @L_8021CDDC
@L_8021CD5C:
    lwz r0, lbl_8047B618(r13)
    mr r3, r28
    mr r4, r31
    rlwinm r0, r0, 0, 22, 20
    stw r0, lbl_8047B618(r13)
    bl fn_80209C1C
    mr r3, r29
    mr r4, r31
    mr r5, r30
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    li r9, -0x1
    bl fn_8022B2CC
    mr r0, r3
    li r3, 0x0
    mr r7, r0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    mr r4, r31
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    lis r4, lbl_80379BFF@ha
    clrlslwi r0, r3, 16, 2
    addi r3, r4, lbl_80379BFF@l
    lwzx r0, r3, r0
    stw r0, lbl_8047B610(r13)
    b @L_8021CE4C
@L_8021CDDC:
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_80201248
    rlwinm. r0, r3, 0, 24, 31
    mr r31, r3
    beq @L_8021CE28
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    rlwinm r4, r31, 0, 24, 31
    divw r0, r5, r4
    addi r3, r1, 0x8
    mullw r0, r0, r4
    subf r0, r0, r5
    clrlslwi r0, r0, 24, 1
    lhzx r31, r3, r0
    cmplwi r31, 0x0
    beq @L_8021CE28
    cmplwi r31, 0x165
    bne @L_8021CD5C
@L_8021CE28:
    mr r3, r29
    li r4, 0x0
    li r5, 0x118
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
@L_8021CE4C:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
