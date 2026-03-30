    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r26, r4
    mr r27, r5
    mr r28, r6
    mr r25, r3
    mr r29, r7
    mr r30, r8
    li r4, 0x0
    li r5, 0xfe
    li r6, 0x0
    bl fn_8012640C
    mr. r31, r3
    bne @L_802050A4
    li r3, 0x0
    b @L_802050E0
@L_802050A4:
    mr r4, r26
    mr r5, r25
    mr r6, r27
    mr r7, r28
    mr r8, r29
    bl fn_801F11CC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802050DC
    mr r3, r31
    mr r4, r30
    bl fn_8020D878
    mr r3, r31
    b @L_802050E0
@L_802050DC:
    li r3, 0x0
@L_802050E0:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
