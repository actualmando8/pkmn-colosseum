    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r27, 0x2c(r1)
    mr r27, r3
    mr r30, r6
    mr r28, r4
    mr r29, r5
    li r3, 0x0
    mr r4, r30
    bl fn_801F4354
    mr r0, r3
    mr r3, r27
    mr r31, r0
    mr r4, r30
    li r5, 0x15
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025AF30
    li r3, 0x0
    b @L_8025AFA4
@L_8025AF30:
    mr r3, r27
    mr r4, r30
    li r5, 0x25
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025AF54
    li r3, 0x0
    b @L_8025AFA4
@L_8025AF54:
    mr r3, r27
    mr r4, r28
    mr r5, r29
    mr r6, r30
    li r7, 0x0
    bl fn_8025C264
    mr r0, r3
    mr r3, r31
    mr r31, r0
    addi r4, r1, 0x8
    bl fn_801F87CC
    clrlwi. r0, r3, 16
    bne @L_8025AF8C
    li r31, 0x0
@L_8025AF8C:
    cmpwi r31, 0x0
    bne @L_8025AF9C
    li r3, 0x0
    b @L_8025AFA4
@L_8025AF9C:
    cmpwi r31, -0x1
    li r3, 0x1
@L_8025AFA4:
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
