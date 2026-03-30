    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    bl fn_801F025C
    mr r30, r3
    bl fn_802040E8
    mr r0, r3
    li r3, 0x12
    mr r28, r0
    li r4, 0x0
    bl fn_801F025C
    mr r29, r3
    bl fn_80207BF4
    mr r31, r3
    mr r3, r29
    bl fn_802040E8
    mr r0, r3
    li r3, 0x0
    mr r27, r0
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80215D7C
    mr r3, r30
    bl fn_80204A10
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80215D7C
    li r0, 0x0
    b @L_80215DC0
@L_80215D7C:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80215DBC
    mr r3, r30
    li r4, 0x3d
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80215DBC
    li r0, 0x0
    b @L_80215DC0
@L_80215DBC:
    li r0, 0x1
@L_80215DC0:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80215DD8
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80216034
@L_80215DD8:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_801F54A4
    cmpwi r3, 0x0
    beq @L_80215E24
    mr r3, r30
    li r4, 0x3d
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80215E1C
    mr r3, r29
    li r4, 0x3d
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80215E24
@L_80215E1C:
    li r0, 0x0
    b @L_80215E28
@L_80215E24:
    li r0, 0x1
@L_80215E28:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80215E40
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80216034
@L_80215E40:
    rlwinm. r0, r28, 0, 16, 31
    bne @L_80215E50
    rlwinm. r0, r27, 0, 16, 31
    beq @L_80215E98
@L_80215E50:
    rlwinm r3, r28, 0, 16, 31
    cmplwi r3, 0xaf
    beq @L_80215E98
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0xaf
    beq @L_80215E98
    cmplwi r3, 0x0
    beq @L_80215E80
    mr r3, r28
    bl fn_80142984
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80215E98
@L_80215E80:
    rlwinm. r0, r27, 0, 16, 31
    beq @L_80215EA8
    mr r3, r27
    bl fn_80142984
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80215EA8
@L_80215E98:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80216034
@L_80215EA8:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x3c
    bne @L_80215EC4
    lis r3, lbl_803798D8@ha
    addi r0, r3, lbl_803798D8@l
    stw r0, lbl_8047B610(r13)
    b @L_80216034
@L_80215EC4:
    rlwinm. r0, r28, 0, 16, 31
    beq @L_80215EE0
    mr r3, r30
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_8020147C
@L_80215EE0:
    rlwinm. r0, r27, 0, 16, 31
    beq @L_80215EFC
    mr r3, r29
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_8020147C
@L_80215EFC:
    rlwinm. r0, r28, 0, 16, 31
    beq @L_80215F18
    mr r3, r29
    mr r4, r28
    li r5, 0x1
    li r6, 0x0
    bl fn_8020147C
@L_80215F18:
    rlwinm. r7, r27, 0, 16, 31
    beq @L_80215F34
    mr r3, r30
    li r4, 0x0
    li r5, 0xfb
    li r6, 0x0
    bl fn_801254B4
@L_80215F34:
    mr r3, r30
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80215F5C
    mr r3, r30
    li r4, 0x82
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80215F5C:
    mr r3, r29
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80215F84
    mr r3, r29
    li r4, 0x82
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80215F84:
    mr r3, r30
    li r4, 0x36
    bl fn_80202810
    mr r3, r29
    li r4, 0x36
    bl fn_80202810
    mr r4, r27
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r4, r28
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xe
    bl fn_80132A38
    rlwinm. r0, r28, 0, 16, 31
    beq @L_80216004
    rlwinm. r0, r27, 0, 16, 31
    beq @L_80216004
    li r0, 0x2
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_80216028
@L_80216004:
    rlwinm. r0, r27, 0, 16, 31
    beq @L_8021601C
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_80216028
@L_8021601C:
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_80216028:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_80216034:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
