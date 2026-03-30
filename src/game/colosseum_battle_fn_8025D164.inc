    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    li r28, 0x0
    bl fn_8006B5A8
    lwz r27, 0xc(r3)
    bl fn_8006B5A8
    lwz r26, 0x0(r3)
    bl fn_8006B5A8
    lwz r30, 0x14(r3)
    li r29, 0x0
@L_8025D194:
    li r3, 0x0
    bl fn_8006B09C
    clrlwi r4, r29, 16
    addi r3, r3, 0xb44
    bl fn_8012AC08
    mr r31, r3
    bl fn_80123FBC
    clrlwi. r0, r3, 24
    beq @L_8025D1EC
    mr r3, r31
    bl fn_8011F5C8
    lwz r4, -31092(r13)
    clrlwi r0, r3, 16
    li r3, 0x0
@L_8025D1CC:
    lhzx r5, r4, r3
    cmplwi r5, 0x0
    beq @L_8025D1EC
    cmplw r0, r5
    bne @L_8025D1E4
    addi r28, r28, 0x1
@L_8025D1E4:
    addi r3, r3, 0x2
    b @L_8025D1CC
@L_8025D1EC:
    addi r29, r29, 0x1
    cmpwi r29, 0x6
    blt @L_8025D194
    lfs f1, -20552(r2)
    lfs f0, -20548(r2)
    mtctr r28
    cmpwi r28, 0x0
    ble @L_8025D214
@L_8025D20C:
    fmuls f1, f1, f0
    bdnz @L_8025D20C
@L_8025D214:
    cmpwi r26, 0x1
    bne @L_8025D24C
    addi r3, r30, 0x1
    li r0, 0xa
    divw r0, r3, r0
    cmpwi r0, 0xa
    ble @L_8025D234
    li r0, 0xa
@L_8025D234:
    lis r3, 0x803A
    slwi r0, r0, 2
    addi r3, r3, -22940
    lfsx f0, r3, r0
    fmuls f1, f1, f0
    b @L_8025D26C
@L_8025D24C:
    cmpwi r27, 0x6
    blt @L_8025D258
    li r27, 0x6
@L_8025D258:
    lis r3, 0x803A
    slwi r0, r27, 2
    addi r3, r3, -22968
    lfsx f0, r3, r0
    fmuls f1, f1, f0
@L_8025D26C:
    fctiwz f0, f1
    stfd f0, 0x8(r1)
    lwz r3, 0xc(r1)
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
