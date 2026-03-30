    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_80135828
    bne @L_80135748
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_8013575C
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_8013575C
    @L_80135748:
    bl fn_80135B0C
    cmplwi r3, 0x0
    beq @L_8013575C
    li r4, 0x0
    bl fn_80135A60
    @L_8013575C:
    cmplwi r31, 0x0
    mr r3, r31
    bne @L_8013578C
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_801357A0
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_801357A0
    @L_8013578C:
    bl fn_80135B0C
    cmplwi r3, 0x0
    beq @L_801357A0
    li r4, 0x0
    bl fn_80135A50
    @L_801357A0:
    cmplwi r31, 0x0
    mr r3, r31
    bne @L_801357D0
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_801357E4
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_801357E4
    @L_801357D0:
    bl fn_80135B0C
    cmplwi r3, 0x0
    beq @L_801357E4
    li r4, 0x0
    bl fn_80135A40
    @L_801357E4:
    cmplwi r31, 0x0
    mr r3, r31
    bne @L_80135814
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_80135828
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_80135828
    @L_80135814:
    bl fn_80135B0C
    cmplwi r3, 0x0
    beq @L_80135828
    li r4, 0x0
    bl fn_80135A30
    @L_80135828:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
