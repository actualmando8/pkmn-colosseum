    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r4, 0x596
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    addi r3, r4, 0x9
    addi r4, r4, 0x8
    bl fn_8011288C
    lwz r0, lbl_8047ACD8(r13)
    lwz r31, lbl_8047ACC8(r13)
    cmpwi r0, 0x2
    bne @L_800FF634
    lwz r0, lbl_8047ACB4(r13)
    lwz r3, lbl_8047ACB0(r13)
    mulli r0, r0, 0x24
    lwz r29, lbl_8047ACB8(r13)
    add r30, r3, r0
    b @L_800FF618
@L_800FF5E4:
    lwz r0, 0x8(r30)
    cmpwi r0, 0x0
    beq @L_800FF614
    lwz r0, 0xc(r30)
    cmpwi r0, 0x1
    bne @L_800FF614
    lwz r3, 0x10(r30)
    lwz r0, 0x4(r31)
    cmplw r3, r0
    bne @L_800FF614
    lhz r3, 0x20(r30)
    bl fn_800F7274
@L_800FF614:
    addi r30, r30, 0x24
@L_800FF618:
    cmplwi r29, 0x0
    subi r29, r29, 0x1
    bne @L_800FF5E4
    li r3, 0x5
    li r0, -0x1
    stw r3, 0x10(r31)
    stw r0, lbl_80478B18(r13)
@L_800FF634:
    li r0, 0x3
    stw r0, lbl_8047ACDC(r13)
    stw r28, lbl_80478B18(r13)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

