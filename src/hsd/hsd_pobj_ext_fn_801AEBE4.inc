    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r5, -32729
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r29, r3
    addi r31, r5, 20840
    mr r30, r4
    b @L_801AED64
@L_801AEC08:
    cmplwi r29, 0x0
    beq @L_801AED5C
    cmplwi r30, 0x0
    beq @L_801AED5C
    lwz r3, 0x4(r29)
    lis r0, 0x1000
    rlwinm r3, r3, 0, 1, 3
    cmpw r3, r0
    beq @L_801AEC3C
    bge @L_801AED5C
    cmpwi r3, 0x0
    beq @L_801AECAC
    b @L_801AED5C
@L_801AEC3C:
    lwz r3, 0x8(r29)
    bl fn_801A053C
    lwz r4, 0x8(r30)
    li r3, 0x0
    li r5, 0x0
    bl fn_8019C128
    stw r3, 0x8(r29)
    lwz r0, 0x8(r29)
    cmplwi r0, 0x0
    bne @L_801AEC74
    addi r5, r31, 0x13c
    addi r3, r2, -22864
    li r4, 0x330
    bl fn_80196E10
@L_801AEC74:
    lwz r4, 0x8(r29)
    cmplwi r4, 0x0
    beq @L_801AED5C
    lhz r3, 0x6(r4)
    addi r0, r3, 0x1
    sth r0, 0x6(r4)
    lhz r0, 0x6(r4)
    cmplwi r0, 0x0
    bne @L_801AED5C
    addi r3, r31, 0x24
    addi r5, r31, 0x30
    li r4, 0x9e
    bl fn_80196E10
    b @L_801AED5C
@L_801AECAC:
    lwz r3, 0x8(r30)
    lwz r28, 0xc(r29)
    lwz r27, 0x4(r3)
    cmplwi r27, 0x0
    bne @L_801AED48
    b @L_801AED5C
@L_801AECC4:
    cmplwi r28, 0x0
    beq @L_801AED40
    cmplwi r27, 0x0
    beq @L_801AED40
    lwz r3, 0x8(r28)
    bl fn_801A053C
    lwz r4, 0x4(r27)
    li r3, 0x0
    li r5, 0x0
    bl fn_8019C128
    stw r3, 0x8(r28)
    lwz r0, 0x8(r28)
    cmplwi r0, 0x0
    bne @L_801AED0C
    addi r5, r31, 0x80
    addi r3, r2, -22864
    li r4, 0x4f2
    bl fn_80196E10
@L_801AED0C:
    lwz r4, 0x8(r28)
    cmplwi r4, 0x0
    beq @L_801AED40
    lhz r3, 0x6(r4)
    addi r0, r3, 0x1
    sth r0, 0x6(r4)
    lhz r0, 0x6(r4)
    cmplwi r0, 0x0
    bne @L_801AED40
    addi r3, r31, 0x24
    addi r5, r31, 0x30
    li r4, 0x9e
    bl fn_80196E10
@L_801AED40:
    lwz r28, 0x0(r28)
    addi r27, r27, 0x8
@L_801AED48:
    cmplwi r28, 0x0
    beq @L_801AED5C
    lwz r0, 0x4(r27)
    cmplwi r0, 0x0
    bne @L_801AECC4
@L_801AED5C:
    lwz r29, 0x0(r29)
    lwz r30, 0x0(r30)
@L_801AED64:
    cmplwi r29, 0x0
    beq @L_801AED74
    cmplwi r30, 0x0
    bne @L_801AEC08
@L_801AED74:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
