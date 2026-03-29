    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r26, r3
    lis r3, lbl_803A1D40@ha
    lbz r28, 0x95(r26)
    addi r30, r1, 0x8
    li r27, 0x0
    addi r31, r3, lbl_803A1D40@l
    li r29, 0x0
    @L_8001AB9C:
    lwz r4, 0x8(r31)
    li r25, 0x0
    lwz r3, 0xc(r31)
    cmpwi r4, 0x1
    beq @L_8001AC74
    bge @L_8001ABC0
    cmpwi r4, 0x0
    bge @L_8001ABCC
    b @L_8001ACE4
    @L_8001ABC0:
    cmpwi r4, 0x3
    bge @L_8001ACE4
    b @L_8001ACD4
    @L_8001ABCC:
    clrlwi r0, r29, 16
    cmplwi r0, 0x6
    blt @L_8001ABDC
    b @L_8001ACFC
    @L_8001ABDC:
    cmpwi r4, 0x1
    beq @L_8001AC1C
    bge @L_8001AC50
    cmpwi r4, 0x0
    bge @L_8001ABF4
    b @L_8001AC50
    @L_8001ABF4:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001AC14
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_8001AC54
    @L_8001AC14:
    bl fn_8006AEEC
    b @L_8001AC54
    @L_8001AC1C:
    cmplwi r3, 0x0
    bne @L_8001AC2C
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001AC2C:
    cmplwi r3, 0x0
    bne @L_8001AC3C
    li r3, 0x0
    b @L_8001AC54
    @L_8001AC3C:
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_8001AC54
    @L_8001AC50:
    li r3, 0x0
    @L_8001AC54:
    cmplwi r3, 0x0
    bne @L_8001AC64
    li r25, 0x0
    b @L_8001ACFC
    @L_8001AC64:
    clrlwi r4, r29, 16
    bl fn_8012AC08
    mr r25, r3
    b @L_8001ACE4
    @L_8001AC74:
    clrlwi r0, r29, 16
    cmplwi r0, 0x6
    blt @L_8001AC84
    b @L_8001ACFC
    @L_8001AC84:
    cmplwi r3, 0x0
    bne @L_8001AC94
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001AC94:
    cmplwi r3, 0x0
    bne @L_8001ACA4
    li r25, 0x0
    b @L_8001ACFC
    @L_8001ACA4:
    clrlwi r4, r29, 16
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8001ACBC
    li r25, 0x0
    b @L_8001ACFC
    @L_8001ACBC:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r25, r3
    b @L_8001ACE4
    @L_8001ACD4:
    clrlwi r0, r29, 16
    cmplwi r0, 0x1e
    blt @L_8001ACE4
    b @L_8001ACFC
    @L_8001ACE4:
    mr r3, r25
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001ACFC
    li r25, 0x0
    @L_8001ACFC:
    cmplwi r25, 0x0
    beq @L_8001AD10
    li r0, 0x1
    stb r0, 0x0(r30)
    b @L_8001AD18
    @L_8001AD10:
    li r0, 0x0
    stb r0, 0x0(r30)
    @L_8001AD18:
    addi r30, r30, 0x1
    addi r29, r29, 0x1
    cmpwi r29, 0x6
    blt @L_8001AB9C
    addi r3, r1, 0x8
    li r0, 0x1
    stbx r0, r3, r29
    bl fn_80105624
    lhz r4, 0x6(r3)
    rlwinm r0, r4, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8001ADE4
    lis r3, lbl_803A1D40@ha
    li r27, 0x1
    addi r3, r3, lbl_803A1D40@l
    lbz r0, 0x4(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8001AD90
    extsb r0, r28
    cmpwi r0, 0x0
    bne @L_8001AE64
    lbz r3, 0x5(r3)
    extsb r0, r3
    cmpwi r0, 0x3
    bne @L_8001AD88
    mr r28, r3
    b @L_8001AE64
    @L_8001AD88:
    li r28, 0x2
    b @L_8001AE64
    @L_8001AD90:
    extsb r0, r28
    cmpwi r0, 0x0
    bne @L_8001ADBC
    lbz r3, 0x5(r3)
    extsb r0, r3
    cmpwi r0, 0x3
    bne @L_8001ADB4
    mr r28, r3
    b @L_8001AE64
    @L_8001ADB4:
    li r28, 0x2
    b @L_8001AE64
    @L_8001ADBC:
    cmpwi r0, 0x1
    bne @L_8001AE64
    lbz r3, 0x5(r3)
    extsb r0, r3
    cmpwi r0, 0x5
    bne @L_8001ADDC
    mr r28, r3
    b @L_8001AE64
    @L_8001ADDC:
    li r28, 0x4
    b @L_8001AE64
    @L_8001ADE4:
    rlwinm r0, r4, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_8001AE64
    lis r3, lbl_803A1D40@ha
    li r27, -0x1
    addi r3, r3, lbl_803A1D40@l
    lbz r0, 0x4(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8001AE2C
    extsb r0, r28
    cmpwi r0, 0x6
    bge @L_8001AE64
    cmpwi r0, 0x1
    bge @L_8001AE24
    b @L_8001AE64
    @L_8001AE24:
    li r28, 0x0
    b @L_8001AE64
    @L_8001AE2C:
    cmpwi r0, 0x1
    bne @L_8001AE64
    extsb r0, r28
    cmpwi r0, 0x4
    bge @L_8001AE4C
    cmpwi r0, 0x2
    bge @L_8001AE58
    b @L_8001AE64
    @L_8001AE4C:
    cmpwi r0, 0x6
    bge @L_8001AE64
    b @L_8001AE60
    @L_8001AE58:
    li r28, 0x0
    b @L_8001AE64
    @L_8001AE60:
    li r28, 0x1
    @L_8001AE64:
    clrlwi r0, r4, 31
    cmpwi r0, 0x0
    beq @L_8001AE8C
    subi r28, r28, 0x1
    li r27, -0x1
    extsb r0, r28
    cmpwi r0, 0x0
    bge @L_8001AEBC
    li r28, 0x6
    b @L_8001AEBC
    @L_8001AE8C:
    rlwinm r0, r4, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8001AEBC
    lwz r3, 0x4(r26)
    li r27, 0x1
    bl fn_8005D9E4
    addi r28, r28, 0x1
    clrlwi r3, r3, 24
    extsb r0, r28
    cmpw r0, r3
    blt @L_8001AEBC
    li r28, 0x0
    @L_8001AEBC:
    extsb r3, r27
    cmpwi r3, 0x0
    beq @L_8001AF08
    extsb r0, r28
    addi r4, r1, 0x8
    add r4, r4, r0
    b @L_8001AEFC
    @L_8001AED8:
    add r28, r28, r27
    add r4, r4, r3
    extsb r0, r28
    cmpwi r0, 0x0
    blt @L_8001AEF4
    cmpwi r0, 0x7
    blt @L_8001AEFC
    @L_8001AEF4:
    li r28, 0x7
    b @L_8001AF08
    @L_8001AEFC:
    lbz r0, 0x0(r4)
    cmplwi r0, 0x0
    beq @L_8001AED8
    @L_8001AF08:
    lbz r4, 0x95(r26)
    extsb r0, r28
    extsb r3, r4
    cmpw r3, r0
    beq @L_8001AF30
    lis r3, lbl_803A1D40@ha
    addi r3, r3, lbl_803A1D40@l
    stb r4, 0x5(r3)
    stb r28, 0x6(r3)
    stb r28, 0x95(r26)
    @L_8001AF30:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
