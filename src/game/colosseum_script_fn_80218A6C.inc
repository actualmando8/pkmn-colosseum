    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r31, r0
    bl fn_80205B8C
    li r4, 0x0
    mr r30, r3
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r29, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    bl fn_8012640C
    mulli r0, r29, 0x30
    rlwinm r3, r3, 0, 16, 31
    divw r0, r0, r3
    rlwinm. r4, r0, 0, 24, 31
    bne @L_80218AF8
    cmplwi r29, 0x0
    beq @L_80218AF8
    li r4, 0x1
@L_80218AF8:
    lis r3, lbl_80279F7C@ha
    li r0, 0x6
    addi r3, r3, lbl_80279F7C@l
    rlwinm r4, r4, 0, 24, 31
    li r5, 0x0
    mtctr r0
@L_80218B10:
    lbz r0, 0x0(r3)
    cmplw r4, r0
    ble @L_80218B28
    addi r5, r5, 0x2
    addi r3, r3, 0x2
    bdnz @L_80218B10
@L_80218B28:
    lis r4, lbl_80279F7C@ha
    mr r3, r31
    addi r0, r4, lbl_80279F7C@l
    add r6, r0, r5
    li r4, 0x0
    lbz r7, 0x1(r6)
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BBD8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
