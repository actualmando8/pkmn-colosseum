    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r23, 0x3c(r1)
    mr r26, r3
    lis r3, lbl_803A1D40@ha
    addi r31, r3, lbl_803A1D40@l
    @L_8001C7D4:
    li r0, 0x1
    li r3, 0x8ae
    stb r0, 0x14(r31)
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001C808
    li r3, 0x6b
    li r4, 0x1
    bl fn_8010264C
    mr r23, r3
    li r3, 0x6b
    bl fn_80102510
    b @L_8001C820
    @L_8001C808:
    li r3, 0x10f
    li r4, 0x1
    bl fn_8010264C
    mr r23, r3
    li r3, 0x10f
    bl fn_80102510
    @L_8001C820:
    cmpwi r23, 0x3d2
    beq @L_8001D354
    bge @L_8001C850
    cmpwi r23, 0x3cf
    beq @L_8001C874
    bge @L_8001C844
    cmpwi r23, -0x1
    beq @L_8001D354
    b @L_8001D358
    @L_8001C844:
    cmpwi r23, 0x3d1
    bge @L_8001D234
    b @L_8001CA84
    @L_8001C850:
    cmpwi r23, 0x52b
    beq @L_8001CA84
    bge @L_8001C868
    cmpwi r23, 0x52a
    bge @L_8001C874
    b @L_8001D358
    @L_8001C868:
    cmpwi r23, 0x52d
    bge @L_8001D358
    b @L_8001D354
    @L_8001C874:
    li r3, 0x63
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lwz r4, 0x8(r31)
    li r24, 0x0
    lwz r3, 0xc(r31)
    cmpwi r4, 0x1
    beq @L_8001C960
    bge @L_8001C8A8
    cmpwi r4, 0x0
    bge @L_8001C8B4
    b @L_8001C9D8
    @L_8001C8A8:
    cmpwi r4, 0x3
    bge @L_8001C9D8
    b @L_8001C9C4
    @L_8001C8B4:
    extsb r23, r26
    clrlwi r0, r23, 16
    cmplwi r0, 0x6
    blt @L_8001C8C8
    b @L_8001C9F0
    @L_8001C8C8:
    cmpwi r4, 0x1
    beq @L_8001C908
    bge @L_8001C93C
    cmpwi r4, 0x0
    bge @L_8001C8E0
    b @L_8001C93C
    @L_8001C8E0:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001C900
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_8001C940
    @L_8001C900:
    bl fn_8006AEEC
    b @L_8001C940
    @L_8001C908:
    cmplwi r3, 0x0
    bne @L_8001C918
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001C918:
    cmplwi r3, 0x0
    bne @L_8001C928
    li r3, 0x0
    b @L_8001C940
    @L_8001C928:
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_8001C940
    @L_8001C93C:
    li r3, 0x0
    @L_8001C940:
    cmplwi r3, 0x0
    bne @L_8001C950
    li r24, 0x0
    b @L_8001C9F0
    @L_8001C950:
    mr r4, r23
    bl fn_8012AC08
    mr r24, r3
    b @L_8001C9D8
    @L_8001C960:
    extsb r23, r26
    clrlwi r0, r23, 16
    cmplwi r0, 0x6
    blt @L_8001C974
    b @L_8001C9F0
    @L_8001C974:
    cmplwi r3, 0x0
    bne @L_8001C984
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001C984:
    cmplwi r3, 0x0
    bne @L_8001C994
    li r24, 0x0
    b @L_8001C9F0
    @L_8001C994:
    mr r4, r23
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8001C9AC
    li r24, 0x0
    b @L_8001C9F0
    @L_8001C9AC:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r24, r3
    b @L_8001C9D8
    @L_8001C9C4:
    extsb r0, r26
    clrlwi r0, r0, 16
    cmplwi r0, 0x1e
    blt @L_8001C9D8
    b @L_8001C9F0
    @L_8001C9D8:
    mr r3, r24
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001C9F0
    li r24, 0x0
    @L_8001C9F0:
    lfs f1, lbl_8047B7C0(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lis r4, 0x8002
    mr r3, r24
    addi r4, r4, -0x62a4
    li r5, 0x0
    bl fn_80097F08
    lbz r0, 0x6(r31)
    lis r3, lbl_803A1C20@ha
    addi r9, r3, lbl_803A1C20@l
    addi r5, r1, 0x28
    extsb r0, r0
    li r3, 0x63
    stw r0, 0x28(r1)
    li r4, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0x63
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_8001CA64
    li r0, 0x1
    stb r0, 0x98(r3)
    @L_8001CA64:
    lfs f1, lbl_8047B7C0(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lbz r26, 0x6(r31)
    li r23, 0x0
    b @L_8001D358
    @L_8001CA84:
    lbz r0, 0x6(r31)
    li r4, 0x2
    lis r3, lbl_803A1C20@ha
    stb r4, 0x14(r31)
    extsb r0, r0
    addi r5, r1, 0x2c
    stb r26, 0x7(r31)
    addi r9, r3, lbl_803A1C20@l
    li r3, 0x63
    li r4, 0x0
    stw r0, 0x2c(r1)
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    lbz r0, 0x1(r31)
    extsb r3, r3
    cmplwi r0, 0x0
    bne @L_8001CAD8
    li r3, -0x2
    @L_8001CAD8:
    li r0, -0x1
    extsb r28, r3
    stb r0, 0x7(r31)
    li r3, 0x63
    bl fn_801022B8
    cmpwi r3, 0x3b6
    beq @L_8001D22C
    cmpwi r28, -0x1
    beq @L_8001D22C
    lwz r4, 0x8(r31)
    li r27, 0x0
    lwz r3, 0xc(r31)
    cmpwi r4, 0x1
    beq @L_8001CBD8
    bge @L_8001CB20
    cmpwi r4, 0x0
    bge @L_8001CB2C
    b @L_8001CC50
    @L_8001CB20:
    cmpwi r4, 0x3
    bge @L_8001CC50
    b @L_8001CC3C
    @L_8001CB2C:
    extsb r23, r26
    clrlwi r0, r23, 16
    cmplwi r0, 0x6
    blt @L_8001CB40
    b @L_8001CC68
    @L_8001CB40:
    cmpwi r4, 0x1
    beq @L_8001CB80
    bge @L_8001CBB4
    cmpwi r4, 0x0
    bge @L_8001CB58
    b @L_8001CBB4
    @L_8001CB58:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001CB78
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_8001CBB8
    @L_8001CB78:
    bl fn_8006AEEC
    b @L_8001CBB8
    @L_8001CB80:
    cmplwi r3, 0x0
    bne @L_8001CB90
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001CB90:
    cmplwi r3, 0x0
    bne @L_8001CBA0
    li r3, 0x0
    b @L_8001CBB8
    @L_8001CBA0:
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_8001CBB8
    @L_8001CBB4:
    li r3, 0x0
    @L_8001CBB8:
    cmplwi r3, 0x0
    bne @L_8001CBC8
    li r27, 0x0
    b @L_8001CC68
    @L_8001CBC8:
    mr r4, r23
    bl fn_8012AC08
    mr r27, r3
    b @L_8001CC50
    @L_8001CBD8:
    extsb r23, r26
    clrlwi r0, r23, 16
    cmplwi r0, 0x6
    blt @L_8001CBEC
    b @L_8001CC68
    @L_8001CBEC:
    cmplwi r3, 0x0
    bne @L_8001CBFC
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001CBFC:
    cmplwi r3, 0x0
    bne @L_8001CC0C
    li r27, 0x0
    b @L_8001CC68
    @L_8001CC0C:
    mr r4, r23
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8001CC24
    li r27, 0x0
    b @L_8001CC68
    @L_8001CC24:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r27, r3
    b @L_8001CC50
    @L_8001CC3C:
    extsb r0, r26
    clrlwi r0, r0, 16
    cmplwi r0, 0x1e
    blt @L_8001CC50
    b @L_8001CC68
    @L_8001CC50:
    mr r3, r27
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001CC68
    li r27, 0x0
    @L_8001CC68:
    lwz r4, 0x8(r31)
    li r29, 0x0
    lwz r3, 0xc(r31)
    cmpwi r4, 0x1
    beq @L_8001CD40
    bge @L_8001CC8C
    cmpwi r4, 0x0
    bge @L_8001CC98
    b @L_8001CDB0
    @L_8001CC8C:
    cmpwi r4, 0x3
    bge @L_8001CDB0
    b @L_8001CDA0
    @L_8001CC98:
    clrlwi r0, r28, 16
    cmplwi r0, 0x6
    blt @L_8001CCA8
    b @L_8001CDC8
    @L_8001CCA8:
    cmpwi r4, 0x1
    beq @L_8001CCE8
    bge @L_8001CD1C
    cmpwi r4, 0x0
    bge @L_8001CCC0
    b @L_8001CD1C
    @L_8001CCC0:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001CCE0
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_8001CD20
    @L_8001CCE0:
    bl fn_8006AEEC
    b @L_8001CD20
    @L_8001CCE8:
    cmplwi r3, 0x0
    bne @L_8001CCF8
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001CCF8:
    cmplwi r3, 0x0
    bne @L_8001CD08
    li r3, 0x0
    b @L_8001CD20
    @L_8001CD08:
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_8001CD20
    @L_8001CD1C:
    li r3, 0x0
    @L_8001CD20:
    cmplwi r3, 0x0
    bne @L_8001CD30
    li r29, 0x0
    b @L_8001CDC8
    @L_8001CD30:
    clrlwi r4, r28, 16
    bl fn_8012AC08
    mr r29, r3
    b @L_8001CDB0
    @L_8001CD40:
    clrlwi r0, r28, 16
    cmplwi r0, 0x6
    blt @L_8001CD50
    b @L_8001CDC8
    @L_8001CD50:
    cmplwi r3, 0x0
    bne @L_8001CD60
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001CD60:
    cmplwi r3, 0x0
    bne @L_8001CD70
    li r29, 0x0
    b @L_8001CDC8
    @L_8001CD70:
    clrlwi r4, r28, 16
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8001CD88
    li r29, 0x0
    b @L_8001CDC8
    @L_8001CD88:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r29, r3
    b @L_8001CDB0
    @L_8001CDA0:
    clrlwi r0, r28, 16
    cmplwi r0, 0x1e
    blt @L_8001CDB0
    b @L_8001CDC8
    @L_8001CDB0:
    mr r3, r29
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001CDC8
    li r29, 0x0
    @L_8001CDC8:
    cmplwi r27, 0x0
    beq @L_8001D22C
    beq @L_8001D22C
    lbz r0, 0x4(r31)
    lis r3, lbl_802E4E58@ha
    extsb r30, r26
    addi r4, r1, 0x1e
    extsb r5, r0
    addi r0, r3, lbl_802E4E58@l
    mulli r3, r5, 0x30
    slwi r24, r30, 3
    addi r5, r1, 0x20
    add r3, r0, r3
    lhax r25, r3, r24
    mr r3, r25
    bl fn_8005D95C
    lha r0, 0x1e(r1)
    mr r3, r25
    cmpwi r0, 0xfa
    ble @L_8001CE20
    li r4, 0x11a
    b @L_8001CE24
    @L_8001CE20:
    li r4, 0x122
    @L_8001CE24:
    bl fn_801080CC
    lbz r4, 0x4(r31)
    lis r3, lbl_802E4E58@ha
    addi r0, r3, lbl_802E4E58@l
    slwi r25, r28, 3
    extsb r3, r4
    addi r4, r1, 0x1a
    mulli r3, r3, 0x30
    addi r5, r1, 0x1c
    add r3, r0, r3
    lhax r28, r3, r25
    mr r3, r28
    bl fn_8005D95C
    lha r0, 0x1a(r1)
    mr r3, r28
    cmpwi r0, 0xfa
    ble @L_8001CE70
    li r4, 0x11a
    b @L_8001CE74
    @L_8001CE70:
    li r4, 0x122
    @L_8001CE74:
    bl fn_801080CC
    lis r3, lbl_802E4E58@ha
    slwi r4, r30, 3
    addi r0, r3, lbl_802E4E58@l
    add r30, r0, r4
    b @L_8001CE90
    @L_8001CE8C:
    bl fn_800F0308
    @L_8001CE90:
    lbz r0, 0x4(r31)
    addi r4, r1, 0x16
    addi r5, r1, 0x18
    extsb r0, r0
    mulli r0, r0, 0x30
    lhax r28, r30, r0
    mr r3, r28
    bl fn_8005D95C
    mr r3, r28
    bl fn_801070F4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001CE8C
    mr r3, r27
    mr r4, r29
    bl fn_80121BB4
    lis r3, lbl_803A1C20@ha
    li r4, 0x0
    addi r3, r3, lbl_803A1C20@l
    li r5, 0x120
    bl memset
    li r29, 0x0
    lis r3, lbl_803A1C20@ha
    addi r27, r3, lbl_803A1C20@l
    b @L_8001D144
    @L_8001CEF4:
    clrlwi r4, r29, 16
    lwz r5, 0x8(r31)
    mulli r0, r4, 0x30
    lwz r3, 0xc(r31)
    cmpwi r5, 0x1
    li r23, 0x0
    add r28, r27, r0
    beq @L_8001CFD4
    bge @L_8001CF24
    cmpwi r5, 0x0
    bge @L_8001CF30
    b @L_8001D03C
    @L_8001CF24:
    cmpwi r5, 0x3
    bge @L_8001D03C
    b @L_8001D030
    @L_8001CF30:
    cmplwi r4, 0x6
    blt @L_8001CF3C
    b @L_8001D054
    @L_8001CF3C:
    cmpwi r5, 0x1
    beq @L_8001CF7C
    bge @L_8001CFB0
    cmpwi r5, 0x0
    bge @L_8001CF54
    b @L_8001CFB0
    @L_8001CF54:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001CF74
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_8001CFB4
    @L_8001CF74:
    bl fn_8006AEEC
    b @L_8001CFB4
    @L_8001CF7C:
    cmplwi r3, 0x0
    bne @L_8001CF8C
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001CF8C:
    cmplwi r3, 0x0
    bne @L_8001CF9C
    li r3, 0x0
    b @L_8001CFB4
    @L_8001CF9C:
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_8001CFB4
    @L_8001CFB0:
    li r3, 0x0
    @L_8001CFB4:
    cmplwi r3, 0x0
    bne @L_8001CFC4
    li r23, 0x0
    b @L_8001D054
    @L_8001CFC4:
    mr r4, r29
    bl fn_8012AC08
    mr r23, r3
    b @L_8001D03C
    @L_8001CFD4:
    cmplwi r4, 0x6
    blt @L_8001CFE0
    b @L_8001D054
    @L_8001CFE0:
    cmplwi r3, 0x0
    bne @L_8001CFF0
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001CFF0:
    cmplwi r3, 0x0
    bne @L_8001D000
    li r23, 0x0
    b @L_8001D054
    @L_8001D000:
    mr r4, r29
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8001D018
    li r23, 0x0
    b @L_8001D054
    @L_8001D018:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r23, r3
    b @L_8001D03C
    @L_8001D030:
    cmplwi r4, 0x1e
    blt @L_8001D03C
    b @L_8001D054
    @L_8001D03C:
    mr r3, r23
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001D054
    li r23, 0x0
    @L_8001D054:
    cmplwi r23, 0x0
    bne @L_8001D068
    li r0, 0x0
    sth r0, 0x0(r28)
    b @L_8001D140
    @L_8001D068:
    mr r3, r23
    mr r4, r28
    bl fn_8011FDC8
    mr r3, r23
    li r4, 0x0
    li r5, 0x7b
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8001D09C
    li r0, 0x0
    sth r0, 0x1a(r28)
    @L_8001D09C:
    mr r3, r23
    li r4, 0x0
    li r5, 0x7b
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8001D0C4
    li r0, 0x1
    b @L_8001D128
    @L_8001D0C4:
    mr r3, r23
    bl fn_80120FE0
    clrlwi r0, r3, 16
    cmpwi r0, 0x3c
    beq @L_8001D10C
    bge @L_8001D0EC
    cmpwi r0, 0x3a
    beq @L_8001D0FC
    bge @L_8001D104
    b @L_8001D124
    @L_8001D0EC:
    cmpwi r0, 0x3e
    beq @L_8001D11C
    bge @L_8001D124
    b @L_8001D114
    @L_8001D0FC:
    li r0, 0x2
    b @L_8001D128
    @L_8001D104:
    li r0, 0x3
    b @L_8001D128
    @L_8001D10C:
    li r0, 0x4
    b @L_8001D128
    @L_8001D114:
    li r0, 0x5
    b @L_8001D128
    @L_8001D11C:
    li r0, 0x6
    b @L_8001D128
    @L_8001D124:
    li r0, 0x0
    @L_8001D128:
    clrlwi r0, r0, 16
    lis r3, lbl_802E4EB8@ha
    slwi r0, r0, 1
    addi r3, r3, lbl_802E4EB8@l
    lhzx r0, r3, r0
    sth r0, 0x24(r28)
    @L_8001D140:
    addi r29, r29, 0x1
    @L_8001D144:
    clrlwi r0, r29, 16
    cmplwi r0, 0x6
    blt @L_8001CEF4
    lfs f1, lbl_8047B7C0(r2)
    bl fn_8001D718
    lbz r5, 0x4(r31)
    lis r3, lbl_802E4E58@ha
    addi r0, r3, lbl_802E4E58@l
    addi r4, r1, 0x12
    extsb r3, r5
    addi r5, r1, 0x14
    mulli r3, r3, 0x30
    add r3, r0, r3
    lhax r24, r3, r24
    mr r3, r24
    bl fn_8005D95C
    lha r0, 0x12(r1)
    mr r3, r24
    cmpwi r0, 0xfa
    ble @L_8001D19C
    li r4, 0x116
    b @L_8001D1A0
    @L_8001D19C:
    li r4, 0x11e
    @L_8001D1A0:
    bl fn_801080CC
    lbz r5, 0x4(r31)
    lis r3, lbl_802E4E58@ha
    addi r0, r3, lbl_802E4E58@l
    addi r4, r1, 0xe
    extsb r3, r5
    addi r5, r1, 0x10
    mulli r3, r3, 0x30
    add r3, r0, r3
    lhax r24, r3, r25
    mr r3, r24
    bl fn_8005D95C
    lha r0, 0xe(r1)
    mr r3, r24
    cmpwi r0, 0xfa
    ble @L_8001D1E8
    li r4, 0x116
    b @L_8001D1EC
    @L_8001D1E8:
    li r4, 0x11e
    @L_8001D1EC:
    bl fn_801080CC
    b @L_8001D1F8
    @L_8001D1F4:
    bl fn_800F0308
    @L_8001D1F8:
    lbz r0, 0x4(r31)
    addi r4, r1, 0xa
    addi r5, r1, 0xc
    extsb r0, r0
    mulli r0, r0, 0x30
    lhax r24, r30, r0
    mr r3, r24
    bl fn_8005D95C
    mr r3, r24
    bl fn_801070F4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001D1F4
    @L_8001D22C:
    li r23, -0x1
    b @L_8001D358
    @L_8001D234:
    li r0, 0x3
    li r3, 0x6c
    stb r0, 0x14(r31)
    li r4, 0x1
    bl fn_8010264C
    mr r24, r3
    li r3, 0x6c
    bl fn_80102510
    cmpwi r24, 0x1
    beq @L_8001D31C
    bge @L_8001D270
    cmpwi r24, -0x1
    beq @L_8001D338
    bge @L_8001D27C
    b @L_8001D33C
    @L_8001D270:
    cmpwi r24, 0x3
    bge @L_8001D33C
    b @L_8001D338
    @L_8001D27C:
    li r3, 0x63
    bl fn_80102510
    li r3, 0x2
    li r4, 0x0
    li r5, 0x0
    bl fn_80018F54
    mr r25, r3
    bl fn_80019064
    lbz r0, 0x6(r31)
    lis r4, lbl_803A1C20@ha
    addi r9, r4, lbl_803A1C20@l
    mr r24, r3
    extsb r0, r0
    addi r5, r1, 0x24
    stw r0, 0x24(r1)
    li r3, 0x63
    li r4, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0x63
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_8001D2EC
    li r0, 0x1
    stb r0, 0x98(r3)
    @L_8001D2EC:
    clrlwi r0, r25, 16
    cmplwi r0, 0x0
    bne @L_8001D300
    li r24, -0x1
    b @L_8001D33C
    @L_8001D300:
    mr r3, r26
    mr r5, r25
    clrlwi r4, r24, 24
    addi r6, r1, 0x8
    bl fn_8001C064
    li r24, 0x0
    b @L_8001D33C
    @L_8001D31C:
    mr r3, r26
    addi r6, r1, 0x8
    li r4, -0x1
    li r5, 0x0
    bl fn_8001C064
    li r24, 0x0
    b @L_8001D33C
    @L_8001D338:
    li r24, -0x1
    @L_8001D33C:
    cmpwi r24, -0x1
    bne @L_8001D34C
    li r23, 0x0
    b @L_8001D358
    @L_8001D34C:
    li r23, -0x1
    b @L_8001D358
    @L_8001D354:
    li r23, -0x1
    @L_8001D358:
    cmpwi r23, -0x1
    bne @L_8001C7D4
    li r3, 0x0
    lmw r23, 0x3c(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
