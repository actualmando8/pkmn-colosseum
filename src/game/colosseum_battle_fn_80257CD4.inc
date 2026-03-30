    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    lis r7, 0x1
    stw r0, 0x54(r1)
    subi r7, r7, 0x1
    stmw r27, 0x3c(r1)
    mr r30, r3
    mr r31, r6
    bl fn_8025C264
    mr r27, r3
    mr r3, r30
    mr r4, r31
    addi r5, r1, 0x1c
    addi r6, r1, 0x8
    li r7, 0x0
    bl fn_802367CC
    mr r28, r3
    mr r3, r30
    mr r4, r31
    bl fn_802364BC
    clrlwi. r0, r3, 16
    mr r29, r3
    beq @L_80257D58
    cmplwi r0, 0x165
    beq @L_80257D58
    cmplwi r0, 0xffff
    beq @L_80257D58
    mr r3, r30
    mr r4, r31
    bl fn_80237288
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80257D60
@L_80257D58:
    li r3, 0x0
    b @L_80257DE4
@L_80257D60:
    clrlwi r7, r28, 16
    addi r6, r1, 0x8
    clrlwi r5, r29, 16
    addi r4, r1, 0x1c
    li r3, 0x0
    li r10, 0x0
    b @L_80257DB0
@L_80257D7C:
    clrlslwi r8, r10, 24, 1
    lhax r9, r6, r8
    extsh. r0, r9
    blt @L_80257DAC
    lhzx r0, r4, r8
    cmplw r5, r0
    bne @L_80257DAC
    mr r3, r30
    mr r4, r31
    clrlwi r5, r9, 24
    bl fn_802381C4
    b @L_80257DBC
@L_80257DAC:
    addi r10, r10, 0x1
@L_80257DB0:
    clrlwi r0, r10, 24
    cmpw r0, r7
    blt @L_80257D7C
@L_80257DBC:
    clrlwi. r0, r3, 24
    bne @L_80257DCC
    li r3, 0x0
    b @L_80257DE4
@L_80257DCC:
    cmpwi r27, 0x0
    bne @L_80257DDC
    li r3, 0x0
    b @L_80257DE4
@L_80257DDC:
    cmpwi r27, -0x1
    li r3, 0x1
@L_80257DE4:
    lmw r27, 0x3c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
