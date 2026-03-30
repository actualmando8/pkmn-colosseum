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
    cmpwi r0, 0x50
    beq @L_80210058
    bge @L_8021004C
    cmpwi r0, 0x28
    beq @L_80210058
    b @L_80210060
@L_8021004C:
    cmpwi r0, 0x78
    beq @L_80210058
    b @L_80210060
@L_80210058:
    li r3, 0x0
    b @L_80210064
@L_80210060:
    li r3, 0x1
@L_80210064:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
