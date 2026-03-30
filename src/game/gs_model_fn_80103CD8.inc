    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_80105410
    bl fn_80103EF4
    bl fn_80109810
    li r3, 0x18
    bl fn_8010C224
    lwz r0, lbl_8047AD00(r13)
    li r5, 0x0
    lis r6, lbl_80404ACC@ha
    la r4, lbl_8047AD04(r13)
    la r3, lbl_8047AD08(r13)
    cmplwi r0, 0x0
    addi r6, r6, lbl_80404ACC@l
    li r0, 0x1
    stb r0, 0x92(r6)
    stb r5, lbl_8047AD04(r13)
    stb r5, lbl_8047AD08(r13)
    stb r5, 0x1(r4)
    stb r5, 0x1(r3)
    stb r5, 0x2(r4)
    stb r5, 0x2(r3)
    stb r5, 0x3(r4)
    stb r5, 0x3(r3)
    bne @L_80103DF8
    bl fn_800D7894
    stw r3, lbl_8047AD00(r13)
    li r4, 0x1
    li r5, 0x0
    li r6, 0x0
    li r7, 0x3
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    lwz r3, lbl_8047AD00(r13)
    li r4, 0x4
    li r5, 0x0
    li r6, 0x6
    li r7, 0xa
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    lwz r3, lbl_8047AD00(r13)
    li r4, 0x6
    li r5, 0x0
    li r6, 0x8
    li r7, 0x4
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    lwz r3, lbl_8047AD00(r13)
    li r4, 0x7
    li r5, 0x0
    li r6, 0x8
    li r7, 0x4
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    lwz r3, lbl_8047AD00(r13)
    li r4, 0x8
    li r5, 0x0
    li r6, 0x8
    li r7, 0x4
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
@L_80103DF8:
    lis r3, lbl_803156E0@ha
    addi r3, r3, lbl_803156E0@l
    bl fn_800EFD3C
    lis r5, 0x31a
    li r4, 0x0
    addi r5, r5, 0x1200
    li r6, 0x0
    bl fn_800F9378
    lis r3, lbl_803254E0@ha
    addi r3, r3, lbl_803254E0@l
    bl fn_800EFD3C
    lis r5, 0x622
    li r4, 0x0
    addi r5, r5, 0x1200
    li r6, 0x0
    bl fn_800F9378
    lis r3, lbl_803357E0@ha
    addi r3, r3, lbl_803357E0@l
    bl fn_800EFD3C
    lis r5, 0x6f7
    li r4, 0x0
    addi r5, r5, 0x1200
    li r6, 0x0
    bl fn_800F9378
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

