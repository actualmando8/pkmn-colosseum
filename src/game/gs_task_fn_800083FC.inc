    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    cmplwi r3, 0x0
    bne @L_80008420
    lis r3, 0x1
    subi r3, r3, 0x149d
    bl fn_800FA280
    b @L_80008450
    @L_80008420:
    cmplwi r3, 0x163
    blt @L_80008438
    lis r3, 0x1
    subi r3, r3, 0x149d
    bl fn_800FA280
    b @L_80008450
    @L_80008438:
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    @L_80008450:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
