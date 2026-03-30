    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    clrlwi. r0, r4, 16
    stw r31, 0x1c(r1)
    mr r31, r5
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    beq @L_8013591C
    cmplwi r0, 0x7
    blt @L_80135874
    b @L_8013591C
    @L_80135874:
    cmplwi r29, 0x0
    bne @L_801358A0
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_8013591C
    li r4, 0x1
    bl fn_80129280
    mr. r29, r3
    beq @L_8013591C
    @L_801358A0:
    mr r3, r29
    bl fn_80135B0C
    cmplwi r3, 0x0
    beq @L_8013591C
    clrlwi r0, r30, 16
    cmpwi r0, 0x3
    beq @L_801358FC
    bge @L_801358D0
    cmpwi r0, 0x1
    beq @L_801358E0
    bge @L_801358F0
    b @L_8013591C
    @L_801358D0:
    cmpwi r0, 0x5
    beq @L_80135914
    bge @L_8013591C
    b @L_80135908
    @L_801358E0:
    mr r3, r29
    mr r4, r31
    bl fn_80135AEC
    b @L_8013591C
    @L_801358F0:
    clrlwi r4, r31, 24
    bl fn_80135A60
    b @L_8013591C
    @L_801358FC:
    clrlwi r4, r31, 24
    bl fn_80135A50
    b @L_8013591C
    @L_80135908:
    clrlwi r4, r31, 24
    bl fn_80135A40
    b @L_8013591C
    @L_80135914:
    clrlwi r4, r31, 24
    bl fn_80135A30
    @L_8013591C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
