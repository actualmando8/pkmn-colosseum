    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    li r5, 0x0
    lis r4, lbl_802677D0@ha
    stw r5, 0x8(r1)
    addi r4, r4, lbl_802677D0@l
    li r30, 0x0
    lwz r0, 0xc(r4)
    cmpwi r0, 0x0
    beq @L_80059A60
    addi r6, r4, 0x10
    li r5, 0x1
    lwz r0, 0xc(r6)
    cmpwi r0, 0x0
    beq @L_80059A60
    addi r6, r6, 0x10
    li r5, 0x2
    lwz r0, 0xc(r6)
    cmpwi r0, 0x0
    beq @L_80059A60
    addi r6, r6, 0x10
    li r5, 0x3
    lwz r0, 0xc(r6)
    cmpwi r0, 0x0
    beq @L_80059A60
    addi r6, r6, 0x10
    li r5, 0x4
    lwz r0, 0xc(r6)
    cmpwi r0, 0x0
    beq @L_80059A60
    addi r6, r6, 0x10
    li r5, 0x5
    lwz r0, 0xc(r6)
    cmpwi r0, 0x0
    beq @L_80059A60
    addi r6, r6, 0x10
    li r5, 0x6
    lwz r0, 0xc(r6)
    cmpwi r0, 0x0
    beq @L_80059A60
    li r5, 0x7
    @L_80059A60:
    stw r5, lbl_8047A59C(r13)
    li r5, 0x0
    lwz r0, 0xc(r4)
    cmpwi r0, 0x1
    beq @L_80059AF0
    addi r4, r4, 0x10
    li r5, 0x1
    lwz r0, 0xc(r4)
    cmpwi r0, 0x1
    beq @L_80059AF0
    addi r4, r4, 0x10
    li r5, 0x2
    lwz r0, 0xc(r4)
    cmpwi r0, 0x1
    beq @L_80059AF0
    addi r4, r4, 0x10
    li r5, 0x3
    lwz r0, 0xc(r4)
    cmpwi r0, 0x1
    beq @L_80059AF0
    addi r4, r4, 0x10
    li r5, 0x4
    lwz r0, 0xc(r4)
    cmpwi r0, 0x1
    beq @L_80059AF0
    addi r4, r4, 0x10
    li r5, 0x5
    lwz r0, 0xc(r4)
    cmpwi r0, 0x1
    beq @L_80059AF0
    addi r4, r4, 0x10
    li r5, 0x6
    lwz r0, 0xc(r4)
    cmpwi r0, 0x1
    beq @L_80059AF0
    li r5, 0x7
    @L_80059AF0:
    stw r5, lbl_8047A598(r13)
    addi r29, r1, 0x8
    stw r3, 0xc(r1)
    b @L_80059B9C
    @L_80059B00:
    bl fn_801046B8
    mr r4, r3
    mr r5, r29
    addi r9, r1, 0xc
    li r3, 0x94
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    li r29, 0x0
    bne @L_80059B3C
    li r30, 0x1
    b @L_80059B9C
    @L_80059B3C:
    cmpwi r3, 0x0
    blt @L_80059B4C
    cmpwi r3, 0x7
    blt @L_80059B54
    @L_80059B4C:
    li r0, 0x2
    b @L_80059B6C
    @L_80059B54:
    lis r4, lbl_802677D0@ha
    slwi r3, r3, 4
    addi r0, r4, lbl_802677D0@l
    add r3, r0, r3
    lwz r31, 0x8(r3)
    lwz r0, 0x4(r3)
    @L_80059B6C:
    cmpwi r0, 0x1
    beq @L_80059B98
    bge @L_80059B9C
    cmpwi r0, 0x0
    bge @L_80059B84
    b @L_80059B9C
    @L_80059B84:
    mr r3, r31
    bl fn_80059034
    stw r3, 0x8(r1)
    addi r29, r1, 0x8
    b @L_80059B9C
    @L_80059B98:
    li r30, 0x1
    @L_80059B9C:
    cmpwi r30, 0x0
    beq @L_80059B00
    li r3, 0x94
    li r4, 0x2
    li r5, 0x0
    bl fn_80102568
    li r3, 0x94
    li r4, 0x1
    bl fn_80102428
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
