    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    lis r3, fn_800087FC@ha
    addi r31, r3, fn_800087FC@l
    @L_80008064:
    lhz r3, lbl_8047882C(r13)
    mr r5, r31
    addi r4, r1, 0x8
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80008088
    li r0, -0x1
    b @L_800080AC
    @L_80008088:
    lwz r3, lbl_80478F50(r13)
    lwz r0, 0x8(r1)
    lwz r3, 0x0(r3)
    cmplw r0, r3
    blt @L_800080A4
    subi r0, r3, 0x1
    stw r0, 0x8(r1)
    @L_800080A4:
    bl fn_8001E200
    lwz r0, 0x8(r1)
    @L_800080AC:
    cmpwi r0, 0x0
    blt @L_8000812C
    clrlwi r3, r0, 16
    clrlwi r0, r3, 16
    sth r3, lbl_8047882C(r13)
    cmplwi r0, 0x0
    beq @L_80008064
    bl fn_80006908
    cmpwi r3, 0x0
    blt @L_80008064
    bl fn_801EF63C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8000812C
    lhz r31, lbl_8047882C(r13)
    bl fn_800FF560
    lis r5, fn_8020DAD0@ha
    mr r4, r3
    addi r8, r5, fn_8020DAD0@l
    li r3, 0x1
    li r5, 0x4000
    li r6, 0x1
    li r7, 0x1
    bl fn_800F07A8
    cmplwi r3, 0x0
    beq @L_80008124
    mr r5, r31
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800F0654
    @L_80008124:
    li r3, 0x0
    b @L_80008130
    @L_8000812C:
    li r3, -0x1
    @L_80008130:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
