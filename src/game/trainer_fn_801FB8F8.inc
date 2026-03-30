    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x43
    stw r0, 0x14(r1)
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801FB93C
    li r0, 0x0
    b @L_801FB958
    @L_801FB93C:
    cmplwi r0, 0x2
    beq @L_801FB94C
    cmplwi r0, 0x3
    bne @L_801FB954
    @L_801FB94C:
    li r0, 0x1
    b @L_801FB958
    @L_801FB954:
    li r0, 0x2
    @L_801FB958:
    clrlwi r0, r0, 24
    cntlzw r0, r0
    extrwi r3, r0, 8, 19
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
