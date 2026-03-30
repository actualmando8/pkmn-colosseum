    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr. r28, r3
    li r30, 0x0
    bne @L_8025D0D4
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    mr r28, r3
@L_8025D0D4:
    li r31, 0x0
@L_8025D0D8:
    mr r3, r28
    clrlwi r4, r31, 16
    bl fn_8012AC08
    mr r29, r3
    bl fn_80123FBC
    clrlwi. r0, r3, 24
    beq @L_8025D128
    mr r3, r29
    bl fn_8011F5C8
    lwz r4, -31092(r13)
    clrlwi r0, r3, 16
    li r3, 0x0
@L_8025D108:
    lhzx r5, r4, r3
    cmplwi r5, 0x0
    beq @L_8025D128
    cmplw r0, r5
    bne @L_8025D120
    addi r30, r30, 0x1
@L_8025D120:
    addi r3, r3, 0x2
    b @L_8025D108
@L_8025D128:
    addi r31, r31, 0x1
    cmpwi r31, 0x6
    blt @L_8025D0D8
    lfs f1, -20552(r2)
    lfs f0, -20548(r2)
    mtctr r30
    cmpwi r30, 0x0
    ble @L_8025D150
@L_8025D148:
    fmuls f1, f1, f0
    bdnz @L_8025D148
@L_8025D150:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
