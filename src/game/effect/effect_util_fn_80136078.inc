    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr. r29, r6
    mr r26, r3
    mr r27, r4
    mr r28, r5
    beq @L_801360A4
    bl fn_8013613C
    b @L_801360AC
    @L_801360A4:
    li r6, 0x0
    bl fn_8013613C
    @L_801360AC:
    mr r3, r26
    bl fn_80135F90
    clrlwi. r31, r3, 16
    beq @L_80136128
    li r30, 0x0
    b @L_8013611C
    @L_801360C4:
    mr r3, r31
    clrlwi r4, r30, 16
    bl fn_80135F58
    clrlwi. r0, r3, 16
    beq @L_80136118
    cmplwi r29, 0x0
    beq @L_80136104
    clrlwi r4, r30, 16
    mr r3, r26
    addi r0, r4, 0x1
    mr r4, r27
    slwi r0, r0, 2
    mr r5, r28
    add r6, r29, r0
    bl fn_8013613C
    b @L_80136118
    @L_80136104:
    mr r3, r26
    mr r4, r27
    mr r5, r28
    li r6, 0x0
    bl fn_8013613C
    @L_80136118:
    addi r30, r30, 0x1
    @L_8013611C:
    clrlwi r0, r30, 16
    cmplwi r0, 0x8
    blt @L_801360C4
    @L_80136128:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
