    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r25, 0x24(r1)
    mr r30, r3
    mr r31, r4
    mr r25, r5
    lwz r0, 0x0(r31)
    cmpwi r0, 0x0
    beq @L_800836DC
    li r3, 0x0
    b @L_80083AE0
@L_800836DC:
    cmplwi r30, 0x0
    beq @L_800836E8
    b @L_800836F4
@L_800836E8:
    li r3, 0x0
    li r4, 0xd
    bl fn_80129280
@L_800836F4:
    addi r6, r3, 0x4000
    li r7, 0x0
@L_800836FC:
    addi r0, r3, 0x24
    cmplw r6, r0
    blt @L_80083770
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80083770
    lbz r0, 0x1b(r3)
    extsb r5, r0
    cmpwi r5, 0x3
    bgt @L_80083744
    lbz r0, 0x1c(r3)
    extsb r4, r0
    cmpwi r4, 0x6
    bgt @L_80083744
    lbz r0, 0x1d(r3)
    extsb r0, r0
    cmpwi r0, 0x5
    ble @L_80083750
@L_80083744:
    li r0, 0x0
    sth r0, 0x0(r3)
    b @L_80083770
@L_80083750:
    mullw r0, r4, r0
    addi r7, r7, 0x1
    rlwinm r4, r0, 4, 0, 27
    addi r0, r4, 0x76
    mullw r0, r5, r0
    add r3, r0, r3
    addi r3, r3, 0x24
    b @L_800836FC
@L_80083770:
    addi r0, r1, 0x10
    cmplwi r0, 0x0
    beq @L_80083780
    stw r7, 0x10(r1)
@L_80083780:
    lwz r27, 0x10(r1)
    li r26, 0x0
    li r28, 0x0
    addi r29, r1, 0xc
    b @L_80083888
@L_80083794:
    cmplwi r30, 0x0
    beq @L_800837A4
    mr r3, r30
    b @L_800837B0
@L_800837A4:
    li r3, 0x0
    li r4, 0xd
    bl fn_80129280
@L_800837B0:
    cmplwi r29, 0x0
    addi r0, r3, 0x4000
    beq @L_800837C4
    li r4, 0x0
    stw r4, 0xc(r1)
@L_800837C4:
    li r4, 0x0
@L_800837C8:
    addi r5, r3, 0x24
    cmplw r0, r5
    blt @L_80083860
    lhz r5, 0x0(r3)
    cmplwi r5, 0x0
    beq @L_80083860
    lbz r5, 0x1b(r3)
    extsb r5, r5
    cmpwi r5, 0x3
    bgt @L_80083810
    lbz r5, 0x1c(r3)
    extsb r5, r5
    cmpwi r5, 0x6
    bgt @L_80083810
    lbz r5, 0x1d(r3)
    extsb r5, r5
    cmpwi r5, 0x5
    ble @L_8008381C
@L_80083810:
    li r0, 0x0
    sth r0, 0x0(r3)
    b @L_80083860
@L_8008381C:
    cmpw r4, r28
    bne @L_80083828
    stw r3, 0xc(r1)
@L_80083828:
    lbz r6, 0x1c(r3)
    addi r4, r4, 0x1
    lbz r5, 0x1d(r3)
    extsb r6, r6
    lbz r7, 0x1b(r3)
    extsb r5, r5
    mullw r5, r6, r5
    extsb r6, r7
    rlwinm r5, r5, 4, 0, 27
    addi r5, r5, 0x76
    mullw r5, r6, r5
    add r3, r5, r3
    addi r3, r3, 0x24
    b @L_800837C8
@L_80083860:
    cmpwi r28, 0x0
    bge @L_8008386C
    stw r3, 0xc(r1)
@L_8008386C:
    lwz r4, 0xc(r1)
    lbz r0, 0x8(r31)
    lbz r3, 0x1a(r4)
    mr r26, r4
    cmplw r3, r0
    beq @L_80083890
    addi r28, r28, 0x1
@L_80083888:
    cmpw r28, r27
    blt @L_80083794
@L_80083890:
    cmpw r28, r27
    bne @L_80083ADC
    rlwinm r0, r25, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800838AC
    li r3, 0x0
    b @L_80083AE0
@L_800838AC:
    cmplwi r30, 0x0
    lbz r27, 0x5a(r31)
    lbz r28, 0x59(r31)
    lbz r29, 0x58(r31)
    beq @L_800838C8
    mr r3, r30
    b @L_800838D4
@L_800838C8:
    li r3, 0x0
    li r4, 0xd
    bl fn_80129280
@L_800838D4:
    addi r0, r1, 0x8
    mr r4, r3
    cmplwi r0, 0x0
    addi r7, r3, 0x4000
    beq @L_800838F0
    li r0, 0x0
    stw r0, 0x8(r1)
@L_800838F0:
    li r8, 0x0
@L_800838F4:
    addi r0, r4, 0x24
    cmplw r7, r0
    blt @L_8008398C
    lhz r0, 0x0(r4)
    cmplwi r0, 0x0
    beq @L_8008398C
    lbz r0, 0x1b(r4)
    extsb r0, r0
    cmpwi r0, 0x3
    bgt @L_8008393C
    lbz r0, 0x1c(r4)
    extsb r0, r0
    cmpwi r0, 0x6
    bgt @L_8008393C
    lbz r0, 0x1d(r4)
    extsb r0, r0
    cmpwi r0, 0x5
    ble @L_80083948
@L_8008393C:
    li r0, 0x0
    sth r0, 0x0(r4)
    b @L_8008398C
@L_80083948:
    cmpwi r8, -0x1
    bne @L_80083954
    stw r4, 0x8(r1)
@L_80083954:
    lbz r5, 0x1c(r4)
    addi r8, r8, 0x1
    lbz r0, 0x1d(r4)
    extsb r5, r5
    lbz r6, 0x1b(r4)
    extsb r0, r0
    mullw r0, r5, r0
    extsb r6, r6
    rlwinm r5, r0, 4, 0, 27
    addi r0, r5, 0x76
    mullw r0, r6, r0
    add r4, r0, r4
    addi r4, r4, 0x24
    b @L_800838F4
@L_8008398C:
    addi r0, r1, 0x8
    cmplwi r0, 0x0
    beq @L_8008399C
    stw r4, 0x8(r1)
@L_8008399C:
    extsb r4, r28
    extsb r0, r27
    mullw r4, r4, r0
    extsb r5, r29
    lwz r6, 0x8(r1)
    addi r0, r3, 0x4000
    rlwinm r3, r4, 4, 0, 27
    addi r3, r3, 0x76
    mullw r3, r5, r3
    add r4, r3, r6
    addi r4, r4, 0x24
    cmplw r0, r4
    bge @L_800839D8
    li r29, 0x0
    b @L_800839EC
@L_800839D8:
    mr r3, r6
    subf r5, r6, r4
    li r4, 0x0
    bl memset
    lwz r29, 0x8(r1)
@L_800839EC:
    cmplwi r29, 0x0
    mr r26, r29
    beq @L_80083ADC
    mr r3, r29
    addi r4, r31, 0xa
    bl fn_800CAA3C
    lbz r0, 0x8(r31)
    mr r27, r31
    li r28, 0x0
    stb r0, 0x1a(r29)
    lbz r0, 0x58(r31)
    stb r0, 0x1b(r29)
    lbz r0, 0x59(r31)
    stb r0, 0x1c(r29)
    lbz r0, 0x5a(r31)
    stb r0, 0x1d(r29)
    b @L_80083ACC
@L_80083A30:
    cmplwi r29, 0x0
    bne @L_80083A4C
    lis r3, lbl_8026F1C8@ha
    li r4, 0x17f
    addi r3, r3, lbl_8026F1C8@l
    addi r5, r2, -29984
    bl fn_80196E10
@L_80083A4C:
    extsb r3, r28
    li r4, 0x0
    cmpwi r3, 0x0
    blt @L_80083A70
    lbz r0, 0x1b(r29)
    extsb r0, r0
    cmpw r3, r0
    bge @L_80083A70
    li r4, 0x1
@L_80083A70:
    cmpwi r4, 0x0
    bne @L_80083A90
    lis r3, lbl_8026F1C8@ha
    lis r5, lbl_8026F1D8@ha
    addi r3, r3, lbl_8026F1C8@l
    li r4, 0x180
    addi r5, r5, lbl_8026F1D8@l
    bl fn_80196E10
@L_80083A90:
    lbz r3, 0x1c(r29)
    extsb r5, r28
    lbz r0, 0x1d(r29)
    addi r4, r27, 0x28
    extsb r3, r3
    extsb r0, r0
    mullw r0, r3, r0
    rlwinm r3, r0, 4, 0, 27
    addi r0, r3, 0x76
    mullw r0, r0, r5
    add r3, r29, r0
    addi r3, r3, 0x24
    bl fn_800CAA3C
    addi r27, r27, 0x10
    addi r28, r28, 0x1
@L_80083ACC:
    lbz r0, 0x1b(r29)
    extsb r0, r0
    cmpw r28, r0
    blt @L_80083A30
@L_80083ADC:
    mr r3, r26
@L_80083AE0:
    lmw r25, 0x24(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

