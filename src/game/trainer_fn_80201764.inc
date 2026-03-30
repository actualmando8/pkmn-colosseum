    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r4
    mr r31, r3
    mr r30, r5
    mr r3, r29
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_802017BC
    mr r3, r29
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    beq @L_802017BC
    mr r3, r29
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_80201858
    @L_802017BC:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
    mr r3, r29
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0x7c
    beq @L_802017FC
    mr r3, r29
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xc8
    bne @L_80201830
    @L_802017FC:
    cmplwi r31, 0x0
    bne @L_8020180C
    li r3, 0x0
    b @L_80201820
    @L_8020180C:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_80201820:
    mr r4, r29
    mr r5, r30
    bl fn_801214FC
    b @L_8020187C
    @L_80201830:
    mr r3, r29
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xcd
    bne @L_8020187C
    mr r3, r31
    mr r4, r29
    mr r5, r30
    bl fn_8011A280
    b @L_8020187C
    @L_80201858:
    mr r3, r29
    bl fn_80119ED0
    clrlwi r0, r3, 16
    cmplwi r0, 0xd8
    bne @L_8020187C
    mr r3, r31
    mr r4, r29
    mr r5, r30
    bl fn_8011A280
    @L_8020187C:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
