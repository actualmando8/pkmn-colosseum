    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r31, 0x60(r29)
    lwz r0, 0x0(r31)
    cmpwi r0, 0x2
    bne @L_80027914
    lwz r0, 0x4(r31)
    li r3, 0x0
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r30, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80027904
    mr r3, r30
    bl fn_8011F5C8
    b @L_80027908
    @L_80027904:
    li r3, 0x1
    @L_80027908:
    clrlwi r4, r3, 16
    li r3, 0x4e
    bl fn_80132A38
    @L_80027914:
    lwz r0, 0x0(r31)
    lis r3, lbl_80266DD8@ha
    addi r3, r3, lbl_80266DD8@l
    lbz r5, 0x8b(r29)
    slwi r0, r0, 4
    li r4, -0x100
    lwzx r6, r3, r0
    or r5, r5, r4
    li r3, 0x0
    li r4, 0x0
    bl fn_800FB680
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
