    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    li r31, 0x0
    bl fn_80057270
    cmplwi r3, 0x0
    beq @L_80053BD8
    bl fn_8011E820
    clrlwi r0, r3, 24
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_80053BD8
    li r31, 0x1
    @L_80053BD8:
    mr r3, r30
    clrlwi r4, r31, 24
    bl fn_80109220
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
