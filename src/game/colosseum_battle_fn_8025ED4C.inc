    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r3, 0x8048
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    lwz r0, -32068(r3)
    cmpwi r0, 0x0
    beq @L_8025ED78
    cmpwi r0, 0x1
    bne @L_8025EF40
@L_8025ED78:
    li r3, 0x1
    bl fn_801DADC0
    lis r3, 0x8048
    addi r31, r3, -32068
    lwz r0, 0x0(r31)
    cmpwi r0, 0x0
    bne @L_8025EE30
    lwz r0, 0x8(r31)
    li r3, 0x0
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r30, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025EDC4
    li r0, 0x0
    b @L_8025EE1C
@L_8025EDC4:
    mr r3, r30
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    mr r4, r30
    addi r3, r1, 0x8
    addi r5, r13, -31352
    li r6, 0x2
    bl fn_8025E0B0
    cmpwi r3, 0x1
    bne @L_8025EE18
    addi r3, r1, 0x8
    addi r4, r13, -31352
    li r5, 0x2
    li r6, 0x1
    li r7, 0x1
    li r8, 0x0
    bl fn_8025DE54
    li r0, 0x1
    b @L_8025EE1C
@L_8025EE18:
    li r0, 0x0
@L_8025EE1C:
    cmpwi r0, 0x1
    bne @L_8025EEF0
    lwz r3, 0x8(r31)
    bl fn_8025E9BC
    b @L_8025EEF0
@L_8025EE30:
    cmpwi r0, 0x1
    bne @L_8025EEF0
    lwz r0, 0x8(r31)
    li r3, 0x0
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r30, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025EE68
    li r0, 0x0
    b @L_8025EEE0
@L_8025EE68:
    mr r3, r30
    li r4, 0x0
    li r5, 0xc5
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r30
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    lis r3, 0x803A
    mr r4, r30
    addi r5, r3, -22872
    li r6, 0x4
    addi r3, r1, 0x8
    bl fn_8025E0B0
    cmpwi r3, 0x1
    bne @L_8025EEDC
    lis r4, 0x803A
    addi r3, r1, 0x8
    addi r4, r4, -22872
    li r5, 0x4
    li r6, 0x1
    li r7, 0x1
    li r8, 0x0
    bl fn_8025DE54
    li r0, 0x1
    b @L_8025EEE0
@L_8025EEDC:
    li r0, 0x0
@L_8025EEE0:
    cmpwi r0, 0x1
    bne @L_8025EEF0
    lwz r3, 0x8(r31)
    bl fn_8025E9BC
@L_8025EEF0:
    lfs f1, -20512(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    bl fn_801DAC90
    li r5, -0x1
    lis r3, 0x8048
    stwu r5, -32068(r3)
    li r0, 0x0
    addi r4, r13, -20920
    stw r5, -20920(r13)
    stw r5, 0x4(r4)
    sth r0, 0x4(r3)
    stw r0, 0x8(r3)
    bl fn_800FF660
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    bl fn_800F0308
@L_8025EF40:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
