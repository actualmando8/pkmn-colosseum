    nofralloc
    stwu r1, -0x150(r1)
    mflr r0
    stw r0, 0x154(r1)
    stmw r28, 0x140(r1)
    mr. r31, r5
    mr r30, r3
    mr r28, r4
    beq @L_801F9F64
    mr r4, r30
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi. r0, r30, 16
    clrlwi r3, r3, 16
    bne @L_801F9D04
    li r0, 0x0
    b @L_801F9D20
    @L_801F9D04:
    cmplwi r3, 0x0
    bne @L_801F9D1C
    cmpwi r28, 0x0
    bne @L_801F9D1C
    li r0, 0x0
    b @L_801F9D20
    @L_801F9D1C:
    li r0, 0x1
    @L_801F9D20:
    clrlwi. r0, r0, 24
    beq @L_801F9F64
    mr r4, r30
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801F9D50
    li r0, 0x0
    b @L_801F9D6C
    @L_801F9D50:
    cmplwi r0, 0x2
    beq @L_801F9D60
    cmplwi r0, 0x3
    bne @L_801F9D68
    @L_801F9D60:
    li r0, 0x1
    b @L_801F9D6C
    @L_801F9D68:
    li r0, 0x2
    @L_801F9D6C:
    clrlwi. r0, r0, 24
    bne @L_801F9D94
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    mr. r4, r3
    beq @L_801F9F64
    mr r3, r31
    bl fn_8012AC64
    b @L_801F9F64
    @L_801F9D94:
    mr r4, r30
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801FB1C0
    mr r4, r30
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r29, r3, 24
    mr r4, r30
    li r3, 0x0
    li r5, 0x3
    li r6, 0x0
    bl fn_801FB1C0
    bl fn_800FA280
    mr r28, r3
    bl fn_800896B8
    clrlwi r0, r30, 16
    cmplw r0, r3
    bne @L_801F9DF4
    bl fn_800896C8
    mr r28, r3
    @L_801F9DF4:
    mr r3, r31
    mr r4, r28
    mr r5, r29
    bl fn_8012A1A4
    mr r4, r30
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r29, r3, 16
    @L_801F9E1C:
    mr r3, r31
    li r4, 0x2
    li r5, 0x0
    bl fn_8012A5B0
    mr r5, r3
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_801F9F78
    clrlwi. r0, r3, 24
    beq @L_801F9E98
    addi r3, r1, 0x8
    clrlwi r7, r29, 16
    li r4, 0x0
    li r5, 0xc9
    li r6, 0x0
    bl fn_801254B4
    mr r4, r29
    li r3, 0x0
    li r5, 0x12
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r6, r3, 16
    mr r3, r31
    addi r4, r1, 0x8
    li r5, 0x0
    li r7, 0x0
    bl fn_80129F20
    extsh. r0, r3
    blt @L_801F9E98
    addi r29, r29, 0x1
    b @L_801F9E1C
    @L_801F9E98:
    li r29, 0x0
    b @L_801F9ED8
    @L_801F9EA0:
    mr r4, r30
    mr r6, r29
    li r3, 0x0
    li r5, 0x6
    bl fn_801FB1C0
    clrlwi. r4, r3, 16
    beq @L_801F9ED4
    mr r3, r31
    li r5, 0x1
    li r6, -0x1
    bl fn_80129A78
    cmpwi r3, 0x0
    blt @L_801F9EE4
    @L_801F9ED4:
    addi r29, r29, 0x1
    @L_801F9ED8:
    clrlwi r0, r29, 16
    cmplwi r0, 0x8
    blt @L_801F9EA0
    @L_801F9EE4:
    mr r3, r31
    li r4, 0xf
    li r5, 0x0
    bl fn_8012A450
    mr r3, r31
    li r4, 0x10
    li r5, 0x0
    bl fn_8012A450
    mr r3, r31
    li r4, 0x11
    li r5, 0x0
    bl fn_8012A450
    mr r3, r31
    li r4, 0x12
    li r5, 0x0
    bl fn_8012A450
    mr r3, r31
    li r4, 0x13
    li r5, 0x0
    bl fn_8012A450
    mr r3, r31
    li r4, 0x14
    li r5, 0x0
    bl fn_8012A450
    mr r3, r31
    li r4, 0x15
    li r5, 0x0
    bl fn_8012A450
    mr r3, r31
    li r4, 0x16
    li r5, 0x0
    bl fn_8012A450
    @L_801F9F64:
    lmw r28, 0x140(r1)
    lwz r0, 0x154(r1)
    mtlr r0
    addi r1, r1, 0x150
    blr
