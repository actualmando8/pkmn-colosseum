    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r3, r4
    li r4, 0x0
    stw r0, 0x14(r1)
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BEB4
    clrlwi r3, r3, 16
    cmplwi r3, 0x18
    bgt @L_8021015C
    li r3, 0x0
    b @L_80210180
@L_8021015C:
    cmplwi r3, 0x24
    bgt @L_8021016C
    li r3, 0x1
    b @L_80210180
@L_8021016C:
    li r0, 0x50
    subfc r0, r3, r0
    addze r0, r3
    subf r3, r0, r3
    addi r3, r3, 0x3
@L_80210180:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
