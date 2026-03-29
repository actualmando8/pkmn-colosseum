    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    lwz r3, 0x60(r3)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x2
    beq @L_80028758
    li r3, 0x0
    b @L_80028818
    @L_80028758:
    lis r3, lbl_803A2094@ha
    addi r3, r3, lbl_803A2094@l
    bl fn_80109934
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_80028814
    li r3, 0x4
    bl fn_800D888C
    li r3, 0x3
    bl fn_800D88DC
    lis r3, lbl_80314F98@ha
    addi r3, r3, lbl_80314F98@l
    bl fn_800D7820
    mr r4, r31
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x7
    bl fn_800D6A00
    li r3, 0x2
    bl fn_800D67BC
    li r3, 0x0
    li r4, 0x0
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047B930(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lha r3, 0x54(r30)
    lha r4, 0x56(r30)
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047B934(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    @L_80028814:
    li r3, 0x0
    @L_80028818:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
