    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr. r30, r3
    mr r31, r4
    beq @L_801F751C
    beq @L_801F7508
    beq @L_801F74AC
    li r4, 0x0
    bl fn_801F78AC
    @L_801F74AC:
    cmplwi r30, 0x0
    bne @L_801F74BC
    li r3, 0x0
    b @L_801F74C8
    @L_801F74BC:
    mr r3, r30
    li r4, 0x0
    bl fn_801F7908
    @L_801F74C8:
    li r4, 0x6
    bl fn_8011B950
    cmplwi r30, 0x0
    bne @L_801F74E0
    li r3, 0x0
    b @L_801F74EC
    @L_801F74E0:
    mr r3, r30
    li r4, 0x0
    bl fn_801F78D4
    @L_801F74EC:
    li r4, 0x2
    bl fn_801FA8CC
    cmplwi r30, 0x0
    beq @L_801F7508
    mr r3, r30
    li r4, 0x0
    bl fn_801F789C
    @L_801F7508:
    cmplwi r30, 0x0
    beq @L_801F751C
    clrlwi r4, r31, 16
    mr r3, r30
    bl fn_801F78AC
    @L_801F751C:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
