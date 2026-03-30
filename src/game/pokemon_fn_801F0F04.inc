    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r30, r3
    lwz r29, lbl_8047B5E8(r13)
    lwz r0, lbl_8047B5EC(r13)
    addi r3, r29, 0x1
    clrlwi r3, r3, 27
    cmplw r3, r0
    beq @L_801F0F7C
    mulli r4, r29, 0x30
    lis r3, lbl_8046D790@ha
    li r0, 0x6
    addi r3, r3, lbl_8046D790@l
    add r31, r3, r4
    subi r4, r30, 0x4
    subi r5, r31, 0x4
    mtctr r0
    @L_801F0F50:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801F0F50
    lwz r3, lbl_8047B5E8(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B5E8(r13)
    clrlwi r0, r0, 27
    stw r0, lbl_8047B5E8(r13)
    b @L_801F0F84
    @L_801F0F7C:
    li r31, 0x0
    b @L_801F0F9C
    @L_801F0F84:
    mr r3, r30
    mr r4, r29
    bl fn_8020D78C
    mr r3, r31
    mr r4, r29
    bl fn_8020D78C
    @L_801F0F9C:
    cmplwi r31, 0x0
    bne @L_801F0FAC
    li r3, 0x2
    b @L_801F1078
    @L_801F0FAC:
    cmplwi r30, 0x0
    bne @L_801F0FBC
    li r0, 0x0
    b @L_801F0FF0
    @L_801F0FBC:
    mr r3, r30
    bl fn_8020D950
    clrlwi. r0, r3, 16
    bne @L_801F0FD4
    li r0, 0x0
    b @L_801F0FF0
    @L_801F0FD4:
    mr r3, r30
    bl fn_8020D920
    cmplwi r3, 0x0
    bne @L_801F0FEC
    li r0, 0x0
    b @L_801F0FF0
    @L_801F0FEC:
    li r0, 0x1
    @L_801F0FF0:
    clrlwi. r0, r0, 24
    bne @L_801F1000
    li r29, 0x0
    b @L_801F1038
    @L_801F1000:
    mr r3, r30
    bl fn_8020D920
    bl fn_8020D82C
    bl fn_8020D7E8
    bl fn_8020D7B4
    cmplwi r3, 0x0
    beq @L_801F1034
    mr r12, r3
    mr r3, r30
    mtctr r12
    bctrl
    mr r29, r3
    b @L_801F1038
    @L_801F1034:
    li r29, 0x1
    @L_801F1038:
    clrlwi r0, r29, 24
    cmplwi r0, 0x1
    beq @L_801F1068
    lwz r0, lbl_8047B5E8(r13)
    lwz r3, lbl_8047B5EC(r13)
    cmplw r0, r3
    beq @L_801F1074
    addi r0, r3, 0x1
    stw r0, lbl_8047B5EC(r13)
    clrlwi r0, r0, 27
    stw r0, lbl_8047B5EC(r13)
    b @L_801F1074
    @L_801F1068:
    mr r3, r31
    mr r4, r30
    bl fn_8020D968
    @L_801F1074:
    mr r3, r29
    @L_801F1078:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
