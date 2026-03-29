    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_800FF548
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800216CC
    li r3, 0x446
    li r4, 0x0
    li r5, 0xff
    bl fn_80165A20
    bl fn_80113F48
    lis r4, 0xffe
    mr r31, r3
    addi r3, r4, 0x1000
    bl fn_801CBA0C
    mr r4, r3
    mr r3, r31
    stw r4, lbl_8047A35C(r13)
    bl fn_800F9318
    lis r4, 0xfff
    li r3, 0x632
    addi r4, r4, 0x1800
    li r5, 0x0
    li r6, 0x1
    bl fn_80176E0C
    li r3, 0x4
    bl fn_80177A44
    lwz r4, lbl_8047A358(r13)
    li r0, 0x0
    la r3, lbl_8047A360(r13)
    stw r4, lbl_8047A360(r13)
    stw r0, 0x4(r3)
    @L_800216CC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
