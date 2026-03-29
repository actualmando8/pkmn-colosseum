    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001DA8C
    li r3, 0xff
    b @L_8001DAB8
    @L_8001DA8C:
    mr r3, r31
    bl fn_8011F5C8
    clrlwi r0, r3, 16
    cmplwi r0, 0x1d
    beq @L_8001DAA8
    cmplwi r0, 0x20
    bne @L_8001DAB0
    @L_8001DAA8:
    li r3, 0x2
    b @L_8001DAB8
    @L_8001DAB0:
    mr r3, r31
    bl fn_801231A4
    @L_8001DAB8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
