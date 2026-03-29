    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r31, r4
    lis r3, lbl_80266E90@ha
    li r30, 0x0
    addi r4, r3, lbl_80266E90@l
    li r6, 0x0
    li r7, 0x0
    li r5, 0x0
    li r0, 0x2
    mtctr r0
    @L_80030D6C:
    lha r3, 0x6(r31)
    lhz r0, 0x8(r4)
    cmpw r3, r0
    bne @L_80030D84
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_80030D84:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0x8(r4)
    cmpw r3, r0
    bne @L_80030DA0
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_80030DA0:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0x8(r4)
    cmpw r3, r0
    bne @L_80030DBC
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_80030DBC:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0x8(r4)
    cmpw r3, r0
    bne @L_80030DD8
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_80030DD8:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0x8(r4)
    cmpw r3, r0
    bne @L_80030DF4
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_80030DF4:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0x8(r4)
    cmpw r3, r0
    bne @L_80030E10
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_80030E10:
    addi r4, r4, 0x12
    addi r5, r5, 0x5
    bdnz @L_80030D6C
    cmpwi r6, 0x2
    beq @L_80030E4C
    bge @L_80030E64
    cmpwi r6, 0x1
    bge @L_80030E34
    b @L_80030E64
    @L_80030E34:
    clrlwi r5, r7, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r30, r3
    b @L_80030E64
    @L_80030E4C:
    lis r3, lbl_803A2688@ha
    clrlwi r5, r7, 16
    addi r3, r3, lbl_803A2688@l
    li r4, 0x3
    bl fn_8012A5B0
    mr r30, r3
    @L_80030E64:
    mr r3, r30
    bl fn_8011E850
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80030E9C
    mr r3, r30
    bl fn_8011F1A0
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_80030E9C
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_80030EF4
    @L_80030E9C:
    mr r3, r30
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80030EE8
    mr r3, r30
    bl fn_80075FEC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80030EE8
    mr r3, r30
    bl fn_8011F1A0
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_80030EE8
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_80030EF4
    @L_80030EE8:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    @L_80030EF4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
