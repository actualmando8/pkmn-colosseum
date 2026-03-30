    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr. r28, r3
    mr r29, r4
    beq @L_801F7B5C
    bne @L_801F797C
    li r0, 0x0
    b @L_801F79F4
    @L_801F797C:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801F7994
    li r0, 0x0
    b @L_801F79F4
    @L_801F7994:
    mr r3, r28
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    cmpwi r3, 0x0
    bne @L_801F79B8
    li r0, 0x0
    b @L_801F79F4
    @L_801F79B8:
    mr r3, r28
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    bne @L_801F79DC
    li r0, 0x0
    b @L_801F79F4
    @L_801F79DC:
    bl fn_8012A130
    clrlwi. r0, r3, 24
    bne @L_801F79F0
    li r0, 0x0
    b @L_801F79F4
    @L_801F79F0:
    li r0, 0x1
    @L_801F79F4:
    clrlwi. r0, r0, 24
    beq @L_801F7B5C
    mr r3, r28
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    mr r30, r3
    li r4, 0x0
    li r3, 0x0
    b @L_801F7A2C
    @L_801F7A20:
    clrlwi r0, r4, 16
    addi r4, r4, 0x1
    stbx r3, r29, r0
    @L_801F7A2C:
    clrlwi r0, r4, 16
    cmplwi r0, 0x6
    blt @L_801F7A20
    li r31, 0x0
    b @L_801F7B50
    @L_801F7A40:
    mr r3, r30
    clrlwi r5, r31, 16
    li r4, 0x3
    bl fn_8012A5B0
    mr r27, r3
    bl fn_80123FBC
    clrlwi. r0, r3, 24
    beq @L_801F7B4C
    li r25, 0x0
    b @L_801F7AB8
    @L_801F7A68:
    mr r3, r28
    mr r6, r25
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    mr r26, r3
    bl fn_80206A04
    clrlwi. r0, r3, 24
    beq @L_801F7AB4
    mr r3, r26
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_801F7AB4
    cmplw r27, r3
    bne @L_801F7AB4
    b @L_801F7AC8
    @L_801F7AB4:
    addi r25, r25, 0x1
    @L_801F7AB8:
    clrlwi r0, r25, 16
    cmplwi r0, 0x6
    blt @L_801F7A68
    li r26, 0x0
    @L_801F7AC8:
    cmplwi r26, 0x0
    beq @L_801F7B4C
    mr r3, r26
    bl fn_80206A04
    clrlwi. r0, r3, 24
    beq @L_801F7B4C
    mr r3, r26
    li r4, 0x0
    li r5, 0xd2
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x1
    beq @L_801F7B4C
    mr r3, r26
    bl fn_80206608
    clrlwi. r0, r3, 24
    bne @L_801F7B1C
    clrlwi r0, r31, 16
    li r3, 0x3
    stbx r3, r29, r0
    b @L_801F7B4C
    @L_801F7B1C:
    mr r3, r26
    bl fn_80205BE8
    bl fn_80122DDC
    clrlwi. r0, r3, 24
    bne @L_801F7B40
    clrlwi r0, r31, 16
    li r3, 0x2
    stbx r3, r29, r0
    b @L_801F7B4C
    @L_801F7B40:
    clrlwi r0, r31, 16
    li r3, 0x1
    stbx r3, r29, r0
    @L_801F7B4C:
    addi r31, r31, 0x1
    @L_801F7B50:
    clrlwi r0, r31, 16
    cmplwi r0, 0x6
    blt @L_801F7A40
    @L_801F7B5C:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
