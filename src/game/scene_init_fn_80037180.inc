    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    lha r0, 0x6(r30)
    cmpwi r0, 0x36c
    beq @L_800371A8
    b @L_800372BC
    @L_800371A8:
    bl fn_80005748
    lwz r0, lbl_8047A498(r13)
    mr r31, r3
    cmplwi r0, 0x0
    beq @L_800371C0
    mr r31, r0
    @L_800371C0:
    bl fn_801EF214
    stw r3, lbl_8047A49C(r13)
    bl fn_801EF274
    stw r3, lbl_8047A4A0(r13)
    mr r4, r31
    li r3, 0x4c
    bl fn_80132A38
    bl fn_801EF274
    mr r4, r3
    li r3, 0x2f
    bl fn_80132A38
    bl fn_801EF214
    mr r4, r3
    li r3, 0x30
    bl fn_80132A38
    lis r3, lbl_803A6498@ha
    addi r3, r3, lbl_803A6498@l
    bl fn_80109934
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_800372D8
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
    lfs f1, lbl_8047BA58(r2)
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
    lfs f1, lbl_8047BA5C(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    b @L_800372D8
    @L_800372BC:
    lwz r0, lbl_8047A49C(r13)
    cmpwi r0, 0x0
    bne @L_800372D8
    lbz r0, 0x4(r30)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r30)
    @L_800372D8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
