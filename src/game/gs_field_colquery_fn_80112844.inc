    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r3, lbl_80408378@ha
    addi r3, r3, lbl_80408378@l
    lwz r3, 0x28(r3)
    cmplwi r3, 0x0
    beq @L_8011287C
    li r4, 0x0
    crxor 6, 6, 6
    bl fn_800F7434
    lis r3, fn_80112844@ha
    addi r3, r3, fn_80112844@l
    bl fn_800FF0A0
@L_8011287C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
