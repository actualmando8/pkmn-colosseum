    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r31, r5
    mr r30, r6
    mr r29, r4
    mr r28, r3
    li r5, 0x48
    mr r4, r30
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80254B64
    li r3, 0x0
    b @L_80254C88
@L_80254B64:
    mr r3, r28
    mr r4, r30
    li r5, 0xf
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80254B88
    li r3, 0x0
    b @L_80254C88
@L_80254B88:
    mr r3, r28
    mr r4, r30
    li r5, 0x14
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80254BAC
    li r3, 0x0
    b @L_80254C88
@L_80254BAC:
    mr r4, r30
    li r3, 0x2
    bl fn_801F025C
    li r4, 0x4b
    bl fn_801F6E98
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80254BD4
    li r3, 0x0
    b @L_80254C88
@L_80254BD4:
    mr r3, r28
    mr r4, r29
    mr r5, r31
    mr r6, r30
    li r7, 0x0
    bl fn_8025C264
    mr r31, r3
    mr r3, r28
    mr r4, r30
    bl fn_8025CC90
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80254C10
    li r3, 0x0
    b @L_80254C88
@L_80254C10:
    mr r3, r28
    mr r4, r30
    li r5, 0x26
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80254C34
    li r3, 0x0
    b @L_80254C88
@L_80254C34:
    mr r3, r28
    mr r4, r30
    bl fn_80237310
    clrlwi. r0, r3, 24
    bne @L_80254C50
    li r3, 0x0
    b @L_80254C88
@L_80254C50:
    mr r4, r30
    li r3, 0x8
    bl fn_80229704
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80254C70
    li r3, 0x0
    b @L_80254C88
@L_80254C70:
    cmpwi r31, 0x0
    bne @L_80254C80
    li r3, 0x0
    b @L_80254C88
@L_80254C80:
    cmpwi r31, -0x1
    li r3, 0x1
@L_80254C88:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
