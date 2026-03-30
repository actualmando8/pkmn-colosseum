    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r7, 0x1
    stw r0, 0x24(r1)
    subi r7, r7, 0x1
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    bl fn_8025C264
    mr r31, r3
    mr r3, r29
    mr r4, r28
    mr r5, r30
    bl fn_80229934
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80254864
    li r3, 0x0
    b @L_802548C4
@L_80254864:
    mr r3, r27
    mr r4, r28
    li r5, 0x19
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80254888
    li r3, 0x0
    b @L_802548C4
@L_80254888:
    mr r3, r27
    mr r4, r30
    li r5, 0x19
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802548AC
    li r3, 0x0
    b @L_802548C4
@L_802548AC:
    cmpwi r31, 0x0
    bne @L_802548BC
    li r3, 0x0
    b @L_802548C4
@L_802548BC:
    cmpwi r31, -0x1
    li r3, 0x1
@L_802548C4:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
