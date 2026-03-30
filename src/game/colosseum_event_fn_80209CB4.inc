    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_80209CD4
    li r3, 0x0
    b @L_80209D7C
@L_80209CD4:
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_8011BEB4
    cmpwi r3, 0x0
    bne @L_80209CF4
    li r3, 0x0
    b @L_80209D7C
@L_80209CF4:
    mr r3, r31
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_8011BEB4
    cmpwi r3, 0x163
    bne @L_80209D18
    li r3, 0x0
    b @L_80209D7C
@L_80209D18:
    mr r3, r31
    li r4, 0x0
    li r5, 0x28
    li r6, 0x0
    bl fn_8011BEB4
    cmpwi r3, 0x0
    bne @L_80209D3C
    li r3, 0x0
    b @L_80209D7C
@L_80209D3C:
    mr r3, r31
    li r4, 0x0
    li r5, 0x28
    li r6, 0x0
    bl fn_8011BEB4
    cmpwi r3, 0x163
    bne @L_80209D60
    li r3, 0x0
    b @L_80209D7C
@L_80209D60:
    mr r3, r31
    li r4, 0x0
    li r5, 0x29
    li r6, 0x0
    bl fn_8011BEB4
    subic r0, r3, 0x1
    subfe r3, r0, r3
@L_80209D7C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
