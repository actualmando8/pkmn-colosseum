    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    bl fn_801EF634
    mr r31, r3
    mr r3, r29
    li r4, 0x0
    li r5, 0x23
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r4, r31, 16
    clrlwi r0, r30, 16
    cmplw r4, r0
    clrlwi r3, r3, 24
    bne @L_801F39D4
    li r3, 0x0
    b @L_801F3B10
    @L_801F39D4:
    cmplwi r4, 0x1
    bne @L_801F39E4
    li r3, 0x1
    b @L_801F3B10
    @L_801F39E4:
    cmplwi r0, 0x0
    beq @L_801F39F4
    cmplwi r0, 0x1
    bne @L_801F3A04
    @L_801F39F4:
    mr r3, r30
    bl fn_801EF62C
    li r3, 0x1
    b @L_801F3B10
    @L_801F3A04:
    cmplwi r3, 0x0
    bne @L_801F3A54
    cmplwi r4, 0x3
    beq @L_801F3A1C
    cmplwi r4, 0x5
    bne @L_801F3A24
    @L_801F3A1C:
    li r3, 0x0
    b @L_801F3B10
    @L_801F3A24:
    cmplwi r0, 0x7
    bne @L_801F3A3C
    li r3, 0x3
    bl fn_801EF62C
    li r3, 0x1
    b @L_801F3B10
    @L_801F3A3C:
    cmplwi r0, 0x6
    bne @L_801F3B04
    li r3, 0x5
    bl fn_801EF62C
    li r3, 0x1
    b @L_801F3B10
    @L_801F3A54:
    cmplwi r4, 0x7
    beq @L_801F3A64
    cmplwi r4, 0x6
    bne @L_801F3A6C
    @L_801F3A64:
    li r3, 0x0
    b @L_801F3B10
    @L_801F3A6C:
    cmplwi r4, 0x3
    bne @L_801F3A8C
    cmplwi r0, 0x2
    bne @L_801F3A8C
    li r3, 0x7
    bl fn_801EF62C
    li r3, 0x1
    b @L_801F3B10
    @L_801F3A8C:
    clrlwi r0, r31, 16
    cmplwi r0, 0x2
    bne @L_801F3AB4
    clrlwi r0, r30, 16
    cmplwi r0, 0x3
    bne @L_801F3AB4
    li r3, 0x7
    bl fn_801EF62C
    li r3, 0x1
    b @L_801F3B10
    @L_801F3AB4:
    clrlwi r0, r31, 16
    cmplwi r0, 0x4
    bne @L_801F3ADC
    clrlwi r0, r30, 16
    cmplwi r0, 0x5
    bne @L_801F3ADC
    li r3, 0x6
    bl fn_801EF62C
    li r3, 0x1
    b @L_801F3B10
    @L_801F3ADC:
    clrlwi r0, r31, 16
    cmplwi r0, 0x5
    bne @L_801F3B04
    clrlwi r0, r30, 16
    cmplwi r0, 0x4
    bne @L_801F3B04
    li r3, 0x6
    bl fn_801EF62C
    li r3, 0x1
    b @L_801F3B10
    @L_801F3B04:
    mr r3, r30
    bl fn_801EF62C
    li r3, 0x1
    @L_801F3B10:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
