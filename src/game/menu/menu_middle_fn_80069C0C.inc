    nofralloc
    stwu r1, -0xbc0(r1)
    mflr r0
    stw r0, 0xbc4(r1)
    stmw r25, 0xba4(r1)
    mr r31, r3
    lwz r0, 0x4(r31)
    lis r3, lbl_80267C18@ha
    addi r29, r3, lbl_80267C18@l
    li r25, 0x0
    cmpwi r0, 0x1
    beq @L_80069C5C
    bge @L_80069C48
    cmpwi r0, 0x0
    bge @L_80069C54
    b @L_80069C68
    @L_80069C48:
    cmpwi r0, 0x3
    bge @L_80069C68
    b @L_80069C64
    @L_80069C54:
    li r25, 0x20a
    b @L_80069C68
    @L_80069C5C:
    li r25, 0x20b
    b @L_80069C68
    @L_80069C64:
    li r25, 0x20c
    @L_80069C68:
    clrlwi r0, r25, 16
    cmplwi r0, 0x0
    bne @L_80069C84
    addi r3, r29, 0x7c
    addi r5, r29, 0x8c
    li r4, 0xf8
    bl fn_80196E10
    @L_80069C84:
    mr r3, r25
    bl fn_8020E0F8
    lwz r0, 0x0(r31)
    mr r30, r3
    cmpwi r0, 0x1
    beq @L_80069D0C
    bge @L_80069D48
    cmpwi r0, 0x0
    bge @L_80069CAC
    b @L_80069D48
    @L_80069CAC:
    lwz r0, 0x14(r31)
    cmpwi r0, 0x7
    beq @L_80069CD0
    bge @L_80069CD0
    cmpwi r0, 0x0
    bge @L_80069CC8
    b @L_80069CD0
    @L_80069CC8:
    li r4, 0x1f
    b @L_80069D7C
    @L_80069CD0:
    lwz r0, 0xc(r31)
    cmpwi r0, 0x4
    beq @L_80069CFC
    bge @L_80069CF4
    cmpwi r0, 0x3
    bge @L_80069D04
    cmpwi r0, 0x0
    bge @L_80069CFC
    b @L_80069D04
    @L_80069CF4:
    cmpwi r0, 0x7
    b @L_80069D04
    @L_80069CFC:
    li r4, 0x20
    b @L_80069D7C
    @L_80069D04:
    li r4, 0x21
    b @L_80069D7C
    @L_80069D0C:
    lwz r0, 0x14(r31)
    cmplwi r0, 0x1e
    bge @L_80069D20
    li r4, 0x1a
    b @L_80069D7C
    @L_80069D20:
    cmplwi r0, 0x3c
    bge @L_80069D30
    li r4, 0x17
    b @L_80069D7C
    @L_80069D30:
    cmplwi r0, 0x63
    bge @L_80069D40
    li r4, 0x18
    b @L_80069D7C
    @L_80069D40:
    li r4, 0x3d5
    b @L_80069D7C
    @L_80069D48:
    lwz r5, lbl_8047A5D8(r13)
    lis r3, 0xaaab
    subi r0, r3, 0x5555
    addi r4, r29, 0x0
    addi r3, r5, 0x1
    slwi r5, r5, 2
    mulhwu r0, r0, r3
    stw r3, lbl_8047A5D8(r13)
    lwzx r4, r4, r5
    srwi r0, r0, 1
    mulli r0, r0, 0x3
    subf r0, r0, r3
    stw r0, lbl_8047A5D8(r13)
    @L_80069D7C:
    mr r3, r30
    bl fn_8020DF00
    lwz r0, 0x0(r31)
    cmpwi r0, 0x1
    bne @L_80069DD8
    lwz r0, 0xc(r31)
    cmpwi r0, 0x6
    bne @L_80069DD8
    lwz r0, 0x14(r31)
    cmplwi r0, 0x1e
    bge @L_80069DB0
    li r4, 0x28
    b @L_80069E08
    @L_80069DB0:
    cmplwi r0, 0x3c
    bge @L_80069DC0
    li r4, 0x29
    b @L_80069E08
    @L_80069DC0:
    cmplwi r0, 0x63
    bge @L_80069DD0
    li r4, 0x2a
    b @L_80069E08
    @L_80069DD0:
    li r4, 0x2e
    b @L_80069E08
    @L_80069DD8:
    lwz r0, 0xc(r31)
    cmplwi r0, 0x7
    blt @L_80069DF4
    addi r3, r29, 0x7c
    addi r5, r29, 0xb8
    li r4, 0x166
    bl fn_80196E10
    @L_80069DF4:
    lwz r0, 0xc(r31)
    addi r3, r29, 0xc
    slwi r0, r0, 1
    lhzx r0, r3, r0
    mr r4, r0
    @L_80069E08:
    mr r3, r30
    bl fn_8020DF90
    lwz r0, 0x0(r31)
    cmpwi r0, 0x2
    beq @L_80069ED0
    bge @L_80069E30
    cmpwi r0, 0x0
    beq @L_80069E38
    bge @L_80069E98
    b @L_80069EEC
    @L_80069E30:
    cmpwi r0, 0x4
    b @L_80069EEC
    @L_80069E38:
    lwz r0, 0x14(r31)
    cmplwi r0, 0x8
    blt @L_80069E54
    addi r3, r29, 0x7c
    addi r5, r29, 0xe0
    li r4, 0x17f
    bl fn_80196E10
    @L_80069E54:
    lwz r0, 0x14(r31)
    cmplwi r0, 0x7
    bge @L_80069E70
    mr r3, r30
    li r4, 0xd
    bl fn_8020DFB0
    b @L_80069E7C
    @L_80069E70:
    mr r3, r30
    li r4, 0xe
    bl fn_8020DFB0
    @L_80069E7C:
    lwz r0, 0x14(r31)
    addi r4, r29, 0x1c
    mr r3, r30
    slwi r0, r0, 2
    lwzx r4, r4, r0
    bl fn_8020DEF0
    b @L_80069F04
    @L_80069E98:
    lwz r0, 0x14(r31)
    cmplwi r0, 0x63
    bge @L_80069EB4
    mr r3, r30
    li r4, 0xf
    bl fn_8020DFB0
    b @L_80069EC0
    @L_80069EB4:
    mr r3, r30
    li r4, 0x12
    bl fn_8020DFB0
    @L_80069EC0:
    mr r3, r30
    li r4, 0x0
    bl fn_8020DEF0
    b @L_80069F04
    @L_80069ED0:
    mr r3, r30
    li r4, 0x10
    bl fn_8020DFB0
    mr r3, r30
    li r4, 0x0
    bl fn_8020DEF0
    b @L_80069F04
    @L_80069EEC:
    mr r3, r30
    li r4, 0xc
    bl fn_8020DFB0
    mr r3, r30
    li r4, 0x0
    bl fn_8020DEF0
    @L_80069F04:
    bl fn_80077DB8
    cmpwi r3, 0x6
    bne @L_80069F64
    lwz r0, 0x4(r31)
    cmpwi r0, 0x1
    beq @L_80069F34
    bge @L_80069F28
    cmpwi r0, 0x0
    b @L_80069F54
    @L_80069F28:
    cmpwi r0, 0x3
    bge @L_80069F54
    b @L_80069F44
    @L_80069F34:
    mr r3, r30
    li r4, 0x1
    bl fn_8020DFA0
    b @L_80069FFC
    @L_80069F44:
    mr r3, r30
    li r4, 0x2
    bl fn_8020DFA0
    b @L_80069FFC
    @L_80069F54:
    mr r3, r30
    li r4, 0x0
    bl fn_8020DFA0
    b @L_80069FFC
    @L_80069F64:
    lwz r0, 0x4(r31)
    cmpwi r0, 0x1
    beq @L_80069F88
    bge @L_80069F7C
    cmpwi r0, 0x0
    b @L_80069FD8
    @L_80069F7C:
    cmpwi r0, 0x3
    bge @L_80069FD8
    b @L_80069FB0
    @L_80069F88:
    cmpwi r3, 0x4
    beq @L_80069FA0
    addi r3, r29, 0x7c
    addi r5, r29, 0x108
    li r4, 0x1c0
    bl fn_80196E10
    @L_80069FA0:
    mr r3, r30
    li r4, 0x5
    bl fn_8020DFA0
    b @L_80069FFC
    @L_80069FB0:
    cmpwi r3, 0x2
    beq @L_80069FC8
    addi r3, r29, 0x7c
    addi r5, r29, 0x114
    li r4, 0x1c5
    bl fn_80196E10
    @L_80069FC8:
    mr r3, r30
    li r4, 0x6
    bl fn_8020DFA0
    b @L_80069FFC
    @L_80069FD8:
    cmpwi r3, 0x3
    beq @L_80069FF0
    addi r3, r29, 0x7c
    addi r5, r29, 0x120
    li r4, 0x1cb
    bl fn_80196E10
    @L_80069FF0:
    mr r3, r30
    li r4, 0x4
    bl fn_8020DFA0
    @L_80069FFC:
    lwz r0, 0x0(r31)
    cmpwi r0, 0x2
    beq @L_8006A4C8
    bge @L_8006A018
    cmpwi r0, 0x0
    bge @L_8006A2C0
    b @L_8006A5A0
    @L_8006A018:
    cmpwi r0, 0x4
    bge @L_8006A5A0
    lwz r0, 0x4(r31)
    cmplwi r0, 0x2
    blt @L_8006A03C
    addi r3, r29, 0x7c
    addi r5, r29, 0x12c
    li r4, 0x221
    bl fn_80196E10
    @L_8006A03C:
    lwz r0, 0x0(r31)
    cmplwi r0, 0x4
    blt @L_8006A058
    addi r3, r29, 0x7c
    addi r5, r29, 0x150
    li r4, 0x222
    bl fn_80196E10
    @L_8006A058:
    lwz r4, 0x4(r31)
    addi r3, r29, 0x3c
    lwz r0, 0x10(r31)
    slwi r4, r4, 5
    slwi r0, r0, 3
    add r27, r4, r0
    add r27, r3, r27
    bl fn_800E0C54
    clrlwi r0, r3, 16
    lwz r4, 0x4(r31)
    clrlwi r0, r0, 29
    lbzx r3, r27, r0
    cmpwi r4, 0x0
    subi r5, r3, 0x1
    beq @L_8006A0A4
    cmpwi r4, 0x1
    beq @L_8006A0A4
    li r25, 0x0
    b @L_8006A0EC
    @L_8006A0A4:
    cmplwi r5, 0x64
    blt @L_8006A0B4
    li r25, 0x0
    b @L_8006A0EC
    @L_8006A0B4:
    mulli r3, r4, 0x64
    lwz r0, lbl_80478938(r13)
    add r25, r3, r5
    addi r25, r25, 0x60
    cmplw r0, r25
    bgt @L_8006A0DC
    addi r3, r29, 0x7c
    addi r5, r29, 0x174
    li r4, 0xca
    bl fn_80196E10
    @L_8006A0DC:
    lis r3, lbl_802EE618@ha
    slwi r0, r25, 1
    addi r3, r3, lbl_802EE618@l
    lhzx r25, r3, r0
    @L_8006A0EC:
    clrlwi r26, r25, 16
    @L_8006A0F0:
    bl fn_800E0C54
    clrlwi r0, r3, 16
    lwz r4, 0x4(r31)
    clrlwi r0, r0, 29
    lbzx r3, r27, r0
    cmpwi r4, 0x0
    subi r5, r3, 0x1
    beq @L_8006A120
    cmpwi r4, 0x1
    beq @L_8006A120
    li r28, 0x0
    b @L_8006A168
    @L_8006A120:
    cmplwi r5, 0x64
    blt @L_8006A130
    li r28, 0x0
    b @L_8006A168
    @L_8006A130:
    mulli r3, r4, 0x64
    lwz r0, lbl_80478938(r13)
    add r28, r3, r5
    addi r28, r28, 0x60
    cmplw r0, r28
    bgt @L_8006A158
    addi r3, r29, 0x7c
    addi r5, r29, 0x174
    li r4, 0xca
    bl fn_80196E10
    @L_8006A158:
    lis r3, lbl_802EE618@ha
    slwi r0, r28, 1
    addi r3, r3, lbl_802EE618@l
    lhzx r28, r3, r0
    @L_8006A168:
    clrlwi r0, r28, 16
    cmplw r26, r0
    beq @L_8006A0F0
    mr r4, r25
    addi r3, r31, 0x1684
    bl fn_8006AABC
    addi r3, r31, 0x1684
    li r4, 0x0
    bl fn_8006A81C
    li r25, 0x0
    b @L_8006A1AC
    @L_8006A194:
    mr r4, r25
    addi r3, r31, 0x16b0
    bl fn_8012AC08
    li r4, 0x0
    bl fn_8011DCB4
    addi r25, r25, 0x1
    @L_8006A1AC:
    clrlwi r0, r25, 16
    cmplwi r0, 0x6
    blt @L_8006A194
    li r25, 0x0
    b @L_8006A1D8
    @L_8006A1C0:
    mr r4, r25
    addi r3, r31, 0x21c8
    bl fn_8012AC08
    li r4, 0x0
    bl fn_8011DCB4
    addi r25, r25, 0x1
    @L_8006A1D8:
    clrlwi r0, r25, 16
    cmplwi r0, 0x6
    blt @L_8006A1C0
    bl fn_800E0C54
    lis r4, 0xaaab
    clrlwi r5, r3, 16
    subi r0, r4, 0x5555
    la r3, lbl_8047C028(r2)
    mulhwu r0, r0, r5
    srwi r0, r0, 1
    mulli r0, r0, 0x3
    subf r0, r0, r5
    slwi r0, r0, 1
    lhzx r3, r3, r0
    bl fn_800FA280
    addi r4, r1, 0x8
    b @L_8006A228
    @L_8006A21C:
    sth r0, 0x0(r4)
    addi r3, r3, 0x2
    addi r4, r4, 0x2
    @L_8006A228:
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_8006A21C
    li r0, 0x0
    li r3, 0x0
    sth r0, 0x0(r4)
    bl fn_8006B1C0
    mr r4, r3
    mr r3, r28
    addi r5, r1, 0x88
    bl fn_801F9CBC
    addi r3, r1, 0x88
    addi r4, r1, 0x8
    bl fn_8012AA64
    li r25, 0x0
    b @L_8006A280
    @L_8006A268:
    mr r4, r25
    addi r3, r1, 0x88
    bl fn_8012AC08
    li r4, 0x0
    bl fn_8011DCB4
    addi r25, r25, 0x1
    @L_8006A280:
    clrlwi r0, r25, 16
    cmplwi r0, 0x6
    blt @L_8006A268
    addi r3, r31, 0x24
    addi r4, r1, 0x88
    li r5, 0x1
    bl fn_8006A990
    li r3, 0x0
    bl fn_8006B1C0
    mr r4, r3
    addi r3, r31, 0x24
    bl fn_8006A81C
    addi r3, r31, 0x24
    li r4, 0x0
    bl fn_8006A7E0
    b @L_8006A5A0
    @L_8006A2C0:
    lwz r0, 0x4(r31)
    lwz r4, 0x14(r31)
    cmpwi r0, 0x0
    lwz r3, 0xc(r31)
    beq @L_8006A2E4
    cmpwi r0, 0x1
    beq @L_8006A2E4
    li r4, 0x0
    b @L_8006A368
    @L_8006A2E4:
    cmpwi r3, 0x6
    beq @L_8006A320
    bge @L_8006A33C
    cmpwi r3, 0x0
    bge @L_8006A2FC
    b @L_8006A33C
    @L_8006A2FC:
    cmplwi r4, 0x8
    blt @L_8006A30C
    li r4, 0x0
    b @L_8006A368
    @L_8006A30C:
    slwi r0, r0, 3
    slwi r3, r3, 4
    add r28, r0, r4
    add r28, r3, r28
    b @L_8006A33C
    @L_8006A320:
    cmplwi r4, 0x64
    blt @L_8006A330
    li r4, 0x0
    b @L_8006A368
    @L_8006A330:
    mulli r0, r0, 0x64
    add r28, r0, r4
    addi r28, r28, 0x60
    @L_8006A33C:
    lwz r0, lbl_80478938(r13)
    cmplw r0, r28
    bgt @L_8006A358
    addi r3, r29, 0x7c
    addi r5, r29, 0x174
    li r4, 0xca
    bl fn_80196E10
    @L_8006A358:
    lis r3, lbl_802EE618@ha
    slwi r0, r28, 1
    addi r3, r3, lbl_802EE618@l
    lhzx r4, r3, r0
    @L_8006A368:
    addi r3, r31, 0x7008
    bl fn_8006AABC
    mr r3, r31
    bl fn_8006AFC4
    mr r4, r3
    addi r3, r31, 0x59a8
    bl fn_8006A7F0
    lwz r0, 0x0(r31)
    cmpwi r0, 0x1
    bne @L_8006A474
    li r27, 0x0
    mr r26, r27
    @L_8006A398:
    addi r3, r31, 0x64ec
    clrlwi r4, r26, 16
    bl fn_8012AC08
    mr r25, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006A3D8
    mr r3, r25
    bl fn_8011F4A8
    clrlwi r0, r3, 24
    cmpw r27, r0
    bge @L_8006A3D8
    mr r3, r25
    bl fn_8011F4A8
    clrlwi r27, r3, 24
    @L_8006A3D8:
    addi r26, r26, 0x1
    cmpwi r26, 0x6
    blt @L_8006A398
    cmplwi r27, 0x64
    ble @L_8006A3F0
    li r27, 0x64
    @L_8006A3F0:
    addi r28, r31, 0x7b4c
    li r26, 0x0
    @L_8006A3F8:
    mr r3, r28
    clrlwi r4, r26, 16
    bl fn_8012AC08
    mr r25, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006A45C
    mr r3, r25
    bl fn_8011F4A8
    clrlwi r0, r3, 24
    cmplw r27, r0
    ble @L_8006A45C
    mr r3, r25
    bl fn_8011F5C8
    bl fn_8011E778
    bl fn_8011E520
    bl fn_8011CE74
    clrlwi r4, r27, 24
    bl fn_8011CE44
    mr r4, r3
    mr r3, r25
    bl fn_8011DE98
    mr r3, r25
    bl fn_8012546C
    @L_8006A45C:
    addi r26, r26, 0x1
    cmpwi r26, 0x6
    blt @L_8006A3F8
    mr r4, r28
    addi r3, r31, 0x7034
    bl fn_8012AC64
    @L_8006A474:
    addi r3, r31, 0x24
    addi r4, r31, 0x59a8
    bl fn_8006A7F0
    addi r3, r31, 0x1684
    addi r4, r31, 0x7008
    bl fn_8006A7F0
    li r3, 0x0
    bl fn_8006B1C0
    mr r4, r3
    addi r3, r31, 0x24
    bl fn_8006A81C
    addi r3, r31, 0x24
    li r4, 0x0
    bl fn_8006A7E0
    li r3, 0x0
    li r0, 0x1
    sth r3, 0x59aa(r31)
    sth r3, 0x26(r31)
    sth r0, 0x700a(r31)
    sth r0, 0x1686(r31)
    b @L_8006A5A0
    @L_8006A4C8:
    lwz r0, 0x4(r31)
    cmpwi r0, 0x2
    beq @L_8006A524
    bge @L_8006A590
    cmpwi r0, 0x0
    bge @L_8006A4E4
    b @L_8006A590
    @L_8006A4E4:
    li r0, 0x0
    li r3, 0x1
    stw r0, 0x59d0(r31)
    addis r5, r31, 0x1
    li r6, 0x2
    li r0, 0x3
    stw r3, 0x7030(r31)
    addi r3, r31, 0x24
    addi r4, r31, 0x59a8
    stw r6, -0x7970(r5)
    stw r0, -0x6310(r5)
    bl fn_8006A7F0
    addi r3, r31, 0x1684
    addi r4, r31, 0x7008
    bl fn_8006A7F0
    b @L_8006A5A0
    @L_8006A524:
    lwz r0, 0x59d0(r31)
    addi r3, r31, 0x24
    mulli r4, r0, 0x1660
    addi r4, r4, 0x59a8
    add r4, r31, r4
    bl fn_8006A7F0
    lwz r0, 0x7030(r31)
    addi r3, r31, 0x1684
    mulli r4, r0, 0x1660
    addi r4, r4, 0x59a8
    add r4, r31, r4
    bl fn_8006A7F0
    addis r4, r31, 0x1
    addi r3, r31, 0x2ce4
    lwz r0, -0x7970(r4)
    mulli r4, r0, 0x1660
    addi r4, r4, 0x59a8
    add r4, r31, r4
    bl fn_8006A7F0
    addis r4, r31, 0x1
    addi r3, r31, 0x4344
    lwz r0, -0x6310(r4)
    mulli r4, r0, 0x1660
    addi r4, r4, 0x59a8
    add r4, r31, r4
    bl fn_8006A7F0
    b @L_8006A5A0
    @L_8006A590:
    addi r3, r29, 0x7c
    li r4, 0x291
    la r5, lbl_8047C030(r2)
    bl fn_80196E10
    @L_8006A5A0:
    lwz r0, 0x4(r31)
    cmpwi r0, 0x2
    beq @L_8006A5BC
    bge @L_8006A63C
    cmpwi r0, 0x0
    bge @L_8006A5FC
    b @L_8006A63C
    @L_8006A5BC:
    lhz r5, 0x2ce4(r31)
    mr r3, r30
    li r4, 0x2
    bl fn_8020DF50
    lhz r5, 0x4344(r31)
    mr r3, r30
    li r4, 0x3
    bl fn_8020DF50
    lwz r5, 0x2d08(r31)
    mr r3, r30
    li r4, 0x2
    bl fn_8020DF10
    lwz r5, 0x4368(r31)
    mr r3, r30
    li r4, 0x3
    bl fn_8020DF10
    @L_8006A5FC:
    lhz r5, 0x24(r31)
    mr r3, r30
    li r4, 0x0
    bl fn_8020DF50
    lhz r5, 0x1684(r31)
    mr r3, r30
    li r4, 0x1
    bl fn_8020DF50
    lwz r5, 0x48(r31)
    mr r3, r30
    li r4, 0x0
    bl fn_8020DF10
    lwz r5, 0x16a8(r31)
    mr r3, r30
    li r4, 0x1
    bl fn_8020DF10
    @L_8006A63C:
    li r0, 0x1
    li r3, 0x0
    stb r0, 0x1c(r31)
    lmw r25, 0xba4(r1)
    lwz r0, 0xbc4(r1)
    mtlr r0
    addi r1, r1, 0xbc0
    blr
