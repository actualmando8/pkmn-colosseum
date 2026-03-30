    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr. r30, r3
    bne @L_801A49F0
    la r3, lbl_8047DBB8(r2)
    li r4, 0x568
    la r5, lbl_8047DBCC(r2)
    bl fn_80196E10
    @L_801A49F0:
    lwz r0, 0x18(r30)
    cmplwi r0, 0x0
    bne @L_801A4A24
    bl fn_80191628
    stw r3, 0x18(r30)
    lwz r0, 0x18(r30)
    cmplwi r0, 0x0
    bne @L_801A4A24
    lis r4, lbl_80274DC8@ha
    la r3, lbl_8047DBB8(r2)
    addi r5, r4, lbl_80274DC8@l
    li r4, 0x56b
    bl fn_80196E10
    @L_801A4A24:
    lwz r3, 0x18(r30)
    mr r4, r31
    bl fn_80191788
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
