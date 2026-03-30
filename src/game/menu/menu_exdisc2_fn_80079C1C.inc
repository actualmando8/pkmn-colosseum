    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r6
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80079CBC
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80079CBC
    li r3, 0x43d2
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x43d3
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_801069FC
    cmpwi r30, 0x0
    bne @L_80079CA4
    li r3, 0x2
    li r4, 0x44cf
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
@L_80079CA4:
    li r3, 0xef
    bl fn_80102510
    li r0, 0x1
    li r3, 0x0
    stw r0, lbl_8047A638(r13)
    b @L_80079EDC
@L_80079CBC:
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80079D1C
    li r3, 0x43d2
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_801069FC
    cmpwi r30, 0x0
    bne @L_80079D04
    li r3, 0x2
    li r4, 0x44cf
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
@L_80079D04:
    li r3, 0xef
    bl fn_80102510
    li r0, 0x1
    li r3, 0x0
    stw r0, lbl_8047A638(r13)
    b @L_80079EDC
@L_80079D1C:
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80079D7C
    li r3, 0x43d3
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    li r3, 0x1
    bl fn_801069FC
    cmpwi r30, 0x0
    bne @L_80079D64
    li r3, 0x2
    li r4, 0x44cf
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
@L_80079D64:
    li r3, 0xef
    bl fn_80102510
    li r0, 0x1
    li r3, 0x0
    stw r0, lbl_8047A638(r13)
    b @L_80079EDC
@L_80079D7C:
    lbz r0, lbl_8047A632(r13)
    cmplwi r0, 0x0
    beq @L_80079DF4
    li r3, 0x2d
    li r4, 0x47
    bl fn_80132A38
    li r3, 0x3ca
    li r4, 0x0
    li r5, 0xff
    bl fn_80165668
    cmpwi r30, 0x1
    beq @L_80079DBC
    bge @L_80079DEC
    cmpwi r30, 0x0
    bge @L_80079DD0
    b @L_80079DEC
@L_80079DBC:
    li r3, 0x43ad
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    b @L_80079DEC
@L_80079DD0:
    mr r4, r31
    li r3, 0x4d
    bl fn_80132A38
    li r3, 0x4436
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
@L_80079DEC:
    li r3, 0x1
    bl fn_801069FC
@L_80079DF4:
    lbz r0, lbl_8047A631(r13)
    cmplwi r0, 0x0
    beq @L_80079E60
    li r3, 0x3d2
    li r4, 0x0
    li r5, 0xff
    bl fn_80165668
    cmpwi r30, 0x1
    beq @L_80079E28
    bge @L_80079E58
    cmpwi r30, 0x0
    bge @L_80079E3C
    b @L_80079E58
@L_80079E28:
    li r3, 0x4437
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    b @L_80079E58
@L_80079E3C:
    mr r4, r31
    li r3, 0x4d
    bl fn_80132A38
    li r3, 0x443b
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
@L_80079E58:
    li r3, 0x1
    bl fn_801069FC
@L_80079E60:
    lbz r0, lbl_8047A630(r13)
    cmplwi r0, 0x0
    beq @L_80079ED8
    li r3, 0x2d
    li r4, 0x1
    bl fn_80132A38
    li r3, 0x3ca
    li r4, 0x0
    li r5, 0xff
    bl fn_80165668
    cmpwi r30, 0x1
    beq @L_80079EA0
    bge @L_80079ED0
    cmpwi r30, 0x0
    bge @L_80079EB4
    b @L_80079ED0
@L_80079EA0:
    li r3, 0x43ad
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    b @L_80079ED0
@L_80079EB4:
    mr r4, r31
    li r3, 0x4d
    bl fn_80132A38
    li r3, 0x4436
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
@L_80079ED0:
    li r3, 0x1
    bl fn_801069FC
@L_80079ED8:
    li r3, 0x1
@L_80079EDC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
