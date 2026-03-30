    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr. r30, r3
    bne @L_801A4924
    la r3, lbl_8047DBB8(r2)
    li r4, 0x58c
    la r5, lbl_8047DBCC(r2)
    bl fn_80196E10
    @L_801A4924:
    lwz r0, 0x1c(r30)
    cmplwi r0, 0x0
    bne @L_801A4958
    bl fn_80191628
    stw r3, 0x1c(r30)
    lwz r0, 0x1c(r30)
    cmplwi r0, 0x0
    bne @L_801A4958
    lis r4, lbl_80274DB8@ha
    la r3, lbl_8047DBB8(r2)
    addi r5, r4, lbl_80274DB8@l
    li r4, 0x58f
    bl fn_80196E10
    @L_801A4958:
    lwz r3, 0x1c(r30)
    mr r4, r31
    bl fn_80191788
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
