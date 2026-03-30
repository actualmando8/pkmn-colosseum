    nofralloc
    stwu r1, -0x340(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x344(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r28, 0x330(r1)
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r30, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    li r0, 0x190
    mr r29, r3
    addi r5, r1, 0x8
    mr r3, r4
    mtctr r0
@L_80219E70:
    sthx r3, r5, r4
    addi r4, r4, 0x2
    bdnz @L_80219E70
    li r0, 0x163
    li r28, 0x0
    li r3, 0x0
    li r7, 0x0
    mtctr r0
@L_80219E90:
    cmplwi r7, 0x0
    beq @L_80219F1C
    rlwinm. r0, r7, 0, 16, 31
    bne @L_80219EA8
    li r0, 0x0
    b @L_80219F04
@L_80219EA8:
    cmplwi r0, 0x165
    bne @L_80219EB8
    li r0, 0x0
    b @L_80219F04
@L_80219EB8:
    cmplwi r0, 0x163
    bne @L_80219EC8
    li r0, 0x0
    b @L_80219F04
@L_80219EC8:
    lis r5, lbl_80279FA0@ha
    li r4, 0x0
    addi r5, r5, lbl_80279FA0@l
@L_80219ED4:
    lhzx r6, r5, r4
    cmplwi r6, 0xffff
    beq @L_80219EF0
    cmplw r0, r6
    beq @L_80219EF0
    addi r4, r4, 0x2
    b @L_80219ED4
@L_80219EF0:
    cmplwi r6, 0xffff
    beq @L_80219F00
    li r0, 0x0
    b @L_80219F04
@L_80219F00:
    li r0, 0x1
@L_80219F04:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_80219F1C
    addi r4, r1, 0x8
    addi r28, r28, 0x1
    sthx r7, r4, r3
    addi r3, r3, 0x2
@L_80219F1C:
    addi r7, r7, 0x1
    bdnz @L_80219E90
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    addi r3, r1, 0x8
    divwu r0, r4, r28
    mullw r0, r0, r28
    subf r0, r0, r4
    rlwinm r0, r0, 1, 0, 30
    lhzx r28, r3, r0
    bl fn_8000814C
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80219F54
    mr r28, r3
@L_80219F54:
    lwz r0, lbl_8047B618(r13)
    mr r3, r29
    mr r4, r28
    rlwinm r0, r0, 0, 22, 20
    stw r0, lbl_8047B618(r13)
    bl fn_80209C1C
    mr r3, r30
    mr r4, r28
    mr r5, r31
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
    mr r4, r28
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    lis r4, lbl_80379BFF@ha
    clrlslwi r0, r3, 16, 2
    addi r3, r4, lbl_80379BFF@l
    lwzx r0, r3, r0
    stw r0, lbl_8047B610(r13)
    lmw r28, 0x330(r1)
    lwz r0, 0x344(r1)
    mtlr r0
    addi r1, r1, 0x340
    blr
