    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bne @L_8025FBF0
    li r3, 0x0
    li r4, 0xc
    bl fn_80129280
@L_8025FBF0:
    li r31, 0x1
    b @L_8025FC7C
@L_8025FBF8:
    li r3, 0x0
    li r4, 0xc
    bl fn_80129280
    mr r30, r3
    clrlwi r3, r31, 16
    lhz r6, 0x0(r30)
    li r5, 0x0
    b @L_8025FC38
@L_8025FC18:
    clrlwi r0, r5, 16
    mulli r4, r0, 0xc
    addi r0, r4, 0x4
    lhzx r0, r30, r0
    clrlwi r0, r0, 18
    cmplw r0, r3
    beq @L_8025FC78
    addi r5, r5, 0x1
@L_8025FC38:
    clrlwi r0, r5, 16
    cmplw r0, r6
    blt @L_8025FC18
    mulli r4, r6, 0xc
    ori r5, r31, 0x8000
    li r3, -0x1
    addi r0, r4, 0x4
    sthx r5, r30, r0
    bl fn_800E0C04
    lhz r0, 0x0(r30)
    mulli r4, r0, 0xc
    addi r0, r4, 0xc
    stwx r3, r30, r0
    lhz r3, 0x0(r30)
    addi r0, r3, 0x1
    sth r0, 0x0(r30)
@L_8025FC78:
    addi r31, r31, 0x1
@L_8025FC7C:
    clrlwi r0, r31, 16
    cmplwi r0, 0xfb
    ble @L_8025FBF8
    li r31, 0x115
    b @L_8025FD14
@L_8025FC90:
    li r3, 0x0
    li r4, 0xc
    bl fn_80129280
    mr r30, r3
    clrlwi r3, r31, 16
    lhz r6, 0x0(r30)
    li r5, 0x0
    b @L_8025FCD0
@L_8025FCB0:
    clrlwi r0, r5, 16
    mulli r4, r0, 0xc
    addi r0, r4, 0x4
    lhzx r0, r30, r0
    clrlwi r0, r0, 18
    cmplw r0, r3
    beq @L_8025FD10
    addi r5, r5, 0x1
@L_8025FCD0:
    clrlwi r0, r5, 16
    cmplw r0, r6
    blt @L_8025FCB0
    mulli r4, r6, 0xc
    ori r5, r31, 0x8000
    li r3, -0x1
    addi r0, r4, 0x4
    sthx r5, r30, r0
    bl fn_800E0C04
    lhz r0, 0x0(r30)
    mulli r4, r0, 0xc
    addi r0, r4, 0xc
    stwx r3, r30, r0
    lhz r3, 0x0(r30)
    addi r0, r3, 0x1
    sth r0, 0x0(r30)
@L_8025FD10:
    addi r31, r31, 0x1
@L_8025FD14:
    clrlwi r0, r31, 16
    cmplwi r0, 0x19b
    ble @L_8025FC90
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
