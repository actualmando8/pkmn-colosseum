    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    clrlwi. r0, r4, 16
    stw r31, 0xc(r1)
    mr r31, r4
    beq @L_8013595C
    cmplwi r0, 0x7
    blt @L_80135964
    @L_8013595C:
    li r3, 0x0
    b @L_80135A1C
    @L_80135964:
    cmplwi r3, 0x0
    bne @L_801359A0
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_80135988
    li r3, 0x0
    b @L_80135A1C
    @L_80135988:
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_801359A0
    li r3, 0x0
    b @L_80135A1C
    @L_801359A0:
    bl fn_80135B0C
    cmplwi r3, 0x0
    bne @L_801359B4
    li r3, 0x0
    b @L_80135A1C
    @L_801359B4:
    clrlwi r0, r31, 16
    cmpwi r0, 0x3
    beq @L_801359F4
    bge @L_801359D4
    cmpwi r0, 0x1
    beq @L_80135A1C
    bge @L_801359E8
    b @L_80135A18
    @L_801359D4:
    cmpwi r0, 0x5
    beq @L_80135A0C
    bge @L_80135A18
    b @L_80135A00
    b @L_80135A1C
    @L_801359E8:
    bl fn_80135AB8
    clrlwi r3, r3, 24
    b @L_80135A1C
    @L_801359F4:
    bl fn_80135AA0
    clrlwi r3, r3, 24
    b @L_80135A1C
    @L_80135A00:
    bl fn_80135A88
    clrlwi r3, r3, 24
    b @L_80135A1C
    @L_80135A0C:
    bl fn_80135A70
    clrlwi r3, r3, 24
    b @L_80135A1C
    @L_80135A18:
    li r3, 0x0
    @L_80135A1C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
