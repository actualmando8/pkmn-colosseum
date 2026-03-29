    nofralloc
    stwu r1, -0x620(r1)
    mflr r0
    stw r0, 0x624(r1)
    stmw r25, 0x604(r1)
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lbz r0, 0x158(r3)
    extsb r4, r0
    cmpwi r4, 0x0
    beq @L_8003DC40
    li r0, 0x0
    cmpwi r0, 0xf
    bge @L_8003D93C
    stb r0, 0x108(r1)
    stb r0, 0x11a(r1)
    stb r0, 0x12c(r1)
    stb r0, 0x13e(r1)
    stb r0, 0x150(r1)
    stb r0, 0x162(r1)
    stb r0, 0x174(r1)
    stb r0, 0x186(r1)
    stb r0, 0x198(r1)
    stb r0, 0x1aa(r1)
    stb r0, 0x1bc(r1)
    stb r0, 0x1ce(r1)
    stb r0, 0x1e0(r1)
    stb r0, 0x1f2(r1)
    stb r0, 0x204(r1)
    @L_8003D93C:
    cmpwi r4, 0x0
    beq @L_8003D998
    lis r3, lbl_802E60B0@ha
    subi r0, r4, 0x1
    addi r3, r3, lbl_802E60B0@l
    addi r26, r1, 0x108
    lwz r3, 0x0(r3)
    slwi r0, r0, 6
    li r25, 0x0
    add r29, r3, r0
    mr r27, r29
    li r28, 0x1
    b @L_8003D98C
    @L_8003D970:
    stb r28, 0x0(r26)
    addi r3, r26, 0x2
    lwz r4, 0x0(r27)
    bl fn_800F9D04
    addi r26, r26, 0x12
    addi r27, r27, 0x4
    addi r25, r25, 0x1
    @L_8003D98C:
    lwz r0, 0x3c(r29)
    cmpw r25, r0
    blt @L_8003D970
    @L_8003D998:
    lwz r0, 0x3c(r29)
    li r31, 0x0
    li r30, 0x0
    clrlwi r28, r0, 16
    b @L_8003DB3C
    @L_8003D9AC:
    clrlwi r0, r30, 16
    lwz r25, lbl_8047A4E0(r13)
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    cmplwi r25, 0x0
    lhzx r29, r3, r0
    mr r27, r29
    beq @L_8003DA30
    cmplwi r29, 0x8000
    blt @L_8003D9D8
    clrlwi r27, r29, 18
    @L_8003D9D8:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r25
    clrlwi r4, r27, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r27
    li r3, 0x0
    bl fn_8025FDDC
    mr r26, r3
    mr r4, r27
    li r3, 0x0
    bl fn_8025FD34
    mr r27, r3
    mr r3, r25
    mr r4, r26
    bl fn_8011DFE0
    mr r3, r25
    mr r4, r27
    bl fn_8011DF90
    @L_8003DA30:
    lwz r25, lbl_8047A4E0(r13)
    mr r26, r29
    cmplwi r25, 0x0
    beq @L_8003DAAC
    cmplwi r29, 0x8000
    blt @L_8003DA4C
    clrlwi r26, r29, 18
    @L_8003DA4C:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r25
    clrlwi r4, r26, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r26
    li r3, 0x0
    bl fn_8025FDDC
    mr r27, r3
    mr r4, r26
    li r3, 0x0
    bl fn_8025FD34
    mr r26, r3
    mr r3, r25
    mr r4, r27
    bl fn_8011DFE0
    mr r3, r25
    mr r4, r26
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_8003DAB0
    @L_8003DAAC:
    li r3, 0x0
    @L_8003DAB0:
    cmplwi r3, 0x0
    beq @L_8003DAC8
    bl fn_8011F5C8
    bl fn_8011E778
    bl fn_8011E760
    b @L_8003DACC
    @L_8003DAC8:
    li r3, 0x0
    @L_8003DACC:
    mr r5, r3
    addi r3, r1, 0x8
    li r4, 0x100
    bl fn_800F96E4
    cmplwi r3, 0x0
    beq @L_8003DB38
    lhz r5, 0x8(r1)
    li r6, 0x0
    addi r3, r1, 0x108
    b @L_8003DB2C
    @L_8003DAF4:
    clrlwi r0, r6, 16
    mulli r4, r0, 0x12
    addi r0, r4, 0x2
    lhzx r0, r3, r0
    cmplw r5, r0
    bne @L_8003DB28
    mr r0, r31
    addi r3, r1, 0x218
    clrlwi r0, r0, 16
    addi r31, r31, 0x1
    slwi r0, r0, 1
    sthx r29, r3, r0
    b @L_8003DB38
    @L_8003DB28:
    addi r6, r6, 0x1
    @L_8003DB2C:
    clrlwi r0, r6, 16
    cmplw r0, r28
    blt @L_8003DAF4
    @L_8003DB38:
    addi r30, r30, 0x1
    @L_8003DB3C:
    lhz r0, lbl_8047A4E8(r13)
    clrlwi r3, r30, 16
    cmplw r3, r0
    blt @L_8003D9AC
    clrlwi r0, r31, 16
    li r3, 0x0
    cmplwi r0, 0x0
    ble @L_8003DC3C
    cmplwi r0, 0x8
    subi r0, r31, 0x8
    ble @L_8003DC0C
    clrlwi r0, r0, 16
    b @L_8003DC00
    @L_8003DB70:
    clrlwi r4, r3, 16
    addi r25, r1, 0x218
    slwi r12, r4, 1
    lwz r5, lbl_8047A4E4(r13)
    add r25, r25, r12
    addi r3, r3, 0x8
    lhz r7, 0x0(r25)
    addi r4, r12, 0x2
    lhz r10, 0x2(r25)
    addi r6, r12, 0x4
    sthx r7, r5, r12
    addi r11, r12, 0x6
    lhz r28, 0x4(r25)
    addi r9, r12, 0x8
    lwz r8, lbl_8047A4E4(r13)
    addi r7, r12, 0xa
    lhz r29, 0x6(r25)
    addi r5, r12, 0xc
    sthx r10, r8, r4
    addi r4, r12, 0xe
    lhz r12, 0x8(r25)
    lwz r8, lbl_8047A4E4(r13)
    lhz r10, 0xa(r25)
    sthx r28, r8, r6
    lhz r8, 0xc(r25)
    lwz r28, lbl_8047A4E4(r13)
    lhz r6, 0xe(r25)
    sthx r29, r28, r11
    lwz r11, lbl_8047A4E4(r13)
    sthx r12, r11, r9
    lwz r9, lbl_8047A4E4(r13)
    sthx r10, r9, r7
    lwz r7, lbl_8047A4E4(r13)
    sthx r8, r7, r5
    lwz r5, lbl_8047A4E4(r13)
    sthx r6, r5, r4
    @L_8003DC00:
    clrlwi r4, r3, 16
    cmplw r4, r0
    blt @L_8003DB70
    @L_8003DC0C:
    addi r6, r1, 0x218
    clrlwi r0, r31, 16
    b @L_8003DC30
    @L_8003DC18:
    clrlwi r5, r3, 16
    lwz r4, lbl_8047A4E4(r13)
    slwi r7, r5, 1
    addi r3, r3, 0x1
    lhzx r5, r6, r7
    sthx r5, r4, r7
    @L_8003DC30:
    clrlwi r4, r3, 16
    cmplw r4, r0
    blt @L_8003DC18
    @L_8003DC3C:
    sth r31, lbl_8047A4E8(r13)
    @L_8003DC40:
    lmw r25, 0x604(r1)
    lwz r0, 0x624(r1)
    mtlr r0
    addi r1, r1, 0x620
    blr
