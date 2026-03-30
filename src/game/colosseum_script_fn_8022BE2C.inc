    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    lis r5, lbl_80279FF8@ha
    stw r0, 0x64(r1)
    addi r5, r5, lbl_80279FF8@l
    li r0, 0x2
    stmw r19, 0x2c(r1)
    mr r21, r4
    mr r31, r3
    subi r4, r5, 0x4
    li r30, 0x0
    li r27, 0x0
    lwz r6, -20636(r2)
    lbz r7, -20632(r2)
    stw r6, 0x8(r1)
    addi r6, r1, 0xc
    stb r7, 0xc(r1)
    mtctr r0
@L_8022BE74:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r6)
    stwu r0, 0x8(r6)
    bdnz @L_8022BE74
    lwz r0, 0x4(r4)
    mr r3, r31
    stw r0, 0x4(r6)
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022BEA8
    li r3, 0x0
    b @L_8022D070
@L_8022BEA8:
    mr r3, r31
    bl fn_802040E8
    mr r19, r3
    mr r3, r31
    bl fn_80203FE4
    mr r20, r3
    mr r3, r31
    bl fn_80203EDC
    mr r0, r3
    mr r3, r31
    mr r28, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r22, r0
    bl fn_80205B8C
    li r4, 0x0
    mr r23, r3
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r26, r3, 0, 16, 31
    mr r3, r23
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    bl fn_8012640C
    rlwinm r25, r3, 0, 16, 31
    rlwinm r7, r19, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    bl fn_801F4C14
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x2b
    bgt @L_8022CFBC
    lis r3, jumptable_8039A3C8@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8039A3C8@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r31
    li r4, 0x2
    bl fn_80203BDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022BF7C
    rlwinm. r0, r21, 0, 24, 31
    beq @L_8022BF98
@L_8022BF7C:
    mr r3, r31
    bl fn_80201704
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022CFBC
    rlwinm r0, r21, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022CFBC
@L_8022BF98:
    add r0, r26, r28
    mr r4, r28
    cmpw r0, r25
    ble @L_8022BFAC
    subf r4, r26, r25
@L_8022BFAC:
    mr r3, r22
    neg r7, r4
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lis r3, lbl_80379B22@ha
    li r30, 0x4
    addi r0, r3, lbl_80379B22@l
    mr r27, r0
    b @L_8022CFBC
    rlwinm. r0, r21, 0, 24, 31
    beq @L_8022BFE8
    cmplwi r0, 0x2
    bne @L_8022CFBC
@L_8022BFE8:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    bl fn_80205BE8
    li r25, 0x0
    mr r19, r3
    b @L_8022C058
@L_8022C00C:
    mr r3, r19
    mr r4, r25
    bl fn_80123CD4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022C054
    mr r3, r19
    mr r6, r25
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r24, r3, 0, 16, 31
    mr r3, r19
    mr r6, r25
    li r4, 0x0
    li r5, 0x80
    bl fn_8012640C
    rlwinm. r29, r3, 0, 24, 31
    beq @L_8022C064
@L_8022C054:
    addi r25, r25, 0x1
@L_8022C058:
    rlwinm r0, r25, 0, 16, 31
    cmplwi r0, 0x4
    blt @L_8022C00C
@L_8022C064:
    rlwinm r0, r25, 0, 16, 31
    cmplwi r0, 0x4
    beq @L_8022CFBC
    mr r3, r19
    mr r4, r25
    bl fn_80123E70
    add r4, r29, r28
    rlwinm r0, r3, 0, 24, 31
    cmpw r4, r0
    rlwinm r5, r4, 0, 24, 31
    ble @L_8022C094
    mr r5, r3
@L_8022C094:
    mr r3, r19
    mr r6, r25
    rlwinm r7, r5, 0, 24, 31
    li r4, 0x0
    li r5, 0x80
    bl fn_801254B4
    mr r3, r31
    li r4, 0x10
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022C120
    mr r3, r31
    li r4, 0x31
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022C120
    mr r3, r31
    li r4, 0x31
    bl fn_80201890
    rlwinm r0, r25, 0, 24, 31
    li r4, 0x1
    slw r0, r4, r0
    and. r0, r3, r0
    bne @L_8022C120
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022C120
    rlwinm r5, r25, 0, 24, 31
    mr r3, r31
    li r4, 0x80
    li r6, 0x1
    bl fn_801FE7EC
@L_8022C120:
    mr r4, r24
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    lis r3, lbl_80379B45@ha
    li r30, 0x3
    addi r0, r3, lbl_80379B45@l
    mr r27, r0
    b @L_8022CFBC
    lis r3, lbl_80279FD0@ha
    li r19, 0x0
    addi r21, r3, lbl_80279FD0@l
    b @L_8022C1A8
@L_8022C168:
    clrlslwi r22, r19, 16, 1
    mr r3, r31
    lhzx r5, r21, r22
    li r4, 0x0
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x6
    bge @L_8022C1A4
    lhzx r5, r21, r22
    mr r3, r31
    li r4, 0x0
    li r6, 0x0
    li r7, 0x6
    bl fn_801254B4
    li r30, 0x5
@L_8022C1A4:
    addi r19, r19, 0x1
@L_8022C1A8:
    rlwinm r0, r19, 0, 16, 31
    cmplwi r0, 0x7
    blt @L_8022C168
    rlwinm. r0, r30, 0, 24, 31
    beq @L_8022CFBC
    lis r3, lbl_80379B06@ha
    addi r0, r3, lbl_80379B06@l
    mr r27, r0
    b @L_8022CFBC
    mr r3, r31
    bl fn_80201704
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022C1E4
    rlwinm. r0, r21, 0, 24, 31
    beq @L_8022C200
@L_8022C1E4:
    mr r3, r31
    bl fn_80201704
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022CFBC
    rlwinm r0, r21, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022CFBC
@L_8022C200:
    mr r3, r31
    li r4, 0x10
    bl fn_80203B5C
    rlwinm r4, r3, 0, 16, 31
    add r0, r26, r4
    cmpw r0, r25
    ble @L_8022C220
    subf r4, r26, r25
@L_8022C220:
    mr r3, r22
    neg r7, r4
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lis r3, lbl_80379B5B@ha
    li r30, 0x4
    addi r0, r3, lbl_80379B5B@l
    mr r27, r0
    b @L_8022CFBC
    mr r3, r31
    li r19, 0x0
    li r4, 0x2
    bl fn_80203BDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022C26C
    rlwinm. r0, r21, 0, 24, 31
    beq @L_8022C288
@L_8022C26C:
    mr r3, r31
    bl fn_80201704
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022C308
    rlwinm r0, r21, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022C308
@L_8022C288:
    li r3, 0x0
    bl fn_80143B08
    bl fn_80143AF0
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    bl fn_80203B5C
    rlwinm r4, r3, 0, 16, 31
    add r0, r26, r4
    cmpw r0, r25
    ble @L_8022C2C4
    subf r4, r26, r25
@L_8022C2C4:
    mr r3, r22
    neg r7, r4
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    bl fn_80201400
    extsb r0, r3
    cmpwi r0, -0x1
    bne @L_8022C300
    lis r3, lbl_80379B82@ha
    addi r19, r3, lbl_80379B82@l
    b @L_8022C308
@L_8022C300:
    lis r3, lbl_80379B22@ha
    addi r19, r3, lbl_80379B22@l
@L_8022C308:
    cmplwi r19, 0x0
    mr r27, r19
    beq @L_8022CFBC
    li r30, 0x4
    b @L_8022CFBC
    mr r3, r31
    li r19, 0x0
    li r4, 0x2
    bl fn_80203BDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022C33C
    rlwinm. r0, r21, 0, 24, 31
    beq @L_8022C358
@L_8022C33C:
    mr r3, r31
    bl fn_80201704
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022C3D8
    rlwinm r0, r21, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022C3D8
@L_8022C358:
    li r3, 0x1
    bl fn_80143B08
    bl fn_80143AF0
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    bl fn_80203B5C
    rlwinm r4, r3, 0, 16, 31
    add r0, r26, r4
    cmpw r0, r25
    ble @L_8022C394
    subf r4, r26, r25
@L_8022C394:
    mr r3, r22
    neg r7, r4
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x1
    bl fn_80201400
    extsb r0, r3
    cmpwi r0, -0x1
    bne @L_8022C3D0
    lis r3, lbl_80379B82@ha
    addi r19, r3, lbl_80379B82@l
    b @L_8022C3D8
@L_8022C3D0:
    lis r3, lbl_80379B22@ha
    addi r19, r3, lbl_80379B22@l
@L_8022C3D8:
    cmplwi r19, 0x0
    mr r27, r19
    beq @L_8022CFBC
    li r30, 0x4
    b @L_8022CFBC
    mr r3, r31
    li r19, 0x0
    li r4, 0x2
    bl fn_80203BDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022C40C
    rlwinm. r0, r21, 0, 24, 31
    beq @L_8022C428
@L_8022C40C:
    mr r3, r31
    bl fn_80201704
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022C4A8
    rlwinm r0, r21, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022C4A8
@L_8022C428:
    li r3, 0x2
    bl fn_80143B08
    bl fn_80143AF0
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    bl fn_80203B5C
    rlwinm r4, r3, 0, 16, 31
    add r0, r26, r4
    cmpw r0, r25
    ble @L_8022C464
    subf r4, r26, r25
@L_8022C464:
    mr r3, r22
    neg r7, r4
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x2
    bl fn_80201400
    extsb r0, r3
    cmpwi r0, -0x1
    bne @L_8022C4A0
    lis r3, lbl_80379B82@ha
    addi r19, r3, lbl_80379B82@l
    b @L_8022C4A8
@L_8022C4A0:
    lis r3, lbl_80379B22@ha
    addi r19, r3, lbl_80379B22@l
@L_8022C4A8:
    cmplwi r19, 0x0
    mr r27, r19
    beq @L_8022CFBC
    li r30, 0x4
    b @L_8022CFBC
    mr r3, r31
    li r19, 0x0
    li r4, 0x2
    bl fn_80203BDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022C4DC
    rlwinm. r0, r21, 0, 24, 31
    beq @L_8022C4F8
@L_8022C4DC:
    mr r3, r31
    bl fn_80201704
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022C578
    rlwinm r0, r21, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022C578
@L_8022C4F8:
    li r3, 0x3
    bl fn_80143B08
    bl fn_80143AF0
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    bl fn_80203B5C
    rlwinm r4, r3, 0, 16, 31
    add r0, r26, r4
    cmpw r0, r25
    ble @L_8022C534
    subf r4, r26, r25
@L_8022C534:
    mr r3, r22
    neg r7, r4
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x3
    bl fn_80201400
    extsb r0, r3
    cmpwi r0, -0x1
    bne @L_8022C570
    lis r3, lbl_80379B82@ha
    addi r19, r3, lbl_80379B82@l
    b @L_8022C578
@L_8022C570:
    lis r3, lbl_80379B22@ha
    addi r19, r3, lbl_80379B22@l
@L_8022C578:
    cmplwi r19, 0x0
    mr r27, r19
    beq @L_8022CFBC
    li r30, 0x4
    b @L_8022CFBC
    mr r3, r31
    li r19, 0x0
    li r4, 0x2
    bl fn_80203BDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022C5AC
    rlwinm. r0, r21, 0, 24, 31
    beq @L_8022C5C8
@L_8022C5AC:
    mr r3, r31
    bl fn_80201704
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022C648
    rlwinm r0, r21, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022C648
@L_8022C5C8:
    li r3, 0x4
    bl fn_80143B08
    bl fn_80143AF0
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    bl fn_80203B5C
    rlwinm r4, r3, 0, 16, 31
    add r0, r26, r4
    cmpw r0, r25
    ble @L_8022C604
    subf r4, r26, r25
@L_8022C604:
    mr r3, r22
    neg r7, r4
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x4
    bl fn_80201400
    extsb r0, r3
    cmpwi r0, -0x1
    bne @L_8022C640
    lis r3, lbl_80379B82@ha
    addi r19, r3, lbl_80379B82@l
    b @L_8022C648
@L_8022C640:
    lis r3, lbl_80379B22@ha
    addi r19, r3, lbl_80379B22@l
@L_8022C648:
    cmplwi r19, 0x0
    mr r27, r19
    beq @L_8022CFBC
    li r30, 0x4
    b @L_8022CFBC
    rlwinm. r0, r21, 0, 24, 31
    li r20, 0x0
    beq @L_8022C670
    cmplwi r0, 0x2
    bne @L_8022C6D0
@L_8022C670:
    mr r3, r31
    li r4, 0x0
    li r5, 0xe6
    li r6, 0x0
    bl fn_8012640C
    rlwinm r19, r3, 0, 24, 31
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    bl fn_80203BDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022C6D0
    cmplwi r19, 0xc
    bge @L_8022C6D0
    lis r4, lbl_80379F58@ha
    lis r3, lbl_80379BB4@ha
    addi r4, r4, lbl_80379F58@l
    li r6, 0x11
    addis r5, r4, 0x1
    li r0, 0x0
    li r4, 0xf
    stb r6, 0x601e(r5)
    addi r20, r3, lbl_80379BB4@l
    stb r4, 0x60a4(r5)
    stb r0, 0x60a5(r5)
@L_8022C6D0:
    cmplwi r20, 0x0
    mr r27, r20
    beq @L_8022CFBC
    li r30, 0x5
    b @L_8022CFBC
    rlwinm. r0, r21, 0, 24, 31
    li r20, 0x0
    beq @L_8022C6F8
    cmplwi r0, 0x2
    bne @L_8022C758
@L_8022C6F8:
    mr r3, r31
    li r4, 0x0
    li r5, 0xe7
    li r6, 0x0
    bl fn_8012640C
    rlwinm r19, r3, 0, 24, 31
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    bl fn_80203BDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022C758
    cmplwi r19, 0xc
    bge @L_8022C758
    lis r4, lbl_80379F58@ha
    lis r3, lbl_80379BB4@ha
    addi r4, r4, lbl_80379F58@l
    li r6, 0x12
    addis r5, r4, 0x1
    li r0, 0x0
    li r4, 0x10
    stb r6, 0x601e(r5)
    addi r20, r3, lbl_80379BB4@l
    stb r4, 0x60a4(r5)
    stb r0, 0x60a5(r5)
@L_8022C758:
    cmplwi r20, 0x0
    mr r27, r20
    beq @L_8022CFBC
    li r30, 0x5
    b @L_8022CFBC
    rlwinm. r0, r21, 0, 24, 31
    li r20, 0x0
    beq @L_8022C780
    cmplwi r0, 0x2
    bne @L_8022C7E0
@L_8022C780:
    mr r3, r31
    li r4, 0x0
    li r5, 0xea
    li r6, 0x0
    bl fn_8012640C
    rlwinm r19, r3, 0, 24, 31
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    bl fn_80203BDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022C7E0
    cmplwi r19, 0xc
    bge @L_8022C7E0
    lis r4, lbl_80379F58@ha
    lis r3, lbl_80379BB4@ha
    addi r4, r4, lbl_80379F58@l
    li r6, 0x13
    addis r5, r4, 0x1
    li r0, 0x0
    li r4, 0x11
    stb r6, 0x601e(r5)
    addi r20, r3, lbl_80379BB4@l
    stb r4, 0x60a4(r5)
    stb r0, 0x60a5(r5)
@L_8022C7E0:
    cmplwi r20, 0x0
    mr r27, r20
    beq @L_8022CFBC
    li r30, 0x5
    b @L_8022CFBC
    rlwinm. r0, r21, 0, 24, 31
    li r20, 0x0
    beq @L_8022C808
    cmplwi r0, 0x2
    bne @L_8022C868
@L_8022C808:
    mr r3, r31
    li r4, 0x0
    li r5, 0xe8
    li r6, 0x0
    bl fn_8012640C
    rlwinm r19, r3, 0, 24, 31
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    bl fn_80203BDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022C868
    cmplwi r19, 0xc
    bge @L_8022C868
    lis r4, lbl_80379F58@ha
    lis r3, lbl_80379BB4@ha
    addi r4, r4, lbl_80379F58@l
    li r6, 0x14
    addis r5, r4, 0x1
    li r0, 0x0
    li r4, 0x12
    stb r6, 0x601e(r5)
    addi r20, r3, lbl_80379BB4@l
    stb r4, 0x60a4(r5)
    stb r0, 0x60a5(r5)
@L_8022C868:
    cmplwi r20, 0x0
    mr r27, r20
    beq @L_8022CFBC
    li r30, 0x5
    b @L_8022CFBC
    rlwinm. r0, r21, 0, 24, 31
    li r20, 0x0
    beq @L_8022C890
    cmplwi r0, 0x2
    bne @L_8022C8F0
@L_8022C890:
    mr r3, r31
    li r4, 0x0
    li r5, 0xe9
    li r6, 0x0
    bl fn_8012640C
    rlwinm r19, r3, 0, 24, 31
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    bl fn_80203BDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022C8F0
    cmplwi r19, 0xc
    bge @L_8022C8F0
    lis r4, lbl_80379F58@ha
    lis r3, lbl_80379BB4@ha
    addi r4, r4, lbl_80379F58@l
    li r6, 0x15
    addis r5, r4, 0x1
    li r0, 0x0
    li r4, 0x13
    stb r6, 0x601e(r5)
    addi r20, r3, lbl_80379BB4@l
    stb r4, 0x60a4(r5)
    stb r0, 0x60a5(r5)
@L_8022C8F0:
    cmplwi r20, 0x0
    mr r27, r20
    beq @L_8022CFBC
    li r30, 0x5
    b @L_8022CFBC
    rlwinm. r0, r21, 0, 24, 31
    beq @L_8022C914
    cmplwi r0, 0x2
    bne @L_8022CFBC
@L_8022C914:
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    bl fn_80203BDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022CFBC
    mr r3, r31
    li r4, 0xf
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022CFBC
    mr r3, r31
    li r4, 0xf
    li r5, 0x0
    bl fn_8020248C
    lis r3, lbl_80379BD1@ha
    li r30, 0x2
    addi r0, r3, lbl_80379BD1@l
    mr r27, r0
    b @L_8022CFBC
    rlwinm. r0, r21, 0, 24, 31
    beq @L_8022C974
    cmplwi r0, 0x2
    bne @L_8022CFBC
@L_8022C974:
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    bl fn_80203BDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022CFBC
    addi r3, r1, 0x10
    li r24, 0x0
    li r5, 0x0
    li r4, -0x1
    b @L_8022C9A8
@L_8022C99C:
    clrlslwi r0, r5, 16, 2
    addi r5, r5, 0x1
    stwx r4, r3, r0
@L_8022C9A8:
    rlwinm r0, r5, 0, 16, 31
    cmplwi r0, 0x5
    blt @L_8022C99C
    addi r22, r1, 0x8
    li r29, 0x0
    b @L_8022CA54
@L_8022C9C0:
    rlwinm r0, r29, 0, 16, 31
    lbzx r19, r22, r0
    cmplwi r19, 0x7
    bgt @L_8022CA20
    lis r3, jumptable_8039A3A8@ha
    rlwinm r0, r19, 2, 0, 29
    addi r3, r3, jumptable_8039A3A8@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r5, 0xe6
    b @L_8022CA24
    li r5, 0xe7
    b @L_8022CA24
    li r5, 0xea
    b @L_8022CA24
    li r5, 0xe8
    b @L_8022CA24
    li r5, 0xe9
    b @L_8022CA24
    li r5, 0xeb
    b @L_8022CA24
    li r5, 0xec
    b @L_8022CA24
@L_8022CA20:
    li r5, 0x0
@L_8022CA24:
    mr r3, r31
    li r4, 0x0
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0xc
    bge @L_8022CA50
    clrlslwi r0, r24, 16, 2
    addi r3, r1, 0x10
    stwx r19, r3, r0
    addi r24, r24, 0x1
@L_8022CA50:
    addi r29, r29, 0x1
@L_8022CA54:
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x5
    blt @L_8022C9C0
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0x1
    blt @L_8022CFBC
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    rlwinm r3, r24, 0, 16, 31
    divw r0, r4, r3
    addi r5, r1, 0x10
    mullw r0, r0, r3
    subf r0, r0, r4
    clrlslwi r0, r0, 16, 2
    lwzx r3, r5, r0
    cmpwi r3, -0x1
    beq @L_8022CFBC
    rlwinm. r0, r21, 0, 24, 31
    rlwinm r21, r3, 0, 24, 31
    addi r3, r21, 0x26
    li r20, 0x0
    rlwinm r22, r3, 0, 24, 31
    beq @L_8022CAB8
    cmplwi r0, 0x2
    bne @L_8022CB6C
@L_8022CAB8:
    cmplwi r21, 0x7
    bgt @L_8022CB10
    lis r3, jumptable_8039A388@ha
    rlwinm r0, r21, 2, 0, 29
    addi r3, r3, jumptable_8039A388@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r5, 0xe6
    b @L_8022CB14
    li r5, 0xe7
    b @L_8022CB14
    li r5, 0xea
    b @L_8022CB14
    li r5, 0xe8
    b @L_8022CB14
    li r5, 0xe9
    b @L_8022CB14
    li r5, 0xeb
    b @L_8022CB14
    li r5, 0xec
    b @L_8022CB14
@L_8022CB10:
    li r5, 0x0
@L_8022CB14:
    mr r3, r31
    li r4, 0x0
    li r6, 0x0
    bl fn_8012640C
    rlwinm r19, r3, 0, 24, 31
    mr r3, r31
    rlwinm r4, r28, 0, 16, 31
    bl fn_80203BDC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022CB6C
    cmplwi r19, 0xc
    bge @L_8022CB6C
    lis r3, lbl_80379F58@ha
    addi r5, r21, 0x20
    addi r4, r3, lbl_80379F58@l
    li r0, 0x0
    addis r4, r4, 0x1
    lis r3, lbl_80379BB4@ha
    stb r5, 0x601e(r4)
    addi r20, r3, lbl_80379BB4@l
    stb r22, 0x60a4(r4)
    stb r0, 0x60a5(r4)
@L_8022CB6C:
    cmplwi r20, 0x0
    mr r27, r20
    beq @L_8022CFBC
    li r30, 0x5
    b @L_8022CFBC
    mr r3, r31
    li r19, 0x0
    li r4, 0x5
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CBB0
    mr r3, r31
    li r4, 0x5
    bl fn_80202810
    lis r3, lbl_80379A36@ha
    addi r19, r3, lbl_80379A36@l
@L_8022CBB0:
    cmplwi r19, 0x0
    mr r27, r19
    beq @L_8022CFBC
    li r30, 0x1
    b @L_8022CFBC
    mr r3, r31
    li r19, 0x0
    li r4, 0x3
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CC00
    mr r3, r31
    li r4, 0x3
    bl fn_80202810
    mr r3, r31
    li r4, 0x4
    bl fn_80202810
    lis r3, lbl_80379A54@ha
    addi r19, r3, lbl_80379A54@l
@L_8022CC00:
    cmplwi r19, 0x0
    mr r27, r19
    beq @L_8022CC14
    li r30, 0x1
    b @L_8022CFBC
@L_8022CC14:
    mr r3, r31
    li r19, 0x0
    li r4, 0x4
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CC50
    mr r3, r31
    li r4, 0x4
    bl fn_80202810
    mr r3, r31
    li r4, 0x3
    bl fn_80202810
    lis r3, lbl_80379A54@ha
    addi r19, r3, lbl_80379A54@l
@L_8022CC50:
    cmplwi r19, 0x0
    mr r27, r19
    beq @L_8022CFBC
    li r30, 0x1
    b @L_8022CFBC
    mr r3, r31
    li r19, 0x0
    li r4, 0x6
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CC94
    mr r3, r31
    li r4, 0x6
    bl fn_80202810
    lis r3, lbl_80379A72@ha
    addi r19, r3, lbl_80379A72@l
@L_8022CC94:
    cmplwi r19, 0x0
    mr r27, r19
    beq @L_8022CFBC
    li r30, 0x1
    b @L_8022CFBC
    mr r3, r31
    li r19, 0x0
    li r4, 0x7
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CCD8
    mr r3, r31
    li r4, 0x7
    bl fn_80202810
    lis r3, lbl_80379A90@ha
    addi r19, r3, lbl_80379A90@l
@L_8022CCD8:
    cmplwi r19, 0x0
    mr r27, r19
    beq @L_8022CFBC
    li r30, 0x1
    b @L_8022CFBC
    mr r3, r31
    li r19, 0x0
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CD28
    mr r3, r31
    li r4, 0x8
    bl fn_80202810
    mr r3, r31
    li r4, 0x17
    bl fn_80202810
    lis r3, lbl_80379AAE@ha
    addi r19, r3, lbl_80379AAE@l
@L_8022CD28:
    cmplwi r19, 0x0
    mr r27, r19
    beq @L_8022CFBC
    li r30, 0x1
    b @L_8022CFBC
    mr r3, r31
    li r19, 0x0
    li r4, 0x9
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CD6C
    mr r3, r31
    li r4, 0x9
    bl fn_80202810
    lis r3, lbl_80379ACC@ha
    addi r19, r3, lbl_80379ACC@l
@L_8022CD6C:
    cmplwi r19, 0x0
    mr r27, r19
    beq @L_8022CFBC
    li r30, 0x2
    b @L_8022CFBC
    mr r3, r31
    bl fn_80203CCC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022CDA8
    mr r3, r31
    li r4, 0x9
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CFBC
@L_8022CDA8:
    li r20, 0x0
    la r19, lbl_80478D78(r13)
    stb r20, 0x5(r19)
    mr r3, r31
    li r4, 0x3
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8022CDE4
    mr r3, r31
    li r4, 0x4
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CE00
@L_8022CDE4:
    li r3, 0x3
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r20, 0x1
@L_8022CE00:
    mr r3, r31
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CE40
    mr r3, r31
    li r4, 0x17
    bl fn_80202810
    li r3, 0x8
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    addi r20, r20, 0x1
@L_8022CE40:
    mr r3, r31
    li r4, 0x5
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CE74
    li r3, 0x5
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    addi r20, r20, 0x1
@L_8022CE74:
    mr r3, r31
    li r4, 0x6
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CEA8
    li r3, 0x6
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    addi r20, r20, 0x1
@L_8022CEA8:
    mr r3, r31
    li r4, 0x7
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CEDC
    li r3, 0x7
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    addi r20, r20, 0x1
@L_8022CEDC:
    mr r3, r31
    li r4, 0x9
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CF10
    li r3, 0x9
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    addi r20, r20, 0x1
@L_8022CF10:
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_8022CF24
    li r0, 0x1
    stb r0, 0x5(r19)
@L_8022CF24:
    mr r3, r23
    bl fn_801252E0
    mr r3, r31
    li r4, 0x9
    bl fn_80202810
    mr r3, r31
    li r4, 0x0
    bl fn_80202998
    lis r3, lbl_80379AE8@ha
    li r30, 0x1
    addi r0, r3, lbl_80379AE8@l
    mr r27, r0
    b @L_8022CFBC
    mr r3, r31
    li r19, 0x0
    li r4, 0xa
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022CF88
    mr r3, r31
    li r4, 0xa
    bl fn_80202810
    lis r3, lbl_80379AE8@ha
    addi r19, r3, lbl_80379AE8@l
@L_8022CF88:
    cmplwi r19, 0x0
    mr r27, r19
    beq @L_8022CFBC
    li r3, 0xa
    bl fn_80119F50
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    li r30, 0x2
@L_8022CFBC:
    rlwinm. r0, r30, 0, 24, 31
    beq @L_8022D06C
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x49
    li r6, 0x0
    bl fn_801F4C14
    rlwinm r0, r30, 0, 24, 31
    cmpwi r0, 0x2
    beq @L_8022D054
    bge @L_8022D054
    cmpwi r0, 0x1
    bge @L_8022D02C
    b @L_8022D054
    b @L_8022D054
@L_8022D02C:
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022D054
    mr r3, r31
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_8022D054:
    cmplwi r27, 0x0
    beq @L_8022D06C
    lwz r3, lbl_8047B62C(r13)
    mr r4, r27
    li r5, 0x0
    bl fn_80211B94
@L_8022D06C:
    mr r3, r30
@L_8022D070:
    lmw r19, 0x2c(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
