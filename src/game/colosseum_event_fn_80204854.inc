    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0xce
    li r6, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r31, r3
    mr r3, r4
    li r4, 0x0
    bl fn_8012640C
    extsh. r30, r3
    bge @L_8020488C
    li r3, 0x0
    b @L_80204914
@L_8020488C:
    mr r3, r31
    li r4, 0x0
    li r5, 0x121
    li r6, 0x0
    bl fn_8012640C
    extsh r0, r3
    cmpw r30, r0
    bne @L_802048B4
    li r3, 0x1
    b @L_80204914
@L_802048B4:
    mr r3, r31
    li r4, 0x0
    li r5, 0xfe
    li r6, 0x0
    bl fn_8012640C
    mr. r31, r3
    beq @L_80204910
    bl fn_801F1170
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80204910
    mr r3, r31
    bl fn_8020D950
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x9
    bne @L_80204910
    mr r3, r31
    bl fn_8020D8D8
    extsh r0, r3
    cmpw r30, r0
    bne @L_80204910
    li r3, 0x1
    b @L_80204914
@L_80204910:
    li r3, 0x0
@L_80204914:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
