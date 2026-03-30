    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, lbl_8036C7E8@ha
    lis r4, lbl_8036CC00@ha
    lis r5, lbl_802747B8@ha
    stw r0, 0x14(r1)
    la r6, lbl_8047DA60(r2)
    addi r3, r3, lbl_8036C7E8@l
    addi r4, r4, lbl_8036CC00@l
    addi r5, r5, lbl_802747B8@l
    li r7, 0x40
    li r8, 0x20
    bl fn_80193B30
    lis r4, fn_8019B874@ha
    lis r3, fn_8019B948@ha
    addi r5, r4, fn_8019B874@l
    lis r4, lbl_8036C7E8@ha
    addi r0, r3, fn_8019B948@l
    lis r3, lbl_8036C7E8@ha
    addi r4, r4, lbl_8036C7E8@l
    addi r3, r3, lbl_8036C7E8@l
    stw r5, 0x30(r4)
    stw r0, 0x3c(r3)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
