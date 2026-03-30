    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_80082684
    lis r3, lbl_8026F1C8@ha
    li r4, 0x17f
    addi r3, r3, lbl_8026F1C8@l
    addi r5, r2, -29984
    bl fn_80196E10
@L_80082684:
    lbz r0, 0x1b(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bgt @L_800826AC
    lis r3, lbl_8026F1C8@ha
    lis r5, lbl_8026F1D8@ha
    addi r3, r3, lbl_8026F1C8@l
    li r4, 0x180
    addi r5, r5, lbl_8026F1D8@l
    bl fn_80196E10
@L_800826AC:
    addi r30, r31, 0x24
    cmplwi r30, 0x0
    bne @L_800826CC
    lis r3, lbl_8026F1C8@ha
    li r4, 0x1f1
    addi r3, r3, lbl_8026F1C8@l
    addi r5, r2, -29976
    bl fn_80196E10
@L_800826CC:
    lbz r3, 0x1c(r31)
    lbz r0, 0x1d(r31)
    extsb r3, r3
    extsb r0, r0
    mullw r0, r3, r0
    mtctr r0
    cmpwi r0, 0x0
    ble @L_80082708
@L_800826EC:
    lbz r0, 0x82(r30)
    cmplwi r0, 0x0
    beq @L_80082700
    li r0, 0x1
    b @L_8008270C
@L_80082700:
    addi r30, r30, 0x10
    bdnz @L_800826EC
@L_80082708:
    li r0, 0x0
@L_8008270C:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80082720
    li r0, 0x0
    sth r0, 0x0(r31)
@L_80082720:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

