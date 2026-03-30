    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stmw r27, 0x7c(r1)
    clrlwi r30, r3, 16
    cmplwi r30, 0x0
    bne @L_8000692C
    li r3, 0x1
    b @L_80006F98
    @L_8000692C:
    lwz r4, lbl_80478F50(r13)
    lwz r0, 0x0(r4)
    cmplw r30, r0
    blt @L_80006944
    li r3, 0x1
    b @L_80006F98
    @L_80006944:
    bl fn_8020E0F8
    mr r31, r3
    lwz r27, 0x0(r31)
    lwz r28, 0x4(r31)
    lwz r29, 0x8(r31)
    lwz r12, 0xc(r31)
    lwz r11, 0x10(r31)
    lwz r10, 0x14(r31)
    lwz r9, 0x18(r31)
    lwz r8, 0x1c(r31)
    lwz r7, 0x20(r31)
    lwz r6, 0x24(r31)
    lwz r5, 0x28(r31)
    lwz r4, 0x2c(r31)
    lwz r3, 0x30(r31)
    lwz r0, 0x34(r31)
    stw r27, 0x34(r1)
    stw r28, 0x38(r1)
    stw r29, 0x3c(r1)
    stw r12, 0x40(r1)
    stw r11, 0x44(r1)
    stw r10, 0x48(r1)
    stw r9, 0x4c(r1)
    stw r8, 0x50(r1)
    stw r7, 0x54(r1)
    stw r6, 0x58(r1)
    stw r5, 0x5c(r1)
    stw r4, 0x60(r1)
    stw r3, 0x64(r1)
    stw r0, 0x68(r1)
    @L_800069BC:
    mr r9, r30
    li r3, 0xcd
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    bne @L_80006A70
    li r3, 0xcd
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lwz r4, 0x34(r1)
    li r3, -0x1
    lwz r0, 0x38(r1)
    stw r4, 0x0(r31)
    lwz r4, 0x3c(r1)
    stw r0, 0x4(r31)
    lwz r0, 0x40(r1)
    stw r4, 0x8(r31)
    lwz r4, 0x44(r1)
    stw r0, 0xc(r31)
    lwz r0, 0x48(r1)
    stw r4, 0x10(r31)
    lwz r4, 0x4c(r1)
    stw r0, 0x14(r31)
    lwz r0, 0x50(r1)
    stw r4, 0x18(r31)
    lwz r4, 0x54(r1)
    stw r0, 0x1c(r31)
    lwz r0, 0x58(r1)
    stw r4, 0x20(r31)
    lwz r4, 0x5c(r1)
    stw r0, 0x24(r31)
    lwz r0, 0x60(r1)
    stw r4, 0x28(r31)
    lwz r4, 0x64(r1)
    stw r0, 0x2c(r31)
    lwz r0, 0x68(r1)
    stw r4, 0x30(r31)
    stw r0, 0x34(r31)
    b @L_80006F98
    @L_80006A70:
    cmpwi r3, -0x2
    bne @L_80006AB4
    li r3, 0x44
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    beq @L_80006AA0
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_800069BC
    @L_80006AA0:
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_80006F84
    @L_80006AB4:
    li r3, 0xcd
    bl fn_801022B8
    subi r0, r3, 0xd3f
    cmplwi r0, 0x6
    bgt @L_800069BC
    lis r3, jumptable_802E28D0@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_802E28D0@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r31
    bl fn_8020E0E0
    lis r4, fn_80008744@ha
    clrlwi r3, r3, 24
    addi r5, r4, fn_80008744@l
    addi r4, r1, 0x30
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80006B10
    li r0, -0x1
    b @L_80006B34
    @L_80006B10:
    lwz r3, lbl_80478F40(r13)
    lwz r0, 0x30(r1)
    lwz r3, 0x0(r3)
    cmplw r0, r3
    blt @L_80006B2C
    subi r0, r3, 0x1
    stw r0, 0x30(r1)
    @L_80006B2C:
    bl fn_8001E200
    lwz r0, 0x30(r1)
    @L_80006B34:
    cmpwi r0, 0x0
    blt @L_800069BC
    mr r3, r31
    clrlwi r4, r0, 24
    bl fn_8020DFB0
    b @L_800069BC
    mr r3, r31
    bl fn_8020E0C8
    lis r4, fn_800086EC@ha
    clrlwi r3, r3, 24
    addi r5, r4, fn_800086EC@l
    addi r4, r1, 0x2c
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80006B7C
    li r0, -0x1
    b @L_80006BA0
    @L_80006B7C:
    lwz r3, lbl_80478F00(r13)
    lwz r0, 0x2c(r1)
    lwz r3, 0x0(r3)
    cmplw r0, r3
    blt @L_80006B98
    subi r0, r3, 0x1
    stw r0, 0x2c(r1)
    @L_80006B98:
    bl fn_8001E200
    lwz r0, 0x2c(r1)
    @L_80006BA0:
    cmpwi r0, 0x0
    blt @L_800069BC
    mr r3, r31
    clrlwi r4, r0, 24
    bl fn_8020DFA0
    b @L_800069BC
    mr r3, r31
    bl fn_8020E0B0
    lis r4, fn_8000879C@ha
    clrlwi r3, r3, 16
    addi r5, r4, fn_8000879C@l
    addi r4, r1, 0x28
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80006BE8
    li r0, -0x1
    b @L_80006C0C
    @L_80006BE8:
    lwz r3, lbl_80478F48(r13)
    lwz r0, 0x28(r1)
    lwz r3, 0x0(r3)
    cmplw r0, r3
    blt @L_80006C04
    subi r0, r3, 0x1
    stw r0, 0x28(r1)
    @L_80006C04:
    bl fn_8001E200
    lwz r0, 0x28(r1)
    @L_80006C0C:
    cmpwi r0, 0x0
    blt @L_800069BC
    mr r3, r31
    clrlwi r4, r0, 16
    bl fn_8020DF90
    b @L_800069BC
    mr r3, r31
    li r4, 0x0
    bl fn_8020E068
    lis r4, fn_8000868C@ha
    clrlwi r3, r3, 16
    addi r5, r4, fn_8000868C@l
    addi r4, r1, 0x24
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80006C58
    li r0, -0x1
    b @L_80006C7C
    @L_80006C58:
    lwz r3, lbl_80478F20(r13)
    lwz r0, 0x24(r1)
    lwz r3, 0x0(r3)
    cmplw r0, r3
    blt @L_80006C74
    subi r0, r3, 0x1
    stw r0, 0x24(r1)
    @L_80006C74:
    bl fn_8001E200
    lwz r0, 0x24(r1)
    @L_80006C7C:
    cmpwi r0, 0x0
    blt @L_800069BC
    mr r3, r31
    clrlwi r5, r0, 16
    li r4, 0x0
    bl fn_8020DF50
    mr r3, r31
    li r4, 0x0
    bl fn_8020E020
    lis r4, fn_800085D8@ha
    clrlwi r3, r3, 16
    addi r5, r4, fn_800085D8@l
    addi r4, r1, 0x20
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80006CC8
    li r5, -0x1
    b @L_80006CE4
    @L_80006CC8:
    lwz r0, 0x20(r1)
    cmplwi r0, 0x5
    blt @L_80006CDC
    li r0, 0x4
    stw r0, 0x20(r1)
    @L_80006CDC:
    bl fn_8001E200
    lwz r5, 0x20(r1)
    @L_80006CE4:
    cmpwi r5, 0x0
    blt @L_800069BC
    mr r3, r31
    li r4, 0x0
    bl fn_8020DF10
    b @L_800069BC
    mr r3, r31
    li r4, 0x1
    bl fn_8020E068
    lis r4, fn_8000868C@ha
    clrlwi r3, r3, 16
    addi r5, r4, fn_8000868C@l
    addi r4, r1, 0x1c
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80006D30
    li r0, -0x1
    b @L_80006D54
    @L_80006D30:
    lwz r3, lbl_80478F20(r13)
    lwz r0, 0x1c(r1)
    lwz r3, 0x0(r3)
    cmplw r0, r3
    blt @L_80006D4C
    subi r0, r3, 0x1
    stw r0, 0x1c(r1)
    @L_80006D4C:
    bl fn_8001E200
    lwz r0, 0x1c(r1)
    @L_80006D54:
    cmpwi r0, 0x0
    blt @L_800069BC
    mr r3, r31
    clrlwi r5, r0, 16
    li r4, 0x1
    bl fn_8020DF50
    mr r3, r31
    li r4, 0x1
    bl fn_8020E020
    lis r4, fn_800085D8@ha
    clrlwi r3, r3, 16
    addi r5, r4, fn_800085D8@l
    addi r4, r1, 0x18
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80006DA0
    li r5, -0x1
    b @L_80006DBC
    @L_80006DA0:
    lwz r0, 0x18(r1)
    cmplwi r0, 0x5
    blt @L_80006DB4
    li r0, 0x4
    stw r0, 0x18(r1)
    @L_80006DB4:
    bl fn_8001E200
    lwz r5, 0x18(r1)
    @L_80006DBC:
    cmpwi r5, 0x0
    blt @L_800069BC
    mr r3, r31
    li r4, 0x1
    bl fn_8020DF10
    b @L_800069BC
    mr r3, r31
    li r4, 0x2
    bl fn_8020E068
    lis r4, fn_8000868C@ha
    clrlwi r3, r3, 16
    addi r5, r4, fn_8000868C@l
    addi r4, r1, 0x14
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80006E08
    li r0, -0x1
    b @L_80006E2C
    @L_80006E08:
    lwz r3, lbl_80478F20(r13)
    lwz r0, 0x14(r1)
    lwz r3, 0x0(r3)
    cmplw r0, r3
    blt @L_80006E24
    subi r0, r3, 0x1
    stw r0, 0x14(r1)
    @L_80006E24:
    bl fn_8001E200
    lwz r0, 0x14(r1)
    @L_80006E2C:
    cmpwi r0, 0x0
    blt @L_800069BC
    mr r3, r31
    clrlwi r5, r0, 16
    li r4, 0x2
    bl fn_8020DF50
    mr r3, r31
    li r4, 0x2
    bl fn_8020E020
    lis r4, fn_800085D8@ha
    clrlwi r3, r3, 16
    addi r5, r4, fn_800085D8@l
    addi r4, r1, 0x10
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80006E78
    li r5, -0x1
    b @L_80006E94
    @L_80006E78:
    lwz r0, 0x10(r1)
    cmplwi r0, 0x5
    blt @L_80006E8C
    li r0, 0x4
    stw r0, 0x10(r1)
    @L_80006E8C:
    bl fn_8001E200
    lwz r5, 0x10(r1)
    @L_80006E94:
    cmpwi r5, 0x0
    blt @L_800069BC
    mr r3, r31
    li r4, 0x2
    bl fn_8020DF10
    b @L_800069BC
    mr r3, r31
    li r4, 0x3
    bl fn_8020E068
    lis r4, fn_8000868C@ha
    clrlwi r3, r3, 16
    addi r5, r4, fn_8000868C@l
    addi r4, r1, 0xc
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80006EE0
    li r0, -0x1
    b @L_80006F04
    @L_80006EE0:
    lwz r3, lbl_80478F20(r13)
    lwz r0, 0xc(r1)
    lwz r3, 0x0(r3)
    cmplw r0, r3
    blt @L_80006EFC
    subi r0, r3, 0x1
    stw r0, 0xc(r1)
    @L_80006EFC:
    bl fn_8001E200
    lwz r0, 0xc(r1)
    @L_80006F04:
    cmpwi r0, 0x0
    blt @L_800069BC
    mr r3, r31
    clrlwi r5, r0, 16
    li r4, 0x3
    bl fn_8020DF50
    mr r3, r31
    li r4, 0x3
    bl fn_8020E020
    lis r4, fn_800085D8@ha
    clrlwi r3, r3, 16
    addi r5, r4, fn_800085D8@l
    addi r4, r1, 0x8
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80006F50
    li r5, -0x1
    b @L_80006F6C
    @L_80006F50:
    lwz r0, 0x8(r1)
    cmplwi r0, 0x5
    blt @L_80006F64
    li r0, 0x4
    stw r0, 0x8(r1)
    @L_80006F64:
    bl fn_8001E200
    lwz r5, 0x8(r1)
    @L_80006F6C:
    cmpwi r5, 0x0
    blt @L_800069BC
    mr r3, r31
    li r4, 0x3
    bl fn_8020DF10
    b @L_800069BC
    @L_80006F84:
    li r3, 0xcd
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1
    @L_80006F98:
    lmw r27, 0x7c(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
