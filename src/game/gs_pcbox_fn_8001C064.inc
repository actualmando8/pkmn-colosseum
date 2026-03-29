    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r28, r4
    mr r30, r5
    mr r29, r6
    lis r4, lbl_803A1D40@ha
    li r27, 0x0
    addi r31, r4, lbl_803A1D40@l
    lwz r5, 0x8(r31)
    lwz r4, 0xc(r31)
    cmpwi r5, 0x1
    beq @L_8001C16C
    bge @L_8001C0AC
    cmpwi r5, 0x0
    bge @L_8001C0B8
    b @L_8001C1EC
    @L_8001C0AC:
    cmpwi r5, 0x3
    bge @L_8001C1EC
    b @L_8001C1D8
    @L_8001C0B8:
    extsb r25, r3
    clrlwi r0, r25, 16
    cmplwi r0, 0x6
    blt @L_8001C0CC
    b @L_8001C204
    @L_8001C0CC:
    cmpwi r5, 0x1
    beq @L_8001C10C
    bge @L_8001C148
    cmpwi r5, 0x0
    bge @L_8001C0E4
    b @L_8001C148
    @L_8001C0E4:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001C104
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_8001C14C
    @L_8001C104:
    bl fn_8006AEEC
    b @L_8001C14C
    @L_8001C10C:
    cmplwi r4, 0x0
    bne @L_8001C120
    li r3, 0x0
    bl fn_801F2A7C
    mr r4, r3
    @L_8001C120:
    cmplwi r4, 0x0
    bne @L_8001C130
    li r3, 0x0
    b @L_8001C14C
    @L_8001C130:
    mr r3, r4
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_8001C14C
    @L_8001C148:
    li r3, 0x0
    @L_8001C14C:
    cmplwi r3, 0x0
    bne @L_8001C15C
    li r27, 0x0
    b @L_8001C204
    @L_8001C15C:
    mr r4, r25
    bl fn_8012AC08
    mr r27, r3
    b @L_8001C1EC
    @L_8001C16C:
    extsb r25, r3
    clrlwi r0, r25, 16
    cmplwi r0, 0x6
    blt @L_8001C180
    b @L_8001C204
    @L_8001C180:
    cmplwi r4, 0x0
    bne @L_8001C194
    li r3, 0x0
    bl fn_801F2A7C
    mr r4, r3
    @L_8001C194:
    cmplwi r4, 0x0
    bne @L_8001C1A4
    li r27, 0x0
    b @L_8001C204
    @L_8001C1A4:
    mr r3, r4
    mr r4, r25
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8001C1C0
    li r27, 0x0
    b @L_8001C204
    @L_8001C1C0:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r27, r3
    b @L_8001C1EC
    @L_8001C1D8:
    extsb r0, r3
    clrlwi r0, r0, 16
    cmplwi r0, 0x1e
    blt @L_8001C1EC
    b @L_8001C204
    @L_8001C1EC:
    mr r3, r27
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001C204
    li r27, 0x0
    @L_8001C204:
    lis r3, lbl_803A1D40@ha
    lis r4, lbl_803A1C20@ha
    addi r5, r3, lbl_803A1D40@l
    li r3, 0x32
    lbz r5, 0x6(r5)
    addi r0, r4, lbl_803A1C20@l
    extsb r4, r5
    mulli r4, r4, 0x30
    add r4, r0, r4
    bl fn_80132A38
    mr r3, r27
    bl fn_801230E0
    lwz r0, 0x8(r31)
    mr r26, r3
    lwz r4, 0xc(r31)
    cmpwi r0, 0x1
    beq @L_8001C280
    bge @L_8001C2BC
    cmpwi r0, 0x0
    bge @L_8001C258
    b @L_8001C2BC
    @L_8001C258:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001C278
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_8001C2C0
    @L_8001C278:
    bl fn_8006AEEC
    b @L_8001C2C0
    @L_8001C280:
    cmplwi r4, 0x0
    bne @L_8001C294
    li r3, 0x0
    bl fn_801F2A7C
    mr r4, r3
    @L_8001C294:
    cmplwi r4, 0x0
    bne @L_8001C2A4
    li r25, 0x0
    b @L_8001C2C4
    @L_8001C2A4:
    mr r3, r4
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_8001C2C0
    @L_8001C2BC:
    li r3, 0x0
    @L_8001C2C0:
    mr r25, r3
    @L_8001C2C4:
    cmplwi r25, 0x0
    bne @L_8001C2D4
    li r3, -0x1
    b @L_8001C7A4
    @L_8001C2D4:
    clrlwi r0, r30, 16
    cmplwi r0, 0x0
    bne @L_8001C384
    clrlwi r0, r26, 16
    cmplwi r0, 0x0
    beq @L_8001C36C
    mr r3, r25
    mr r4, r26
    bl fn_801298B8
    cmpwi r3, 0x0
    bgt @L_8001C318
    li r3, 0x2
    li r4, 0x2b6b
    li r5, 0x1
    li r6, 0x1
    bl fn_80106D3C
    b @L_8001C518
    @L_8001C318:
    mr r3, r25
    mr r4, r26
    li r5, 0x1
    li r6, -0x1
    bl fn_80129A78
    mr r3, r27
    li r4, 0x0
    li r5, 0x0
    bl fn_80123110
    cmplwi r29, 0x0
    beq @L_8001C348
    sth r3, 0x0(r29)
    @L_8001C348:
    clrlwi r4, r3, 16
    li r3, 0x2d
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x2b69
    li r5, 0x1
    li r6, 0x1
    bl fn_80106D3C
    b @L_8001C518
    @L_8001C36C:
    li r3, 0x2
    li r4, 0x2b6a
    li r5, 0x1
    li r6, 0x1
    bl fn_80106D3C
    b @L_8001C518
    @L_8001C384:
    clrlwi r4, r26, 16
    cmplwi r4, 0x0
    beq @L_8001C4CC
    li r3, 0x2d
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x2b66
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x0
    li r4, -0x1
    li r5, -0x1
    li r6, 0x0
    bl fn_8001E074
    extsb r24, r3
    li r3, 0x1
    bl fn_801069FC
    cmpwi r24, 0x0
    beq @L_8001C3DC
    li r3, -0x1
    b @L_8001C7A4
    @L_8001C3DC:
    mr r3, r25
    mr r4, r30
    mr r6, r28
    li r5, 0x1
    bl fn_801299C8
    cmpwi r3, 0x0
    beq @L_8001C410
    li r3, 0x2
    li r4, 0x2b6b
    li r5, 0x1
    li r6, 0x1
    bl fn_80106D3C
    b @L_8001C518
    @L_8001C410:
    mr r3, r25
    mr r4, r26
    bl fn_801298B8
    cmpwi r3, 0x0
    bgt @L_8001C458
    lis r4, lbl_803A1D40@ha
    mr r3, r25
    addi r5, r4, lbl_803A1D40@l
    mr r4, r30
    lbz r6, 0x11(r5)
    li r5, 0x1
    bl fn_80129A78
    li r3, 0x2
    li r4, 0x2b6b
    li r5, 0x1
    li r6, 0x1
    bl fn_80106D3C
    b @L_8001C518
    @L_8001C458:
    mr r3, r25
    mr r4, r26
    li r5, 0x1
    li r6, -0x1
    bl fn_80129A78
    mr r3, r27
    li r4, 0x0
    li r5, 0x0
    bl fn_80123110
    cmplwi r29, 0x0
    mr r26, r3
    beq @L_8001C48C
    sth r26, 0x0(r29)
    @L_8001C48C:
    mr r3, r27
    mr r4, r30
    li r5, 0x1
    bl fn_80123110
    clrlwi r4, r26, 16
    li r3, 0x2d
    bl fn_80132A38
    clrlwi r4, r30, 16
    li r3, 0x2e
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x2b67
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_8001C518
    @L_8001C4CC:
    mr r3, r25
    mr r4, r30
    mr r6, r28
    li r5, 0x1
    bl fn_801299C8
    cmpwi r3, 0x0
    bne @L_8001C518
    mr r3, r27
    mr r4, r30
    li r5, 0x1
    bl fn_80123110
    clrlwi r4, r30, 16
    li r3, 0x2d
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x2b68
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    @L_8001C518:
    li r3, 0x1
    bl fn_801069FC
    lis r3, lbl_803A1C20@ha
    li r4, 0x0
    addi r3, r3, lbl_803A1C20@l
    li r5, 0x120
    bl memset
    li r27, 0x0
    lis r3, lbl_803A1C20@ha
    addi r28, r3, lbl_803A1C20@l
    b @L_8001C794
    @L_8001C544:
    clrlwi r4, r27, 16
    lwz r5, 0x8(r31)
    mulli r0, r4, 0x30
    lwz r3, 0xc(r31)
    cmpwi r5, 0x1
    li r25, 0x0
    add r29, r28, r0
    beq @L_8001C624
    bge @L_8001C574
    cmpwi r5, 0x0
    bge @L_8001C580
    b @L_8001C68C
    @L_8001C574:
    cmpwi r5, 0x3
    bge @L_8001C68C
    b @L_8001C680
    @L_8001C580:
    cmplwi r4, 0x6
    blt @L_8001C58C
    b @L_8001C6A4
    @L_8001C58C:
    cmpwi r5, 0x1
    beq @L_8001C5CC
    bge @L_8001C600
    cmpwi r5, 0x0
    bge @L_8001C5A4
    b @L_8001C600
    @L_8001C5A4:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_8001C5C4
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_8001C604
    @L_8001C5C4:
    bl fn_8006AEEC
    b @L_8001C604
    @L_8001C5CC:
    cmplwi r3, 0x0
    bne @L_8001C5DC
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001C5DC:
    cmplwi r3, 0x0
    bne @L_8001C5EC
    li r3, 0x0
    b @L_8001C604
    @L_8001C5EC:
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_8001C604
    @L_8001C600:
    li r3, 0x0
    @L_8001C604:
    cmplwi r3, 0x0
    bne @L_8001C614
    li r25, 0x0
    b @L_8001C6A4
    @L_8001C614:
    mr r4, r27
    bl fn_8012AC08
    mr r25, r3
    b @L_8001C68C
    @L_8001C624:
    cmplwi r4, 0x6
    blt @L_8001C630
    b @L_8001C6A4
    @L_8001C630:
    cmplwi r3, 0x0
    bne @L_8001C640
    li r3, 0x0
    bl fn_801F2A7C
    @L_8001C640:
    cmplwi r3, 0x0
    bne @L_8001C650
    li r25, 0x0
    b @L_8001C6A4
    @L_8001C650:
    mr r4, r27
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_8001C668
    li r25, 0x0
    b @L_8001C6A4
    @L_8001C668:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r25, r3
    b @L_8001C68C
    @L_8001C680:
    cmplwi r4, 0x1e
    blt @L_8001C68C
    b @L_8001C6A4
    @L_8001C68C:
    mr r3, r25
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001C6A4
    li r25, 0x0
    @L_8001C6A4:
    cmplwi r25, 0x0
    bne @L_8001C6B8
    li r0, 0x0
    sth r0, 0x0(r29)
    b @L_8001C790
    @L_8001C6B8:
    mr r3, r25
    mr r4, r29
    bl fn_8011FDC8
    mr r3, r25
    li r4, 0x0
    li r5, 0x7b
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8001C6EC
    li r0, 0x0
    sth r0, 0x1a(r29)
    @L_8001C6EC:
    mr r3, r25
    li r4, 0x0
    li r5, 0x7b
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8001C714
    li r0, 0x1
    b @L_8001C778
    @L_8001C714:
    mr r3, r25
    bl fn_80120FE0
    clrlwi r0, r3, 16
    cmpwi r0, 0x3c
    beq @L_8001C75C
    bge @L_8001C73C
    cmpwi r0, 0x3a
    beq @L_8001C74C
    bge @L_8001C754
    b @L_8001C774
    @L_8001C73C:
    cmpwi r0, 0x3e
    beq @L_8001C76C
    bge @L_8001C774
    b @L_8001C764
    @L_8001C74C:
    li r0, 0x2
    b @L_8001C778
    @L_8001C754:
    li r0, 0x3
    b @L_8001C778
    @L_8001C75C:
    li r0, 0x4
    b @L_8001C778
    @L_8001C764:
    li r0, 0x5
    b @L_8001C778
    @L_8001C76C:
    li r0, 0x6
    b @L_8001C778
    @L_8001C774:
    li r0, 0x0
    @L_8001C778:
    clrlwi r0, r0, 16
    lis r3, lbl_802E4EB8@ha
    slwi r0, r0, 1
    addi r3, r3, lbl_802E4EB8@l
    lhzx r0, r3, r0
    sth r0, 0x24(r29)
    @L_8001C790:
    addi r27, r27, 0x1
    @L_8001C794:
    clrlwi r0, r27, 16
    cmplwi r0, 0x6
    blt @L_8001C544
    li r3, -0x1
    @L_8001C7A4:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
