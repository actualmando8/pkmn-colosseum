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
    @L_80031680:
    lha r3, 0x6(r31)
    lhz r0, 0xa(r4)
    cmpw r3, r0
    bne @L_80031698
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_80031698:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0xa(r4)
    cmpw r3, r0
    bne @L_800316B4
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_800316B4:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0xa(r4)
    cmpw r3, r0
    bne @L_800316D0
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_800316D0:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0xa(r4)
    cmpw r3, r0
    bne @L_800316EC
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_800316EC:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0xa(r4)
    cmpw r3, r0
    bne @L_80031708
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_80031708:
    addi r4, r4, 0x12
    lha r3, 0x6(r31)
    lhz r0, 0xa(r4)
    cmpw r3, r0
    bne @L_80031724
    lbz r6, 0x0(r4)
    lbz r7, 0x1(r4)
    @L_80031724:
    addi r4, r4, 0x12
    addi r5, r5, 0x5
    bdnz @L_80031680
    cmpwi r6, 0x2
    beq @L_80031760
    bge @L_80031778
    cmpwi r6, 0x1
    bge @L_80031748
    b @L_80031778
    @L_80031748:
    clrlwi r5, r7, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r30, r3
    b @L_80031778
    @L_80031760:
    lis r3, lbl_803A2688@ha
    clrlwi r5, r7, 16
    addi r3, r3, lbl_803A2688@l
    li r4, 0x3
    bl fn_8012A5B0
    mr r30, r3
    @L_80031778:
    mr r3, r30
    bl fn_8011E850
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800317F4
    li r3, 0x56c
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    li r3, 0xe7
    bl fn_800FA444
    srwi r0, r3, 16
    lha r3, 0x54(r31)
    extsh r6, r0
    li r4, 0x0
    srwi r5, r6, 31
    srwi r0, r3, 31
    add r6, r5, r6
    srawi r7, r6, 1
    add r0, r0, r3
    srawi r0, r0, 1
    li r5, -0x1
    subf r0, r7, r0
    li r6, 0xe7
    extsh r3, r0
    bl fn_800FB680
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_800318C0
    @L_800317F4:
    mr r3, r30
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800318B4
    mr r3, r30
    bl fn_80075FEC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_800318B4
    mr r3, r30
    bl fn_8011E8DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80031848
    li r3, 0x56b
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    b @L_80031860
    @L_80031848:
    mr r3, r30
    bl fn_8011F4F0
    mr r0, r3
    li r3, 0x37
    mr r4, r0
    bl fn_80132A38
    @L_80031860:
    li r3, 0xe7
    bl fn_800FA444
    srwi r0, r3, 16
    lha r3, 0x54(r31)
    extsh r6, r0
    li r4, 0x0
    srwi r5, r6, 31
    srwi r0, r3, 31
    add r6, r5, r6
    srawi r7, r6, 1
    add r0, r0, r3
    srawi r0, r0, 1
    li r5, -0x1
    subf r0, r7, r0
    li r6, 0xe7
    extsh r3, r0
    bl fn_800FB680
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_800318C0
    @L_800318B4:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    @L_800318C0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
