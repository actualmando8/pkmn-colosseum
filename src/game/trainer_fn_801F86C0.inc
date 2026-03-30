    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr. r26, r3
    mr r27, r4
    beq @L_801F87B8
    li r29, 0x0
    b @L_801F87AC
    @L_801F86E4:
    mr r3, r26
    mr r6, r29
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    mr r30, r3
    bl fn_80206A04
    clrlwi. r0, r3, 24
    bne @L_801F870C
    li r30, 0x0
    @L_801F870C:
    cmplwi r30, 0x0
    beq @L_801F87A8
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr. r28, r3
    beq @L_801F87A8
    bl fn_801230E0
    mr r31, r3
    mr r3, r30
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_801F87A8
    mr r4, r28
    bl fn_8011F5FC
    mr r3, r30
    li r4, 0x4
    bl fn_80121ADC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F8784
    mr r3, r30
    li r4, 0x4
    li r5, 0x1
    bl fn_8012190C
    @L_801F8784:
    clrlwi r0, r27, 24
    cmplwi r0, 0x1
    bne @L_801F87A8
    mr r3, r30
    clrlwi r7, r31, 16
    li r4, 0x0
    li r5, 0x82
    li r6, 0x0
    bl fn_801254B4
    @L_801F87A8:
    addi r29, r29, 0x1
    @L_801F87AC:
    clrlwi r0, r29, 16
    cmplwi r0, 0x6
    blt @L_801F86E4
    @L_801F87B8:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
