    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r4
    mr r30, r3
    bne @L_801ED010
    li r3, 0x0
    li r4, 0xb
    bl fn_80129280
    mr r30, r3
@L_801ED010:
    cmplwi r29, 0x0
    bne @L_801ED020
    li r3, 0x0
    b @L_801ED0B8
@L_801ED020:
    cmplwi r30, 0x0
    mr r3, r30
    bne @L_801ED038
    li r3, 0x0
    li r4, 0xb
    bl fn_80129280
@L_801ED038:
    addic. r31, r3, 0x8
    bne @L_801ED048
    li r3, 0x0
    b @L_801ED0B8
@L_801ED048:
    li r0, 0x27
    subi r5, r29, 0x4
    subi r4, r31, 0x4
    mtctr r0
@L_801ED058:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801ED058
    mr r3, r31
    bl fn_8011ED68
    clrlwi. r0, r3, 24
    beq @L_801ED084
    mr r3, r31
    bl fn_8011EE28
@L_801ED084:
    cmplwi r30, 0x0
    bne @L_801ED09C
    li r3, 0x0
    li r4, 0xb
    bl fn_80129280
    mr r30, r3
@L_801ED09C:
    cmplwi r30, 0x0
    beq @L_801ED0AC
    li r0, 0x0
    stb r0, 0x0(r30)
@L_801ED0AC:
    mr r3, r31
    bl fn_80124A60
    li r3, 0x1
@L_801ED0B8:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
