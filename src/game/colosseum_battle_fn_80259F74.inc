    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r31, r6
    mr r28, r4
    mr r29, r5
    li r3, 0x2
    mr r4, r31
    bl fn_801F025C
    mr r0, r3
    mr r3, r27
    mr r30, r0
    mr r4, r31
    li r5, 0x14
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80259FCC
    li r3, 0x0
    b @L_8025A158
@L_80259FCC:
    mr r3, r27
    mr r4, r31
    li r5, 0x14
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80259FF0
    li r3, 0x0
    b @L_8025A158
@L_80259FF0:
    mr r3, r27
    mr r4, r31
    li r5, 0x9
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025A014
    li r3, 0x0
    b @L_8025A158
@L_8025A014:
    mr r3, r27
    mr r4, r31
    li r5, 0x11
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025A0F0
    mr r3, r27
    mr r4, r31
    li r5, 0x14
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025A054
    li r0, 0x0
    b @L_8025A0F4
@L_8025A054:
    mr r3, r27
    mr r4, r31
    li r5, 0x7
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025A0F0
    mr r3, r27
    mr r4, r31
    li r5, 0xf
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025A0F0
    mr r3, r27
    mr r4, r31
    li r5, 0x48
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025A0F0
    mr r3, r27
    mr r4, r31
    li r5, 0x29
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025A0F0
    mr r3, r27
    mr r4, r31
    li r5, 0x28
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8025A0F0
    mr r3, r27
    mr r4, r31
    li r5, 0xc
    bl fn_80237F74
@L_8025A0F0:
    li r0, 0x1
@L_8025A0F4:
    clrlwi. r0, r0, 24
    bne @L_8025A104
    li r3, 0x0
    b @L_8025A158
@L_8025A104:
    mr r3, r27
    mr r4, r28
    mr r5, r29
    mr r6, r31
    li r7, 0x0
    bl fn_8025C264
    mr r31, r3
    mr r3, r30
    li r4, 0x4b
    bl fn_801F6E98
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025A140
    li r3, 0x0
    b @L_8025A158
@L_8025A140:
    cmpwi r31, 0x0
    bne @L_8025A150
    li r3, 0x0
    b @L_8025A158
@L_8025A150:
    cmpwi r31, -0x1
    li r3, 0x1
@L_8025A158:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
