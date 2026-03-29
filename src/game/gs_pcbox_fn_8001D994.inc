    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8001D9C4
    li r3, 0xff
    b @L_8001DA48
    @L_8001D9C4:
    mr r3, r30
    bl fn_8011F5C8
    clrlwi r0, r3, 16
    cmplwi r0, 0x1d
    beq @L_8001D9E0
    cmplwi r0, 0x20
    bne @L_8001DA40
    @L_8001D9E0:
    mr r3, r30
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    clrlwi r3, r3, 16
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_8001DA40
    bl fn_8011E760
    bl fn_800FA280
    mr r0, r3
    mr r3, r30
    mr r31, r0
    li r4, 0x0
    li r5, 0x77
    li r6, 0x0
    bl fn_8012640C
    mr r4, r31
    bl fn_800F9EE4
    cmpwi r3, 0x0
    bne @L_8001DA40
    li r3, 0x2
    b @L_8001DA48
    @L_8001DA40:
    mr r3, r30
    bl fn_801231A4
    @L_8001DA48:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
