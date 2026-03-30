    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r28, r3
    li r3, 0x2
    li r4, 0x2
    li r5, 0x0
    bl fn_801D0748
    mr r29, r3
    cmpwi r29, 0x3
    bne @L_8007992C
    li r3, 0x0
    li r4, 0x4
    bl fn_80135168
    cmplwi r3, 0x0
    bne @L_80079964
@L_8007992C:
    cmpwi r29, -0x1
    beq @L_80079950
    li r3, 0x2
    li r4, 0x44db
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
@L_80079950:
    li r3, 0xef
    bl fn_80102510
    li r0, 0x1
    stw r0, lbl_8047A638(r13)
    b @L_80079C08
@L_80079964:
    bl fn_80075AC0
    stb r3, lbl_8047A635(r13)
    bl fn_80075B08
    stb r3, lbl_8047A634(r13)
    bl fn_80075B50
    stb r3, lbl_8047A633(r13)
    li r3, 0x0
    li r4, 0xe
    li r5, 0x0
    bl fn_8012A5B0
    mr r0, r3
    li r3, 0x1
    mr r4, r0
    bl fn_80079EF4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80079C08
    li r31, 0x1
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    bl fn_80128DD4
    lbz r0, lbl_8047A632(r13)
    mr r30, r3
    cmplwi r0, 0x0
    beq @L_800799E4
    li r4, 0x47
    bl fn_80134420
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bge @L_800799E4
    li r31, 0x0
@L_800799E4:
    lbz r0, lbl_8047A630(r13)
    cmplwi r0, 0x0
    beq @L_80079A0C
    mr r3, r30
    li r4, 0x1
    bl fn_80134420
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bge @L_80079A0C
    li r31, 0x0
@L_80079A0C:
    li r27, 0x1
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    lbz r0, lbl_8047A631(r13)
    mr r29, r3
    cmplwi r0, 0x0
    beq @L_80079A74
    li r27, 0x0
    b @L_80079A60
@L_80079A34:
    mr r3, r29
    rlwinm r5, r27, 0, 24, 31
    li r4, 0x3
    bl fn_8012A5B0
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80079A5C
    li r0, 0x1
    b @L_80079A70
@L_80079A5C:
    addi r27, r27, 0x1
@L_80079A60:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x6
    blt @L_80079A34
    li r0, 0x0
@L_80079A70:
    mr r27, r0
@L_80079A74:
    li r3, 0x0
    li r4, 0x1
    li r5, 0x0
    bl fn_8012A5B0
    mr r0, r3
    li r3, 0x0
    mr r26, r0
    li r4, 0x0
    bl fn_80129280
    li r0, 0x3bfa
    subi r5, r28, 0x4
    subi r4, r3, 0x4
    mtctr r0
@L_80079AA8:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80079AA8
    mr r4, r31
    mr r5, r27
    mr r6, r26
    li r3, 0x1
    bl fn_80079C1C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80079C08
    lbz r0, lbl_8047A632(r13)
    cmplwi r0, 0x0
    beq @L_80079AFC
    mr r3, r30
    li r4, 0x47
    li r5, 0x1
    bl fn_8013467C
    bl fn_80075A9C
@L_80079AFC:
    lbz r0, lbl_8047A631(r13)
    cmplwi r0, 0x0
    beq @L_80079B20
    bl fn_80115BD8
    bl fn_801159F0
    mr r4, r3
    mr r3, r29
    bl fn_80130770
    bl fn_80075AE4
@L_80079B20:
    lbz r0, lbl_8047A630(r13)
    cmplwi r0, 0x0
    beq @L_80079B40
    mr r3, r30
    li r4, 0x1
    li r5, 0x1
    bl fn_8013467C
    bl fn_80075B2C
@L_80079B40:
    li r3, 0x4
    li r4, 0x2
    li r5, 0x0
    bl fn_801D0748
    cmpwi r3, 0x4
    beq @L_80079B9C
    li r3, 0xef
    bl fn_80102510
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    li r0, 0x3bfa
    subi r5, r3, 0x4
    subi r4, r28, 0x4
    mtctr r0
@L_80079B7C:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80079B7C
    li r0, 0x1
    stw r0, lbl_8047A638(r13)
    b @L_80079C08
@L_80079B9C:
    li r3, 0x43c5
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    bl fn_8001E184
    extsb r0, r3
    cmpwi r0, 0x0
    beq @L_80079BF8
    bge @L_80079BCC
    cmpwi r0, -0x1
    bge @L_80079BD4
    b @L_80079BF8
@L_80079BCC:
    cmpwi r0, 0x2
    bge @L_80079BF8
@L_80079BD4:
    li r3, 0x43c8
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0xef
    bl fn_80102510
    li r0, 0x0
    stw r0, lbl_8047A638(r13)
    b @L_80079C08
@L_80079BF8:
    li r3, 0xef
    bl fn_80102510
    li r0, 0x1
    stw r0, lbl_8047A638(r13)
@L_80079C08:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
