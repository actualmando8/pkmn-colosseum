    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    lha r0, 0x6(r30)
    li r31, 0x0
    cmpwi r0, 0x10cd
    beq @L_80030C60
    bge @L_80030C70
    cmpwi r0, 0x10cc
    bge @L_80030C4C
    b @L_80030C70
    @L_80030C4C:
    lis r3, lbl_803A3230@ha
    addi r3, r3, lbl_803A3230@l
    bl fn_80109934
    mr r31, r3
    b @L_80030C70
    @L_80030C60:
    lis r3, lbl_803A31E8@ha
    addi r3, r3, lbl_803A31E8@l
    bl fn_80109934
    mr r31, r3
    @L_80030C70:
    cmplwi r31, 0x0
    beq @L_80030D1C
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    li r3, 0x7
    bl fn_800D6A00
    lis r3, lbl_80314F98@ha
    addi r3, r3, lbl_80314F98@l
    bl fn_800D7820
    mr r4, r31
    li r3, 0x0
    bl fn_800D85D4
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
    lfs f1, lbl_8047B9D4(r2)
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
    lfs f1, lbl_8047B9F0(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    @L_80030D1C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
