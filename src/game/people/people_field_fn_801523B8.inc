    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r5, lbl_80445F18@ha
    lis r6, lbl_804380F8@ha
    stw r0, 0x14(r1)
    addi r8, r5, lbl_80445F18@l
    lis r5, fn_801523A8@ha
    addi r0, r6, lbl_804380F8@l
    stw r31, 0xc(r1)
    addi r7, r5, fn_801523A8@l
    mr r31, r4
    mr r4, r0
    sth r3, 0x4(r8)
    mr r3, r8
    lhz r5, lbl_8047AFA4(r13)
    li r6, 0xc
    bl fn_80162118
    cmplwi r3, 0x0
    stw r3, lbl_8047AF68(r13)
    beq @L_8015241C
    lhz r0, 0x6(r3)
    sth r0, 0x0(r31)
    lwz r3, lbl_8047AF68(r13)
    lwz r3, 0x0(r3)
    b @L_80152420
    @L_8015241C:
    li r3, 0x0
    @L_80152420:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
