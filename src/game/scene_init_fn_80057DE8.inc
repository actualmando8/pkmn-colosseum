    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80057E14
    li r3, 0x0
    b @L_80057E2C
    @L_80057E14:
    mr r3, r31
    bl fn_8011FC74
    clrlwi r3, r3, 24
    neg r0, r3
    or r0, r0, r3
    srwi r3, r0, 31
    @L_80057E2C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
