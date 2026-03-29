    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    lha r0, 0x6(r30)
    cmpwi r0, 0x2d2
    beq @L_80020FD0
    bge @L_80020F94
    cmpwi r0, 0x2cd
    beq @L_8002108C
    blt @L_800210D8
    cmpwi r0, 0x2d1
    bge @L_8002108C
    b @L_800210D8
    @L_80020F94:
    cmpwi r0, 0x912
    bge @L_80020FA8
    cmpwi r0, 0x90e
    beq @L_80020FB4
    b @L_800210D8
    @L_80020FA8:
    cmpwi r0, 0x918
    bge @L_800210D8
    b @L_800210AC
    @L_80020FB4:
    lwz r0, lbl_8047A360(r13)
    cmpwi r0, 0x0
    blt @L_800210D8
    lbz r0, 0x95(r3)
    extsb r0, r0
    stw r0, lbl_8047A360(r13)
    b @L_800210D8
    @L_80020FD0:
    lis r3, lbl_803A1FF8@ha
    addi r3, r3, lbl_803A1FF8@l
    bl fn_80109934
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_8002108C
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
    lfs f1, lbl_8047B898(r2)
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
    lfs f1, lbl_8047B89C(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    @L_8002108C:
    bl fn_801EF214
    cmpwi r3, 0x0
    bne @L_800210D8
    lbz r0, 0x4(r30)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r30)
    b @L_800210D8
    @L_800210AC:
    la r3, lbl_8047A360(r13)
    lwz r0, 0x4(r3)
    cmpwi r0, 0x5
    bne @L_800210CC
    li r3, 0xaa
    li r4, 0x0
    bl fn_801021F8
    b @L_800210D8
    @L_800210CC:
    li r3, 0xaa
    li r4, 0x1
    bl fn_801021F8
    @L_800210D8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
