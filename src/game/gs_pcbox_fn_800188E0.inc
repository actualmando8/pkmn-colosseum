    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r5
    mr r30, r6
    mr r31, r7
    li r0, 0x0
    cmpwi r28, 0x3
    sth r0, 0x8(r1)
    beq @L_80018A14
    bge @L_80018934
    cmpwi r28, 0x2
    bge @L_800189AC
    cmpwi r28, 0x0
    bge @L_80018940
    b @L_80018A40
    @L_80018934:
    cmpwi r28, 0x5
    bge @L_80018A40
    b @L_80018A2C
    @L_80018940:
    mr r3, r4
    mr r4, r29
    mr r5, r30
    addi r6, r1, 0x8
    bl fn_80018594
    cmpwi r3, 0x2
    bne @L_80018964
    li r3, 0x2
    b @L_80018A40
    @L_80018964:
    cmpwi r3, 0x3
    bne @L_80018974
    li r3, 0x3
    b @L_80018A40
    @L_80018974:
    cmpwi r3, 0x4
    bne @L_80018984
    li r3, 0x1
    b @L_80018A40
    @L_80018984:
    cmpwi r28, 0x0
    bne @L_80018994
    li r3, 0x0
    b @L_80018A40
    @L_80018994:
    cmpwi r3, 0x0
    bne @L_800189A4
    li r3, 0x1
    b @L_80018A40
    @L_800189A4:
    li r3, 0x0
    b @L_80018A40
    @L_800189AC:
    mr r3, r29
    bl fn_801440A0
    bl fn_80143FCC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800189F4
    clrlwi r4, r29, 16
    li r3, 0x2d
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x4262
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x0
    b @L_800189FC
    @L_800189F4:
    stw r30, lbl_8047A2B8(r13)
    li r0, 0x1
    @L_800189FC:
    cmpwi r0, 0x0
    beq @L_80018A0C
    li r3, 0x1
    b @L_80018A40
    @L_80018A0C:
    li r3, 0x0
    b @L_80018A40
    @L_80018A14:
    mr r3, r4
    mr r4, r29
    mr r5, r30
    bl fn_800181C4
    li r3, 0x0
    b @L_80018A40
    @L_80018A2C:
    mr r3, r4
    mr r4, r29
    mr r5, r30
    bl fn_80017E8C
    li r3, 0x0
    @L_80018A40:
    lhz r0, 0x8(r1)
    sth r0, 0x0(r31)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
