    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r30, r4
    lwz r5, 0x64(r30)
    lis r4, lbl_80266E90@ha
    lbz r0, 0x8b(r3)
    addi r4, r4, lbl_80266E90@l
    clrrwi r3, r5, 8
    li r29, 0x0
    li r6, 0x0
    li r7, 0x0
    or r31, r3, r0
    li r5, 0x0
    li r0, 0x2
    mtctr r0
    @L_80031454:
    lha r3, 0x6(r30)
    lhz r0, 0xc(r4)
    cmpw r3, r0
    bne @L_8003146C
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_8003146C:
    addi r4, r4, 0x12
    lha r3, 0x6(r30)
    lhz r0, 0xc(r4)
    cmpw r3, r0
    bne @L_80031488
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_80031488:
    addi r4, r4, 0x12
    lha r3, 0x6(r30)
    lhz r0, 0xc(r4)
    cmpw r3, r0
    bne @L_800314A4
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_800314A4:
    addi r4, r4, 0x12
    lha r3, 0x6(r30)
    lhz r0, 0xc(r4)
    cmpw r3, r0
    bne @L_800314C0
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_800314C0:
    addi r4, r4, 0x12
    lha r3, 0x6(r30)
    lhz r0, 0xc(r4)
    cmpw r3, r0
    bne @L_800314DC
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_800314DC:
    addi r4, r4, 0x12
    lha r3, 0x6(r30)
    lhz r0, 0xc(r4)
    cmpw r3, r0
    bne @L_800314F8
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_800314F8:
    addi r4, r4, 0x12
    addi r5, r5, 0x5
    bdnz @L_80031454
    cmpwi r6, 0x2
    beq @L_80031534
    bge @L_8003154C
    cmpwi r6, 0x1
    bge @L_8003151C
    b @L_8003154C
    @L_8003151C:
    clrlwi r5, r7, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r29, r3
    b @L_8003154C
    @L_80031534:
    lis r3, lbl_803A2688@ha
    clrlwi r5, r7, 16
    addi r3, r3, lbl_803A2688@l
    li r4, 0x3
    bl fn_8012A5B0
    mr r29, r3
    @L_8003154C:
    mr r3, r29
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003161C
    mr r3, r29
    bl fn_80075FEC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8003161C
    mr r3, r29
    bl fn_8001DA60
    clrlwi r0, r3, 24
    cmpwi r0, 0x1
    beq @L_800315A8
    bge @L_80031598
    cmpwi r0, 0x0
    bge @L_800315A0
    b @L_800315B0
    @L_80031598:
    cmpwi r0, 0x3
    b @L_800315B0
    @L_800315A0:
    li r28, 0xd67
    b @L_800315B4
    @L_800315A8:
    li r28, 0xd68
    b @L_800315B4
    @L_800315B0:
    li r28, 0x0
    @L_800315B4:
    mr r3, r29
    bl fn_8011E8DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_800315CC
    li r28, 0x0
    @L_800315CC:
    cmplwi r28, 0x0
    beq @L_8003160C
    mr r3, r28
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    mr r5, r31
    li r3, 0x2
    li r4, 0x0
    li r6, 0xcf
    bl fn_800FB680
    mr r3, r30
    li r4, 0x1
    bl fn_80109220
    b @L_80031628
    @L_8003160C:
    mr r3, r30
    li r4, 0x0
    bl fn_80109220
    b @L_80031628
    @L_8003161C:
    mr r3, r30
    li r4, 0x0
    bl fn_80109220
    @L_80031628:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
