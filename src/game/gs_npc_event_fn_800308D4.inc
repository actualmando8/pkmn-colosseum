    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r31, r4
    lwz r4, 0x64(r31)
    li r30, 0x0
    lbz r0, 0x8b(r3)
    lis r3, lbl_80266F68@ha
    clrrwi r4, r4, 8
    mr r5, r30
    or r29, r4, r0
    addi r4, r3, lbl_80266F68@l
    lha r3, 0x6(r31)
    lhz r0, 0x6(r4)
    cmpw r3, r0
    bne @L_80030928
    lhz r5, 0x0(r4)
    @L_80030928:
    addi r4, r4, 0x14
    lha r3, 0x6(r31)
    lhz r0, 0x6(r4)
    cmpw r3, r0
    bne @L_80030940
    lhz r5, 0x0(r4)
    @L_80030940:
    cmpwi r5, 0x2
    beq @L_80030974
    bge @L_80030990
    cmpwi r5, 0x1
    bge @L_80030958
    b @L_80030990
    @L_80030958:
    lwz r0, lbl_8047A424(r13)
    li r3, 0x0
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r30, r3
    b @L_80030990
    @L_80030974:
    lwz r0, lbl_8047A420(r13)
    lis r3, lbl_803A2688@ha
    addi r3, r3, lbl_803A2688@l
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r30, r3
    @L_80030990:
    cmplwi r30, 0x0
    beq @L_80030A18
    li r3, 0x2bd4
    bl fn_800FA444
    srwi r0, r3, 16
    mr r5, r29
    extsh r28, r0
    li r3, 0x0
    li r4, 0x0
    li r6, 0x2bd4
    bl fn_800FB680
    mr r3, r30
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    clrlwi r3, r3, 16
    bl fn_8011E778
    bl fn_8011E760
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    mr r3, r28
    mr r5, r29
    li r4, 0x0
    li r6, 0xe7
    bl fn_800FB680
    li r0, 0x0
    mr r3, r31
    stw r0, 0x4c(r31)
    li r4, 0x1
    bl fn_80109220
    b @L_80030A24
    @L_80030A18:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    @L_80030A24:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
