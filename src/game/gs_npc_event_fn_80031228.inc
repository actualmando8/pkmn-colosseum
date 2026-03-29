    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r30, r3
    mr r31, r4
    lis r3, lbl_80266E90@ha
    li r29, 0x0
    addi r4, r3, lbl_80266E90@l
    li r6, 0x0
    li r7, 0x0
    li r5, 0x0
    li r0, 0x2
    mtctr r0
    @L_80031268:
    lha r3, 0x6(r31)
    lhz r0, 0x2(r4)
    cmpw r3, r0
    bne @L_80031280
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_80031280:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0x2(r4)
    cmpw r3, r0
    bne @L_8003129C
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_8003129C:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0x2(r4)
    cmpw r3, r0
    bne @L_800312B8
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_800312B8:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0x2(r4)
    cmpw r3, r0
    bne @L_800312D4
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_800312D4:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0x2(r4)
    cmpw r3, r0
    bne @L_800312F0
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_800312F0:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0x2(r4)
    cmpw r3, r0
    bne @L_8003130C
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_8003130C:
    addi r4, r4, 0x12
    addi r5, r5, 0x5
    bdnz @L_80031268
    cmpwi r6, 0x2
    beq @L_80031348
    bge @L_80031360
    cmpwi r6, 0x1
    bge @L_80031330
    b @L_80031360
    @L_80031330:
    clrlwi r5, r7, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r29, r3
    b @L_80031360
    @L_80031348:
    lis r3, lbl_803A2688@ha
    clrlwi r5, r7, 16
    addi r3, r3, lbl_803A2688@l
    li r4, 0x3
    bl fn_8012A5B0
    mr r29, r3
    @L_80031360:
    mr r3, r29
    bl fn_8011E850
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80031394
    mr r3, r30
    mr r4, r31
    mr r5, r29
    bl fn_8010B718
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_800313E8
    @L_80031394:
    mr r3, r29
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800313DC
    mr r3, r29
    bl fn_80075FEC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_800313DC
    mr r3, r30
    mr r4, r31
    mr r5, r29
    bl fn_8010B718
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_800313E8
    @L_800313DC:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    @L_800313E8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
