    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r27, 0x3c(r1)
    mr r29, r3
    mr r30, r4
    li r31, 0x0
    b @L_8020BE14
@L_8020BCB4:
    mr r3, r29
    mr r6, r31
    li r4, 0x0
    li r5, 0x59
    bl fn_801F54A4
    mr. r27, r3
    beq @L_8020BE10
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020BCF8
    mr r3, r27
    li r4, 0x0
    li r5, 0x112
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    b @L_8020BE10
@L_8020BCF8:
    mr r3, r27
    li r4, 0x0
    li r5, 0xfe
    li r6, 0x0
    bl fn_8012640C
    mr. r28, r3
    bne @L_8020BD30
    mr r3, r27
    li r4, 0x0
    li r5, 0x112
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    b @L_8020BE10
@L_8020BD30:
    bl fn_801F1170
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020BD58
    mr r3, r27
    li r4, 0x0
    li r5, 0x112
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    b @L_8020BE10
@L_8020BD58:
    rlwinm. r0, r30, 0, 24, 31
    bne @L_8020BD78
    mr r3, r28
    bl fn_801F0898
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x8
    bne @L_8020BE10
    b @L_8020BD8C
@L_8020BD78:
    mr r3, r28
    bl fn_801F0898
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x8
    beq @L_8020BE10
@L_8020BD8C:
    mr r3, r27
    li r4, 0x0
    li r5, 0x112
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x1
    beq @L_8020BE10
    mr r3, r27
    li r4, 0x0
    li r5, 0x112
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    li r0, 0x6
    addi r5, r1, 0x4
    subi r4, r28, 0x4
    mtctr r0
@L_8020BDD0:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8020BDD0
    addi r3, r1, 0x8
    bl fn_801F0F04
    rlwinm. r0, r30, 0, 24, 31
    beq @L_8020BE10
    li r3, 0x0
    bl fn_801F4AC0
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    beq @L_8020BE10
    li r3, 0x1
    b @L_8020BE24
@L_8020BE10:
    addi r31, r31, 0x1
@L_8020BE14:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x8
    blt @L_8020BCB4
    li r3, 0x1
@L_8020BE24:
    lmw r27, 0x3c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
