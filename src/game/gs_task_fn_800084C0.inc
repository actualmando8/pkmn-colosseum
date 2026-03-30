    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    cmplwi r3, 0x0
    bne @L_800084E4
    lis r3, 0x1
    subi r3, r3, 0x149d
    bl fn_800FA280
    b @L_80008508
    @L_800084E4:
    cmplwi r3, 0xb
    ble @L_800084FC
    lis r3, 0x1
    subi r3, r3, 0x149d
    bl fn_800FA280
    b @L_80008508
    @L_800084FC:
    clrlwi r3, r3, 24
    bl fn_8011BA0C
    bl fn_800FA280
    @L_80008508:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
