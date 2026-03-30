    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    lwz r4, lbl_8047A450(r13)
    lwz r0, lbl_8047A44C(r13)
    cmplw r4, r0
    ble @L_80034E70
    lbz r0, 0x8b(r3)
    oris r31, r0, 0xffa0
    ori r31, r31, 0x8000
    b @L_80034E7C
@L_80034E70:
    lbz r3, 0x8b(r3)
    li r0, -0x100
    or r31, r3, r0
@L_80034E7C:
    mr r5, r31
    li r3, 0x0
    li r4, 0x0
    li r6, 0x3cc7
    bl fn_800FB680
    lis r3, 0x99
    lwz r4, lbl_8047A450(r13)
    subi r0, r3, 0x6981
    cmplw r4, r0
    bgt @L_80034EB0
    li r3, 0x34
    bl fn_80132A38
    b @L_80034EBC
@L_80034EB0:
    mr r4, r0
    li r3, 0x34
    bl fn_80132A38
@L_80034EBC:
    lha r5, 0x54(r30)
    mr r7, r31
    lha r6, 0x56(r30)
    li r3, 0x0
    li r4, 0x0
    li r8, 0x3cc9
    bl fn_800FBB34
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
