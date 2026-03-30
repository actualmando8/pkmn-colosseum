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
    rlwinm r0, r3, 0, 16, 31
    cmpwi r0, 0x46
    beq @L_8020FE1C
    bge @L_8020FDF0
    cmpwi r0, 0x1e
    beq @L_8020FE14
    bge @L_8020FDE4
    cmpwi r0, 0xa
    beq @L_8020FE14
    b @L_8020FE2C
@L_8020FDE4:
    cmpwi r0, 0x32
    beq @L_8020FE1C
    b @L_8020FE2C
@L_8020FDF0:
    cmpwi r0, 0x6e
    beq @L_8020FE24
    bge @L_8020FE08
    cmpwi r0, 0x5a
    beq @L_8020FE1C
    b @L_8020FE2C
@L_8020FE08:
    cmpwi r0, 0x96
    beq @L_8020FE24
    b @L_8020FE2C
@L_8020FE14:
    li r3, 0x0
    b @L_8020FE30
@L_8020FE1C:
    li r3, 0x1
    b @L_8020FE30
@L_8020FE24:
    li r3, 0x2
    b @L_8020FE30
@L_8020FE2C:
    li r3, 0x0
@L_8020FE30:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
