    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr. r29, r3
    mr r30, r4
    bne @L_801F8A3C
    li r3, 0x0
    b @L_801F8BEC
    @L_801F8A3C:
    cmplwi r30, 0x0
    bne @L_801F8BC8
    li r3, 0x0
    b @L_801F8BEC
    b @L_801F8BC8
    @L_801F8A50:
    mr r3, r29
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    mr r31, r3
    bl fn_80206608
    clrlwi. r0, r3, 24
    bne @L_801F8A78
    li r0, 0x1
    b @L_801F8BB4
    @L_801F8A78:
    cmplwi r29, 0x0
    bne @L_801F8A88
    li r27, 0x0
    b @L_801F8B00
    @L_801F8A88:
    cmplwi r31, 0x0
    bne @L_801F8A98
    li r27, 0x0
    b @L_801F8B00
    @L_801F8A98:
    li r28, 0x0
    b @L_801F8AF0
    @L_801F8AA0:
    mr r3, r29
    mr r6, r28
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr r27, r3
    bl fn_80206780
    clrlwi. r0, r3, 24
    bne @L_801F8AC8
    li r27, 0x0
    @L_801F8AC8:
    cmplwi r27, 0x0
    beq @L_801F8AEC
    mr r3, r31
    mr r4, r27
    bl fn_80204928
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F8AEC
    b @L_801F8B00
    @L_801F8AEC:
    addi r28, r28, 0x1
    @L_801F8AF0:
    clrlwi r0, r28, 16
    cmplwi r0, 0x2
    blt @L_801F8AA0
    li r27, 0x0
    @L_801F8B00:
    cmplwi r27, 0x0
    beq @L_801F8B10
    li r0, 0x2
    b @L_801F8BB4
    @L_801F8B10:
    cmplwi r29, 0x0
    bne @L_801F8B20
    li r0, 0x0
    b @L_801F8B9C
    @L_801F8B20:
    cmplwi r31, 0x0
    bne @L_801F8B30
    li r0, 0x0
    b @L_801F8B9C
    @L_801F8B30:
    li r28, 0x0
    b @L_801F8B8C
    @L_801F8B38:
    mr r3, r29
    mr r6, r28
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr r27, r3
    bl fn_80206780
    clrlwi. r0, r3, 24
    bne @L_801F8B60
    li r27, 0x0
    @L_801F8B60:
    cmplwi r27, 0x0
    beq @L_801F8B88
    mr r3, r27
    mr r4, r31
    bl fn_80204854
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F8B88
    li r0, 0x1
    b @L_801F8B9C
    @L_801F8B88:
    addi r28, r28, 0x1
    @L_801F8B8C:
    clrlwi r0, r28, 16
    cmplwi r0, 0x2
    blt @L_801F8B38
    li r0, 0x0
    @L_801F8B9C:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_801F8BB0
    li r0, 0x3
    b @L_801F8BB4
    @L_801F8BB0:
    li r0, 0x0
    @L_801F8BB4:
    clrlwi. r0, r0, 24
    beq @L_801F8BD4
    lhz r3, 0x0(r30)
    addi r0, r3, 0x1
    sth r0, 0x0(r30)
    @L_801F8BC8:
    lhz r6, 0x0(r30)
    cmplwi r6, 0x6
    blt @L_801F8A50
    @L_801F8BD4:
    lhz r0, 0x0(r30)
    cmplwi r0, 0x6
    blt @L_801F8BE8
    li r3, 0x0
    b @L_801F8BEC
    @L_801F8BE8:
    mr r3, r31
    @L_801F8BEC:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
