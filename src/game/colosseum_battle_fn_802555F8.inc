    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r31, r5
    mr r30, r6
    mr r29, r4
    mr r28, r3
    li r5, 0x14
    mr r4, r30
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80255638
    li r3, 0x0
    b @L_8025580C
@L_80255638:
    mr r3, r28
    mr r4, r30
    li r5, 0x6
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025565C
    li r3, 0x0
    b @L_8025580C
@L_8025565C:
    mr r3, r28
    mr r4, r30
    li r5, 0xa
    bl fn_80237DBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80255680
    li r3, 0x0
    b @L_8025580C
@L_80255680:
    mr r3, r28
    mr r4, r30
    li r5, 0x29
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802556A4
    li r3, 0x0
    b @L_8025580C
@L_802556A4:
    mr r3, r28
    mr r4, r30
    li r5, 0x11
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80255780
    mr r3, r28
    mr r4, r30
    li r5, 0x14
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80255780
    mr r3, r28
    mr r4, r30
    li r5, 0x7
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80255780
    mr r3, r28
    mr r4, r30
    li r5, 0xf
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80255780
    mr r3, r28
    mr r4, r30
    li r5, 0x48
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80255780
    mr r3, r28
    mr r4, r30
    li r5, 0x29
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80255754
    li r0, 0x0
    b @L_80255784
@L_80255754:
    mr r3, r28
    mr r4, r30
    li r5, 0x28
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80255780
    mr r3, r28
    mr r4, r30
    li r5, 0xc
    bl fn_80237F74
@L_80255780:
    li r0, 0x1
@L_80255784:
    clrlwi. r0, r0, 24
    bne @L_80255794
    li r3, 0x0
    b @L_8025580C
@L_80255794:
    mr r3, r28
    mr r4, r30
    bl fn_80237310
    clrlwi. r0, r3, 24
    bne @L_802557B0
    li r3, 0x0
    b @L_8025580C
@L_802557B0:
    mr r3, r28
    mr r4, r29
    mr r5, r31
    mr r6, r30
    li r7, 0x0
    bl fn_8025C264
    mr r31, r3
    mr r4, r30
    li r3, 0x2
    bl fn_801F025C
    li r4, 0x4b
    bl fn_801F6E98
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802557F4
    li r3, 0x0
    b @L_8025580C
@L_802557F4:
    cmpwi r31, 0x0
    bne @L_80255804
    li r3, 0x0
    b @L_8025580C
@L_80255804:
    cmpwi r31, -0x1
    li r3, 0x1
@L_8025580C:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
