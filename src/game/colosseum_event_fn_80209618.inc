    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r4, lbl_80279D08@ha
    stw r0, 0x34(r1)
    addi r6, r4, lbl_80279D08@l
    stmw r27, 0x1c(r1)
    mr r31, r3
    addi r30, r1, 0x8
    li r27, 0x0
    li r28, 0x0
    lwz r5, 0x0(r6)
    lwz r4, 0x4(r6)
    lwz r3, 0x8(r6)
    lhz r0, 0xc(r6)
    stw r5, 0x8(r1)
    stw r4, 0xc(r1)
    stw r3, 0x10(r1)
    sth r0, 0x14(r1)
    b @L_802096B0
@L_80209664:
    clrlslwi r0, r28, 16, 1
    lhzx r29, r30, r0
    mr r3, r29
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x2a
    beq @L_80209688
    li r3, 0x0
    b @L_80209694
@L_80209688:
    mr r3, r31
    mr r4, r29
    bl fn_8011B67C
@L_80209694:
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802096AC
    clrlwi r3, r27, 16
    addi r0, r3, 0x1
    clrlwi r27, r0, 16
@L_802096AC:
    addi r28, r28, 0x1
@L_802096B0:
    clrlwi r0, r28, 16
    cmplwi r0, 0x7
    blt @L_80209664
    clrlwi r3, r27, 16
    li r0, 0x2
    subfc r0, r0, r3
    lmw r27, 0x1c(r1)
    li r0, -0x1
    subfze r0, r0
    clrlwi r3, r0, 24
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
